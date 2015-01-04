/**
 * @file point.h
 *
 * @brief
 * This file exports a Point class, which represents an integer-valued (\em x,&nbsp;\em y)
 * pair.
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

#ifndef _point_h
#define _point_h

#include <string>


/**
 * This class represents an (\em x, \em y) coordinate point on a
 * two-dimensional integer grid.  If you need to work with real-valued coordinates,
 * you should use the GPoint class from the \ref gtypes.h interface instead.
 */

class Point {

public:

/** \_overload */
   Point();
/**
 * Creates a `%Point` object with the specified \em x and \em y coordinates.
 * If the coordinates are not supplied, the default constructor sets these
 * fields to 0.
 *
 * Sample usages:
 *
 *     Point origin;
 *     Point pt(x, y);
 */
   Point(int x, int y);


/**
 * Returns the <i>x</i>-coordinate of this point.
 *
 * Sample usage:
 *
 *     int x = pt.getX();
 */
   int getX() const;


/**
 * Returns the <i>y</i>-coordinate of this point.
 *
 * Sample usage:
 *
 *     int y = pt.getY();
 */
   int getY() const;


/**
 * Returns a printable string representation of this point.
 *
 * Sample usage:
 *
 *     string str = pt.toString();
 */
   std::string toString() const;


/*
 * Friend operator: ==
 * Usage: if (p1 == p2) ...
 * ------------------------
 * Returns <code>true</code> if <code>p1</code> and <code>p2</code>
 * are the same point.
 */

   bool operator==(const Point & p2) const;

/*
 * Friend operator: !=
 * Usage: if (p1 != p2) ...
 * ------------------------
 * Returns <code>true</code> if <code>p1</code> and <code>p2</code>
 * are different.
 */

   bool operator!=(const Point & p2) const;

/* Private section */

/**********************************************************************/
/* Note: Everything below this point in the file is logically part    */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

private:

/* Instance variables */

   int x;                         /* The x-coordinate of the point */
   int y;                         /* The y-coordinate of the point */

/* Friend declarations */

   friend int hashCode(const Point & p);

};


/* Free functions */

/**
 * Overloads the `<<` operator so that it is able
 * to display `%Point` values.
 *
 * Sample usage:
 *
 *     cout << pt;
 */
std::ostream & operator<<(std::ostream & os, const Point & pt);


/**
 * Returns a hash code for the given point. This function is provided
 * for convenience so that clients do not need to write their own
 * \c hashCode function in order to be able to create a HashSet of
 * Point objects or a HashMap whose keys are Point objects.
 *
 * Sample usage:
 *
 *     int hash = hashCode(p);
 */
int hashCode(const Point & p);


#endif
