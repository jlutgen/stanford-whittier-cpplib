/*
 * File: base64.h
 * --------------
 * This file declares a set of functions for encoding and decoding binary data
 * in the base64 format.  See:
 * http://en.wikipedia.org/wiki/Base64
 *
 * @author Marty Stepp, based upon open-source Apache Base64 en/decoder
 */

/*************************************************************************/
/* Stanford Portable Library                                             */
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

#ifndef _base64_h
#define _base64_h


#ifdef __cplusplus
extern "C" {
#endif

int Base64encode_len(int len);
int Base64encode(char* coded_dst, const char* plain_src, int len_plain_src);

int Base64decode_len(const char* coded_src);
int Base64decode(char* plain_dst, const char* coded_src);

#ifdef __cplusplus
}

#include <string>

namespace Base64 {
/*
 * Returns a Base64-encoded equivalent of the given string.
 */
std::string encode(const std::string& s);

/*
 * Decodes the given Base64-encoded string and returns the decoded
 * original contents.
 */
std::string decode(const std::string& s);
}
#endif


#endif
