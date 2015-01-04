/**
 * @file console.h
 *
 * @brief
 * This file redirects the `cin`, `cout`,
 * and `cerr` streams to use a graphical console window.
 *
 * This file must be included in the source file that contains the `main`
 * method, although it may be included in other source files as well.
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

#ifndef _console_h
#define _console_h

#include <string>


/**
 * Erases the contents of the console window.
 */
void clearConsole();


/**
 * Changes the font used for the console.  The `font` parameter
 * is typically a string in the form `family-style-size`.
 * In this string, `family` is the name of the font family;
 * `style` is either missing (indicating a plain font) or one
 * of the strings `Bold`, `Italic`, or
 * `BoldItalic`; and `size` is an integer
 * indicating the point size.  If any of these components is
 * specified as an asterisk, the existing value is retained.
 * The `font` parameter can also be a sequence of
 * such specifications separated by semicolons, in which case the
 * first available font on the system is used.
 *
 * Sample usages:
 *
 *      setConsoleFont("Monospaced-14");
 *      setConsoleFont("SansSerif-Bold-*");
 *      setConsoleFont("Serif-*-12");
 *
 */
void setConsoleFont(const std::string & font);


/**
 * Changes the size of the console to the specified dimensions, measured
 * in pixels.
 */
void setConsoleSize(double width, double height);


/**
 * Changes the location of the console to the specified (\em x, \em y)
 * coordinates, relative to the top-left corner of the screen.
 */
void setConsoleLocation(int x, int y);


/**
 * Sets whether the overall C++ program should terminate if the console
 * window is closed.  By default this is true initially in the Whittier
 * version of the libraries.
 */
void setConsoleExitProgramOnClose(bool exitOnClose);


/**
 * Returns whether the overall C++ program will terminate if the console
 * window is closed.  By default this is true initially in the Whittier
 * version of the libraries.
 */
bool getConsoleExitProgramOnClose();


/**
 * Enables or disables a feature that causes exceptions to be echoed to the
 * graphical console window when they are thrown.
 * Disabled (false) by default.
 * Note that using this feature may make it harder to get a stack trace in the
 * debugger if you are debugging the cause of an exception.
 */
void setConsolePrintExceptions(bool printExceptions);


/**
 * Closes the graphical console window (and any graphics windows that are
 * open) and exits from the application.
 */
void closeConsoleAndExit();


#include "private/main.h"

#endif
