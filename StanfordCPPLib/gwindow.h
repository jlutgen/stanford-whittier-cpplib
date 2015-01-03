/**
 * @file gwindow.h
 *
 * This file exports the GWindow class, which supports
 * drawing graphical objects on the screen.
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

#ifndef _gwindow_h
#define _gwindow_h

#include <string>
#include "gtypes.h"
#include "vector.h"

class GCompound;
class GInteractor;
class GLabel;
class GObject;

/*
 * Friend type: GWindowData
 * ------------------------
 * This block contains all data pertaining to the window.  Shallow copying
 * of the <code>GWindow</code> object ensures that all copies refer to the
 * same onscreen window.
 */

struct GWindowData {
   double windowWidth;
   double windowHeight;
   std::string windowTitle;
   std::string color;
   bool visible;
   GCompound *top;
};

/**
 * \class GWindow
 *
 * This class represents a graphics window that supports simple graphics.
 * Each `%GWindow` consists of two layers.  The background layer
 * provides a surface for drawing static pictures that involve no animation.
 * Graphical objects drawn in the background layer are persistent and do
 * not require the client to update the contents of the window.  The
 * foreground layer contains graphical objects that are redrawn as necessary.
 *
 * The `%GWindow` class includes several methods that draw
 * lines, rectangles, and ovals on the background layer without making
 * use of the facilities of the \ref gobjects.h interface.  For
 * example, the following program draws a diamond, rectangle, and oval
 * at the center of the window.
 *
 * ~~~
 *    int main() {
 *       GWindow gw;
 *       cout << "This program draws a diamond, rectangle, and oval." << endl;
 *       double width = gw.getWidth();
 *       double height = gw.getHeight();
 *       gw.drawLine(0, height / 2, width / 2, 0);
 *       gw.drawLine(width / 2, 0, width, height / 2);
 *       gw.drawLine(width, height / 2, width / 2, height);
 *       gw.drawLine(width / 2, height, 0, height / 2);
 *       gw.setColor("BLUE");
 *       gw.fillRect(width / 4, height / 4, width / 2, height / 2);
 *       gw.setColor("GRAY");
 *       gw.fillOval(width / 4, height / 4, width / 2, height / 2);
 *       return 0;
 *    }
 * ~~~
 *
 * A `%GWindow` object may be freely copied, after which all
 * copies refer to the same window.
 */
class GWindow {

public:

/** \_overload */
   GWindow();
/**
 * Creates a new window. The default constructor creates a window
 * of a default size. The second version allows the client specify
 * the width and height in pixels.
 *
 * Sample usages:
 * ~~~
 * GWindow gw;
 * GWindow gw(width, height);
 * ~~~
 */ 
   GWindow(double width, double height);


/**
 * Reclaims the internal storage for the window.  Note that the window
 * is not closed by this operation, but persists until it is closed by
 * the client or the user exits the program.
 */
   virtual ~GWindow();


/**
 * Deletes this window from the screen.
 *
 * Sample usage:
 * ~~~
 * gw.close();
 * ~~~
 */
   void close();


/**
 * Asks the system to assign the keyboard focus to this window, which
 * brings it to the top and ensures that key events are delivered to
 * the window.  Clicking in the window automatically requests the focus.
 *
 * Sample usage:
 * ~~~
 * gw.requestFocus();
 * ~~~
 */
   void requestFocus();


/**
 * Clears the contents of this window.
 *
 * Sample usage:
 * ~~~
 * gw.clear();
 * ~~~
 */
   void clear();


/**
 * Determines whether this window is visible on the screen.
 *
 * Sample usage:
 * ~~~
 * gw.setVisible(flag);
 * ~~~
 */
   void setVisible(bool flag);


/**
 * Tests whether this window is visible.
 *
 * Sample usage:
 * ~~~
 * if (gw.isVisible()) ...
 * ~~~
 */
   bool isVisible();


/** \_overload */
   void drawLine(const GPoint & p0, const GPoint & p1);
/**
 * Draws a line connecting the specified points.
 *
 * Sample usages:
 * ~~~
 * gw.drawLine(p0, p1);
 * gw.drawLine(x0, y0, x1, y1);
 * ~~~
 */
   void drawLine(double x0, double y0, double x1, double y1);


/** \_overload */
   GPoint drawPolarLine(const GPoint & p0, double r, double theta);
/**
 * Draws a line of length \em r in the direction \em theta
 * from the initial point.  The angle \em theta is measured in
 * degrees counterclockwise from the positive <i>x</i>-axis.  This method returns
 * the endpoint of the line.
 *
 * Sample usages:
 * ~~~
 * GPoint p1 = gw.drawPolarLine(p0, r, theta);
 * GPoint p1 = gw.drawPolarLine(x0, y0, r, theta);
 * ~~~
 */  
   GPoint drawPolarLine(double x0, double y0, double r, double theta);


/** \_overload */
   void drawOval(const GRectangle & bounds);
/**
 * Draws an oval with the specified bounding box.
 *
 * Sample usages:
 * ~~~
 * gw.drawOval(bounds);
 * gw.drawOval(x, y, width, height);
 * ~~~
 */ 
   void drawOval(double x, double y, double width, double height);


/** \_overload */
   void fillOval(const GRectangle & bounds);
/**
 * Draws a filled oval with the specified bounding box.
 *
 * Sample usages:
 * ~~~
 * gw.fillOval(bounds);
 * gw.fillOval(x, y, width, height);
 * ~~~
 */ 
   void fillOval(double x, double y, double width, double height);


/** \_overload */
   void drawRect(const GRectangle & bounds);
/**
 * Draws a rectangle with the specified bounds.
 *
 * Sample usages:
 * ~~~
 * gw.drawRect(bounds);
 * gw.drawRect(x, y, width, height);
 * ~~~
 */ 
   void drawRect(double x, double y, double width, double height);


/** \_overload */
   void fillRect(const GRectangle & bounds);
/**
 * Draws a filled rectangle with the specified bounds.
 *
 * Sample usages:
 * ~~~
 * gw.fillRect(bounds);
 * gw.fillRect(x, y, width, height);
 * ~~~
 */
   void fillRect(double x, double y, double width, double height);


/** \_overload */
   void setColor(std::string color);
/**
 * Sets the color used for drawing in this window.  The \em color parameter is
 * usually one of the predefined color names:
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
 * ~~~
 * gw.setColor("LIGHT_GRAY");  // these five statements have the same effect
 * gw.setColor("#bfbfbf");
 * gw.setColor("#ffbfbfbf");
 * gw.setColor(0xffbfbfbf);
 * gw.setColor(4290756543);
 * ~~~
 */
   void setColor(int color);


/**
 * Returns the current drawing color for this window
 * as a string in the form <code>"#rrggbb"</code>
 * or <code>"#aarrggbb"</code>.
 * In this string, the values <code>rr</code>, <code>gg</code>,
 * and <code>bb</code> are two-digit hexadecimal values representing
 * the red, green, and blue components of the color, respectively.
 * The value <code>aa</code> represents the alpha component of the color,
 * and is displayed only if it is not equal to the default value of
 * <code>0xff</code>, or 255. (For more about the alpha component,
 * see the description of \ref setColor.)
 *
 * Sample usage:
 * ~~~
 * string color = gw.getColor();
 * ~~~
 */
   std::string getColor();


/**
 * Returns the width of the drawing canvas of this window in pixels. The
 * drawing canvas is smaller than the entire window if any labels or interactors
 * have been placed into control strips by \ref addToRegion.
 *
 * Sample usage:
 * ~~~
 * double width = gw.getCanvasWidth();
 * ~~~
 */
   double getCanvasWidth();


/**
 * Returns the height of the drawing canvas of this window in pixels. The
 * drawing canvas is smaller than the entire window if any labels or interactors
 * have been placed into control strips by \ref addToRegion.
 *
 * Sample usage:
 * ~~~
 * double height = gw.getCanvasHeight();
 * ~~~
 */
   double getCanvasHeight();


/**
 * Returns the width of this window in pixels.
 *
 * Sample usage:
 * ~~~
 * double width = gw.getWidth();
 * ~~~
 */
   double getWidth();


/**
 * Returns the height of this window in pixels.
 *
 * Sample usage:
 * ~~~
 * double height = gw.getHeight();
 * ~~~
 */
   double getHeight();


/**
 * Schedules a repaint on this window.
 *
 * Sample usage:
 * ~~~
 * gw.repaint();
 * ~~~
 */
   void repaint();


/**
 * Sets the title of this window.
 *
 * Sample usage:
 * ~~~
 * gw.setWindowTitle(title);
 * ~~~
 */
   void setWindowTitle(std::string title);


/**
 * Returns the title of the graphics window.
 *
 * Sample usage:
 * ~~~
 * string title = gw.getWindowTitle();
 * ~~~
 */
   std::string getWindowTitle();


/** \_overload */
   void draw(const GObject & gobj);
/** \_overload */
   void draw(GObject *gobj);
/** \_overload */
   void draw(const GObject *gobj);
/** \_overload */
   void draw(GObject & gobj, double x, double y);
/**
 * Draws the given GObject on the background layer of this window.  For convenience,
 * the \em gobj parameter may be passed either as a constant
 * reference or as a pointer.  If the \em x and \em y
 * parameters are included, the object is moved to that location before
 * drawing.
 *
 * Sample usages:
 * ~~~
 * gw.draw(gobj);
 * gw.draw(gobj, x, y);
 * ~~~
 */
   void draw(GObject *gobj, double x, double y);


/** \_overload */
void add(GObject *gobj);
/**
 * Adds the given GObject to the foreground layer of this window.
 * The second version sets the location of the object to
 * (\em x, \em y) first.
 *
 * In terms of memory management, adding a `%GObject` pointer to
 * a `%GWindow` transfers control of that object from the client to
 * the window manager.  Deleting a `%GWindow` automatically deletes
 * any `%GObjects` it contains.
 *
 * Sample usages:
 * ~~~
 * gw.add(gobj);
 * gw.add(gobj, x, y);
 * ~~~
 */
   void add(GObject *gobj, double x, double y);


/**
 * Removes the given GObject from this window.
 *
 * Sample usage:
 * ~~~
 * gw.remove(gobj);
 * ~~~
 */
   void remove(GObject *gobj);


/** \_overload */
   void addToRegion(GInteractor *gobj, std::string region);
/**
 * Adds the given GInteractor or GLabel to
 * the control strip specified by the \em region parameter.
 * The \em region parameter must be one of the strings
 * <code>"NORTH"</code>, <code>"EAST"</code>, <code>"SOUTH"</code>,
 * or <code>"WEST"</code>.
 *
 * Sample usage:
 * ~~~
 * gw.addToRegion(interactor, region);
 * ~~~
 */ 
   void addToRegion(GLabel *gobj, std::string region);


/** \_overload */
   void removeFromRegion(GInteractor *gobj, std::string region);
/**
 * Removes the given GInteractor or GLabel from
 * the control strip specified by the \em region parameter.
 * The \em region parameter must be one of the strings
 * <code>"NORTH"</code>, <code>"EAST"</code>, <code>"SOUTH"</code>,
 * or <code>"WEST"</code>.
 *
 * Sample usage:
 * ~~~
 * gw.removeFromRegion(interactor, region);
 * ~~~
 */
   void removeFromRegion(GLabel *gobj, std::string region);


/**
 * Returns a pointer to the topmost GObject containing the
 * point (\em x, \em y). This method returns <code>NULL</code> if no such
 * object exists.
 *
 * Sample usage:
 * ~~~
 * GObject *gobj = gw.getGObjectAt(x, y);
 * ~~~
 */
   GObject *getGObjectAt(double x, double y);


/**
 * Sets the alignment used by the specified side region of this window
 * as specified by the string \em align.  The \em region parameter must be
 * one of the strings <code>"NORTH"</code>, <code>"EAST"</code>,
 * <code>"SOUTH"</code>, or <code>"WEST"</code> and the \em align
 * parameter must be <code>"LEFT"</code>, <code>"RIGHT"</code>, or
 * <code>"CENTER"</code>.  By default, side panels use
 * <code>CENTER</code> alignment.
 *
 * Sample usage:
 * ~~~
 * gw.setRegionAlignment(region, align);
 * ~~~
 */
   void setRegionAlignment(std::string region, std::string align);


/**
 * Checks whether the two objects refer to the same window.
 *
 * Sample usage:
 * ~~~
 * if (w1 == w2) ...
 * ~~~
 */
   bool operator==(GWindow w2);


/**
 * Checks whether the two objects refer to different windows.
 *
 * Sample usage:
 * ~~~
 * if (w1 != w2) ...
 * ~~~
 */
   bool operator!=(GWindow w2);


/* Private section */

/**********************************************************************/
/* Note: Everything below this point in the file is logically part    */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

   explicit GWindow(bool visible);
   GWindow(double width, double height, bool visible);
   GWindow(GWindowData *gwd);

private:

/* Instance variables */

   GWindowData *gwd;

/* Private methods */

   void initGWindow(double width, double height, bool visible);

   friend class Platform;
   friend class GKeyEvent;
   friend class GMouseEvent;
   friend class GWindowEvent;

};


/* Free functions */

/**
 * Issues a request to update all graphics windows.  This function
 * is called automatically when the program pauses, waits for an
 * event, waits for user input on the console, or terminates.  As
 * a result, most clients never need to call <code>repaint</code>
 * explicitly.
 *
 * Sample usage:
 * ~~~
 * repaint();
 * ~~~
 */

void repaint();


/**
 * Pauses for the specified number of milliseconds.  This function is
 * useful for animation where the motion would otherwise be too fast.
 *
 * Sample usage:
 * ~~~
 * pause(milliseconds);
 * ~~~
 */

void pause(double milliseconds);


/**
 * Returns the width of the entire display screen in pixels.
 *
 * Sample usage:
 * ~~~
 * width = getScreenWidth();
 * ~~~
 */

double getScreenWidth();


/**
 * Returns the height of the entire display screen in pixels.
 *
 * Sample usage:
 * ~~~
 * height = getScreenHeight();
 * ~~~
 */

double getScreenHeight();


/**
 * Converts a color name into an integer that encodes the
 * alpha, red, green, and blue components of the color.
 *
 * See \link GWindow::setColor(std::string) GWindow::setColor \endlink
 * for more information about colors and valid color names.
 *
 * Sample usage:
 * ~~~
 * int rgb = convertColorToRGB("ORANGE");
 * int rgb = convertColorToRGB("#ffc800");
 * int rgb = convertColorToRGB("#85ffc800");
 * ~~~
 */

int convertColorToRGB(std::string colorName);

/**
 * Converts an integer <code>rgb</code> value into a color name in the
 * form <code>"#rrggbb"</code> or <code>"#aarrggbb"</code>.
 * Each of the <code>rr</code>, <code>gg</code>, and <code>bb</code> values
 * are two-digit hexadecimal numbers indicating the intensity of red,
 * green, and blue components, respectively, of the color.
 * The value <code>aa</code> represents the alpha component of the color,
 * and is displayed only if it is not equal to the default value of
 * <code>0xff</code>, or 255. (For more about the alpha component,
 * see the description of \link GWindow::setColor(std::string)
 * GWindow::setColor\endlink.)
 *
 * Sample usage:
 * ~~~
 * int colorName = convertRGBToColor(rgb);
 * ~~~
 */

std::string convertRGBToColor(int rgb);


/**
 * Waits for a mouse click to occur anywhere in any window.
 *
 * Sample usage:
 * ~~~
 * waitForClick();
 * ~~~
 */

void waitForClick();


/**
 * Closes all graphics windows and exits from the application without
 * waiting for any additional user interaction.
 *
 * Sample usage:
 * ~~~
 * exitGraphics();
 * ~~~
 */

void exitGraphics();


#include "console.h"
#include "private/main.h"

#endif
