/**
 * @file direction.h
 *
 * @brief
 * This file exports an enumerated type called \ref Direction
 * whose elements are the four compass points: <code>NORTH</code>,
 * <code>EAST</code>, <code>SOUTH</code>, and <code>WEST</code>.
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

#ifndef _direction_h
#define _direction_h

#include <iostream>
#include <string>
#include "foreach.h"


/**
 * This enumerated type is used to represent the four compass directions.
 */
enum Direction {
    NORTH,          ///<.
    EAST,           ///<.
    SOUTH,          ///<.
    WEST            ///<.
};


/**
 * Returns the direction that is to the left of \em dir.
 *
 * Sample usage:
 *
 *     Direction newdir = leftFrom(dir);
 */
Direction leftFrom(Direction dir);


/**
 * Returns the direction that is to the right of \em dir.
 *
 * Sample usage:
 *
 *     Direction newdir = rightFrom(dir);
 */
Direction rightFrom(Direction dir);


/**
 * Returns the direction that is to opposite to \em dir.
 *
 * Sample usage:
 *
 *     Direction newdir = opposite(dir);
 */
Direction opposite(Direction dir);


/**
 * Returns the name of the direction \em dir as a string.
 */
std::string directionToString(Direction dir);


/**
 * Overloads the `<<` operator so that it is able
 * to display \ref Direction values.
 *
 * Sample usage:
 *
 *     cout << dir;
 */
std::ostream & operator<<(std::ostream & os, const Direction & dir);


/**
 * Overloads the `>>` operator so that it is able
 * to read \ref Direction values. This method signals an error if
 * it reads an invalid string from the input stream. Valid strings
 * are \c "NORTH", \c "SOUTH", \c "EAST", and \c "WEST".
 *
 *
 * Sample usage:
 *
 *     cin >> dir;
 */
std::istream & operator>>(std::istream & os, Direction & dir);


/**
 * Overloads the suffix version of the `++` operator to
 * work with \ref Direction values.  The sole purpose of this
 * definition is to support the following idiom:
 *
 *     for (Direction dir = NORTH; dir <= WEST; dir++) {...}
 */
Direction operator++(Direction & dir, int);

#endif
