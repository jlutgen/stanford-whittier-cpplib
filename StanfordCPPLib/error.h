/**
 * @file error.h
 *
 * @brief
 * This file exports the ErrorException class and the
 * \ref error function.
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

#ifndef _error_h
#define _error_h

#include <string>
#include <exception>


/**
 * @class ErrorException
 *
 * @brief An %ErrorException is thrown by calls to the \ref error
 * function.
 *
 * Typical code for catching errors looks like this:
 *
 *   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *    try {
 *       ... code in which an error might occur ...
 *    } catch (ErrorException & ex) {
 *       ... code to handle the error condition ...
 *    }
 *   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * If an %ErrorException is thrown at any point in the
 * range of the <code>try</code> (including in functions called from
 * that code), control will jump immediately to the error-handling code.
 */
class ErrorException : public std::exception {
public:
   ErrorException(std::string msg);
   virtual ~ErrorException() throw ();
   virtual std::string getMessage() const;
   virtual const char *what() const throw ();

private:
   std::string msg;
};


/**
 * Signals an error condition in a program by throwing an
 * ErrorException with the specified message.
 *
 * Sample usage:
 *
 *      error(msg);
 */
void error(std::string msg);

#include "private/main.h"

#endif
