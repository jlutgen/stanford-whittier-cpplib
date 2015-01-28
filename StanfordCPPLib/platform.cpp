/*
 * File: platform.cpp
 * ------------------
 * This file implements the platform interface by passing commands to
 * a Java back end that manages the display.
 */

/*************************************************************************/
/* Stanford Portable Library                                             */
/* Copyright (c) 2014 by Eric Roberts <eroberts@cs.stanford.edu>         */
/*                                                                       */
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */
/*************************************************************************/

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "exceptions.h"

#ifdef _WIN32
#  include <windows.h>
#  include <tchar.h>
#  undef MOUSE_EVENT
#  undef KEY_EVENT
#  undef MOUSE_MOVED
#  undef HELP_KEY

#else
#define inline
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <dirent.h>
#  include <errno.h>
#  include <pwd.h>
#  include <unistd.h>
#  include <signal.h>
#endif

#include "error.h"
#include "filelib.h"
#include "gevents.h"
#include "gtimer.h"
#include "gtypes.h"
#include "hashmap.h"
#include "queue.h"
#include "platform.h"
#include "stack.h"
#include "strlib.h"
#include "tokenscanner.h"
#include "vector.h"

using namespace std;

static string getLineConsole();
static void putConsole(const string & str, bool isStdErr = false);
static void endLineConsole();
static int scanInt(TokenScanner & scanner);
static double scanDouble(TokenScanner & scanner);
static GDimension scanDimension(const string & str);
static GRectangle scanRectangle(const string & str);
static void setConsoleProperties();


class ConsoleStreambuf : public streambuf {

private:

/* Constants */

   static const int BUFFER_SIZE = 1024;


/* Instance variables */

   char inBuffer[BUFFER_SIZE];
   char outBuffer[BUFFER_SIZE];
   bool blocked;

public:

   ConsoleStreambuf() {
      setg(inBuffer, inBuffer, inBuffer);
      setp(outBuffer, outBuffer + BUFFER_SIZE);
   }

   ~ConsoleStreambuf() {
      /* Empty */
   }

   bool isBlocked() {
       return blocked;
   }

   virtual int underflow() {
      // Allow long strings at some point
      blocked = true;
      string line = getLineConsole();
      blocked = false;
      int n = line.length();
      if (n + 1 >= BUFFER_SIZE) error("ConsoleStreamBuf: String too long");
      for (int i = 0; i < n; i++) {
         inBuffer[i] = line[i];
      }
      inBuffer[n++] = '\n';
      inBuffer[n] = '\0';
      setg(inBuffer, inBuffer, inBuffer + n);
      return inBuffer[0];
   }

   virtual int overflow(int ch = EOF) {
       return overflow(ch, false);
   }

   virtual int overflow(int ch, bool isStdErr) {
      string line = "";
      for (char *cp = pbase(); cp < pptr(); cp++) {
         if (*cp == '\n') {
            putConsole(line, isStdErr);
            endLineConsole();
            line = "";
         } else {
            line += *cp;
         }
      }
      if (line != "") {
         putConsole(line, isStdErr);
      }
      setp(outBuffer, outBuffer + BUFFER_SIZE);
      if (ch != EOF) {
         outBuffer[0] = ch;
         pbump(1);
      }
      return ch != EOF;
   }

   virtual int sync() {
      return overflow();
   }

   virtual int sync(bool isStdErr) {
       return overflow(EOF, isStdErr);
   }
};

/*
 * A stream buffer that just "wraps" another stream buffer.
 * This is used here to distinguish cout (black text) from cerr (red text).
 */
class ForwardingStreambuf : public std::streambuf {
private:
    ConsoleStreambuf& delegate;
    bool isStderr;

public:
    ForwardingStreambuf(ConsoleStreambuf& del, bool err = false)
            : delegate(del), isStderr(err) {
        // empty
    }

    virtual int underflow() {
        return delegate.underflow();
    }

    virtual int overflow(int ch = EOF) {
        return delegate.overflow(ch, isStderr);
    }

    virtual int sync() {
        return delegate.sync(isStderr);
    }

    // functions below are overridden for completeness,
    // but all just delegate to underlying ConsoleStreambuf
    std::streamsize in_avail() {
        return delegate.in_avail();
    }

    int snextc() {
        return delegate.snextc();
    }

    int sbumpc() {
        return delegate.sbumpc();
    }

    int sgetc() {
        return delegate.sgetc();
    }

    std::streamsize sgetn(char* s, std::streamsize n) {
        return delegate.sgetn(s, n);
    }

    int sputbackc(char c) {
        return delegate.sputbackc(c);
    }

    int sungetc() {
        return delegate.sungetc();
    }

    int sputc(char c) {
        return delegate.sputc(c);
    }

    std::streamsize sputn(const char* s, std::streamsize n) {
        return delegate.sputn(s, n);
    }
};

/* Private data */

static Queue<GEvent> eventQueue;
static HashMap<string,GTimerData *> timerTable;
static HashMap<string,GWindowData *> windowTable;
static HashMap<string,GObject *> sourceTable;
static HashMap<string,string> optionTable;
static string programName;
static ofstream logfile;
static bool tracePipe;
static ConsoleStreambuf* cinout_buf;

#ifdef _WIN32
static HANDLE rdFromJBE = NULL;
static HANDLE wrFromJBE = NULL;
static HANDLE rdToJBE = NULL;
static HANDLE wrToJBE = NULL;
static PROCESS_INFORMATION pInfo;
#else
static int pin;
static int pout;
static int child;
#endif


/* Prototypes */

static void initPipe();
static void putPipe(string line);
static string getPipe();
static string getResult(bool consumeAcks = true);
static void getStatus();
static GEvent parseEvent(string line);
static GEvent parseMouseEvent(TokenScanner & scanner, EventType type);
static GEvent parseKeyEvent(TokenScanner & scanner, EventType type);
static GEvent parseTimerEvent(TokenScanner & scanner, EventType type);
static GEvent parseWindowEvent(TokenScanner & scanner, EventType type);
static GEvent parseActionEvent(TokenScanner & scanner, EventType type);

/* Implementation of the Platform class */

Platform::Platform() {
  // empty
}

Platform::~Platform() {
  if (logfile.is_open())
    logfile.close();
}


#ifdef _WIN32
/* Windows implementations of filelib.h primitives */

bool Platform::fileExists(string filename) {
   return GetFileAttributesA(filename.c_str()) != INVALID_FILE_ATTRIBUTES;
}

bool Platform::isFile(string filename) {
   DWORD attr = GetFileAttributesA(filename.c_str());
   return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_NORMAL);
}

bool Platform::isSymbolicLink(string filename) {
   DWORD attr = GetFileAttributesA(filename.c_str());
   return attr != INVALID_FILE_ATTRIBUTES
       && (attr & FILE_ATTRIBUTE_REPARSE_POINT);
}

bool Platform::isDirectory(string filename) {
   DWORD attr = GetFileAttributesA(filename.c_str());
   return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY);
}

void Platform::setCurrentDirectory(string path) {
   if (!isDirectory(path) || !SetCurrentDirectoryA(path.c_str())) {
      error("setCurrentDirectory: Can't change to " + path);
   }
}

string Platform::getCurrentDirectory() {
   char path[MAX_PATH + 1];
   int n = GetCurrentDirectoryA(MAX_PATH + 1, path);
   return string(path, n);
}

void Platform::createDirectory(string path) {
   if (!CreateDirectoryA(path.c_str(), NULL)) {
      if ((GetLastError() == ERROR_ALREADY_EXISTS) && isDirectory(path)) return;
      error("createDirectory: Can't create " + path);
   }
}

string Platform::getDirectoryPathSeparator() {
   return "\\";
}

string Platform::getSearchPathSeparator() {
   return ";";
}

string Platform::expandPathname(string filename) {
   if (filename == "") return "";
   int len = filename.length();
   for (int i = 0; i < len; i++) {
      if (filename[i] == '/') filename[i] = '\\';
   }
   return filename;
}

void Platform::listDirectory(string path, vector<string> & list) {
   if (path == "") path = ".";
   string pattern = path + "\\*.*";
   WIN32_FIND_DATAA fd;
   HANDLE h = FindFirstFileA(pattern.c_str(), &fd);
   if (h == INVALID_HANDLE_VALUE) {
      error("listDirectory: Can't list directory");
   }
   list.clear();
   while (true) {
      string name = string(fd.cFileName);
      if (name != "." && name != "..") list.push_back(name);
      if (!FindNextFileA(h, &fd)) break;
   }
   FindClose(h);
   sort(list.begin(), list.end());
}

/* Unix implementations of filelib.h primitives */

#else

bool Platform::fileExists(string filename) {
   struct stat fileInfo;
   return stat(filename.c_str(), &fileInfo) == 0;
}

bool Platform::isFile(string filename) {
   struct stat fileInfo;
   if (stat(filename.c_str(), &fileInfo) != 0) return false;
   return S_ISREG(fileInfo.st_mode) != 0;
}

bool Platform::isSymbolicLink(string filename) {
   struct stat fileInfo;
   if (stat(filename.c_str(), &fileInfo) != 0) return false;
   return S_ISLNK(fileInfo.st_mode) != 0;
}

bool Platform::isDirectory(string filename) {
   struct stat fileInfo;
   if (stat(filename.c_str(), &fileInfo) != 0) return false;
   return S_ISDIR(fileInfo.st_mode) != 0;
}

void Platform::setCurrentDirectory(string path) {
   if (chdir(path.c_str()) != 0) { // BUGFIX (JL): was `== 0`, but chdir returns 0 on success, -1 on error
      string msg = "setCurrentDirectory: ";
      string err = string(strerror(errno));
      error(msg + err);
   }
}

string Platform::getCurrentDirectory() {
   char *cwd = getcwd(NULL, 0);
   if (cwd == NULL) {
      string msg = "getCurrentDirectory: ";
      string err = string(strerror(errno));
      error(msg + err);
   }
   string result = string(cwd);
   free(cwd);
   return result;
}

void Platform::createDirectory(string path) {
   if (endsWith(path, "/")) {
      path = path.substr(0, path.length() - 1); // BUGFIX (JL): was `path.length() - 2`
   }
   if (mkdir(path.c_str(), 0777) != 0) {
      if (errno == EEXIST && isDirectory(path)) return;
      string msg = "createDirectory: ";
      string err = string(strerror(errno));
      error(msg + err);
   }
}

string Platform::getDirectoryPathSeparator() {
   return "/";
}

string Platform::getSearchPathSeparator() {
   return ":";
}

string Platform::expandPathname(string filename) {
   if (filename == "") return "";
   int len = filename.length();
   if (filename[0] == '~') {
      int spos = 1;
      while (spos < len && filename[spos] != '\\' && filename[spos] != '/') {
         spos++;
      }
      char *homedir = NULL;
      if (spos == 1) {
         homedir = getenv("HOME");
         if (homedir == NULL) homedir = getpwuid(getuid())->pw_dir;
      } else {
         struct passwd *pw = getpwnam(filename.substr(1, spos - 1).c_str());
         if (pw == NULL) error("expandPathname: No such user");
         homedir = pw->pw_dir;
      }
      filename = string(homedir) + filename.substr(spos);
      len = filename.length();
   }
   for (int i = 0; i < len; i++) {
      if (filename[i] == '\\') filename[i] = '/';
   }
   return filename;
}

void Platform::listDirectory(string path, vector<string> & list) {
   if (path == "") path = ".";
   DIR *dir = opendir(path.c_str());
   if (dir == NULL) error(string("listDirectory: Can't open ") + path);
   list.clear();
   while (true) {
      struct dirent *ep = readdir(dir);
      if (ep == NULL) break;
      string name = string(ep->d_name);
      if (name != "." && name != "..") list.push_back(name);
   }
   closedir(dir);
   sort(list.begin(), list.end());
}

#endif



string Platform::openFileDialog(string title, string mode, string path, string patternName) {
   ostringstream os;
   os << "File.openFileDialog(";
   writeQuotedString(os, title);
   // BUGFIX (JL): empty path or "." caused dialog to start at "/" (Mac/Linux) or "C:\" (Windows)
   if (path == "" || path == ".")
       path = getCurrentDirectory();
   // BUGFIX (Stepp): (2014/10/09) wasn't working if dirs didn't have slashes at end
   std::string sep = getDirectoryPathSeparator();
   if (isDirectory(path) && !endsWith(path, sep)) {
       path += sep;
   }
   path += " "; // BUGFIX (JL): hack to circumvent back-end bug when path ends with backslash (back end will strip off this space)
   os << ", \"" << mode << "\", ";
   writeQuotedString(os, path); // BUGFIX (JL): Must use writeQuotedString because path might contain backslashes
   os << ", \"" << patternName << "\"";
   os << ")";
   putPipe(os.str());
   return getResult();
}

std::string Platform::os_getLastError() {
#ifdef _WIN32
    // Windows error-reporting code
    DWORD lastErrorCode = ::GetLastError();
    char* errorMsg = NULL;
    // Ask Windows to prepare a standard message for a GetLastError() code:
    ::FormatMessageA(
                   /* dwFlags */ FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   /* lpSource */ NULL,
                   /* dwMessageId */ lastErrorCode,
                   /* dwLanguageId */ LANG_NEUTRAL,
                   /* lpBuffer */ (LPSTR) &errorMsg,
                   /* dwSize */ 0,
                   /* arguments */ NULL);
    if (errorMsg) {
        return std::string(errorMsg);
    } else {
        return "";
    }
#else
    // Linux/Mac error-reporting code
    return std::string(strerror(errno));
#endif // _WIN32
}

void Platform::createGWindow(const GWindow & gw, double width, double height,
                             GObject *topCompound) {
   ostringstream os;
   os << gw.gwd;
   string id = os.str();
   windowTable.put(id, gw.gwd);
   os.str("");
   os << "GWindow.create(\"" << id << "\", " << width << ", " << height
                             << ", \"" << topCompound << "\")";
   putPipe(os.str());
   getStatus();
}

void Platform::deleteGWindow(const GWindow & gw) {
   ostringstream os;
   os << gw.gwd;
   string id = os.str();
   windowTable.remove(id);
   os.str("");
   os << "GWindow.delete(\"" << gw.gwd << "\")";
   putPipe(os.str());
}

void Platform::close(const GWindow & gw) {
   ostringstream os;
   os << "GWindow.close(\"" << gw.gwd << "\")";
   putPipe(os.str());
}

void Platform::requestFocus(const GWindow & gw) {
   ostringstream os;
   os << "GWindow.requestFocus(\"" << gw.gwd << "\")";
   putPipe(os.str());
}

void Platform::clear(const GWindow & gw) {
   ostringstream os;
   os << "GWindow.clear(\"" << gw.gwd << "\")";
   putPipe(os.str());
}

void Platform::repaint(const GWindow & gw) {
   ostringstream os;
   os << "GWindow.repaint(\"" << gw.gwd << "\")";
   putPipe(os.str());
}

void Platform::setVisible(const GWindow & gw, bool flag) {
   ostringstream os;
   os << boolalpha << "GWindow.setVisible(\"" << gw.gwd << "\", "
                   << flag << ")";
   putPipe(os.str());
}

void Platform::setWindowTitle(const GWindow & gw, string title) {
   ostringstream os;
   os << "GWindow.setTitle(\"" << gw.gwd << "\", ";
   writeQuotedString(os, title);
   os << ")";
   putPipe(os.str());
}

double Platform::getCanvasWidth(const GWindow & gw) {
  ostringstream os;
  os << "GWindow.getCanvasWidth(\"" << gw.gwd << "\")";
  putPipe(os.str());
  return stringToReal(getResult());
}

double Platform::getCanvasHeight(const GWindow & gw) {
  ostringstream os;
  os << "GWindow.getCanvasHeight(\"" << gw.gwd << "\")";
  putPipe(os.str());
  return stringToReal(getResult());
}

double Platform::getScreenWidth() {
   putPipe("GWindow.getScreenWidth()");
   return stringToReal(getResult());
}

double Platform::getScreenHeight() {
   putPipe("GWindow.getScreenHeight()");
   return stringToReal(getResult());
}

void Platform::pause(double milliseconds) {
   ostringstream os;
   os << "GTimer.pause(" << milliseconds << ")";
   putPipe(os.str());
   getStatus();
}

void Platform::createTimer(const GTimer & timer, double delay) {
   ostringstream os;
   os << timer.gtd;
   string id = os.str();
   timerTable.put(id, timer.gtd);
   os.str("");
   os << "GTimer.create(\"" << id << "\", " << delay << ")";
   putPipe(os.str());
}

void Platform::deleteTimer(const GTimer & timer) {
   ostringstream os;
   os << timer.gtd;
   string id = os.str();
   timerTable.remove(id);
   os.str("");
   os << "GTimer.deleteTimer(\"" << id << "\")";
   putPipe(os.str());
}

void Platform::startTimer(const GTimer & timer) {
   ostringstream os;
   os << "GTimer.startTimer(\"" << timer.gtd << "\")";
   putPipe(os.str());
}

void Platform::stopTimer(const GTimer & timer) {
   ostringstream os;
   os << "GTimer.stopTimer(\"" << timer.gtd << "\")";
   putPipe(os.str());
}

void Platform::createSound(Sound *sound, string filename) {
   ostringstream os;
   os << "Sound.create(\"" << sound << "\", \"" << filename << "\")";
   putPipe(os.str());
   getStatus();
}

void Platform::deleteSound(Sound *sound) {
   ostringstream os;
   os << "Sound.delete(\"" << sound << "\")";
   putPipe(os.str());
}

void Platform::playSound(Sound *sound) {
   ostringstream os;
   os << "Sound.play(\"" << sound << "\")";
   putPipe(os.str());
}

void Platform::deleteGObject(GObject *gobj) {
   ostringstream os;
   os << "GObject.delete(\"" << gobj << "\")";
   putPipe(os.str());
}

void Platform::add(GObject *compound, GObject *gobj) {
   ostringstream os;
   os << "GCompound.add(\"" << compound << "\", \"" << gobj << "\")";
   putPipe(os.str());
}

void Platform::remove(GObject *gobj) {
   ostringstream os;
   os << "GObject.remove(\"" << gobj << "\")";
   putPipe(os.str());
}

void Platform::setRegionAlignment(const GWindow & gw, string region,
                                                      string align) {
   ostringstream os;
   os << "GWindow.setRegionAlignment(\"" << gw.gwd << "\", \"" << region
                                         << "\", \"" << align << "\")";
   putPipe(os.str());
}

void Platform::addToRegion(const GWindow & gw, GObject *gobj, string region) {
   ostringstream os;
   os << "GWindow.addToRegion(\"" << gw.gwd << "\", \"" << gobj << "\", \""
                                   << region << "\")";
   putPipe(os.str());
}

void Platform::removeFromRegion(const GWindow & gw, GObject *gobj,
                                                    string region) {
   ostringstream os;
   os << "GWindow.removeFromRegion(\"" << gw.gwd << "\", \""
                                       << gobj << "\", \"" << region << "\")";
   putPipe(os.str());
}

void Platform::sendForward(GObject *gobj) {
   ostringstream os;
   os << "GObject.sendForward(\"" << gobj << "\")";
   putPipe(os.str());
}

void Platform::sendToFront(GObject *gobj) {
   ostringstream os;
   os << "GObject.sendToFront(\"" << gobj << "\")";
   putPipe(os.str());
}

void Platform::sendBackward(GObject *gobj) {
   ostringstream os;
   os << "GObject.sendBackward(\"" << gobj << "\")";
   putPipe(os.str());
}

void Platform::sendToBack(GObject *gobj) {
   ostringstream os;
   os << "GObject.sendToBack(\"" << gobj << "\")";
   putPipe(os.str());
}

void Platform::setVisible(GObject *gobj, bool flag) {
   ostringstream os;
   os << boolalpha << "GObject.setVisible(\"" << gobj << "\", " << flag << ")";
   putPipe(os.str());
}

void Platform::setColor(GObject *gobj, string color) {
   ostringstream os;
   os << "GObject.setColor(\"" << gobj << "\", \"" << color << "\")";
   putPipe(os.str());
}

void Platform::scale(GObject *gobj, double sx, double sy) {
   ostringstream os;
   os << "GObject.scale(\"" << gobj << "\", " << sx << ", " << sy << ")";
   putPipe(os.str());
}

void Platform::rotate(GObject *gobj, double theta) {
   ostringstream os;
   os << "GObject.rotate(\"" << gobj << "\", " << theta << ")";
   putPipe(os.str());
}

// Move this computation into gobjects.cpp
// (Done for GRect, GOval, GRoundRect, GLine, GPolygon, GImage, GCompound, GLabel by JL)
bool Platform::contains(const GObject *gobj, double x, double y) {
   ostringstream os;
   os << "GObject.contains(\"" << gobj << "\", " << x << ", " << y << ")";
   putPipe(os.str());
   return getResult() == "true";
}

// Move this computation into gobjects.cpp
// (Done for GRect, GOval, GRoundRect, GLine, GPolygon, GImage, GCompound, GLabel by JL)
GRectangle Platform::getBounds(const GObject *gobj) {
   ostringstream os;
   os << "GObject.getBounds(\"" << gobj << "\")";
   putPipe(os.str());
   string result = getResult();
   if (!startsWith(result, "GRectangle(")) error(std::string("getBounds: unexpected result: ") + result);
   return scanRectangle(result);
}

void Platform::setLineWidth(GObject *gobj, double lineWidth) {
   ostringstream os;
   os << "GObject.setLineWidth(\"" << gobj << "\", " << lineWidth << ")";
   putPipe(os.str());
}

void Platform::setLocation(GObject *gobj, double x, double y) {
   ostringstream os;
   os << "GObject.setLocation(\"" << gobj << "\", " << x << ", " << y << ")";
   putPipe(os.str());
}

void Platform::setSize(GObject *gobj, double width, double height) {
   ostringstream os;
   os << "GObject.setSize(\"" << gobj << "\", " << width << ", "
                                                 << height << ")";
   putPipe(os.str());
}

void Platform::setFrameRectangle(GObject *gobj, double x, double y,
                                                double width, double height) {
   ostringstream os;
   os << "GArc.setFrameRectangle(\"" << gobj << "\", "
                                      << x << ", " << y << ", "
                                      << width << ", " << height << ")";
   putPipe(os.str());
}

void Platform::draw(const GWindow & gw, const GObject *gobj) {
   ostringstream os;
   os << "GWindow.draw(\"" << gw.gwd << "\", \"" << gobj << "\")";
   putPipe(os.str());
}

void Platform::setFilled(GObject *gobj, bool flag) {
   ostringstream os;
   os << boolalpha << "GObject.setFilled(\"" << gobj << "\", " << flag << ")";
   putPipe(os.str());
}

void Platform::setFillColor(GObject *gobj, string color) {
   ostringstream os;
   os << "GObject.setFillColor(\"" << gobj << "\", \"" << color << "\")";
   putPipe(os.str());
}

void Platform::createGRect(GObject *gobj, double width, double height) {
   ostringstream os;
   os << "GRect.create(\"" << gobj << "\", " << width << ", "
                                              << height << ")";
   putPipe(os.str());
}

void Platform::createGRoundRect(GObject *gobj, double width, double height,
                                               double corner) {
   ostringstream os;
   os << "GRoundRect.create(\"" << gobj << "\", " << width << ", " << height
                                         << ", " << corner << ")";
   putPipe(os.str());
}

void Platform::createG3DRect(GObject *gobj, double width, double height,
                                            bool raised) {
   ostringstream os;
   os << boolalpha << "G3DRect.create(\"" << gobj << "\", "
                    << width << ", " << height << ", " << raised << ")";
   putPipe(os.str());
}

void Platform::setRaised(GObject *gobj, bool raised) {
   ostringstream os;
   os << boolalpha << "G3DRect.setRaised(\"" << gobj << "\", "
                                             << raised << ")";
   putPipe(os.str());
}

void Platform::createGLabel(GObject *gobj, string label) {
   ostringstream os;
   os << "GLabel.create(\"" << gobj << "\", \"" << label << "\")";
   putPipe(os.str());
}

void Platform::createGLine(GObject *gobj, double x1, double y1,
                                          double x2, double y2) {
   ostringstream os;
   os << "GLine.create(\"" << gobj << "\", " << x1 << ", " << y1
                                    << ", " << x2 << ", " << y2 << ")";
   putPipe(os.str());
}

void Platform::setStartPoint(GObject *gobj, double x, double y) {
   ostringstream os;
   os << "GLine.setStartPoint(\"" << gobj << "\", " << x << ", " << y << ")";
   putPipe(os.str());
}

void Platform::setEndPoint(GObject *gobj, double x, double y) {
   ostringstream os;
   os << "GLine.setEndPoint(\"" << gobj << "\", " << x << ", " << y << ")";
   putPipe(os.str());
}

void Platform::createGArc(GObject *gobj, double width, double height,
                                         double start, double sweep) {
   ostringstream os;
   os << "GArc.create(\"" << gobj << "\", " << width << ", " << height
                                   << ", " << start << ", " << sweep << ")";
   putPipe(os.str());
}

void Platform::setStartAngle(GObject *gobj, double angle) {
   ostringstream os;
   os << "GArc.setStartAngle(\"" << gobj << "\", " << angle << ")";
   putPipe(os.str());
}

void Platform::setSweepAngle(GObject *gobj, double angle) {
   ostringstream os;
   os << "GArc.setSweepAngle(\"" << gobj << "\", " << angle << ")";
   putPipe(os.str());
}

GDimension Platform::createGImage(GObject *gobj, string filename) {
   ostringstream os;
   os << "GImage.create(\"" << gobj << "\", \"" << filename << "\")";
   putPipe(os.str());
   string result = getResult();
   if (!startsWith(result, "GDimension(")) error(std::string("createGImage: unexpected result: ") + result);
   return scanDimension(result);
}

void Platform::createGPolygon(GObject *gobj) {
   ostringstream os;
   os << "GPolygon.create(\"" << gobj << "\")";
   putPipe(os.str());
}

void Platform::addVertex(GObject *gobj, double x, double y) {
   ostringstream os;
   os << "GPolygon.addVertex(\"" << gobj << "\", " << x << ", " << y << ")";
   putPipe(os.str());
}

void Platform::createGOval(GObject *gobj, double width, double height) {
   ostringstream os;
   os << "GOval.create(\"" << gobj << "\", " << width << ", "
                                              << height << ")";
   putPipe(os.str());
}

void Platform::setActionCommand(GObject *gobj, string cmd) {
   ostringstream os;
   os << "GInteractor.setActionCommand(\"" << gobj << "\", ";
   writeQuotedString(os, cmd);
   os << ")";
   putPipe(os.str());
}

GDimension Platform::getSize(GObject *gobj) {
   ostringstream os;
   os << "GInteractor.getSize(\"" << gobj << "\")";
   putPipe(os.str());
   return scanDimension(getResult());
}

void Platform::createGButton(GObject *gobj, string label) {
   ostringstream os;
   os << gobj;
   sourceTable.put(os.str(), gobj);
   os.str("");
   os << "GButton.create(\"" << gobj << "\", ";
   writeQuotedString(os, label);
   os << ")";
   putPipe(os.str());
}

void Platform::gbutton_setEnabled(GObject *gobj, bool enabled) {
   ostringstream os;
   os << boolalpha << "GButton.setEnabled(\"" << gobj << "\", "
                                              << enabled << ")";
   putPipe(os.str());
}

void Platform::createGCheckBox(GObject *gobj, string label) {
   ostringstream os;
   os << gobj;
   sourceTable.put(os.str(), gobj);
   os.str("");
   os << "GCheckBox.create(\"" << gobj << "\", ";
   writeQuotedString(os, label);
   os << ")";
   putPipe(os.str());
}

bool Platform::isSelected(GObject *gobj) {
   ostringstream os;
   os << "GCheckBox.isSelected(\"" << gobj << "\")";
   putPipe(os.str());
   return getResult() == "true";
}

void Platform::setSelected(GObject *gobj, bool state) {
   ostringstream os;
   os << boolalpha << "GCheckBox.setSelected(\"" << gobj << "\", "
                                                  << state << ")";
   putPipe(os.str());
}

void Platform::createGSlider(GObject *gobj, int min, int max, int value) {
   ostringstream os;
   os << gobj;
   sourceTable.put(os.str(), gobj);
   os.str("");
   os << "GSlider.create(\"" << gobj << "\", " << min << ", " << max
                                      << ", " << value << ")";
   putPipe(os.str());
}

int Platform::getValue(GObject *gobj) {
   ostringstream os;
   os << "GSlider.getValue(\"" << gobj << "\")";
   putPipe(os.str());
   return stringToInteger(getResult());
}

void Platform::setValue(GObject *gobj, int value) {
   ostringstream os;
   os << "GSlider.setValue(\"" << gobj << "\", " << value << ")";
   putPipe(os.str());
}

void Platform::createGTextField(GObject *gobj, int nChars) {
   ostringstream os;
   os << gobj;
   sourceTable.put(os.str(), gobj);
   os.str("");
   os << "GTextField.create(\"" << gobj << "\", " << nChars << ")";
   putPipe(os.str());
}

string Platform::getText(GObject *gobj) {
   ostringstream os;
   os << "GTextField.getText(\"" << gobj << "\")";
   putPipe(os.str());
   return getResult();
}

void Platform::setText(GObject *gobj, string str) {
   ostringstream os;
   os << "GTextField.setText(\"" << gobj << "\", ";
   writeQuotedString(os, str);
   os << ")";
   putPipe(os.str());
}

void Platform::createGChooser(GObject *gobj) {
   ostringstream os;
   os << gobj;
   sourceTable.put(os.str(), gobj);
   os.str("");
   os << "GChooser.create(\"" << gobj << "\")";
   putPipe(os.str());
}

void Platform::addItem(GObject *gobj, string item) {
   ostringstream os;
   os << "GChooser.addItem(\"" << gobj << "\", ";
   writeQuotedString(os, item);
   os << ")";
   putPipe(os.str());
}

string Platform::getSelectedItem(GObject *gobj) {
   ostringstream os;
   os << "GChooser.getSelectedItem(\"" << gobj << "\")";
   putPipe(os.str());
   return getResult();
}

void Platform::setSelectedItem(GObject *gobj, string item) {
   ostringstream os;
   os << "GChooser.setSelectedItem(\"" << gobj << "\", ";
   writeQuotedString(os, item);
   os << ")";
   putPipe(os.str());
}

void Platform::createGCompound(GObject *gobj) {
   ostringstream os;
   os << "GCompound.create(\"" << gobj << "\")";
   putPipe(os.str());
}

void Platform::setFont(GObject *gobj, string font) {
   ostringstream os;
   os << "GLabel.setFont(\"" << gobj << "\", \"" << font << "\")";
   putPipe(os.str());
}

void Platform::setLabel(GObject *gobj, string str) {
   ostringstream os;
   os << "GLabel.setLabel(\"" << gobj << "\", ";
   writeQuotedString(os, str);
   os << ")";
   putPipe(os.str());
}

double Platform::getFontAscent(const GObject *gobj) {
   ostringstream os;
   os << "GLabel.getFontAscent(\"" << gobj << "\")";
   putPipe(os.str());
   return stringToReal(getResult());
}

double Platform::getFontDescent(const GObject *gobj) {
   ostringstream os;
   os << "GLabel.getFontDescent(\"" << gobj << "\")";
   putPipe(os.str());
   return stringToReal(getResult());
}

GDimension Platform::getGLabelSize(const GObject *gobj) {
   ostringstream os;
   os << "GLabel.getGLabelSize(\"" << gobj << "\")";
   putPipe(os.str());
   return scanDimension(getResult());
}


void Platform::gbufferedimage_constructor(GObject* gobj, double x, double y,
                                          double width, double height, int rgb) {
    std::ostringstream os;
    os << gobj;
    sourceTable.put(os.str(), gobj);
    os.str("");
    os << "GBufferedImage.create(\"" << gobj << "\", " << (int) x << ", "
       << (int) y << ", " << (int) width << ", " << (int) height << ", " << rgb << ")";
    putPipe(os.str());
}

void Platform::gbufferedimage_fill(GObject* gobj, int rgb) {
    std::ostringstream os;
    os << "GBufferedImage.fill(\"" << gobj << "\", " << rgb << ")";
    putPipe(os.str());
}

void Platform::gbufferedimage_fillRegion(GObject* gobj, double x, double y, double width, double height, int rgb) {
    std::ostringstream os;
    os << "GBufferedImage.fillRegion(\"" << gobj << "\", " << (int) x << ", "
       << (int) y << ", " << (int) width << ", " << (int) height << ", " << rgb << ")";   // BUGBUG: was missing ", " token
    putPipe(os.str());
}

std::string Platform::gbufferedimage_load(GObject* gobj, const std::string& filename) {
    std::ostringstream os;
    os << "GBufferedImage.load(\"" << gobj << "\", ";
    writeQuotedString(os, filename);
    os << ")";
    putPipe(os.str());
    return getResult();
}

void Platform::gbufferedimage_resize(GObject* gobj, double width, double height, bool retain) {
    std::ostringstream os;
    os << "GBufferedImage.resize(\"" << gobj << "\", " << (int) width << ", " << (int) height
       << ", " << std::boolalpha << retain << ")";
    putPipe(os.str());
}

void Platform::gbufferedimage_save(const GObject* const gobj, const std::string& filename) {
    std::ostringstream os;
    os << "GBufferedImage.save(\"" << gobj << "\", ";
    writeQuotedString(os, filename);
    os << ")";
    putPipe(os.str());
    getStatus();
}

std::string Platform::gbufferedimage_scale(const GObject* const gobj, GObject* const gobj2, int w, int h) {
    std::ostringstream os;
    os << "GBufferedImage.scale(\"" << gobj << "\", \"" << gobj2 << "\", "
       << w << ", " << h << ")";
    putPipe(os.str());
    return getResult();
}

void Platform::gbufferedimage_setRGB(GObject* gobj, double x, double y,
                                     int rgb) {
    std::ostringstream os;
    os << "GBufferedImage.setRGB(\"" << gobj << "\", " << (int) x << ", "
       << (int) y << ", " << rgb << ")";
    putPipe(os.str());
}

int Platform::goptionpane_showConfirmDialog(std::string message, std::string title, int type, GWindow *parent) {
    std::ostringstream os;
    os << "GOptionPane.showConfirmDialog(";
    writeQuotedString(os, message);
    os << ",";
    writeQuotedString(os, title);
    os << "," << type;
    if (parent == NULL)
        os << "," << "\"\""; // empty string
    else
        os << "," << "\"" << parent->gwd << "\"";
    os << ")";
    putPipe(os.str());
    return stringToInteger(getResult());
}

std::string Platform::goptionpane_showInputDialog(std::string message, std::string title, GWindow *parent) {
    std::ostringstream os;
    os << "GOptionPane.showInputDialog(";
    writeQuotedString(os, message);
    os << ",";
    writeQuotedString(os, title);
    if (parent == NULL)
        os << "," << "\"\""; // empty string
    else
        os << "," << "\"" << parent->gwd << "\"";
    os << ")";
    putPipe(os.str());
    return getResult();
}

void Platform::goptionpane_showMessageDialog(std::string message, std::string title, int type, GWindow *parent) {
    std::ostringstream os;
    os << "GOptionPane.showMessageDialog(";
    writeQuotedString(os, message);
    os << ",";
    writeQuotedString(os, title);
    os << "," << type;
    if (parent == NULL)
        os << "," << "\"\""; // empty string
    else
        os << "," << "\"" << parent->gwd << "\"";
    os << ")";
    putPipe(os.str());
    getResult();   // wait for dialog to close
}

int Platform::goptionpane_showOptionDialog(std::string message,
                                           std::string title,
                                           const Vector<std::string>& options,
                                           std::string initiallySelected,
                                           GWindow *parent) {
    std::ostringstream os;
    os << "GOptionPane.showOptionDialog(";
    writeQuotedString(os, message);
    os << ",";
    writeQuotedString(os, title);
    os << ",";
    os << "{";
    if (!options.isEmpty()) {
        writeQuotedString(os, options[0]);
        for (int i = 1, sz = options.size(); i < sz; ++i) {
            os << ", ";
            writeQuotedString(os, options[i]);
        }
    }
    os << "}";
    os << ",";
    writeQuotedString(os, initiallySelected);
    if (parent == NULL)
        os << "," << "\"\""; // empty string
    else
        os << "," << "\"" << parent->gwd << "\"";
    os << ")";
    putPipe(os.str());
    std::string result = getResult();
    return stringToInteger(result);
}

void Platform::gtextarea_create(GObject* gobj, double width, double height) {
    ostringstream os;
    os << "GTextArea.create(\"" << gobj << "\", " << width << ", "
                                               << height << ")";
    putPipe(os.str());
}

void Platform::gtextarea_setText(GObject* gobj, std::string text) {
    ostringstream os;
    os << "GTextArea.setText(\"" << gobj << "\", ";
    writeQuotedString(os, text);
    os << ")";
    putPipe(os.str());
}

std::string Platform::gtextarea_getText(const GObject* gobj) {
    ostringstream os;
    os << "GTextArea.getText(\"" << gobj << "\")";
    putPipe(os.str());
    return getResult();
}

void Platform::gtextarea_setFont(GObject* gobj, std::string font) {
    ostringstream os;
    os << "GTextArea.setFont(\"" << gobj << "\", ";
    writeQuotedString(os, font);
    os << ")";
    putPipe(os.str());
}

void Platform::gtextarea_setEditable(GObject* gobj, bool isEditable) {
    ostringstream os;
    os << boolalpha << "GTextArea.setEditable(\"" << gobj << "\", "
                                                         << isEditable << ")";
    putPipe(os.str());
}

void Platform::gtextarea_setBackgroundColor(GObject* gobj, std::string rgb) {
    ostringstream os;
    os << "GTextArea.setBackgroundColor(\"" << gobj << "\", ";
    writeQuotedString(os, rgb);
    os << ")";
    putPipe(os.str());
}

GEvent Platform::getNextEvent(int mask) {
   cout.flush();
   if (eventQueue.isEmpty()) {
      putPipe("GEvent.getNextEvent(" + integerToString(mask) + ")");
      getResult(false);
      if (eventQueue.isEmpty()) return GEvent();
   }
   return eventQueue.dequeue();
}

GEvent Platform::waitForEvent(int mask) {
   cout.flush();
   while (eventQueue.isEmpty()) {
      putPipe("GEvent.waitForEvent(" + integerToString(mask) + ")");
      getResult(false);
   }
   return eventQueue.dequeue();
}

bool Platform::isBlockedForConsoleIO() {
    return cinout_buf && cinout_buf->isBlocked();
}

void Platform::exitGraphics() {
  if (isBlockedForConsoleIO()) {
      // graphical console is blocked waiting for an I/O read;
      // won't be able to exit graphics in the JBE anyway; just kill the JBE and exit
#ifdef _WIN32
      // A bit harsh, but seems to work.
      TerminateProcess(pInfo.hProcess, 0);
#else
      kill(child, SIGTERM);
#endif
      exit(0);
  } else {
      putPipe("GWindow.exitGraphics()");
      exit(0);
  }
}

Platform *getPlatform() {
   static Platform gp;
   return &gp;
}

#ifdef _WIN32

/* Windows implementation of interface to Java back end */

int startupMain(int argc, char **argv) {
   extern int Main();
   extern int argcMain;
   extern char **argvMain;
   argcMain = argc;
   argvMain = argv;
   exceptions::setProgramNameForStackTrace(argv[0]);
   programName = getRoot(getTail(argv[0]));
   initPipe();
   cinout_buf = new ConsoleStreambuf();
   cin.rdbuf(cinout_buf);
   cout.rdbuf(cinout_buf);
   cerr.rdbuf(new ForwardingStreambuf(*cinout_buf, true));
   //ShowWindow(GetConsoleWindow(), SW_HIDE);
   //ShowWindow(GetConsoleWindow(), SW_SHOW);
   setConsoleProperties();
   return Main();
}

static void initPipe() {
   SECURITY_ATTRIBUTES attr;
   attr.nLength = sizeof(SECURITY_ATTRIBUTES);
   attr.bInheritHandle = true;
   attr.lpSecurityDescriptor = NULL;
   if (!CreatePipe(&rdFromJBE, &wrFromJBE, &attr, 0)) {
      error("Can't create fromJBE");
   }
   if (!SetHandleInformation(rdFromJBE, HANDLE_FLAG_INHERIT, 0)) {
      error("SetHandleInformation failed for fromJBE");
   }
   if (!CreatePipe(&rdToJBE, &wrToJBE, &attr, 0)) {
      error("Can't create toJBE");
   }
   if (!SetHandleInformation(wrToJBE, HANDLE_FLAG_INHERIT, 0)) {
      error("SetHandleInformation failed for toJBE");
   }
   string cmd = "java";
   if (tracePipe) {
    cmd += " -Dstanfordspl.debug=true";
   }
   cmd += " -jar spl.jar ";
   cmd += programName;
   int n = cmd.length();
   char *cmdLine = new char[n + 1];
   for (int i = 0; i < n; i++) {
      cmdLine[i] = cmd[i];
   }
   cmdLine[n] = '\0';
   //PROCESS_INFORMATION pInfo;
   memset(&pInfo, 0, sizeof(PROCESS_INFORMATION));
   STARTUPINFOA sInfo;
   memset(&sInfo, 0, sizeof(STARTUPINFOA));
   sInfo.cb = sizeof(STARTUPINFOA);
   sInfo.dwFlags = STARTF_USESTDHANDLES;
   sInfo.hStdInput = rdToJBE;
   sInfo.hStdOutput = wrFromJBE;
   sInfo.hStdError = wrFromJBE;
   int ok = CreateProcessA(NULL, cmdLine, NULL, NULL, true, 0,
                           NULL, NULL, &sInfo, &pInfo);
   if (!ok) {
      DWORD err = GetLastError();
      string msg = "Error creating java process for back end: error code ";
      msg += strerror(err);
      error(msg);
      //cerr << err << endl;
   } else {
      // We need a handle to the process so we can kill it if necessary
      //CloseHandle(pInfo.hProcess);
      CloseHandle(pInfo.hThread);
   }

   char *trace = getenv("JBETRACE");
   tracePipe = trace != NULL && startsWith(toLowerCase(trace), "t");
   if (tracePipe)
    logfile.open("JBElog.txt");
}

static void putPipe(string line) {
   DWORD nch;
   if (!WriteFile(wrToJBE, line.c_str(), line.length(), &nch, NULL))
     error("Could not write to JBE");
   if (!WriteFile(wrToJBE, "\n", 1, &nch, NULL))
     error("Could not write to JBE");
   if (tracePipe) {
       logfile << "--> " << line << endl; // JL
   }
   //FlushFileBuffers(wrToJBE);


}

static string getPipe() {
   string line = "";
   DWORD nch;
   while (true) {
      char ch;
      ReadFile(rdFromJBE, &ch, 1, &nch, NULL);
      if (ch == '\n' || ch == '\r') break;
      line += ch;
   }
   if (tracePipe) {
       logfile << "<-- " << line << endl; // JL
   }
   return line;
}

#else

/* Linux/Mac implementation of interface to Java back end */

static void scanOptions() {
   char *home = getenv("HOME");
   if (home != NULL) {
      string filename = string() + home + "/.spl";
      ifstream infile(filename.c_str());
      if (!infile.fail()) {
         string line;
         while (getline(infile, line)) {
            int equals = line.find('=');
            if (equals != string::npos) {
               string key = line.substr(0, equals);
               string value = line.substr(equals + 1);
               optionTable.put(key, value);
            }
         }
         infile.close();
      }
   }
}

static string getOption(string key) {
   char *str = getenv(key.c_str());
   if (str != NULL) return string(str);
   return optionTable.get(key);
}

int startupMain(int argc, char **argv) {
   extern int Main();
   extern int argcMain;
   extern char **argvMain;
   argcMain = argc;
   argvMain = argv;
   string arg0 = argv[0];
   exceptions::setProgramNameForStackTrace(argv[0]);
   programName = getRoot(getTail(arg0));
   size_t ax = arg0.find(".app/Contents/");
   if (ax != string::npos) {
      while (ax > 0 && arg0[ax] != '/') {
         ax--;
      }
      if (ax > 0) {
         string cwd = arg0.substr(0, ax);
         chdir(cwd.c_str());
      }
   }
   char *noConsoleFlag = getenv("NOCONSOLE");
   if (noConsoleFlag != NULL && startsWith(string(noConsoleFlag), "t")) {
      return Main();
   }
   scanOptions();
   initPipe();
   cinout_buf = new ConsoleStreambuf();
   cin.rdbuf(cinout_buf);
   cout.rdbuf(cinout_buf);
   cerr.rdbuf(new ForwardingStreambuf(*cinout_buf, true));
   string font = getOption("CPPFONT");
   if (font != "")
       setConsoleFont(font);
   setConsoleProperties();
   return Main();
}

static void initPipe() {
   char *trace = getenv("JBETRACE");
   logfile.open("/dev/tty");
   tracePipe = trace != NULL && startsWith(toLowerCase(trace), "t");
   int toJBE[2], fromJBE[2];
   pipe(toJBE);
   pipe(fromJBE);
   child = fork();
   if (child == 0) {
      dup2(toJBE[0], 0);
      close(toJBE[0]);
      close(toJBE[1]);
      dup2(fromJBE[1], 1);
      close(fromJBE[0]);
      close(fromJBE[1]);
   if (tracePipe) {
#ifdef __APPLE__
      string option = "-Xdock:name=" + programName;
      string debugger = "-Xdebug";
      string debugger2 = "-Xrunjdwp:transport=dt_socket,address=8888,server=y,suspend=n";
      execlp("java", "java", option.c_str(), debugger.c_str(), debugger2.c_str(), "-Dstanfordspl.debug=true",
             "-jar", "spl.jar",
              programName.c_str(), NULL);
#else
      execlp("java", "java", "-Dstanfordspl.debug=true",
             "-jar", "spl.jar", programName.c_str(), NULL);
#endif
   } else {
#ifdef __APPLE__
      string option = "-Xdock:name=" + programName;
      execlp("java", "java", option.c_str(), "-jar", "spl.jar",
             programName.c_str(), NULL);
#else
      execlp("java", "java", "-jar", "spl.jar", programName.c_str(), NULL);
#endif
   }
      throw ErrorException("Could not exec spl.jar"); // BUGFIX (JL) was `throw new Error...`
   } else {
      pin = fromJBE[0];
      pout = toJBE[1];
      close(fromJBE[1]);
      close(toJBE[0]);
   }
}

static void putPipe(string line) {
   write(pout, line.c_str(), line.length());
   write(pout, "\n", 1);
   if (tracePipe) logfile << "-> " << line << endl;
}

static string getPipe() {
   string line = "";
   while (true) {
      char ch;
      read(pin, &ch, 1);
      if (ch == '\n') break;
      line += ch;
   }
   if (tracePipe) logfile << "<- " << line << endl;
   return line;
}

#endif

static string getResult(bool consumeAcks) {
   while (true) {
      string line = getPipe();
      if (startsWith(line, "result:")) {
          if (!(consumeAcks && line == "result:___jbe___ack___")) {
              // this is just an acknowledgment of some previous event;
              // keep waiting for a real result
              return line.substr(7);
          }
      }
      if (startsWith(line, "event:")) {
         eventQueue.enqueue(parseEvent(line.substr(6)));
      }
      if (startsWith(line, "error:")) {
          error(string("Java backend: ") + line.substr(6));
      }
   }
}

static void getStatus() {
   string result = getResult();
   if (result != "ok") error(result);
}

static GEvent parseEvent(string line) {
   TokenScanner scanner(line);
   scanner.ignoreWhitespace();
   scanner.scanNumbers();
   scanner.scanStrings();
   string name = scanner.nextToken();
   if (name == "mousePressed") {
      return parseMouseEvent(scanner, MOUSE_PRESSED);
   } else if (name == "mouseReleased") {
      return parseMouseEvent(scanner, MOUSE_RELEASED);
   } else if (name == "mouseClicked") {
      return parseMouseEvent(scanner, MOUSE_CLICKED);
   } else if (name == "mouseMoved") {
      return parseMouseEvent(scanner, MOUSE_MOVED);
   } else if (name == "mouseDragged") {
      return parseMouseEvent(scanner, MOUSE_DRAGGED);
   } else if (name == "keyPressed") {
      return parseKeyEvent(scanner, KEY_PRESSED);
   } else if (name == "keyReleased") {
      return parseKeyEvent(scanner, KEY_RELEASED);
   } else if (name == "keyTyped") {
      return parseKeyEvent(scanner, KEY_TYPED);
   } else if (name == "actionPerformed") {
      return parseActionEvent(scanner, ACTION_PERFORMED);
   } else if (name == "timerTicked") {
      return parseTimerEvent(scanner, TIMER_TICKED);
   } else if (name == "windowClosed") {
      GWindowEvent e = parseWindowEvent(scanner, WINDOW_CLOSED);
      e.getGWindow().close();
      return e;
   } else if (name == "windowResized") {
      return parseWindowEvent(scanner, WINDOW_RESIZED);
   } else if (name == "lastWindowClosed") {
      exit(0);
   } else if (name == "consoleWindowClosed") {
     // Java console window was closed; possibly exit the C++ program now
     extern bool getConsoleExitProgramOnClose();
     if (getConsoleExitProgramOnClose()) {
         extern streambuf *saveerr;
         cerr.rdbuf(saveerr);
         cerr << endl;
         cerr << "***" << endl;
         cerr << "*** STANFORD C++ LIBRARY (WHITTIER VERSION)" << endl;
         cerr << "*** Prematurely exiting program because console window was closed." << endl;
         cerr << "***" << endl;
         cerr << endl;

         // close any other graphical windows and exit program
         exitGraphics();
     }
   } else {
      /* Ignore for now */
   }
   return GEvent();
}

static GEvent parseMouseEvent(TokenScanner & scanner, EventType type) {
   scanner.verifyToken("(");
   string id = scanner.getStringValue(scanner.nextToken());
   scanner.verifyToken(",");
   double time = scanDouble(scanner);
   scanner.verifyToken(",");
   int modifiers = scanInt(scanner);
   scanner.verifyToken(",");
   double x = scanDouble(scanner);
   scanner.verifyToken(",");
   double y = scanDouble(scanner);
   scanner.verifyToken(")");
   GMouseEvent e(type, GWindow(windowTable.get(id)), x, y);
   e.setEventTime(time);
   e.setModifiers(modifiers);
   return e;
}

static GEvent parseKeyEvent(TokenScanner & scanner, EventType type) {
   scanner.verifyToken("(");
   string id = scanner.getStringValue(scanner.nextToken());
   scanner.verifyToken(",");
   double time = scanDouble(scanner);
   scanner.verifyToken(",");
   int modifiers = scanInt(scanner);
   scanner.verifyToken(",");
   int keyChar = scanInt(scanner);
   scanner.verifyToken(",");
   int keyCode = scanInt(scanner);
   scanner.verifyToken(")");
   GKeyEvent e(type, GWindow(windowTable.get(id)), char(keyChar), keyCode);
   e.setEventTime(time);
   e.setModifiers(modifiers);
   return e;
}

static GEvent parseTimerEvent(TokenScanner & scanner, EventType type) {
   scanner.verifyToken("(");
   string id = scanner.getStringValue(scanner.nextToken());
   scanner.verifyToken(",");
   double time = scanDouble(scanner);
   scanner.verifyToken(")");
   GTimerEvent e(type, GTimer(timerTable.get(id)));
   e.setEventTime(time);
   return e;
}

static GEvent parseWindowEvent(TokenScanner & scanner, EventType type) {
   scanner.verifyToken("(");
   string id = scanner.getStringValue(scanner.nextToken());
   scanner.verifyToken(",");
   double time = scanDouble(scanner);
   scanner.verifyToken(")");
   GWindowEvent e(type, GWindow(windowTable.get(id)));
   e.setEventTime(time);
   return e;
}

static GEvent parseActionEvent(TokenScanner & scanner, EventType type) {
   scanner.verifyToken("(");
   string id = scanner.getStringValue(scanner.nextToken());
   scanner.verifyToken(",");
   string action = scanner.getStringValue(scanner.nextToken());
   scanner.verifyToken(",");
   double time = scanDouble(scanner);
   scanner.verifyToken(")");
   GActionEvent e(type, sourceTable.get(id), action);
   e.setEventTime(time);
   return e;
}

/* Console code */

void Platform::clearConsole() {
   putPipe("JBEConsole.clear()");
}

void Platform::setConsoleFont(const string & font) {
   ostringstream os;
   os << "JBEConsole.setFont(\"" << font << "\")";
   putPipe(os.str());
}

void Platform::setConsoleLocation(int x, int y) {
   ostringstream os;
   os << "JBEConsole.setLocation(" << x << ", " << y << ")";
   putPipe(os.str());
}

void Platform::setConsoleSize(double width, double height) {
   ostringstream os;
   os << "JBEConsole.setSize(" << width << ", " << height << ")";
   putPipe(os.str());
}

static string getLineConsole() {
   putPipe("JBEConsole.getLine()");
   return getResult(true);
}

static void putConsole(const string & str, bool isStdErr) {
   ostringstream os;

   os << "JBEConsole.print(";
   // BUGFIX (Stepp): strings that end with \\ don't print because of back-end error;
   //         kludge fix by appending an invisible space after it
   if (!str.empty() && str[str.length() - 1] == '\\') {
       std::string str2 = str + ' ';
       writeQuotedString(os, str2);
   } else {
       writeQuotedString(os, str);
   }

   os << "," << boolalpha << isStdErr << ")";
   putPipe(os.str());
}

static void endLineConsole() {
   putPipe("JBEConsole.println()");
}

static int scanInt(TokenScanner & scanner) {
   string token = scanner.nextToken();
   if (token == "-") token += scanner.nextToken();
   return stringToInteger(token);
}

static double scanDouble(TokenScanner & scanner) {
   string token = scanner.nextToken();
   if (token == "-") token += scanner.nextToken();
   return stringToReal(token);
}

static GDimension scanDimension(const string & str) {
   TokenScanner scanner(str);
   scanner.scanNumbers();
   scanner.ignoreWhitespace();
   scanner.verifyToken("GDimension");
   scanner.verifyToken("(");
   double width = scanDouble(scanner);
   scanner.verifyToken(",");
   double height = scanDouble(scanner);
   scanner.verifyToken(")");
   return GDimension(width, height);
}

static GRectangle scanRectangle(const string & str) {
   TokenScanner scanner(str);
   scanner.scanNumbers();
   scanner.ignoreWhitespace();
   scanner.verifyToken("GRectangle");
   scanner.verifyToken("(");
   double x = scanDouble(scanner);
   scanner.verifyToken(",");
   double y = scanDouble(scanner);
   scanner.verifyToken(",");
   double width = scanDouble(scanner);
   scanner.verifyToken(",");
   double height = scanDouble(scanner);
   scanner.verifyToken(")");
   return GRectangle(x, y, width, height);
}

/*
 * Sets up console settings like window size, location, exit-on-close, etc.
 * based on compiler options (which may be set in the .pro file).
 */
static void setConsoleProperties() {
#if defined(SPL_CONSOLE_FONTSIZE)
    std::string fontStr = std::string("Monospaced-") + integerToString(SPL_CONSOLE_FONTSIZE);
    setConsoleFont(fontStr);
#endif

#if defined(SPL_CONSOLE_WIDTH) && defined(SPL_CONSOLE_HEIGHT)
    setConsoleSize(SPL_CONSOLE_WIDTH, SPL_CONSOLE_HEIGHT);
#endif

#if defined(SPL_CONSOLE_X) && defined(SPL_CONSOLE_Y)
    setConsoleLocation(SPL_CONSOLE_X, SPL_CONSOLE_Y);
#endif

#if defined(SPL_CONSOLE_PRINT_EXCEPTIONS)
    setConsolePrintExceptions(true);
#endif

#if defined(SPL_CONSOLE_EXIT_ON_CLOSE)
    setConsoleExitProgramOnClose(true);
#endif
}
