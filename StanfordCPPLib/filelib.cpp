/*
 * File: filelib.cpp
 * -----------------
 * This file implements the filelib.h interface.  All platform dependencies
 * are managed through the platform interface.
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
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "filelib.h"
#include "platform.h"
#include "strlib.h"
#include "vector.h"
using namespace std;

static Platform *pp = getPlatform();

/* Prototypes */

static void splitPath(string path, Vector<string> &list); // BUGFIX (JL): added '&'
static bool recursiveMatch(string str, int sx, string pattern, int px);

/* Implementations */

bool openFile(ifstream & stream, string filename) {
   stream.clear();
   stream.open(expandPathname(filename).c_str());
   return !stream.fail();
}

bool openFile(ofstream & stream, string filename) {
   stream.clear();
   stream.open(expandPathname(filename).c_str());
   return !stream.fail();
}

string promptUserForFile(ifstream & stream, string prompt) {
   while (true) {
      cout << prompt;
      string filename;
      getline(cin, filename);
      openFile(stream, filename);
      if (!stream.fail()) return filename;
      stream.clear();
      cout << "Unable to open that file.  Try again." << endl;
      if (prompt == "") prompt = "Input file: ";
   }
}

string promptUserForFile(ofstream & stream, string prompt) {
   while (true) {
      cout << prompt;
      string filename;
      getline(cin, filename);
      openFile(stream, filename);
      if (!stream.fail()) return filename;
      stream.clear();
      cout << "Unable to open that file.  Try again." << endl;
      if (prompt == "") prompt = "Output file: ";
   }
}

string openFileDialog() {
   return openFileDialog("Open File", "");
}

string openFileDialog(string title) {
   return openFileDialog(title, "");
}

string openFileDialog(string title, string path, string patternName) {
   return pp->openFileDialog(title, "load", expandPathname(path), patternName);
}

string saveFileDialog() {
   return saveFileDialog("Save file", "");
}

string saveFileDialog(string title) {
   return saveFileDialog(title, "");
}

string saveFileDialog(string title, string path, string patternName) {
   return pp->openFileDialog(title, "save", expandPathname(path), patternName);
}

void readEntireFile(istream & is, Vector<string> & lines) {
   lines.clear();
   while (true) {
      string line;
      getline(is, line);
      if (is.fail()) break;
      lines.add(line);
   }
}

void readEntireFile(istream & is, vector<string> & lines) {
   lines.clear();
   while (true) {
      string line;
      getline(is, line);
      if (is.fail()) break;
      lines.push_back(line);
   }
}

string readEntireFile(string filename) {
    string out;
    if (readEntireFile(filename, out)) {
        return out;
    } else {
        error(string("readEntireFile: file not found or cannot be opened: ") + filename);
        return "";
    }
}

bool readEntireFile(string filename, string& out) {
    ifstream input;
    input.open(filename.c_str());
    if (input.fail()) {
        return false;
    }
    ostringstream output;
    while (true) {
        int ch = input.get();
        if (input.fail()) break;
        output << (char) ch;
    }
    input.close();
    out = output.str();
    return true;
}

string getRoot(string filename) {
   int dot = -1;
   int len = filename.length();
   for (int i = 0; i < len; i++) {
      char ch = filename[i];
      if (ch == '.') dot = i;
      if (ch == '/' || ch == '\\') dot = -1;
   }
   if (dot == -1) {
      return filename;
   } else {
      return filename.substr(0, dot);
   }
}

string getExtension(string filename) {
   int dot = -1;
   int len = filename.length();
   for (int i = 0; i < len; i++) {
      char ch = filename[i];
      if (ch == '.') dot = i;
      if (ch == '/' || ch == '\\') dot = -1;
   }
   if (dot == -1) {
      return "";
   } else {
      return filename.substr(dot);
   }
}

string getHead(string filename) {
   int slash = -1;
   int len = filename.length();
   for (int i = 0; i < len; i++) {
      char ch = filename[i];
      if (ch == '/' || ch == '\\') slash = i;
   }
   if (slash < 0) {
      return "";
   } else if (slash == 0) {
      return "/";
   } else {
      return filename.substr(0, slash);
   }
}

string getTail(string filename) {
   int slash = -1;
   int len = filename.length();
   for (int i = 0; i < len; i++) {
      char ch = filename[i];
      if (ch == '/' || ch == '\\') slash = i;
   }
   if (slash < 0) {
      return filename;
   } else {
      return filename.substr(slash + 1);
   }
}

string defaultExtension(string filename, string ext) {
   bool force = (ext[0] == '*');
   if (force) ext = ext.substr(1);
   int dot = -1;
   int len = filename.length();
   for (int i = 0; i < len; i++) {
      char ch = filename[i];
      if (ch == '.') dot = i;
      if (ch == '/' || ch == '\\') dot = -1;
   }
   if (dot == -1) {
      force = true;
      dot = len;
   }
   if (force) {
      return filename.substr(0, dot) + ext;
   } else {
      return filename;
   }
}

string openOnPath(ifstream & stream, string path, string filename) {
   Vector<string> paths;
   splitPath(path, paths);
   for (string dir : paths) {
      string pathname = dir + "/" + filename;
      if (openFile(stream, pathname)) return pathname;
   }
   return "";
}

string openOnPath(ofstream & stream, string path, string filename) {
   Vector<string> paths;
   splitPath(path, paths);
   for (string dir : paths) {
      string pathname = dir + "/" + filename;
      if (openFile(stream, pathname)) return pathname;
   }
   return "";
}

string findOnPath(string path, string filename) {
   ifstream stream;
   string result = openOnPath(stream, path, filename);
   if (result != "") stream.close();
   return result;
}

void deleteFile(string filename) {
   remove(expandPathname(filename).c_str());
}

void renameFile(string oldname, string newname) {
   oldname = expandPathname(oldname);
   newname = expandPathname(newname);
   rename(oldname.c_str(), newname.c_str());
}

// JL rewrote this method. It wasn't working correctly in many cases.
void createDirectoryPath(string path) {
   int cp = 0;  // BUGFIX (JL): was 1
   if (path == "") return;
   path = expandPathname(path); // JL
   char sep = getDirectoryPathSeparator()[0]; // JL
   while ((cp = path.find(sep, cp + 1)) != string::npos) { // JL changed '/' to sep
      createDirectory(path.substr(0, cp)); // BUGFIX (JL): was `cp - 1`
   }
   createDirectory(path);
}

bool matchFilenamePattern(string filename, string pattern) {
   return recursiveMatch(filename, 0, pattern, 0);
}

bool fileExists(string filename) {
   return pp->fileExists(filename);
}

bool isFile(string filename) {
   return pp->isFile(filename);
}

bool isSymbolicLink(string filename) {
   return pp->isSymbolicLink(filename);
}

bool isDirectory(string filename) {
   return pp->isDirectory(filename);
}

void setCurrentDirectory(string path) {
   return pp->setCurrentDirectory(expandPathname(path)); // JL added expandPathname
}

string getCurrentDirectory() {
   return pp->getCurrentDirectory();
}

void createDirectory(string path) {
   return pp->createDirectory(path);
}

string getDirectoryPathSeparator() {
   return pp->getDirectoryPathSeparator();
}

string getSearchPathSeparator() {
   return pp->getSearchPathSeparator();
}

string expandPathname(string filename) {
   return pp->expandPathname(filename);
}

void listDirectory(string path, Vector<string> & list) {
   vector<string> vec;
   listDirectory(path, vec);
   list.clear();
   for (string file : vec) {
      list.add(file);
   }
}

void listDirectory(string path, vector<string> & list) {
   return pp->listDirectory(path, list);
}

/* Private functions */

// JL corrected some off-by-one errors.
static void splitPath(string path, Vector<string>& list) { // JL BUGFIX: added `&`
   char sep = (path.find(';') == string::npos) ? ':' : ';';
   path += sep;
   int start = 0;
   while (true) {
      int finish = path.find(sep, start);
      if (finish == string::npos) break;
      if (finish > start ) { // BUGFIX (JL): was `start + 1`
         list.add(path.substr(start, finish - start)); // BUGFIX (JL): was `finish - start - 1`
      }
      start = finish + 1;
   }
}

static bool recursiveMatch(string str, int sx, string pattern, int px) {
   int slen = str.length();
   int plen = pattern.length();
   if (px == plen) return (sx == slen);
   char pch = pattern[px];
   if (pch == '*') {
      for (int i = sx; i <= slen; i++) {
         if (recursiveMatch(str, i, pattern, px + 1)) return true;
      }
      return false;
   }
   if (sx == slen) return false;
   char sch = str[sx];
   if (pch == '[') {
      bool match = false;
      bool invert = false;
      px++;
      if (px == plen) {
         error("matchFilenamePattern: missing ]");
      }
      if (pattern[px] == '^') {
         px++;
         invert = true;
      }
      while (px < plen && pattern[px] != ']') {
         if (px + 2 < plen && pattern[px + 1] == '-') {
            match |= (sch >= pattern[px] && sch <= pattern[px + 2]);
            px += 3;
         } else {
            match |= (sch == pattern[px]);
            px++;
         }
      }
      if (px == plen) {
         error("matchFilenamePattern: missing ]");
      }
      if (match == invert) return false;
   } else if (pch != '?') {
      if (pch != sch) return false;
   }
   return recursiveMatch(str, sx + 1, pattern, px + 1);
}
