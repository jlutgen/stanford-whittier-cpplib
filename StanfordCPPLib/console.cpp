/*
 * File: console.cpp
 * -----------------
 * This file implements the console.h interface.
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

#include <string>
#include <cstdlib>
#include "console.h"
#include "platform.h"
#include "exceptions.h"
#include "error.h"
using namespace std;

//static void sclTerminateHandler();
static Platform *pp = getPlatform();
static bool consoleExitProgramOnClose = false;
//static bool consolePrintExceptions = false;
//static void (*old_terminate)() = NULL;

void clearConsole() {
   pp->clearConsole();
}

void setConsoleFont(const string & font) {
   pp->setConsoleFont(font);
}

void setConsoleLocation(int x, int y) {
   pp->setConsoleLocation(x, y);
}

void setConsoleSize(double width, double height) {
   pp->setConsoleSize(width, height);
}

void setConsoleExitProgramOnClose(bool exitOnClose) {
    consoleExitProgramOnClose = exitOnClose;
}

bool getConsoleExitProgramOnClose() {
    return consoleExitProgramOnClose;
}

void setConsolePrintExceptions(bool printExceptions) {
    //if (consoleLocked) { return; }
    exceptions::setTopLevelExceptionHandlerEnabled(printExceptions);
}

void closeConsoleAndExit() {
    pp->exitGraphics();
}
