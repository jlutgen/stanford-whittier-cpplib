/**
 * @file hashcode.h
 *
 * This file exports \ref hashCode functions for \c string and
 * the C++ primitive types. The HashSet and HashMap classes have
 * access to these functions, so that clients do not need to write their
 * own `hashCode` functions in order to be able to create a `%HashSet` of
 * strings or primitive types, or to create a `%HashMap` whose
 * keys are strings or primitive types.
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

#ifndef _hashcode_h
#define _hashcode_h

#include <string>


/** \_overload */
int hashCode(bool key);
/** \_overload */
int hashCode(char key);
/** \_overload */
int hashCode(double key);
/** \_overload */
int hashCode(float key);
/** \_overload */
int hashCode(int key);
/** \_overload */
int hashCode(long key);
/** \_overload */
int hashCode(const char* str);
/** \_overload */
int hashCode(const std::string& str);
/**
 * Returns a hash code for the specified key. The hash code is always a
 * nonnegative integer.  This function is overloaded to support
 * all of the primitive types and the C++ \c string type.
 *
 * Sample usage:
 * ~~~
 * int hash = hashCode(key);
 * ~~~
 */
int hashCode(void* key);


/*
 * Constants that are used to help implement these functions
 * (see hashcode.cpp for example usage)
 */
extern const int HASH_SEED;         // Starting point for first cycle
extern const int HASH_MULTIPLIER;   // Multiplier for each cycle
extern const int HASH_MASK;         // All 1 bits except the sign

#endif // _hashcode_h
