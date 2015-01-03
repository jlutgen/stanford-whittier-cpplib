/**
 * @file random.h
 *
 * This file exports functions for generating pseudorandom numbers.
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

#ifndef _random_h
#define _random_h


/**
 * Returns a random integer in the range \em low to
 * \em high, inclusive.
 *
 * Sample usage:
 * ~~~
 * int n = randomInteger(low, high);
 * ~~~
 */
int randomInteger(int low, int high);


/**
 * Returns a random real number in the half-open interval
 * [\em low,&nbsp;\em high).  A half-open
 * interval includes the first endpoint but not the second, which
 * means that the result is always greater than or equal to
 * \em low but strictly less than \em high.
 *
 * Sample usage:
 * ~~~
 * double d = randomReal(low, high);
 * ~~~
 */
double randomReal(double low, double high);


/**
 * Returns \c true with probability \em p.
 * The argument \em p  must be a real number between
 * 0 (representing 0% probability) and 1 (representing 100% probability).
 * For example, calling <code>randomChance(.30)</code> returns <code>true</code>
 * 30% of the time.
 *
 * Sample usage:
 * ~~~
 * if (randomChance(p)) ...
 * ~~~
 */
bool randomChance(double p);


/**
 * Sets the internal seed for the random number generator to the specified value.
 * Clients can use this function to set a specific starting point for the
 * pseudorandom sequence or to ensure that program behavior is
 * repeatable during the debugging phase.
 *
 * Sample usage:
 * ~~~
 * setRandomSeed(seed);
 * ~~~
 */
void setRandomSeed(int seed);


#endif
