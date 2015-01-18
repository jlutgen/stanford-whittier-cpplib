/**
 * @file: filelib.h
 *
 * @brief
 * This file exports a standardized set of tools for working with
 * files.
 *
 * The library offers at least some portability across the
 * file systems used in the three supported platforms: Mac OS X,
 * Windows, and Linux.  Pathnames and search paths are allowed to
 * contain separators in any of the supported styles described below,
 * which usually makes it possible to use the same code on different platforms.
 *
 * <h2>Supported styles for pathnames and search paths</h2>
 * <h4>All platforms</h4>
 * <ul>
 *  <li>Either a forward slash or a backslash may be used as
 *      the directory separator in a pathname.
 *  <li>The usual abbreviations . and .. (for the current directory
 *      and the parent of a directory, respectively) may be used in a pathname.
 *  <li>Either a colon or a semicolon may be used as the separator
 *      between pathnames in a search path.
 * </ul>
 *
 * <h4>Mac OS X and Linux only</h4>
 * <ul>
 *  <li>Home directory abbreviations such as <code>~</code> and <code>~jeff</code>
 *      may be used in pathnames.
 * </ul>
 *
 * <h4>Windows only</h4>
 * <ul>
 *  <li>If a drive letter specification (such as `C:`) is omitted from a pathname,
 *      the current drive is used by default.
 * </ul>
 *
 * Please see the description of \ref openOnPath for examples.
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

#ifndef _filelib_h
#define _filelib_h

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "vector.h"

/** \_overload */
bool openFile(std::ifstream & stream, std::string filename);
/**
 * Opens the filestream \em stream using the specified
 * filename.  This function is similar to the \c open
 * method of the stream classes, but uses a C++ \c string
 * object instead of the older C-style string.  If the operation
 * succeeds, this function returns \c true.
 * If it fails, this function sets the failure flag in the
 * stream and returns \c false.
 *
 * Sample usage:
 *
 *     if ( openFile(stream, filename) ) {...}
 *
 */
bool openFile(std::ofstream & stream, std::string filename);


/** \_overload */
std::string promptUserForFile(std::ifstream & stream, std::string prompt = "");
/**
 * Asks the user for the name of a file.  The file is opened using
 * the reference parameter \em stream, and the function
 * returns the name of the file.  If the requested file cannot be
 * opened, the user is given additional chances to enter a valid file.
 * The optional \em prompt argument provides an input prompt
 * for the user.
 *
 * Sample usage:
 *
 *     string filename = promptUserForFile(stream, prompt);
 *
 */
std::string promptUserForFile(std::ofstream & stream, std::string prompt = "");


/** \_overload */
std::string openFileDialog();
/** \_overload */
std::string openFileDialog(std::string title);
/**
 * Displays a dialog that allows the user to choose a file to be opened.  The
 * \em title parameter is displayed in the dialog title.
 * The \em path parameter is used to set the initial directory for the dialog;
 * if \em path does not appear or is the empty string, <code>openFileDialog</code>
 * uses the current directory. If \em path is not a valid directory, the initial
 * directory is platform-dependent; on Linux and Mac OS X, it is the user's home directory,
 * while on Windows, it is the user's "My Documents" directory.
 *
 * By default, the dialog shows all files in the chosen directory. However, if the
 * final component of \em path is a semicolon-separated list of one or more patterns
 * (of the sort accepted by \ref matchFilenamePattern), then the dialog will show only
 * those files matching the given pattern or patterns.
 * For example, if \em path is <code>/Users/jeff/images/</code><code>*.png;*.jpg</code>, then the dialog
 * will initially show only those files in <code>/Users/jeff/images</code> whose names end in
 * <code>.png</code> or <code>.jpg</code>.
 *
 * If \em path is supplied and contains a pattern list, the optional parameter \em patternName
 * may be used to specify a description of the pattern. This description will be displayed
 * in the file dialog. For example, the following call might be used to display a dialog
 * for opening an HTML file:
 *
 *       string filename = openFileDialog(title, "*.htm;*.html", "HTML files");
 *
 * This function returns the full pathname of the selected file.
 * If the user cancels the dialog, this function returns
 * the empty string.
 *
 * Sample usages:
 *
 *     string filename = openFileDialog();
 *     string filename = openFileDialog(title);
 *     string filename = openFileDialog(title, path);
 *     string filename = openFileDialog(title, path, patternName);
 *
 */
std::string openFileDialog(std::string title, std::string path, std::string patternName = "");


/** \_overload */
std::string saveFileDialog();
/** \_overload */
std::string saveFileDialog(std::string title);
/**
 * Displays a dialog that allows the user to specify a location and filename for saving
 * a file.  The \em title parameter is displayed in the dialog title.
 * The \em path parameter is used to set the initial directory for the dialog;
 * if \em path does not appear or is the empty string, <code>saveFileDialog</code>
 * uses the current directory. If \em path is not a valid directory, the initial
 * directory is platform-dependent; on Linux and Mac OS X, it is the user's home directory,
 * while on Windows, it is the user's "My Documents" directory.
 *
 * By default, the dialog shows all files in the chosen directory. However, if the
 * final component of \em path is a semicolon-separated list of one or more patterns
 * (of the sort accepted by \ref matchFilenamePattern), then the dialog will show only
 * those files matching the given pattern or patterns.
 * For example, if \em path is <code>/Users/jeff/images/</code><code>*.png;*.jpg</code>, then the dialog
 * will initially show only those files in <code>/Users/jeff/images</code> whose names end in
 * <code>.png</code> or <code>.jpg</code>.
 *
 * If \em path is supplied and contains a pattern list, the optional parameter \em patternName
 * may be used to specify a description of the pattern. This description will be displayed
 * in the file dialog. For example, the following call might be used to display a dialog
 * for saving an HTML file:
 *
 *       string filename = saveFileDialog(title, "*.htm;*.html", "HTML files");
 *
 * This function returns the full pathname corresponding to the user's choice of location and
 * filename. If the user cancels the dialog, this function returns the empty string.
 *
 * Sample usages:
 *
 *     string filename = saveFileDialog();
 *     string filename = saveFileDialog(title);
 *     string filename = saveFileDialog(title, path);
 *     string filename = saveFileDialog(title, path, patternName);
 *
 */
std::string saveFileDialog(std::string title, std::string path, std::string patternName = "");


/** \_overload */
void readEntireFile(std::istream & is, Vector<std::string> & lines);
/**
 * Reads the entire contents of the specified input stream into the
 * string vector \em lines.  The client is responsible for
 * opening and closing the stream.  The vector can be either an STL
 * \c vector or a Vector as defined in the
 * Stanford C++ libraries.
 *
 * Sample usage:
 *
 *     readEntireFile(stream, lines);
 *
 */
void readEntireFile(std::istream & is, std::vector<std::string> & lines);


/** \_overload */
std::string readEntireFile(std::string filename);
/**
 * Reads the entire file into a
 * single string (which might be very long!) rather than a vector
 * of lines.
 *
 * The first version returns a string containing the entire file's text.
 * It signals an error if the file is not found or cannot be read.
 *
 * The second version fills the string reference \em out with the entire
 * file's text. It returns \c true if successful and \c false if the file
 * was not found or could not be opened for reading.
 *
 * Sample usage:
 *
 *     string s = readEntireFile(filename);
 *     bool success = readEntireFile(filename, s);
 */
bool readEntireFile(std::string filename, std::string& out);


/**
 * Returns the root of \em filename.  The root consists
 * of everything in \em filename to the left of the last dot and
 * the subsequent extension.  If no dot appears in the final component
 * of the filename, this function returns the entire name.
 */
std::string getRoot(std::string filename);


/**
 * Returns the extension of \em filename.  The extension
 * consists of the last dot and all subsequent characters.
 * If no dot exists in the final component, this function
 * returns the empty string.  These semantics ensure that concatenating
 * the root and the extension always returns the original filename.
 */
std::string getExtension(std::string filename);


/**
 * Returns all but the last component of a path name.  The components
 * of the path name can be separated by any of the directory path
 * separators (forward slash or backslash).  The special cases are
 * illustrated by the following examples:
 *
 *
 *     getHead("a/b")  = "a"     getTail("a/b")   = "b"
 *     getHead("a")    = ""      getTail("a")     = "a"
 *     getHead("/a")   = "/"     getTail("/a")    = "a"
 *     getHead("/")    = "/"     getTail("/")     = ""
 *
 */
std::string getHead(std::string filename);


/**
 * Returns the last component of a path name.  The components of the
 * path name can be separated by any of the directory path separators
 * (forward slash or backslash).  For details on the interpretation of
 * special cases, see the description of the \ref getHead function.
 */
std::string getTail(std::string filename);


/**
 * Adds an extension to a file name if none already exists.  If the
 * \em ext argument begins with a leading <code>*</code>,
 * any existing extension in \em filename is replaced by
 * \em ext.
 */
std::string defaultExtension(std::string filename, std::string ext);


/** \_overload */
std::string openOnPath(std::ifstream & stream, std::string path,
                                               std::string filename);
/**
 * Opens a file using a search path.  If <code>openOnPath</code>
 * is successful, it returns the first path name on the search path
 * for which <i>stream</i><code>.open</code> succeeds.  The \em path
 * argument consists of a list of pathnames that are prepended to
 * \em filename, unless \em filename begins with an absolute
 * directory marker, such as <code>/</code> or <code>~</code>.
 * The pathnames in the search path may be separated either
 * by colons (Linux or Mac OS X) or semicolons (Windows).
 *
 * On Linux or Mac OS X, an example of a search path is
 *
 *      ".:~/images:/usr/local/share/images:/usr/share/images"
 *
 * where, as usual, the dot indicates the current directory and the tilde
 * is an abbreviation for the current user's home directory.
 *
 * On Windows, the separator in pathnames is a backslash, which in a literal string must
 * be specified with the escape sequence `\\`, so a typical search path on Windows can be
 * quite ugly, as in
 *
 *      "Images;C:\\Temp\\Images"
 *
 * As a convenience, however, the functions in this library accept forward slashes as separators even
 * on Windows, so the search path above may be replaced by the following, slightly more attractive, one:
 *
 *      "Images;C:/Temp/Images"
 *
 * As a further convenience, if the drive letter specification for the current drive is omitted from an
 * absolute path on Windows, the current drive is used by default, so that the search path
 * above may be further simplified to
 *
 *      "Images;/Temp/Images"
 *
 * If the search path contains only one pathname and that pathname contains
 * a colon, then the search path must be terminated with a semicolon, like this:
 *
 *      "J:/MyUSB/Images;"
 *
 * If the file cannot be opened, the failure bit is set in the \em stream
 * parameter, and this function returns the
 * empty string.
 */
std::string openOnPath(std::ofstream & stream, std::string path,
                                               std::string filename);


/**
 * Returns the canonical name of a file found using a search path.
 * This function is similar to
 * \ref openOnPath, except that it doesn't
 * return an opened stream.  If no matching file is found,
 * this function returns the empty string.
 */
std::string findOnPath(std::string path, std::string filename);


/**
 * Deletes the specified file. If the file does not exist
 * or could not be deleted, this function does nothing.
 */
void deleteFile(std::string filename);


/**
 * Renames a file.  If the file \em oldname does not exist or could
 * not be renamed, this function does nothing.
 */
void renameFile(std::string oldname, std::string newname);


/**
 * Returns \c true if the specified file exists.
 *
 * Sample usage:
 *
 *     if (fileExists(filename)) ...
 *
 */
bool fileExists(std::string filename);


/**
 * Returns \c true if the specified file is a regular file.
 *
 * Sample usage:
 *
 *     if (isFile(filename)) ...
 *
 */
bool isFile(std::string filename);


/**
 * Returns \c true if the specified file is a symbolic link.
 *
 * Sample usage:
 *
 *     if (isSymbolicLink(filename)) ...
 *
 */
bool isSymbolicLink(std::string filename);


/**
 * Returns \c true if the specified file is a directory.
 *
 * Sample usage:
 *
 *     if (isDirectory(filename)) ...
 *
 */
bool isDirectory(std::string filename);


/**
 * Changes the current directory to the specified path.
 */
void setCurrentDirectory(std::string path);


/**
 * Returns an absolute filename for the current directory.
 */
std::string getCurrentDirectory();


/**
 * Creates a new directory for the specified path.  This function does not report an error if
 * the directory already exists.  Unlike <code>\ref createDirectoryPath</code>,
 * `%createDirectory` does not create missing directories
 * along the path.  If some component of \em path does
 * not exist, this function signals an error.
 */
void createDirectory(std::string path);


/**
 * Creates a new directory for the specified path.   If intermediate
 * components of \em path do not exist, this function creates
 * them as needed.
 *
 * Errors are reported by calling \ref error.
 */
void createDirectoryPath(std::string path);


/**
 * Expands a filename into a canonical name for the platform.
 *
 * On Mac OS X and Linux, this function replaces each backslash character
 * in \em filename with a forward slash.
 * In addition, if the first component of \em filename is a home directory abbreviation
 * such as <code>~</code> or <code>~jeff</code>, it is expanded
 * as appropriate for the platform (ordinarily into <code>/Users/jeff</code> on
 * Mac OS X and <code>/home/jeff</code> on Linux).
 *
 * On Windows, this function simply replaces each forward slash in \em filename
 * with a backslash character.
 */
std::string expandPathname(std::string filename);


/** \_overload */
void listDirectory(std::string path, Vector<std::string> & list);
/**
 * Adds an alphabetized list of the files in the specified directory
 * to the string vector \em list. The \em list argument may be either
 * an STL \c vector or a Stanford Vector.  The generated list excludes the
 * <code>.</code> and <code>..</code> entries. If \em path is the empty
 * string, the contents of the current directory are listed.
 *
 * Errors are reported by calling \ref error.
 *
 * Sample usage:
 *
 *     listDirectory(path, list);
 *
 */
void listDirectory(std::string path, std::vector<std::string> & list);


/**
 * Determines whether the filename matches the specified pattern.  The
 * pattern string is interpreted in much the same way that a Unix shell
 * expands filenames and supports the following wildcard options:
 *
 *     ?      Matches any single character
 *     *      Matches any sequence of characters
 *     [...]  Matches any of the specified characters
 *     [^...] Matches any character except the specified ones
 *
 *
 * The last two options allow a range of characters to be specified in the
 * form <code>a-z</code>.
 *
 * Sample usage:
 *
 *     if (matchFilenamePattern(filename, pattern)) ...
 *
 */
bool matchFilenamePattern(std::string filename, std::string pattern);


/**
 * Returns the standard directory path separator used on this platform.
 */
std::string getDirectoryPathSeparator();


/**
 * Returns the standard search path separator used on this platform.
 */
std::string getSearchPathSeparator();


#include "private/main.h"

#endif
