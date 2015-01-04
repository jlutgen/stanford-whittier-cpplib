/**
 * @file strlib.h
 *
 * @brief
 * This file exports several useful string functions that are not
 * included in the C++ string library.
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

#ifndef _strlib_h
#define _strlib_h

#include <iostream>
#include <string>
#include <vector>
#include "private/genericio.h"
#include "vector.h"


/**
 * Returns the string \c "true" if \em b is true, or \c "false" if \em b is false.
 */
std::string boolToString(bool b);


/**
 * Returns the string \c "true" if \em b is nonzero, or \c "false" if \em b is zero.
 */
std::string boolToString(int b);


/**
 * Returns a single-character string containing the given character.
 * For example, <code>charToString('Q')</code> returns the string
 * <code>"Q"</code>.
 */
std::string charToString(char c);


/**
 * Returns \c true if \em str is \c "true", and returns \c false if
 * \em str is \c "false".
 * If \em str is any other value, this function signals an error.
 */
bool stringToBool(const std::string& str);


/**
 * Converts a single-character string (or a string containing a single non-whitespace
 * character) into its corresponding \c char value.
 * For example, both <code>stringToChar("h")</code> and <code>stringToChar("  h ")</code>
 * return \c 'h'.
 * This function signals an error if the given string does not contain exactly 1
 * non-whitespace character.
 */
char stringToChar(const std::string& str);


/**
 * Converts an integer into the corresponding string of digits.
 * For example, calling <code>integerToString(2112)</code> returns
 * the string <code>"2112"</code>.
 *
 * Sample usage:
 *
 *     string s = integerToString(n);
 */
std::string integerToString(int n);


/**
 * Converts a string of digits into an integer.  If the string is not a
 * legal integer or contains extraneous characters other than whitespace,
 * this function signals an error.
 *
 * Sample usage:
 *
 *     int n = stringToInteger(str);
 */
int stringToInteger(std::string str);


/**
 * Converts an integer into the corresponding string of digits.
 * For example, calling <code>longToString(-123)</code> returns
 * the string <code>"-123"</code>.
 *
 * Sample usage:
 *
 *     string s = longToString(n);
 */
std::string longToString(long n);


/**
 * Converts a string of digits into a long integer.  If the string is not a
 * legal \c long or contains extraneous characters other than whitespace,
 * this function signals an error.
 *
 * Sample usage:
 *
 *     long n = stringToLong(str);
 */
long stringToLong(const std::string& str);


/**
 * Converts a floating-point number into the corresponding string form.
 * For example, calling <code>realToString(23.45)</code> returns
 * the string <code>"23.45"</code>.
 *
 * Sample usage:
 *
 * string s = realToString(d);
 */
std::string realToString(double d);


/**
 * Converts a string representing a real number into its corresponding
 * value.  If the string is not a legal floating-point number or contains
 * extraneous characters other than whitespace, this function
 * signals an error.
 *
 * Sample usage:
 *
 *     double d = stringToReal(str);
 */
double stringToReal(std::string str);


/**
 * Returns \c true if the given string is either \c "true" or \c "false".
 */
bool stringIsBool(const std::string& str);


/**
 * Returns \c true if the given string could be converted to an integer
 * successfully by the \ref stringToInteger
 * function, which will be true if the string has the format of an integer,
 * such as \c "1234" or \c "-8".
 */
bool stringIsInteger(const std::string& str);


/**
 * Returns \c true if the given string could be converted to a long
 * successfully by the \ref stringToLong
 * function, which will be true if the string has the format of an integer,
 * such as \c "1234" or \c "-8".
 */
bool stringIsLong(const std::string& str);


/**
 * Returns \c true if the given string could be converted to an real number
 * successfully by the \ref stringToReal
 * function, which will be true if the string has the format of a real number,
 * such as \c "3.14", \c "-46", or \c "3.2815E-04".
 */
bool stringIsReal(const std::string& str);
/**
 * Alias for \ref stringIsReal.
 */
bool stringIsDouble(const std::string& str);


/**
 * Returns a Vector whose elements are strings formed by splitting the
 * given string \em str into pieces separated by the given delimiter.
 * For example,
 *
 *      split("Lions, tigers, and bears!", " ")
 *
 * returns a vector of size 4: <code>{"Lions,", "tigers,", "and", "bears!"}</code>
 *
 * By default, this function splits the string at each occurrence of the
 * delimiter. If present, the argument \em limit specifies the maximum
 * number of points at which to split the string. For example,
 *
 *      split("Lions, tigers, and bears!", " ", 2)
 *
 * returns a vector of size 3: <code>{"Lions,", "tigers,", "and bears!"}</code>
 *
 * Sample usages:
 *
 *     Vector<string> v = split(str, delimiter);
 *     Vector<string> v = split(str, delimiter, limit);
 */
Vector<std::string> split(const std::string& str, const std::string& delimiter, int limit = -1);
std::vector<std::string> stringSplit(const std::string& str, const std::string& delimiter, int limit = -1);


/**
 * Returns a new string in which all lowercase characters have been converted
 * into their uppercase equivalents.
 *
 * Sample usage:
 *
 *     string s = toUpperCase(str);
 */
std::string toUpperCase(std::string str);


/**
 * Returns a new string in which all uppercase characters have been converted
 * into their lowercase equivalents.
 *
 * Sample usage:
 *
 *     string s = toLowerCase(str);
 */
std::string toLowerCase(std::string str);


/**
 * Returns \c true if \em s1 and \em s2 are
 * equal, ignoring differences in case. For example,
 * <code>equalsIgnoreCase("bozo", "bOzO")</code> returns \c true.
 *
 * Sample usage:
 *
 *     if (equalsIgnoreCase(s1, s2)) ...
 */
bool equalsIgnoreCase(std::string s1, std::string s2);


/** \_overload */
bool startsWith(std::string str, char prefix);
/**
 * Returns \c true if the string \em str starts with
 * the specified prefix, which may be either a string or a character.
 *
 * Sample usage:
 *
 *     if (startsWith(str, prefix)) ...
 */
bool startsWith(std::string str, std::string prefix);


/** \_overload */
bool endsWith(std::string str, char suffix);
/**
 * Returns \c true if the string \em str ends with
 * \em suffix, which may be either a string or a character.
 *
 * Sample usage:
 *
 *     if (endsWith(str, suffix)) ...
 */
bool endsWith(std::string str, std::string suffix);


/**
 * Returns a new string obtained by removing any whitespace characters
 * from the beginning and end of \em str.
 *
 * Sample usage:
 *
 *     string trimmed = trim(str);
 */
std::string trim(std::string str);


/**
 * Returns the starting index of the first occurrence of \em substring
 * in \em str, if it occurs.  If it does not occur, returns -1.
 * This function is very similar to the standard string function \c find,
 * but \c find returns <code>string::npos</code> if the substring is not found.
 *
 * By default, this function begins searching at the beginning of \em str.
 * If the optional argument \em startIndex is present, the search begins at that
 * index instead.
 *
 * Sample usages:
 *
 *     int pos = stringIndexOf(str, substring);
 *     int pos = stringIndexOf(str, substring, 3);
 */
int stringIndexOf(const std::string& str, const std::string& substring, int startIndex = 0);


/** \_overload */
std::string stringReplace(const std::string& str, const std::string& old, const std::string& replacement, int limit = -1);
/**
 * Returns a new string obtained from \em str by replacing occurrences of \em old
 * text with \em new.
 *
 * The `InPlace` variant modifies the existing string \em str rather than returning a new one,
 * and returns the number of occurrences of \em old that were replaced.
 *
 * In both functions, the optional argument \em limit specifies the maximum number of
 * occurrences of \em old that should be replaced. If \em limit is not present,
 * all occurrences are replaced.
 *
 * Sample usages:
 *
 *     string newString = stringReplace(str, old, replacement, 1);
 *     int numReplacements = stringReplaceInPlace(str, old, replacement);
 */
int stringReplaceInPlace(std::string& str, const std::string& old, const std::string& replacement, int limit = -1);


#endif
