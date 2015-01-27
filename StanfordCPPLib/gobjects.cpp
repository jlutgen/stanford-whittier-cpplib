/*
 * File: gobjects.cpp
 * ------------------
 * This file implements the gobjects.h interface.
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

#include <cmath>
#include <iostream>
#include <sstream>
#include "gevents.h"
#include "gmath.h"
#include "gobjects.h"
#include "gtypes.h"
#include "gwindow.h"
#include "platform.h"
#include "vector.h"

static Platform *pp = getPlatform();

const double LINE_TOLERANCE = 1.5;
const double ARC_TOLERANCE = 2.5;
const double DEFAULT_CORNER = 10;
const string DEFAULT_GLABEL_FONT = "Dialog-13";

static double dsq(double x0, double y0, double x1, double y1);

double GObject::getX() const {
   return x;
}

double GObject::getY() const {
   return y;
}

GPoint GObject::getLocation() const {
   return GPoint(x, y);
}

void GObject::setLocation(const GPoint & pt) {
   setLocation(pt.getX(), pt.getY());
}

void GObject::setLocation(double x, double y) {
   this->x = x;
   this->y = y;
   pp->setLocation(this, x, y);
}

void GObject::move(double dx, double dy) {
   setLocation(x + dx, y + dy);
}

double GObject::getWidth() const {
   return getBounds().getWidth();
}

double GObject::getHeight() const {
   return getBounds().getHeight();
}

GDimension GObject::getSize() const {
   GRectangle bounds = getBounds();
   return GDimension(bounds.getWidth(), bounds.getHeight());
}

void GObject::setLineWidth(double lineWidth) {
   this->lineWidth = lineWidth;
   pp->setLineWidth(this, lineWidth);
}

double GObject::getLineWidth() const {
   return lineWidth;
}

void GObject::setColor(string color) {
   this->color = color;
   pp->setColor(this, this->color);
}

void GObject::setColor(int rgb) {
   setColor(convertRGBToColor(rgb));
}

string GObject::getColor() const {
   return convertRGBToColor(convertColorToRGB(color));
}

void GObject::scale(double sf) {
   scale(sf, sf);
}

void GObject::scale(double sx, double sy) {
   // Apply local transform
   transformed = true;
   matrix.applyScale(sx, sy);
   pp->scale(this, sx, sy);
}

void GObject::rotate(double theta) {
   // Apply local transform
   transformed = true;
   matrix.applyRotate(theta);
   pp->rotate(this, theta);
}

void GObject::setVisible(bool flag) {
   visible = flag;
   pp->setVisible(this, flag);
}

bool GObject::isVisible() const {
   return visible;
}

void GObject::sendForward() {
   GCompound *parent = getParent();
   if (parent != NULL) parent->sendForward(this);
}

void GObject::sendToFront() {
   GCompound *parent = getParent();
   if (parent != NULL) parent->sendToFront(this);
}

void GObject::sendBackward() {
   GCompound *parent = getParent();
   if (parent != NULL) parent->sendBackward(this);
}

void GObject::sendToBack() {
   GCompound *parent = getParent();
   if (parent != NULL) parent->sendToBack(this);
}

bool GObject::contains(GPoint pt) const {
   return contains(pt.getX(), pt.getY());
}

bool GObject::contains(double x, double y) const {
   if (transformed) return pp->contains(this, x, y);
   return getBounds().contains(x, y);
}

GCompound *GObject::getParent() const {
   return parent;
}

GObject::GObject() {
   x = 0;
   y = 0;
   color = "";
   lineWidth = 1.0;
   transformed = false;
   visible = true;
}

GObject::~GObject() {
   pp->deleteGObject(this);
}

/*
 * Implementation notes: GRect class
 * ---------------------------------
 * The GRect class is the most straightforward of the shape classes.
 */

GRect::GRect(double width, double height) {
   create(width, height);
}

GRect::GRect(double x, double y, double width, double height) {
   create(width, height);
   setLocation(x, y);
}

GRect::~GRect() {
   /* Empty */
}

void GRect::setSize(const GDimension & size) {
   setSize(size.getWidth(), size.getHeight());
}

void GRect::setSize(double width, double height) {
   if (transformed) error("GRect::setSize: Object has been transformed");
   this->width = width;
   this->height = height;
   pp->setSize(this, width, height);
}

void GRect::setBounds(const GRectangle & bounds) {
   setLocation(bounds.getX(), bounds.getY());
   setSize(bounds.getWidth(), bounds.getHeight());
}

void GRect::setBounds(double x, double y, double width, double height) {
   setLocation(x, y);
   setSize(width, height);
}

// computation for transformed case added by JL
GRectangle GRect::getBounds() const {
    if (!transformed)
       return GRectangle(x, y, width, height);
    GPoint vertices[4];
    vertices[0] = GPoint(0, 0); // unused!
    vertices[1] = matrix.image(0, height);
    vertices[2] = matrix.image(width, height);
    vertices[3] = matrix.image(width, 0);
    double x1, y1, x2, y2;
    x1 = x2 = y1 = y2 = 0;
    for (int i = 1; i < 4; i++) {
        double x = vertices[i].getX();
        double y = vertices[i].getY();
        if (x < x1) x1 = x;
        if (y < y1) y1 = y;
        if (x > x2) x2 = x;
        if (y > y2) y2 = y;
    }
    return GRectangle(this->x + x1, this->y + y1, x2 - x1, y2 - y1);
}

// added by JL
bool GRect::contains(double x, double y) const {
    GPoint p = matrix.preimage(x - this->x, y - this->y);
    double xx = p.getX();
    double yy = p.getY();
    return 0 < xx && xx <= width
        &&  0 < yy && yy <= height;
}

void GRect::setFilled(bool flag) {
   fillFlag = flag;
   pp->setFilled(this, flag);
}

bool GRect::isFilled() const {
   return fillFlag;
}

void GRect::setFillColor(string color) {
   fillColor = color;
   if (fillColor != "") {
      fillColor = convertRGBToColor(convertColorToRGB(color));
   }
   pp->setFillColor(this, fillColor);
}

void GRect::setFillColor(int rgb) {
   fillColor = convertRGBToColor(rgb);
   pp->setFillColor(this, fillColor);
}

string GRect::getFillColor() const {
   return fillColor;
}

string GRect::getType() const {
   return "GRect";
}

string GRect::toString() const {
   ostringstream oss;
   oss << "GRect(" << x << ", " << y << ", "
                   << width << ", " << height << ")";
   return oss.str();
}

GRect::GRect() {
   /* Called only by the GRoundRect and G3DRect subclasses */
}

void GRect::create(double width, double height) {
   this->x = 0;
   this->y = 0;
   this->width = width;
   this->height = height;
   fillFlag = false;
   fillColor = "";
   pp->createGRect(this, width, height);
}

/*
 * Implementation notes: GRoundRect class
 * --------------------------------------
 * Most of the GRoundRect class is inherited from the GRect class.
 */

GRoundRect::GRoundRect(double width, double height) {
   createRoundRect(width, height, DEFAULT_CORNER);
}

GRoundRect::GRoundRect(double width, double height, double corner) {
   createRoundRect(width, height, corner);
}

GRoundRect::GRoundRect(double x, double y, double width, double height) {
   createRoundRect(width, height, DEFAULT_CORNER);
   setLocation(x, y);
}

GRoundRect::GRoundRect(double x, double y, double width, double height,
                       double corner) {
   createRoundRect(width, height, corner);
   setLocation(x, y);
}

GRoundRect::~GRoundRect() {
   /* Empty */
}

// Rewritten by JL to handle transformed case and take corners into account
bool GRoundRect::contains(double x, double y) const {
   GPoint p = matrix.preimage(x - this->x, y - this->y);
   double xx = p.getX();
   double yy = p.getY();
   if (xx < 0 || xx > width || yy < 0 || yy > height)
       return false;

   // If corner diameter is too big, the largest sensible value is used by Java back end.
   double a = std::min(corner, width) / 2;
   double b = std::min(corner, height) / 2;

   // Get distances from nearest edges of bounding rectangle
   double dx = std::min(xx, width - xx);
   double dy = std::min(yy, height - yy);

   if (dx > a || dy > b)
       return true; // in "central cross" of rounded rect

   return (dx - a)*(dx - a)/(a*a) + (dy - b)*(dy - b)/(b*b) <= 1;
}

string GRoundRect::getType() const {
   return "GRoundRect";
}

string GRoundRect::toString() const {
   ostringstream oss;
   oss << "GRoundRect(" << x << ", " << y << ", "
                        << width << ", " << height << ", " << corner << ")";
   return oss.str();
}

void GRoundRect::createRoundRect(double width, double height, double corner) {
   this->x = 0;
   this->y = 0;
   this->width = width;
   this->height = height;
   this->corner = corner;
   fillFlag = false;
   fillColor = "";
   pp->createGRoundRect(this, width, height, corner);
}

/*
 * Implementation notes: G3DRect class
 * -----------------------------------
 * Most of the G3DRect class is inherited from the GRect class.
 */

G3DRect::G3DRect(double width, double height) {
   create3DRect(width, height, false);
}

G3DRect::G3DRect(double width, double height, bool raised) {
   create3DRect(width, height, raised);
}

G3DRect::G3DRect(double x, double y, double width, double height) {
   create3DRect(width, height, false);
   setLocation(x, y);
}

G3DRect::G3DRect(double x, double y, double width, double height,
                           bool raised) {
   create3DRect(width, height, raised);
   setLocation(x, y);
}

G3DRect::~G3DRect() {
   /* Empty */
}

void G3DRect::setRaised(bool raised) {
   this->raised = raised;
   pp->setRaised(this, raised);
}

bool G3DRect::isRaised() const {
   return raised;
}

string G3DRect::getType() const {
   return "G3DRect";
}

string G3DRect::toString() const {
   ostringstream oss;
   oss << boolalpha << "G3DRect(" << x << ", " << y << ", "
                    << width << ", " << height << ", " << raised << ")";
   return oss.str();
}

void G3DRect::create3DRect(double width, double height, bool raised) {
   this->x = 0;
   this->y = 0;
   this->width = width;
   this->height = height;
   this->raised = raised;
   fillFlag = false;
   fillColor = "";
   pp->createG3DRect(this, width, height, raised);
}

GOval::GOval(double width, double height) {
   create(width, height);
}

GOval::GOval(double x, double y, double width, double height) {
   create(width, height);
   setLocation(x, y);
}

GOval::~GOval() {
   /* Empty */
}

void GOval::setSize(const GDimension & size) {
   setSize(size.getWidth(), size.getHeight());
}

void GOval::setSize(double width, double height) {
   if (transformed) error("GOval::setSize: Object has been transformed");
   this->width = width;
   this->height = height;
   pp->setSize(this, width, height);
}

void GOval::setBounds(const GRectangle & bounds) {
   setLocation(bounds.getX(), bounds.getY());
   setSize(bounds.getWidth(), bounds.getHeight());
}

void GOval::setBounds(double x, double y, double width, double height) {
   setLocation(x, y);
   setSize(width, height);
}

// computation for transformed case added by JL
GRectangle GOval::getBounds() const {
    if (!transformed)
       return GRectangle(x, y, width, height);
    GPoint vertices[4];
    vertices[0] = GPoint(0, 0); // unused!
    vertices[1] = matrix.image(0, height);
    vertices[2] = matrix.image(width, height);
    vertices[3] = matrix.image(width, 0);
    double x1, y1, x2, y2;
    x1 = x2 = y1 = y2 = 0;
    for (int i = 1; i < 4; i++) {
        double x = vertices[i].getX();
        double y = vertices[i].getY();
        if (x < x1) x1 = x;
        if (y < y1) y1 = y;
        if (x > x2) x2 = x;
        if (y > y2) y2 = y;
    }
    return GRectangle(this->x + x1, this->y + y1, x2 - x1, y2 - y1);
}

bool GOval::contains(double x, double y) const {
   GPoint p = matrix.preimage(x - this->x, y - this->y);
   double xx = p.getX();
   double yy = p.getY();
   double rx = width / 2;
   double ry = height / 2;
   if (rx == 0 || ry == 0) return false;
   double dx = xx - rx;
   double dy = yy - ry;
   return (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry) <= 1.0;
}

void GOval::setFilled(bool flag) {
   fillFlag = true;
   pp->setFilled(this, flag);
}

bool GOval::isFilled() const {
   return fillFlag;
}

void GOval::setFillColor(string color) {
   fillColor = color;
   if (fillColor != "") {
      fillColor = convertRGBToColor(convertColorToRGB(color));
   }
   pp->setFillColor(this, fillColor);
}

void GOval::setFillColor(int color) {
   fillColor = convertRGBToColor(color);
   pp->setFillColor(this, fillColor);
}

string GOval::getFillColor() const {
   return fillColor;
}

string GOval::getType() const {
   return "GOval";
}

string GOval::toString() const {
   ostringstream oss;
   oss << "GOval(" << x << ", " << y << ", "
                   << width << ", " << height << ")";
   return oss.str();
}

void GOval::create(double width, double height) {
   this->x = 0;
   this->y = 0;
   this->width = width;
   this->height = height;
   fillFlag = false;
   fillColor = "";
   pp->createGOval(this, width, height);
}

GArc::GArc(double width, double height, double start, double sweep) {
   create(width, height, start, sweep);
}

GArc::GArc(double x, double y, double width, double height,
                               double start, double sweep) {
   create(width, height, start, sweep);
   setLocation(x, y);
}

void GArc::setStartAngle(double start) {
   this->start = start;
   pp->setStartAngle(this, start);
}

double GArc::getStartAngle() const {
   return start;
}

void GArc::setSweepAngle(double sweep) {
   this->sweep = sweep;
   pp->setSweepAngle(this, sweep);
}

double GArc::getSweepAngle() const {
   return sweep;
}

GPoint GArc::getStartPoint() const {
   return getArcPoint(start);
}

GPoint GArc::getEndPoint() const {
   return getArcPoint(start + sweep);
}

void GArc::setFrameRectangle(const GRectangle & rect) {
   setFrameRectangle(rect.getX(), rect.getY(), rect.getWidth(),
                                                   rect.getHeight());
}

void GArc::setFrameRectangle(double x, double y, double width, double height) {
   this->x = x;
   this->y = y;
   frameWidth = width;
   frameHeight = height;
   pp->setFrameRectangle(this, x, y, width, height);
}

GRectangle GArc::getFrameRectangle() const {
   return GRectangle(0, 0, 0, 0);
}

void GArc::setFilled(bool flag) {
   fillFlag = true;
   pp->setFilled(this, flag);
}

bool GArc::isFilled() const {
   return fillFlag;
}

void GArc::setFillColor(string color) {
   fillColor = color;
   if (fillColor != "") {
      fillColor = convertRGBToColor(convertColorToRGB(color));
   }
   pp->setFillColor(this, fillColor);
}

void GArc::setFillColor(int color) {
   fillColor = convertRGBToColor(color);
   pp->setFillColor(this, fillColor);
}

string GArc::getFillColor() const {
   return fillColor;
}

GRectangle GArc::getBounds() const {
   if (transformed) return pp->getBounds(this);
   double rx = frameWidth / 2;
   double ry = frameHeight / 2;
   double cx = x + rx;
   double cy = y + ry;
   double startRadians = start * PI / 180;
   double sweepRadians = sweep * PI / 180;
   double p1x = cx + cos(startRadians) * rx;
   double p1y = cy - sin(startRadians) * ry;
   double p2x = cx + cos(startRadians + sweepRadians) * rx;
   double p2y = cy - sin(startRadians + sweepRadians) * ry;
   double xMin = min(p1x, p2x);
   double xMax = max(p1x, p2x);
   double yMin = min(p1y, p2y);
   double yMax = max(p1y, p2y);
   if (containsAngle(0)) xMax = cx + rx;
   if (containsAngle(90)) yMin = cy - ry;
   if (containsAngle(180)) xMin = cx - rx;
   if (containsAngle(270)) yMax = cy + ry;
   if (isFilled()) {
      xMin = min(xMin, cx);
      yMin = min(yMin, cy);
      xMax = max(xMax, cx);
      yMax = max(yMax, cy);
   }
   return GRectangle(xMin, yMin, xMax - xMin, yMax - yMin);
}

// JL moved computation for transformed case into front end here
bool GArc::contains(double x, double y) const {
   if (transformed) {
       GPoint pt = matrix.preimage(x - this->x, y - this->y);
       x = this->x + pt.getX();
       y = this->y + pt.getY();
   }
   double rx = frameWidth / 2;
   double ry = frameHeight / 2;
   if (rx == 0 || ry == 0) return false;
   double dx = x - (this->x + rx);
   double dy = y - (this->y + ry);
   double r = (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry);
   if (fillFlag) {
      if (r > 1.0) return false;
   } else {
      double t = ARC_TOLERANCE / ((rx + ry) / 2);
      if (abs(1.0 - r) > t) return false;
   }
   return containsAngle(atan2(-dy/ry, dx/rx) * 180 / PI); // BUGFIX (JL): must scale by ry, rx.
}

string GArc::getType() const {
   return "GArc";
}

string GArc::toString() const {
   ostringstream oss;
   oss << "GArc(" << x << ", " << y << ", " << frameWidth << ", "
                  << frameHeight << ", " << start << ", " << sweep << ")";
   return oss.str();
}

GPoint GArc::getArcPoint(double theta) const {
   double rx = frameWidth / 2;
   double ry = frameHeight / 2;
   double cx = x + rx;
   double cy = y + ry;
   double radians = theta * PI / 180;
   return GPoint(cx + rx * cos(radians), cy - ry * sin(radians));
}

bool GArc::
containsAngle(double theta) const {
   double start = min(this->start, this->start + this->sweep);
   double sweep = abs(this->sweep);
   if (sweep >= 360) return true;
   theta = (theta < 0) ? 360 - fmod(-theta, 360) : fmod(theta, 360);
   start = (start < 0) ? 360 - fmod(-start, 360) : fmod(start, 360);
   if (start + sweep > 360) {
      return theta >= start || theta <= start + sweep - 360;
   } else {
      return theta >= start && theta <= start + sweep;
   }
}

void GArc::create(double width, double height, double start, double sweep) {
   this->x = 0;
   this->y = 0;
   frameWidth = width;
   frameHeight = height;
   this->start = start;
   this->sweep = sweep;
   fillFlag = false;
   fillColor = "";
   pp->createGArc(this, width, height, start, sweep);
}

GCompound::GCompound() {
   pp->createGCompound(this);
}

void GCompound::add(GObject *gobj) {
   pp->add(this, gobj);
   contents.add(gobj);
   gobj->parent = this;
}

void GCompound::add(GObject *gobj, double x, double y) {
   gobj->setLocation(x, y);
   add(gobj);
}

void GCompound::remove(GObject *gobj) {
   int index = findGObject(gobj);
   if (index != -1) removeAt(index);
}

void GCompound::removeAll() {
   while (!contents.isEmpty()) {
      removeAt(0);
   }
}

int GCompound::getElementCount() {
   return contents.size();
}

GObject *GCompound::getElement(int index) {
   return contents.get(index);
}

// JL rewrote to handle transformed case
GRectangle GCompound::getBounds() const {
   double x1, y1, x2, y2;
   x1 = 1E20;
   x2 = -1E20;
   y1 = 1E20;
   y2 = -1E20;
   for (int i = 0; i < contents.size(); i++) {
      GRectangle bounds = contents.get(i)->getBounds();
      GPoint vertices[4];
      vertices[0] = GPoint(bounds.getX(), bounds.getY());
      vertices[1] = GPoint(bounds.getX() + bounds.getWidth(), bounds.getY());
      vertices[2] = GPoint(bounds.getX(), bounds.getY() + bounds.getHeight());
      vertices[3] = GPoint(bounds.getX()+ bounds.getWidth(), bounds.getY() + bounds.getHeight());
      if (transformed) {
          for (int j = 0; j < 4; j++)
             vertices[j] = matrix.image(vertices[j]);
      }
      for (int j = 0; j < 4; j++) {
          double x = vertices[j].getX();
          double y = vertices[j].getY();
          if (x < x1) x1 = x;
          if (y < y1) y1 = y;
          if (x > x2) x2 = x;
          if (y > y2) y2 = y;
      }
   }
   return GRectangle(x + x1, y + y1, x2 - x1, y2 - y1);
}

// JL rewrote to handle transformed case
bool GCompound::contains(double x, double y) const {
   x -= this->x;
   y -= this->y;
   if (transformed) {
       GPoint pt = matrix.preimage(x, y);
       x = pt.getX();
       y = pt.getY();
   }
   for (int i = 0; i < contents.size(); i++) {
       if (contents.get(i)->contains(x, y)) return true; // BUGFIX (JL): shift by this->x, this->y
   }
   return false;
}

string GCompound::getType() const {
   return "GCompound";
}

string GCompound::toString() const {
   return "GCompound(...)";
}

void GCompound::sendForward(GObject *gobj) {
   int index = findGObject(gobj);
   if (index == -1) return;
   if (index != contents.size() - 1) {
      contents.remove(index);
      contents.insert(index + 1, gobj);
      pp->sendForward(gobj);
   }
}

void GCompound::sendToFront(GObject *gobj) {
   int index = findGObject(gobj);
   if (index == -1) return;
   if (index != contents.size() - 1) {
      contents.remove(index);
      contents.add(gobj);
      pp->sendToFront(gobj);
   }
}

void GCompound::sendBackward(GObject *gobj) {
   int index = findGObject(gobj);
   if (index == -1) return;
   if (index != 0) {
      contents.remove(index);
      contents.insert(index - 1, gobj);
      pp->sendBackward(gobj);
   }
}

void GCompound::sendToBack(GObject *gobj) {
   int index = findGObject(gobj);
   if (index == -1) return;
   if (index != 0) {
      contents.remove(index);
      contents.insert(0, gobj);
      pp->sendToBack(gobj);
   }
}

int GCompound::findGObject(GObject *gobj) {
   int n = contents.size();
   for (int i = 0; i < n; i++) {
      if (contents.get(i) == gobj) return i;
   }
   return -1;
}

void GCompound::removeAt(int index) {
   GObject *gobj = contents[index];
   contents.remove(index);
   pp->remove(gobj);
   gobj->parent = NULL;
}

GImage::GImage(string filenameOrURL) {
   create(filenameOrURL);
}

GImage::GImage(string filenameOrURL, double x, double y) {
   create(filenameOrURL);
   setLocation(x, y);
}

// added by JL
bool GImage::contains(double x, double y) const {
    GPoint p = matrix.preimage(x - this->x, y - this->y);
    double xx = p.getX();
    double yy = p.getY();
    return 0 < xx && xx <= width
        &&  0 < yy && yy <= height;
}

// JL rewrote to handle transformed case
GRectangle GImage::getBounds() const {
    if (!transformed)
       return GRectangle(x, y, width, height);
    GPoint vertices[4];
    vertices[0] = GPoint(0, 0); // unused!
    vertices[1] = matrix.image(0, height);
    vertices[2] = matrix.image(width, height);
    vertices[3] = matrix.image(width, 0);
    double x1, y1, x2, y2;
    x1 = x2 = y1 = y2 = 0;
    for (int i = 1; i < 4; i++) {
        double x = vertices[i].getX();
        double y = vertices[i].getY();
        if (x < x1) x1 = x;
        if (y < y1) y1 = y;
        if (x > x2) x2 = x;
        if (y > y2) y2 = y;
    }
    return GRectangle(this->x + x1, this->y + y1, x2 - x1, y2 - y1);
}

string GImage::getType() const {
   return "GImage";
}

string GImage::toString() const {
   return "GImage(\"" + filename + "\")";
}

void GImage::create(string filename) {
   this->filename = filename;
   GDimension size = pp->createGImage(this, filename);
   width = size.getWidth();
   height = size.getHeight();
}

/*
 * Implementation notes: GLabel class
 * ----------------------------------
 */

GLabel::GLabel(string str) {
   create(str);
}

GLabel::GLabel(string str, double x, double y) {
   create(str);
   setLocation(x, y);
}

void GLabel::create(const string & str) {
   this->str = str;
   pp->createGLabel(this, str);
   setFont(DEFAULT_GLABEL_FONT);
   GDimension size = pp->getGLabelSize(this);
   width = size.getWidth();
   height = size.getHeight();
   ascent = pp->getFontAscent(this);
   descent = pp->getFontDescent(this);
}

void GLabel::setFont(string font) {
   this->font = font;
   pp->setFont(this, font);
   GDimension size = pp->getGLabelSize(this);
   width = size.getWidth();
   height = size.getHeight();
   ascent = pp->getFontAscent(this);
   descent = pp->getFontDescent(this);
}

string GLabel::getFont() const {
   return font;
}

void GLabel::setLabel(string str) {
   this->str = str;
   pp->setLabel(this, str);
   GDimension size = pp->getGLabelSize(this);
   width = size.getWidth();
   height = size.getHeight();
}

string GLabel::getLabel() const {
   return str;
}

double GLabel::getFontAscent() const {
   return ascent;
}

double GLabel::getFontDescent() const {
   return descent;
}

GRectangle GLabel::getBounds() const {
   if (transformed) return pp->getBounds(this);
   return GRectangle(x, y - ascent, width, height);
}

string GLabel::getType() const {
   return "GLabel";
}

string GLabel::toString() const {
   return "GLabel(\"" + str + "\")";
}

/*
 * Implementation notes: GLine class
 * ---------------------------------
 */

GLine::GLine(double x0, double y0, double x1, double y1) {
   pp->createGLine(this, x0, y0, x1, y1);
   x = x0;
   y = y0;
   dx = x1 - x0;
   dy = y1 - y0;
}

void GLine::setStartPoint(double x, double y) {
   dx += this->x - x;
   dy += this->y - y;
   this->x = x;
   this->y = y;
   pp->setStartPoint(this, x, y);
}

GPoint GLine::getStartPoint() const {
   return GPoint(x, y);
}

void GLine::setEndPoint(double x, double y) {
   dx = x - this->x;
   dy = y - this->y;
   pp->setEndPoint(this, x, y);
}

GPoint GLine::getEndPoint() const {
   return GPoint(x + dx, y + dy);
}

// JL rewrote to include transformed case in front end
GRectangle GLine::getBounds() const {
   double tdx = dx;
   double tdy = dy;
   if (transformed) {
       GPoint pt = matrix.image(dx, dy);
       tdx = pt.getX();
       tdy = pt.getY();
   }
   double x0 = (tdx < 0) ? x + tdx : x;
   double y0 = (tdy < 0) ? y + tdy : y;
   return GRectangle(x0, y0, abs(tdx), abs(tdy));
}

// JL rewrote to include transformed case in front end
bool GLine::contains(double x, double y) const {
   double x0 = getX();
   double y0 = getY();
   double x1 = x0 + dx;
   double y1 = y0 + dy;
   if (transformed) {
       GPoint pt = matrix.image(dx, dy);
       x1 = x0 + pt.getX();
       y1 = y0 + pt.getY();
   }
   double tSquared = LINE_TOLERANCE * LINE_TOLERANCE;
   if (dsq(x, y, x0, y0) < tSquared) return true;
   if (dsq(x, y, x1, y1) < tSquared) return true;
   if (x < min(x0, x1) - LINE_TOLERANCE) return false;
   if (x > max(x0, x1) + LINE_TOLERANCE) return false;
   if (y < min(y0, y1) - LINE_TOLERANCE) return false;
   if (y > max(y0, y1) + LINE_TOLERANCE) return false;
   if ((float) (x0 - x1) == 0 && (float) (y0 - y1) == 0) return false;
   double u = ((x - x0) * (x1 - x0) + (y - y0) * (y1 - y0))
            / dsq(x0, y0, x1, y1);
   return dsq(x, y, x0 + u * (x1 - x0), y0 + u * (y1 - y0)) < tSquared;
}

string GLine::getType() const {
   return "GLine";
}

string GLine::toString() const {
   ostringstream oss;
   oss << "GLine(" << x << ", " << y << ", "
                   << (x + dx) << ", " << (y + dy) << ")";
   return oss.str();
}

/*
 * Implementation notes: GPolygon class
 * ------------------------------------
 */

GPolygon::GPolygon() {
   fillFlag = false;
   fillColor = "";
   pp->createGPolygon(this);
}

void GPolygon::addVertex(double x, double y) {
   cx = x;
   cy = y;
   vertices.add(GPoint(cx, cy));
   pp->addVertex(this, cx, cy);
}

void GPolygon::addEdge(double dx, double dy) {
   addVertex(cx + dx, cy + dy);
}

void GPolygon::addPolarEdge(double r, double theta) {
   addEdge(r * cos(theta * PI / 180), -r * sin(theta * PI / 180));
}

Vector<GPoint> GPolygon::getVertices() const {
   return vertices;
}

void GPolygon::setFilled(bool flag) {
   fillFlag = true;
   pp->setFilled(this, flag);
}

bool GPolygon::isFilled() const {
   return fillFlag;
}

void GPolygon::setFillColor(string color) {
   fillColor = color;
   if (fillColor != "") {
      fillColor = convertRGBToColor(convertColorToRGB(color));
   }
   pp->setFillColor(this, fillColor);
}

void GPolygon::setFillColor(int rgb) {
   fillColor = convertRGBToColor(rgb);
   pp->setFillColor(this, fillColor);
}

string GPolygon::getFillColor() const {
   return fillColor;
}

// JL added code to handle transformed case
GRectangle GPolygon::getBounds() const {
   double xMin = 0;
   double yMin = 0;
   double xMax = 0;
   double yMax = 0;
   double x0 = getX();
   double y0 = getY();
   for (int i = 0; i < vertices.size(); i++) {
      double x = vertices[i].getX();
      double y = vertices[i].getY();
      if (transformed) {
          GPoint pt = matrix.image(x, y);
          x = pt.getX();
          y = pt.getY();
      }
      if (i == 0 || x < xMin) xMin = x;
      if (i == 0 || y < yMin) yMin = y;
      if (i == 0 || x > xMax) xMax = x;
      if (i == 0 || y > yMax) yMax = y;
   }
   return GRectangle(xMin + x0, yMin + y0, xMax - xMin, yMax - yMin); // BUGFIX (JL): add x0, y0
}

// JL added code to handle transformed case
bool GPolygon::contains(double x, double y) const {
   int crossings = 0;
   int n = vertices.size();
   if (n < 2) return false;
   if (vertices[0] == vertices[n - 1]) n--;
   x = x - getX(); // BUGFIX (JL) - must translate by anchor point
   y = y - getY();
   double x0 = vertices[0].getX();
   double y0 = vertices[0].getY();
   for (int i = 1; i <= n; i++) {
      double x1 = vertices[i % n].getX();
      double y1 = vertices[i % n].getY();
      if (transformed) {
          GPoint pt = matrix.image(x1, y1);
          x1 = pt.getX();
          y1 = pt.getY();
      }
      if ((y0 > y) != (y1 > y) && x - x0 < (x1 - x0) * (y - y0) / (y1 - y0)) {
         crossings++;
      }
      x0 = x1;
      y0 = y1;
   }
   return (crossings % 2 == 1);
}

string GPolygon::getType() const {
   return "GPolygon";
}

string GPolygon::toString() const {
   ostringstream oss;
   oss << "GPolygon(" << vertices.size() << " vertices)";
   return oss.str();
}

GObject::Matrix2D::Matrix2D() {
    m[0][1] = m[1][0] = 0;
    m[0][0] = m[1][1] = 1;
}

void GObject::Matrix2D::applyRotate(double theta) {
    // Counterintuitive sign weirdness
    // because positive y-axis points downward.
    double m00 = cosDegrees(theta) * m[0][0] - sinDegrees(theta) * m[0][1];
    double m01 = sinDegrees(theta) * m[0][0] + cosDegrees(theta) * m[0][1];
    double m10 = cosDegrees(theta) * m[1][0] - sinDegrees(theta) * m[1][1];
    double m11 = sinDegrees(theta) * m[1][0] + cosDegrees(theta) * m[1][1];
    m[0][0] = m00;
    m[0][1] = m01;
    m[1][0] = m10;
    m[1][1] = m11;
}

void GObject::Matrix2D::applyScale(double sx, double sy) {
    m[0][0] *= sx;
    m[0][1] *= sy;
    m[1][0] *= sx;
    m[1][1] *= sy;
}

GPoint GObject::Matrix2D::image(const GPoint &pt) const {
    return image(pt.getX(), pt.getY());
}

GPoint GObject::Matrix2D::image(double x, double y) const {
    double xx = m[0][0] * x + m[0][1] * y;
    double yy = m[1][0] * x + m[1][1] * y;
    return GPoint(xx, yy);
}

GPoint GObject::Matrix2D::preimage(const GPoint &pt) const {
    return preimage(pt.getX(), pt.getY());
}

GPoint GObject::Matrix2D::preimage(double x, double y) const {
    double det = m[0][0] * m[1][1] - m[1][0] * m[0][1];
    double xx = (m[1][1] * x - m[0][1] * y) / det;
    double yy = (-m[1][0] * x + m[0][0] * y) / det;
    return GPoint(xx, yy);
}

static double dsq(double x0, double y0, double x1, double y1) {
   return (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
}
