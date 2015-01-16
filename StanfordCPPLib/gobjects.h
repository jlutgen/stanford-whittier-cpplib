/**
 * @file gobjects.h
 *
 * @brief
 * This file exports a hierarchy of graphical shapes based on
 * the model developed for the ACM Java Graphics library.
 *
 * <div class=inset>
 * <img src="../images/GObjectHierarchy-h.png"
 *   width=768 alt="GObjectHierarchy">
 * </div>
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

#ifndef _gobjects_h
#define _gobjects_h

#include "gtypes.h"
#include "gwindow.h"
#include "vector.h"

/* Forward declaration */

class GCompound;


/**
 * \class GObject
 *
 * This abstract class is the common superclass of all graphical objects that can
 * be displayed on a graphical window.  The class `%GObject`
 * itself is an <b><i>abstract class</i></b>, which means that you are not
 * allowed to construct a `%GObject` directly but must instead
 * construct one of the concrete subclasses.
 * <include src="pictures/ClassHierarchies/GObjectHierarchy.html">
 *
 * Most methods used for graphics take a pointer to a `%GObject`
 * rather than the `%GObject` itself.  Applications that use
 * `%GObject` pointers therefore use the arrow operator
 * (<code>-></code>) to apply methods to the object pointer.
 * For examples illustrating the use of the `%GObject` class, see
 * the descriptions of the individual subclasses.
 */
class GObject {

public:

/**
 * Frees the storage for this object.
 */
   virtual ~GObject();


/**
 * Returns the <i>x</i>-coordinate of this object.
 *
 * Sample usage:
 *
 *     double x = gobj->getX();
 */
   double getX() const;


/**
 * Returns the <i>y</i>-coordinate of this object.
 *
 * Sample usage:
 *
 *     double y = gobj->getY();
 */
   double getY() const;


/**
 * Returns the location of this object as a GPoint.
 *
 * Sample usage:
 *
 *     GPoint pt = gobj->getLocation();
 */
   GPoint getLocation() const;


/** \_overload */
   void setLocation(double x, double y);
/**
 * Sets the location of this object to the specified point.
 * The second version takes a single GPoint argument instead
 * of separate \em x and \em y coordinates.
 *
 * Sample usages:
 *
 *     gobj->setLocation(x, y);
 *     gobj->setLocation(pt);
 */
   void setLocation(const GPoint & pt);


/**
 * Moves this object on the screen by the displacements
 * \em dx and \em dy.
 *
 * Sample usage:
 *
 *     gobj->move(dx, dy);
 */
   void move(double dx, double dy);


/**
 * Returns the width of this object, which is defined to be the width of
 * its bounding box.
 *
 * Sample usage:
 *
 *     double width = gobj->getWidth();
 */
   double getWidth() const;


/**
 * Returns the height of this object, which is defined to be the height
 * of its bounding box.
 *
 * Sample usage:
 *
 *     double height = gobj->getHeight();
 */
   double getHeight() const;


/**
 * Returns the size of this object as a GDimension.
 *
 * Sample usage:
 *
 *     GDimension size = gobj->getSize();
 */
   GDimension getSize() const;


/**
 * Returns the bounding box of this object, which is defined to be the
 * smallest rectangle that covers everything drawn by the figure.  The
 * coordinates of this rectangle do not necessarily match the location
 * returned by \ref getLocation.  Given a GLabel
 * object, for example, `getLocation` returns the coordinates
 * of the point on the baseline at which the string begins; the
 * \ref getBounds method, by contrast, returns a
 * rectangle that covers the entire window area occupied by the string.
 *
 * The coordinates of the bounding box are given in local coordinates
 * relative to the upper-left corner of the GCompound to which this
 * object belongs.
 *
 * Sample usage:
 *
 *     GRectangle rect = gobj->getBounds();
 */
   virtual GRectangle getBounds() const = 0;


/**
 * Sets the width of the line used to draw this object.
 *
 * Sample usage:
 *
 *     gobj->setLineWidth(lineWidth);
 */
   void setLineWidth(double lineWidth);


/**
 * Returns the width of the line used to draw this object.
 *
 * Sample usage:
 *
 *     double lineWidth = gobj->getLineWidth();
 */
   double getLineWidth() const;


/** \_overload */
   void setColor(std::string color);
/**
 * Sets the color used to display this object.  The \em color
 * string is usually one of the predefined color names:
 *
 *    <code>BLACK</code>,
 *    <code>BLUE</code>,
 *    <code>CYAN</code>,
 *    <code>DARK_GRAY</code>,
 *    <code>GRAY</code>,
 *    <code>GREEN</code>,
 *    <code>LIGHT_GRAY</code>,
 *    <code>MAGENTA</code>,
 *    <code>ORANGE</code>,
 *    <code>PINK</code>,
 *    <code>RED</code>,
 *    <code>WHITE</code>, and
 *    <code>YELLOW</code>.
 *
 * The case of the individual letters in the color name is ignored, as
 * are spaces and underscores, so that the color <code>DARK_GRAY</code>
 * can be written as <code>"Dark Gray"</code>.
 *
 * The color can also be specified as a string in the form
 * <code>"#rrggbb"</code> where <code>rr</code>, <code>gg</code>, and
 * <code>bb</code> are pairs of hexadecimal digits indicating the
 * red, green, and blue components of the color, respectively.
 *
 * A color string may also have the form <code>"#aarrggbb"</code>, where
 * the extra pair <code>aa</code> of hexadecimal digits indicates the
 * <b>alpha component</b> of the color. Like the red, green, and blue components,
 * the alpha component may have any value in the range 0..255 (`0x00`..`0xff`).
 * The alpha component determines the transparency
 * of the color; a color with an alpha value of 0 is completely transparent,
 * while one with an alpha value of 255 (`0xff`) is completely opaque. If the <code>aa</code>
 * pair is omitted from the color string, the alpha value is set to 255 by default.
 *
 * Sample usages:
 *
 *     gw.setColor("LIGHT_GRAY");  // these five statements have the same effect
 *     gw.setColor("#bfbfbf");
 *     gw.setColor("#ffbfbfbf");
 *     gw.setColor(0xffbfbfbf);
 *     gw.setColor(4290756543);
 */
   void setColor(int rgb);


/**
 * Returns the color used to display this object in the form
 * <code>"#rrggbb"</code> or <code>"#aarrggbb"</code>.
 * In this string, the values <code>rr</code>, <code>gg</code>,
 * and <code>bb</code> are two-digit hexadecimal values representing
 * the red, green, and blue components of the color, respectively.
 * The value <code>aa</code> represents the alpha component of the color,
 * and is displayed only if it is not equal to the default value of
 * <code>0xff</code>, or 255. (For more about the alpha component,
 * see the description of \ref setColor.
 *
 * Sample usage:
 *
 *     string color = gobj->getColor();
 */
   std::string getColor() const;


/** \_overload */
   void scale(double sf);
/**
 * Scales the object by the specified scale factors.  Most clients will use
 * the first form, which scales the object by \em sf in both
 * dimensions, so that invoking <code>gobj->scale(2)</code> doubles the
 * size of the object.  The second form applies independent scale factors
 * to the \em x and \em y dimensions.
 *
 * Sample usages:
 *
 *     gobj->scale(sf);
 *     gobj->scale(sx, sy);
 */ 
   void scale(double sx, double sy);


/**
 * Transforms the object by rotating it \em theta degrees
 * counterclockwise around its origin.
 *
 * Sample usage:
 *
 *     gobj->rotate(theta);
 */
   void rotate(double theta);


/**
 * Sets whether this object is visible.
 *
 * Sample usage:
 *
 *     gobj->setVisible(flag);
 */
   void setVisible(bool flag);


/**
 * Returns <code>true</code> if this object is visible.
 *
 * Sample usage:
 *
 *     if (gobj->isVisible()) ...
 */
   bool isVisible() const;


/**
 * Moves this object one step toward the front in the \em z dimension.
 * If it was already at the front of the stack, nothing happens.
 *
 * Sample usage:
 *
 *     gobj->sendForward();
 */
   void sendForward();


/**
 * Moves this object to the front of the display in the \em z dimension.
 * By moving it to the front, this object will appear to be on top of the
 * other graphical objects on the display and may hide any objects that
 * are further back.
 *
 * Sample usage:
 *
 *     gobj->sendToFront();
 */
   void sendToFront();


/**
 * Moves this object one step toward the back in the \em z dimension.
 * If it was already at the back of the stack, nothing happens.
 *
 * Sample usage:
 *
 *     gobj->sendBackward();
 */
   void sendBackward();


/**
 * Moves this object to the back of the display in the \em z dimension.
 * By moving it to the back, this object will appear to be behind the other
 * graphical objects on the display and may be obscured by other objects
 * in front.
 *
 * Sample usage:
 *
 *     gobj->sendToBack();
 */
   void sendToBack();


/** \_overload */
   virtual bool contains(double x, double y) const;
/**
 * Returns <code>true</code> if the specified point is inside this object.
 * The second version takes a single GPoint argument instead of separate
 * \em x and \em y coordinates.
 *
 * Sample usages:
 *
 *     if (gobj->contains(x, y)) ...
 *     if (gobj->contains(pt)) ...
 */   
   bool contains(GPoint pt) const;


/**
 * Returns the concrete type of this object as a string, as in
 * <code>"GOval"</code> or <code>"GLabel"</code>.
 *
 * Sample usage:
 *
 *     string type = gobj->getType();
 */
   virtual std::string getType() const = 0;


/**
 * Returns a printable string representation of this object.
 *
 * Sample usage:
 *
 *     string s = gobj->toString();
 */
   virtual std::string toString() const = 0;


/**
 * Returns a pointer to the GCompound that contains this
 * object.  Every GWindow is initialized to contain a single
 * GCompound that is aligned with the window.  Adding
 * objects to the window adds them to that GCompound,
 * which means that every object you add to the window has a parent.
 * Calling <code>getParent</code> on the top-level GCompound
 * returns <code>NULL</code>.
 *
 * Sample usage:
 *
 *     GCompound *parent = gobj->getParent();
 */
   GCompound *getParent() const;


/* Private section */

private:
   const GObject & operator=(const GObject & src) { return *this; }
   GObject(const GObject & src) { }

/* Instance variables */

protected:
   double x;                       /* The x coordinate of the origin     */
   double y;                       /* The y coordinate of the origin     */
   double lineWidth;               /* The width of the line in pixels    */
   std::string color;              /* The color of this object            */
   bool visible;                   /* Indicates if object is visible     */
   bool transformed;               /* Indicates if object is transformed */
   GCompound *parent;              /* Pointer to the parent              */

protected:
   GObject();

   friend class GArc;
   friend class GButton;
   friend class GCheckBox;
   friend class GChooser;
   friend class GCompound;
   friend class GImage;
   friend class GInteractor;
   friend class GLabel;
   friend class GLine;
   friend class GOval;
   friend class GPolygon;
   friend class GRect;
   friend class GRoundRect;
   friend class GSlider;
   friend class GTextField;
   friend class G3DRect;

};


/**
 * \class GRect
 *
 * This graphical object subclass represents a graphical object whose appearance consists of
 * a rectangular box.  For example, the following code adds a filled, red
 * 200-by-100 rectangle
 * at the upper-left corner of the graphics window:
 *
 * ~~~
 *    int main() {
 *       GWindow gw;
 *       cout << "This program draws a red rectangle at (0, 0)." << endl;
 *       GRect *rect = new GRect(0, 0, 200, 100);
 *       rect->setFilled(true);
 *       rect->setColor("RED");
 *       gw.add(rect);
 *       return 0;
 *    }
 * ~~~
 */
class GRect : public GObject {

public:

/** \_overload */
   GRect(double width, double height);
/**
 * Constructs a rectangle with the specified width and height.  The first
 * form is positioned at the origin; the second at the coordinates
 * given by \em x and \em y.
 *
 * Sample usages:
 *
 *     GRect *rect = new GRect(width, height);
 *     GRect *rect = new GRect(x, y, width, height);
 */
   GRect(double x, double y, double width, double height);


/**
 * Frees any resources maintained by this object.
 */
   virtual ~GRect();


/** \_overload */
   void setSize(double width, double height);
/**
 * Changes the size of this rectangle to the specified width and height.
 * The second version takes a single GDimension parameter instead of
 * separate width and height values.
 *
 * Sample usages:
 *
 *     rect->setSize(width, height);
 *     rect->setSize(size);
 */
   void setSize(const GDimension & size);


/** \_overload */
   void setBounds(double x, double y, double width, double height);
/**
 * Changes the bounds of this rectangle to the specified values.
 * The second version takes a single GRectangle parameter.
 *
 * Sample usages:
 *
 *     rect->setBounds(x, y, width, height);
 *     rect->setBounds(size);
 */
   void setBounds(const GRectangle & size);


/**
 * Sets the fill status for this rectangle, where <code>false</code> is
 * outlined and <code>true</code> is filled.
 *
 * Sample usage:
 *
 *     rect->setFilled(flag);
 */
   void setFilled(bool flag);


/**
 * Returns <code>true</code> if this rectangle is filled.
 *
 * Sample usage:
 *
 *     if (rect->isFilled()) ...
 */
   bool isFilled() const;


/** \_overload */
   void setFillColor(std::string color);
/**
 * Sets the color used to display the filled region of this rectangle.
 *
 * Colors may specified as strings or integers as described in the
 * documentation for the \ref setColor method.
 *
 * Sample usage:
 *
 *     rect->setFillColor(color);
 */
   void setFillColor(int rgb);


/**
 * Returns the color used to display the filled region of this rectangle.  If
 * none has been set, this method returns the empty string.
 *
 * Please see \ref getColor for more information about the format of the string
 * returned by this method.
 *
 * Sample usage:
 *
 *     string color = rect->getFillColor();
 */
   std::string getFillColor() const;


/* Prototypes for the virtual methods */

   virtual GRectangle getBounds() const;
   virtual std::string getType() const;
   virtual std::string toString() const;

protected:

/* Instance variables */

   double width;           /* The width of the rectangle              */
   double height;          /* The height of the rectangle             */
   bool fillFlag;          /* Indicates whether this object is filled  */
   std::string fillColor;  /* Color used to fill this object           */

/* Protected methods */

   GRect();
   virtual void create(double width, double height);

};


/**
 * \class GRoundRect
 *
 * This graphical object subclass represents a graphical object having the appearance
 * of a rectangular box with rounded corners.
 */
class GRoundRect : public GRect {

public:

/** \_overload */
   GRoundRect(double width, double height);
/** \_overload */
   GRoundRect(double width, double height, double corner);
/** \_overload */
   GRoundRect(double x, double y, double width, double height);
/**
 * Constructs a new rounded rectangle at the origin with the
 * specified width and height.  In the alternative versions, the
 * the \em x and \em y parameters specify the location of the
 * upper-left corner of the rectangle, while the \em corner parameter
 * specifies the diameter of the arc forming the rounded corners.
 *
 * Sample usages:
 *
 *     GRoundRect *rect = new GRoundRect(width, height);
 *     GRoundRect *rect = new GRoundRect(width, height, corner);
 *     GRoundRect *rect = new GRoundRect(x, y, width, height);
 *     GRoundRect *rect = new GRoundRect(x, y, width, height, corner);
 */
   GRoundRect(double x, double y, double width, double height, double corner);


/**
 * Frees any resources maintained by this object.
 */
   virtual ~GRoundRect();


/* Prototypes for the virtual methods */

   virtual std::string getType() const;
   virtual std::string toString() const;

protected:

   double corner;

/* Protected methods */

   void createRoundRect(double width, double height, double corner);

};


/**
 * \class G3DRect
 *
 * This graphical object subclass represents a rectangular box that can
 * be drawn in a way that suggests that it is raised above the background.
 */
class G3DRect : public GRect {

public:

/** \_overload */
   G3DRect(double width, double height);
/** \_overload */
   G3DRect(double width, double height, bool raised);
/** \_overload */
   G3DRect(double x, double y, double width, double height);
/**
 * Constructs a new 3D rectangle with the specified width and height at the origin.
 * In the alternative versions, the
 * the \em x and \em y parameters specify the location of the
 * upper-left corner of the rectangle, while  the \em raised parameter
 * determines whether the rectangle should be drawn with highlights that
 * suggest that it is raised above the background.
 *
 * Sample usages:
 *
 *     G3DRect *rect = new G3DRect(width, height);
 *     G3DRect *rect = new G3DRect(width, height, raised);
 *     G3DRect *rect = new G3DRect(x, y, width, height);
 *     G3DRect *rect = new G3DRect(x, y, width, height, raised);
 */
   G3DRect(double x, double y, double width, double height, bool raised);


/**
 * Frees any resources maintained by this object.
 */
   virtual ~G3DRect();


/**
 * Requests that this rectangle should appear raised.
 *
 * Sample usage:
 *
 *     rect->setRaised(raised);
 */
   void setRaised(bool raised);


/**
 * Returns <code>true</code> if this rectangle appears raised.
 *
 * Sample usage:
 *
 *     if (rect->isRaised()) ...
 */
   bool isRaised() const;


/* Prototypes for the virtual methods */

   virtual std::string getType() const;
   virtual std::string toString() const;

protected:

   bool raised;

/* Protected methods */

   void create3DRect(double width, double height, bool raised);

};


/**
 * \class GOval
 *
 * This graphical object subclass represents an oval inscribed in
 * a rectangular box.  For example, the following code displays a
 * filled green oval inscribed in the graphics window:
 *
 * ~~~
 *    int main() {
 *       GWindow gw;
 *       cout << "This program draws a green oval filling the window." << endl;
 *       GOval *oval = new GOval(gw.getWidth(), gw.getHeight());
 *       oval->setFilled(true);
 *       oval->setColor("GREEN");
 *       gw.add(oval);
 *       return 0;
 *    }
 * ~~~
 */
class GOval : public GObject {

public:

/** \_overload */
   GOval(double width, double height);
/**
 * Constructs a new oval with the specified bounding rectangle.  The
 * first form is positioned at the origin; the second at the coordinates
 * given by \em x and \em y.
 *
 * Sample usages:
 *
 *     GOval *oval = new GOval(width, height);
 *     GOval *oval = new GOval(x, y, width, height);
 */
   GOval(double x, double y, double width, double height);


/**
 * Frees any resources maintained by this object.
 */
   virtual ~GOval();


/** \_overload */
  void setSize(const GDimension & size);
/**
 * Changes the size of the bounding rectangle of this oval to the specified width
 * and height.
 *
 * Sample usages:
 *
 *     oval->setSize(size);
 *     oval->setSize(width, height);
 */
   void setSize(double width, double height);


/** \_overload */
   void setBounds(const GRectangle & size);
/**
 * Changes the bounding rectangle of this oval to the
 * rectangle specified by the argument(s).
 *
 * Sample usages:
 *
 *     oval->setBounds(rect);
 *     oval->setBounds(x, y, width, height);
 */
   void setBounds(double x, double y, double width, double height);


/**
 * Sets the fill status for this oval, where <code>false</code> is
 * outlined and <code>true</code> is filled.
 *
 * Sample usage:
 *
 *     oval->setFilled(flag);
 */
   void setFilled(bool flag);


/**
 * Returns <code>true</code> if this oval is filled.
 *
 * Sample usage:
 *
 *     if (oval->isFilled()) ...
 */
   bool isFilled() const;


/** \_overload */
   void setFillColor(std::string color);
/**
 * Sets the color used to display the filled region of this oval.
 *
 * Colors may specified as strings or integers as described in the
 * documentation for the \ref setColor method.
 *
 * Sample usage:
 *
 *     oval->setFillColor(color);
 */
   void setFillColor(int rgb);


/**
 * Returns the color used to display the filled region of this oval.  If
 * none has been set, this method returns the empty string.
 *
 * Please see \ref getColor for more information about the format of the string
 * returned by this method.
 *
 * Sample usage:
 *
 *     string color = oval->getFillColor();
 */
   std::string getFillColor() const;


/* Prototypes for the virtual methods */

   virtual GRectangle getBounds() const;
   virtual bool contains(double x, double y) const;
   virtual std::string getType() const;
   virtual std::string toString() const;

protected:

/* Instance variables */

   double width;           /* The width of the bounding oval          */
   double height;          /* The height of the bounding oval         */
   bool fillFlag;          /* Indicates whether this object is filled  */
   std::string fillColor;  /* Color used to fill this object           */

/* Protected methods */

   void create(double width, double height);

};


/**
 * \class GArc
 *
 * This graphical object subclass represents an elliptical arc.  The
 * arc is specified by the following parameters:
 *
 * <ul>
 *   <li>The coordinates of the bounding rectangle (<code>x</code>,
 *       <code>y</code>, <code>width</code>, <code>height</code>)
 *   <li>The angle at which the arc starts (<code>start</code>)
 *   <li>The number of degrees that the arc covers (<code>sweep</code>)
 * </ul>
 *
 * All angles in a `%GArc` description are measured in
 * degrees moving counterclockwise from the positive <i>x</i>-axis.  Negative
 * values for either \em start or \em sweep indicate
 * motion in a clockwise direction.
 *
 * The following diagram illustrates the interpretation of these parameters
 * for the arc shown in <strong class=redfont>red</strong>:
 * <div class=inset>
 * <img src="../images/GArcGeometry.png" width=260 alt="GArcGeometry">
 * </div>
 */
class GArc : public GObject {

public:

/** \_overload */
   GArc(double width, double height, double start, double sweep);
/**
 * Creates a new `%GArc` object consisting of an elliptical arc.
 * The first form creates an arc whose origin is the point
 * (0,&nbsp;0); the second form positions the arc at the
 * point (\em x, \em y).
 *
 * The following screenshots provide examples of how the
 * `%GArc` constructor interprets the \em start and \em sweep parameters:
 * <div class=inset>
 * <img src="../images/GArcExamples.png" width=700 alt="GArcExamples">
 * </div>
 * In the code fragments underneath each of these diagrams, \em r
 * is the radius of the circular arc, and \em cx and \em cy
 * are the coordinates of the center or the window.
 *
 * Sample usages:
 *
 *     GArc *arc = new GArc(width, height, start, sweep);
 *     GArc *arc = new GArc(x, y, width, height, start, sweep);
 */
   GArc(double x, double y, double width, double height,
                            double start, double sweep);


/**
 * Sets the starting angle for this arc.
 *
 * Sample usage:
 *
 *     arc->setStartAngle(start);
 */
   void setStartAngle(double start);


/**
 * Returns the starting angle for this arc.
 *
 * Sample usage:
 *
 *     double angle = arc->getStartAngle();
 */
   double getStartAngle() const;


/**
 * Sets the sweep angle for this arc.
 *
 * Sample usage:
 *
 *     arc->setSweepAngle(start);
 */
   void setSweepAngle(double start);


/**
 * Returns the sweep angle for this arc.
 *
 * Sample usage:
 *
 *     double angle = arc->getSweepAngle();
 */
   double getSweepAngle() const;


/**
 * Returns the point at which this arc starts.
 *
 * Sample usage:
 *
 *     GPoint pt = arc->getStartPoint();
 */
   GPoint getStartPoint() const;


/**
 * Returns the point at which this arc ends.
 *
 * Sample usage:
 *
 *     GPoint pt = arc->getEndPoint();
 */
   GPoint getEndPoint() const;


/** \_overload */
   void setFrameRectangle(const GRectangle & rect);
/**
 * Changes the rectangle used to frame this arc.
 *
 * Sample usages:
 *
 *     arc->setFrameRectangle(rect);
 *     arc->setFrameRectangle(x, y, width, height);
 */
   void setFrameRectangle(double x, double y, double width, double height);


/**
 * Returns the rectangle used to frame this arc.
 *
 * Sample usage:
 *
 *     GRectangle rect = arc->getFrameRectangle();
 */
   GRectangle getFrameRectangle() const;


/**
 * Sets the fill status for the arc, where \c false is
 * outlined and \c true is filled.  If a `%GArc` is
 * unfilled, the figure consists only of the arc itself.  If a
 * `%GArc` is filled, the figure consists of the
 * pie-shaped wedge formed by connecting the endpoints of the arc to
 * the center.  As an example, the following program draws a 270-degree
 * arc starting at 45 degrees, filled in yellow, which resembles the character
 * in the PacMan video game:
 *
 * ~~~
 *    int main() {
 *       GWindow gw;
 *       cout << "This program draws the PacMan character." << endl;
 *       double cx = gw.getWidth() / 2;
 *       double cy = gw.getHeight() / 2;
 *       double r = 25;
 *       GArc *pacman = new GArc(cx - r, cy - r, 2 * r, 2 * r, 45, 270);
 *       pacman->setFilled(true);
 *       pacman->setFillColor("YELLOW");
 *       gw.add(pacman);
 *       return 0;
 *    }
 * ~~~
 * The program results in the following picture:
 * <div class=inset>
 * <img src="../images/PacMan.png" width=322 alt="PacMan">
 * </div>
 *
 * Sample usage:
 *
 *     arc->setFilled(flag);
 */
   void setFilled(bool flag);


/**
 * Returns \c true if this arc is filled.
 *
 * Sample usage:
 *
 *     if (arc->isFilled()) ...
 */
   bool isFilled() const;


/** \_overload */
   void setFillColor(std::string color);
/**
 * Sets the color used to display the filled region of this arc.
 *
 * Colors may specified as strings or integers as described in the
 * documentation for the \ref setColor method.
 *
 * Sample usage:
 *
 *     arc->setFillColor(color);
 */
   void setFillColor(int rgb);


/**
 * Returns the color used to display the filled region of this arc.  If
 * none has been set, this method returns the empty string.
 *
 * Please see \ref getColor for more information about the format of the string
 * returned by this method.
 *
 * Sample usage:
 *
 *     string color = arc->getFillColor();
 */
   std::string getFillColor() const;


/* Prototypes for the virtual methods */

   virtual GRectangle getBounds() const;
   virtual bool contains(double x, double y) const;
   virtual std::string getType() const;
   virtual std::string toString() const;

private:

   GPoint getArcPoint(double theta) const;
   bool containsAngle(double theta) const;
   void create(double width, double height, double start, double sweep);

/* Instance variables */

   double start;                   /* Starting angle of the arc       */
   double sweep;                   /* How many degrees the arc runs   */
   double frameWidth;              /* The width of the bounding box   */
   double frameHeight;             /* The height of the bounding box  */
   std::string fillColor;          /* The color of the interior       */
   bool fillFlag;                  /* Indicates if the arc is filled  */

};


/**
 * \class GCompound
 *
 * This graphical object subclass represents a collection
 * of other graphical objects.  Once assembled, the internal objects
 * can be manipulated as a unit.  A `%GCompound` keeps
 * track of its own position, and all objects within it are drawn
 * relative to that location.
 */
class GCompound : public GObject {

public:

/**
 * Creates a GCompound object with no internal components.
 *
 * Sample usage:
 *
 *     GCompound *comp = new GCompound();
 */
   GCompound();


/** \_overload */
   void add(GObject *gobj);
/**
 * Adds a new graphical object to this compound.  The second
 * form moves the object to the point (\em x, \em y) first.
 *
 * Sample usages:
 *
 *     comp->add(gobj);
 *     comp->add(gobj, x, y);
 */
   void add(GObject *gobj, double x, double y);


/**
 * Removes the specified object from this compound.
 *
 * Sample usage:
 *
 *     comp->remove(gobj);
 */
   void remove(GObject *gobj);


/**
 * Removes all graphical objects from this compound.
 *
 * Sample usage:
 *
 *     comp->removeAll();
 */
   void removeAll();


/**
 * Returns the number of graphical objects stored in this
 * compound.
 *
 * Sample usage:
 *
 *     int n = comp->getElementCount();
 */
   int getElementCount();


/**
 * Returns a pointer to the graphical object at the specified index in
 * this compound, numbering from back to front in the \em z dimension.
 *
 * Sample usage:
 *
 *     GObject *gobj = comp->getElement(index);
 */
   GObject *getElement(int index);


/* Prototypes for the virtual methods */

   virtual GRectangle getBounds() const;
   virtual bool contains(double x, double y) const;
   virtual std::string getType() const;
   virtual std::string toString() const;

private:
   void sendForward(GObject *gobj);
   void sendToFront(GObject *gobj);
   void sendBackward(GObject *gobj);
   void sendToBack(GObject *gobj);
   int findGObject(GObject *gobj);
   void removeAt(int index);

/* Instance variables */

   Vector<GObject *> contents;

/* Friend declarations */

   friend class GObject;

};


/**
 * \class GImage
 *
 * This graphical object subclass represents an image from a file.
 * For example, the following code displays a `%GImage`
 * containing the Whittier College logo at the center of the window, assuming
 * that the image file <code>WhittierLogo.png</code> exists, either in
 * the current directory or an <code>images</code> subdirectory:
 *
 * ~~~
 *    int main() {
 *       GWindow gw;
 *       cout << "This program draws the Whittier College logo." << endl;
 *       GImage *logo = new GImage("WhittierLogo.png");
 *       double x = (gw.getWidth() - logo->getWidth()) / 2;
 *       double y = (gw.getHeight() - logo->getHeight()) / 2;
 *       gw.add(logo, x, y);
 *       return 0;
 *    }
 * ~~~
 */
class GImage : public GObject {

public:

/** \_overload */
   GImage(std::string filenameOrURL);
/**
 * Constructs a new image by loading the image from the specified
 * file or URL. If \em filenameOrURL is a relative filename, the current directory
 * is searched first, followed by any subdirectory named
 * <code>images</code>.  By default, the upper left corner of the image
 * appears at the origin; the second form automatically sets the location
 * to the point (\em x, \em y).
 *
 * The file must have one of the following formats:
 * <ul>
 *  <li>Bitmap (BMP) format
 *  <li>Graphics Interchange Format (GIF)
 *  <li>Joint Photographic Experts Group (JPEG) format
 *  <li>Portable Network Graphics (PNG) format
 * </ul>
 *
 * This method throws an error if the file could not be found, could not be
 * opened for reading, or is not a valid image file in one of the supported formats.
 *
 * Sample usages:
 *
 *     GImage *image = new GImage(filename);
 *     GImage *image = new GImage(filename, x, y);
 */
   GImage(std::string filename, double x, double y);


/* Prototypes for the virtual methods */

   virtual GRectangle getBounds() const;
   virtual std::string getType() const;
   virtual std::string toString() const;

private:
   std::string filename;
   double width;
   double height;

   void create(std::string filename);

};


/**
 * \class GLabel
 *
 * This graphical object subclass represents a text string.  For
 * example, the following code adds a `%GLabel` containing
 * the string <code>"hello, world"</code> to the center of the window:
 *
 * ~~~
 *    int main() {
 *       GWindow gw;
 *       cout << "This program draws the 'hello, world' message." << endl;
 *       GLabel *label = new GLabel("hello, world");
 *       label->setFont("SansSerif-18");
 *       double x = (gw.getWidth() - label->getWidth()) / 2;
 *       double y = (gw.getHeight() + label->getFontAscent()) / 2;
 *       gw.add(label, x, y);
 *       return 0;
 *    }
 * ~~~
 *
 * Controlling the appearance and positioning of a <code>GLabel</code>
 * depends on understanding the following terms:
 *
 * <ul>
 *   <li>The <b><i>baseline</i></b> is the horizontal line on which the
 *       characters rest.
 *   <li>The <b><i>origin</i></b> is the point on the baseline at which
 *       the label begins.
 *   <li>The <b><i>height</i></b> is the distance that separate two
 *       successive lines.
 *   <li>The <b><i>ascent</i></b> is the maximum distance a character
 *       in this font extends above the baseline.
 *   <li>The <b><i>descent</i></b> is the maximum distance a character
 *       in this font extends below the baseline.
 * </ul>
 *
 * The following diagram illustrates the interpretation of these terms:
 * <div class=inset>
 *  <img src="../images/GLabelGeometry.png" width=625 alt="GLabelGeometry">
 * </div>
 */
class GLabel : public GObject {

public:

/** \_overload */
   GLabel(std::string str);
/**
 * Creates a `%GLabel' object containing the specified string.
 * By default, the baseline of the first character appears at the origin;
 * the second form resets the location of the
 * label to the point (\em x, \em y).
 *
 * Sample usages:
 *
 *     GLabel *label = new GLabel(str);
 *     GLabel *label = new GLabel(str, x, y);
 */
   GLabel(std::string str, double x, double y);


/**
 * Changes the font used to display this label as specified by
 * the string \em font, which has the following format:
 *
 *      `family-style-size`
 *
 * where both \em style and \em size are optional.
 * If any of these elements is missing or specified as an asterisk,
 * the current value is retained.
 *
 * Sample usages:
 *
 *     label->setFont("Serif-Plain-18");
 *     label->setFont("Monospaced-Bold-*);
 */
   void setFont(std::string font);


/**
 * Returns the current font used for this label.
 *
 * Sample usage:
 *
 *     string font = label->getFont();
 */
   std::string getFont() const;


/**
 * Changes the string stored in this label, so that
 * a new text string appears on the display.
 *
 * Sample usage:
 *
 *     label->setLabel(str);
 */
   void setLabel(std::string str);


/**
 * Returns the string displayed by this label.
 *
 * Sample usage:
 *
 *     string str = label->getLabel();
 */
   std::string getLabel() const;


/**
 * Returns the maximum distance that strings in this label's font extend above
 * the baseline.
 *
 * Sample usage:
 *
 *     double ascent = label->getFontAscent();
 */
   double getFontAscent() const;


/**
 * Returns the maximum distance that strings in this label's font descend below
 * the baseline.
 *
 * Sample usage:
 *
 *     double descent = label->getFontDescent();
 */
   double getFontDescent() const;


/* Prototypes for the virtual methods */

   virtual GRectangle getBounds() const;
   virtual std::string getType() const;
   virtual std::string toString() const;

private:

/* Instance variables */

   std::string str;                /* The string displayed by the label */
   std::string font;               /* The font string of the label      */
   double width;                   /* Width of the bounding box         */
   double height;                  /* Height of the bounding box        */
   double ascent;                  /* Font ascent                       */
   double descent;                 /* Font descent                      */

   void create(const std::string & str);

};


/**
 * \class GLine
 *
 * This graphical object subclass represents a line segment.  For
 * example, the following code adds lines that mark the diagonals
 * of the graphics window:
 *
 * ~~~
 *    int main() {
 *       GWindow gw;
 *       cout << "This program draws the diagonals on the window." << endl;
 *       gw.add(new GLine(0, 0, gw.getWidth(), gw.getHeight()));
 *       gw.add(new GLine(0, gw.getHeight(), gw.getWidth(), 0));
 *       return 0;
 *    }
 * ~~~
 */
class GLine : public GObject {

public:

/**
 * Constructs a line segment with the given endpoints.  The point
 * (\em x0,&nbsp;\em y0) defines the start of the
 * line and the point (\em x1,&nbsp;\em y1) defines
 * the end.
 *
 * Sample usage:
 *
 *     GLine *gline = new GLine(x0, y0, x1, y1);
 */
   GLine(double x0, double y0, double x1, double y1);


/**
 * Sets the initial point of this line to (\em x,&nbsp;\em y),
 * leaving the end point unchanged.  This method is therefore different from
 * \ref setLocation, which moves both endpoints of the line segment.
 *
 * Sample usage:
 *
 *     line->setStartPoint(x, y);
 */
   void setStartPoint(double x, double y);


/**
 * Returns the point at which this line starts.
 *
 * Sample usage:
 *
 *     GPoint pt = line->getStartPoint();
 */
   GPoint getStartPoint() const;


/**
 * Sets the end point of this line to (\em x, \em y),
 * leaving the start point unchanged.  This method is therefore different from
 * \ref setLocation, which moves both endpoints of the line segment.
 *
 * Sample usage:
 *
 *     line->setEndPoint(x, y);
 */
   void setEndPoint(double x, double y);


/**
 * Returns the point at which this line ends.
 *
 * Sample usage:
 *
 *     GPoint pt = line->getEndPoint();
 */
   GPoint getEndPoint() const;


/* Prototypes for the virtual methods */

   virtual GRectangle getBounds() const;
   virtual bool contains(double x, double y) const;
   virtual std::string getType() const;
   virtual std::string toString() const;

protected:

/* Instance variables */

   double dx;                   /* The x displacement of the line */
   double dy;                   /* The y displacement of the line */

};


/**
 * \class GPolygon
 *
 * This graphical object subclass represents a polygon.
 *
 * Conceptually, a polygon is a list of vertices, and represents a closed curve
 * formed from line segments joining consecutive vertices in cyclical fashion.
 * For example, a polygon with vertex list {\em A, \em B, <i>C</i>} has line
 * segments \em AB, \em BC, and \em CA.
 *
 * The `%GPolygon` constructor creates an
 * empty polygon (i.e., a polygon having no vertices).
 * You then need to add a sequence of vertices to the polygon using
 * one or more of the methods \ref addVertex, \ref addEdge,
 * and \ref addPolarEdge (the initial vertex must be added with \ref addVertex).
 *
 * As an example, the following code adds a filled red octagon to the center
 * of the window:
 *
 * ~~~
 *    int main() {
 *       GWindow gw;
 *       cout << "This program draws a red octagon." << endl;
 *       double edge = 75;
 *       GPolygon *stopSign = new GPolygon();
 *       stopSign->addVertex(-edge / 2, edge / 2 + edge / sqrt(2.0));
 *       for (int i = 0; i < 8; i++) {
 *          stopSign->addPolarEdge(edge, 45 * i);
 *       }
 *       stopSign->setFilled(true);
 *       stopSign->setColor("RED");
 *       gw.add(stopSign, gw.getWidth() / 2, gw.getHeight() / 2);
 *       return 0;
 *    }
 * ~~~
 *
 * The program results in the following picture:
 * <div class=inset>
 *  <img src="../images/StopSign.png" width=322 alt="StopSign">
 * </div>
 */
class GPolygon : public GObject {

public:

/**
 * Constructs a new empty polygon based at the origin.
 *
 * Sample usage:
 *
 *     GPolygon *poly = new GPolygon();
 */
   GPolygon();


/**
 * Adds a vertex at (\em x, \em y) relative to this polygon's
 * origin.
 *
 * The polygon is <b>not</b> automatically redrawn after every call to \c addVertex.
 * Call the containing <code>GWindow</code>'s \link GWindow::repaint repaint \endlink method
 * if you need the polygon to be immediately redrawn after calling this method.
 *
 * Sample usage:
 *
 *     poly->addVertex(x, y);
 */
   void addVertex(double x, double y);


/**
 * Adds an edge to this polygon. The new edge begins at the most recently added vertex
 * and ends at displacement (\em dx, \em dy) relative to that vertex.
 * In other words, if the most recently added vertex was (\em x, \em y), then the new
 * edge begins at (\em x, \em y) and ends at (\em x + \em dx, \em y + \em dy).
 *
 * The endpoint of the new edge is appended to this polygon's list of vertices.
 *
 * The polygon is <b>not</b> automatically redrawn after every call to \c addEdge.
 * Call the containing <code>GWindow</code>'s \link GWindow::repaint repaint \endlink method
 * if you need the polygon to be immediately redrawn after calling this method.
 *
 * Sample usage:
 *
 *     poly->addEdge(dx, dy);
 */
   void addEdge(double dx, double dy);


/**
 * Adds an edge specified in polar coordinates to this polygon.
 * The new edge begins at the most recently added vertex. The length
 * of the edge is given by \em r, and the edge extends in the
 * direction \em theta, measured in degrees counterclockwise
 * from the positive <i>x</i>-axis.
 *
 * The endpoint of the new edge is appended to this polygon's list of vertices.
 *
 * The polygon is <b>not</b> automatically redrawn after every call to \c addPolarEdge.
 * Call the containing <code>GWindow</code>'s \link GWindow::repaint repaint \endlink method
 * if you need the polygon to be immediately redrawn after calling this method.
 *
 * Sample usage:
 *
 *     poly->addPolarEdge(r, theta);
 */
   void addPolarEdge(double r, double theta);


/**
 * Returns the list of the vertices of this polygon.
 *
 * Sample usage:
 *
 *     Vector<GPoint> vec = poly->getVertices();
 */
   Vector<GPoint> getVertices() const;


/**
 * Sets the fill status for this polygon, where \c false means
 * "not filled"  and \c true means "filled."
 *
 * Sample usage:
 *
 *     poly->setFilled(flag);
 */
   void setFilled(bool flag);


/**
 * Returns \c true if this polygon is filled.
 *
 * Sample usage:
 *
 *     if (poly->isFilled()) ...
 */
   bool isFilled() const;


/** \_overload */
   void setFillColor(std::string color);
/**
 * Sets the color used to display the filled region of this polygon.
 *
 * Colors may specified as strings or integers as described in the
 * documentation for the \ref setColor method.
 *
 * Sample usage:
 *
 *     poly->setFillColor(color);
 */
   void setFillColor(int rgb);


/**
 * Returns the color used to display the filled region of this polygon.  If
 * none has been set, this method returns the empty string.
 *
 * Please see \ref getColor for more information about the format of the string
 * returned by this method.
 *
 * Sample usage:
 *
 *     string color = poly->getFillColor();
 */
   std::string getFillColor() const;


/* Prototypes for the virtual methods */

   virtual GRectangle getBounds() const;
   virtual bool contains(double x, double y) const;
   virtual std::string getType() const;
   virtual std::string toString() const;

private:

/* Instance variables */

   Vector<GPoint> vertices;        /* The vertices of the polygon   */
   double cx;                      /* The most recent x coordinate  */
   double cy;                      /* The most recent y coordinate  */
   std::string fillColor;          /* The color of the interior     */
   bool fillFlag;                  /* Indicates if object is filled */

};

#endif
