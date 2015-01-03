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

#ifndef _genericio_h
#define _genericio_h

/* Private section */

/**********************************************************************/
/* Note: Everything below this point in the file is logically part    */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

/*
 * Usage: writeQuotedString(outfile, str, forceQuotes);
 * ----------------------------------------------------
 * Writes the string str to outfile surrounded by double quotes, converting
 * special characters to escape sequences, as necessary.  If the optional
 * parameter forceQuotes is explicitly set to false, quotes are included
 * in the output only if necessary.
 */

void writeQuotedString(std::ostream & os, const std::string & str,
                       bool forceQuotes = true);

/*
 * Usage: readQuotedString(infile, str);
 * -------------------------------------
 * Reads the next string from infile into the reference parameter str.
 * If the first character (other than whitespace) is either a single
 * or a double quote, this function reads characters up to the
 * matching quote, processing standard escape sequences as it goes.
 * If not, readString reads characters up to any of the characters
 * in the string STRING_DELIMITERS in the implementation file.
 */

void readQuotedString(std::istream & is, std::string & str);

/*
 * Usage: if (stringNeedsQuoting(str)) ...
 * ---------------------------------------
 * Checks whether the string needs quoting in order to be read correctly.
 */

bool stringNeedsQuoting(const std::string & str);

/*
 * Usage: writeGenericValue(os, value, forceQuotes);
 * -------------------------------------------------
 * Writes a generic value to the output stream.  If that value is a string,
 * this function uses writeQuotedString to write the value.
 */

template <typename ValueType>
void writeGenericValue(std::ostream & os, const ValueType & value,
                       bool forceQuotes) {
   os << value;
}

template <>
inline void writeGenericValue(std::ostream & os, const std::string & value,
                              bool forceQuotes) {
   writeQuotedString(os, value, forceQuotes);
}

/*
 * Usage: readGenericValue(is, value);
 * -----------------------------------
 * Reads a generic value from the input stream.  If that value is a string,
 * this function uses readQuotedString to read the value.
 */

template <typename ValueType>
void readGenericValue(std::istream & is, ValueType & value) {
   is >> value;
}

template <>
inline void readGenericValue(std::istream & is, std::string & value) {
   readQuotedString(is, value);
}

#endif // _genericio_h
