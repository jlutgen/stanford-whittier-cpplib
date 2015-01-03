/**
 * @file gtypes.h
 *
 * This file exports classes for representing points, dimensions, and
 * rectangles.
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

#ifndef _gtypes_h
#define _gtypes_h

#include <iostream>
#include <string>

/**
 * This class contains real-valued \em x and \em y fields.  It is used to
 * represent a location on the graphics plane.
 */
class GPoint {

public:

/** \_overload */
   GPoint();
/**
 * Creates a `%GPoint` object with the specified \em x
 * and \em y coordinates.  If the coordinates are not supplied,
 * the default constructor sets these fields to 0.
 *
 * Sample usages:
 * ~~~
 * GPoint origin;
 * GPoint pt(x, y);
 * ~~~
 */
   GPoint(double x, double y);


/**
 * Returns the \em x component of this point.
 *
 * Sample usage:
 * ~~~
 * double x = pt.getX();
 * ~~~
 */
   double getX() const;

/**
 * Returns the \em y component of this point.
 *
 * Sample usage:
 * ~~~
 * double y = pt.getY();
 * ~~~
 */
   double getY() const;


/**
 * Returns a printable string representation of this point.
 *
 * Sample usage:
 * ~~~
 * string str = pt.toString();
 * ~~~
 */
   std::string toString() const;


/* Private section */

/**********************************************************************/
/* Note: Everything below this point in this class is logically part  */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

private:

/* Instance variables */

   double x;                       /* The x-coordinate of the point */
   double y;                       /* The y-coordinate of the point */

/* Friend declarations */

   friend bool operator==(const GPoint & p1, const GPoint & p2);
   friend bool operator!=(const GPoint & p1, const GPoint & p2);
   friend int hashCode(const GPoint & pt);

};


/**
 * This class contains real-valued width and height fields.  It is used
 * to represent the size of a graphical object.
 */
class GDimension {

public:

/** \_overload */
   GDimension();
/**
 * Creates a `%GDimension` object with the specified
 * <code>width</code> and <code>height</code> coordinates.  If the
 * coordinates are not supplied, the default constructor sets these
 * fields to 0.
 *
 * Sample usages:
 * ~~~
 * GDimension empty;
 * GDimension dim(width, height);
 * ~~~
 */ 
   GDimension(double width, double height);


/**
 * Returns the width component of this GDimension object.
 *
 * Sample usage:
 * ~~~
 * double width = dim.getWidth();
 * ~~~
 */
   double getWidth() const;


/**
 * Returns the height component of this GDimension object.
 *
 * Sample usage:
 * ~~~
 * double height = dim.getHeight();
 * ~~~
 */
   double getHeight() const;


/**
 * Returns a printable string representation of this GDimension object.
 *
 * Sample usage:
 * ~~~
 * string str = dim.toString();
 * ~~~
 */
   std::string toString() const;


/* Private section */

/**********************************************************************/
/* Note: Everything below this point in this class is logically part  */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

private:

/* Instance variables */

   double width;                   /* The width of the GDimension       */
   double height;                  /* The height of the GDimension      */

/* Friend declarations */

   friend bool operator==(const GDimension & d1, const GDimension & d2);
   friend bool operator!=(const GDimension & d1, const GDimension & d2);
   friend int hashCode(const GDimension & dim);

};


/**
 * This class contains real-valued \em x, \em y, \em width, and \em height
 * fields.  It is used to represent the bounding box of a graphical object.
 * The \em x and \em y values specify the location of the upper-left corner
 * of the rectangle.
 */
class GRectangle {

public:

/** \_overload */
   GRectangle();
/**
 * Creates a `%GRectangle` object with the specified components.
 * If no parameters are not supplied, the default constructor sets
 * these fields to 0.
 *
 * Sample usages:
 * ~~~
 * GRectangle empty;
 * GRectangle r(x, y, width, height);
 * ~~~
 */
   GRectangle(double x, double y, double width, double height);


/**
 * Returns the \em x component of this rectangle.
 *
 * Sample usage:
 * ~~~
 * double x = r.getX();
 * ~~~
 */
   double getX() const;


/**
 * Returns the \em y component of this rectangle.
 *
 * Sample usage:
 * ~~~
 * double y = pt.getY();
 * ~~~
 */
   double getY() const;


/**
 * Returns the \em width component of this rectangle.
 *
 * Sample usage:
 * ~~~
 * double width = r.getWidth();
 * ~~~
 */
   double getWidth() const;


/**
 * Returns the \em height component of this rectangle.
 *
 * Sample usage:
 * ~~~
 * double height = pt.getHeight();
 * ~~~
 */
   double getHeight() const;


/**
 * Returns \c true if this rectangle is empty.
 *
 * Sample usage:
 * ~~~
 * if (r.isEmpty()) ...
 * ~~~
 */
   bool isEmpty() const;


/** \_overload */
   bool contains(GPoint pt) const;
/**
 * Returns \c true if the rectangle contains the given point,
 * which may be specified either as a GPoint or as separate
 * \em x and \em y coordinates.
 *
 * Sample usages:
 * ~~~
 * if (r.contains(pt)) ...
 * if (r.contains(x, y)) ...
 * ~~~
 */
   bool contains(double x, double y) const;


/**
 * Returns a printable string representation of this rectangel.
 *
 * Sample usage:
 * ~~~
 * string str = r.toString();
 * ~~~
 */
   std::string toString() const;


/* Private section */

/**********************************************************************/
/* Note: Everything below this point in this class is logically part  */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

private:

/* Instance variables */

   double x;                       /* The x-coordinate of the rectangle */
   double y;                       /* The y-coordinate of the rectangle */
   double width;                   /* The width of the rectangle        */
   double height;                  /* The height of the rectangle       */

/* Friend declarations */

   friend bool operator==(const GRectangle & r1, const GRectangle & r2);
   friend bool operator!=(const GRectangle & r1, const GRectangle & r2);
   friend int hashCode(const GRectangle & r);

};


/*
 * Free functions
 * --------------
 * This section of the interface declares the insertion, comparison,
 * and hashCode functions for the geometric types.
 */

std::ostream & operator<<(std::ostream & os, const GPoint & pt);
bool operator==(const GPoint & p1, const GPoint & p2);
bool operator!=(const GPoint & p1, const GPoint & p2);
int hashCode(const GPoint & pt);

std::ostream & operator<<(std::ostream & os, const GDimension & dim);
bool operator==(const GDimension & d1, const GDimension & d2);
bool operator!=(const GDimension & d1, const GDimension & d2);
int hashCode(const GDimension & dim);

std::ostream & operator<<(std::ostream & os, const GRectangle & rect);
bool operator==(const GRectangle & r1, const GRectangle & r2);
bool operator!=(const GRectangle & r1, const GRectangle & r2);
int hashCode(const GRectangle & r);

#endif
