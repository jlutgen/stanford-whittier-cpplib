/**
 * @file gmath.h
 *
 * This file exports several functions for working with graphical
 * geometry along with the mathematical constants \em PI
 * and \em E.
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

#ifndef _gmath_h
#define _gmath_h

#include "gtypes.h"


/**
 * The mathematical constant &pi;, which is the ratio of the circumference
 * of a circle to its diameter.
 */
extern const double PI;


/**
 * The mathematical constant \em e, which is the base of natural logarithms.
 */
extern const double E;


/**
 * Returns the trigonometric sine of <code>angle</code>, which is
 * expressed in degrees.
 *
 * Sample usage:
 * ~~~
 * double sine = sinDegrees(angle);
 * ~~~
 */
double sinDegrees(double angle);


/**
 * Returns the trigonometric cosine of <code>angle</code>, which is
 * expressed in degrees.
 *
 * Sample usage:
 * ~~~
 * double cosine = cosDegrees(angle);
 * ~~~
 */
double cosDegrees(double angle);


/**
 * Returns the trigonometric tangent of <code>angle</code>, which is
 * expressed in degrees.
 *
 * Sample usage:
 * ~~~
 * double tangent = tanDegrees(angle);
 * ~~~
 */
double tanDegrees(double angle);


/**
 * Converts an angle from radians to degrees.
 *
 * Sample usage:
 * ~~~
 * double degrees = toDegrees(radians);
 * ~~~
 */
double toDegrees(double radians);


/**
 * Converts an angle from degrees to radians.
 *
 * Sample usage:
 * ~~~
 * double radians = toRadians(degrees);
 * ~~~
 */
double toRadians(double degrees);


/** \_overload */
double vectorDistance(const GPoint & pt);
/**
 * Computes the distance between the origin and the specified point.
 *
 * Sample usages:
 * ~~~
 * double r = vectorDistance(pt);
 * double r = vectorDistance(x, y);
 * ~~~
 */
double vectorDistance(double x, double y);


/** \_overload */
double vectorAngle(const GPoint & pt);
/**
 * Returns the angle in degrees from the origin to the specified point.
 * This function takes account of the fact that the graphics coordinate
 * system is flipped in the \em y direction from the traditional
 * Cartesian plane.
 *
 * Sample usages:
 * ~~~
 * double angle = vectorAngle(pt);
 * double angle = vectorAngle(x, y);
 * ~~~
 */
double vectorAngle(double x, double y);


#endif
