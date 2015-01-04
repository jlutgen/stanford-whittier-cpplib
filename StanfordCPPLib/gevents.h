/**
 * @file gevents.h
 *
 * @brief
 * This file defines the event types used in the StanfordCPPLib
 * graphics libraries.
 *
 * The structure of this package is adapted from
 * the Java event model.
 * <div class=inset>
 * <img src="../images/GEventHierarchy.png"
 *   width=558 alt="GEventHierarchy">
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

#ifndef _gevents_h
#define _gevents_h

#include <string>
#include "gtimer.h"
#include "gwindow.h"


#ifdef _WIN32
#  undef MOUSE_EVENT
#  undef KEY_EVENT
#  undef MOUSE_MOVED
#  undef HELP_KEY
#endif


/**
 * This enumeration type defines the general categories of events.  The element values
 * are each a single bit and can be added or ORed together to generate
 * an event mask.  The <code>CLICK_EVENT</code> category corresponds only to
 * the <code>MOUSE_CLICKED</code> event type.  The <code>ANY_EVENT</code> category
 * corresponds to any event.
 */
enum EventClassType {
   NULL_EVENT   = 0x000,            ///<.
   ACTION_EVENT = 0x010,            ///<.
   KEY_EVENT    = 0x020,            ///<.
   TIMER_EVENT  = 0x040,            ///<.
   WINDOW_EVENT = 0x080,            ///<.
   MOUSE_EVENT  = 0x100,            ///<.
   CLICK_EVENT  = 0x200,            ///<.
   ANY_EVENT    = 0x3F0             ///<.
};


/**
 * This enumeration type defines the various specific event types.
 */
typedef enum {
   WINDOW_CLOSED    = WINDOW_EVENT + 1, ///< .
   WINDOW_RESIZED   = WINDOW_EVENT + 2, ///< .
   ACTION_PERFORMED = ACTION_EVENT + 1, ///< .
   MOUSE_CLICKED    = MOUSE_EVENT + 1,  ///< .
   MOUSE_PRESSED    = MOUSE_EVENT + 2,  ///< .
   MOUSE_RELEASED   = MOUSE_EVENT + 3,  ///< .
   MOUSE_MOVED      = MOUSE_EVENT + 4,  ///< .
   MOUSE_DRAGGED    = MOUSE_EVENT + 5,  ///< .
   KEY_PRESSED      = KEY_EVENT + 1,    ///< .
   KEY_RELEASED     = KEY_EVENT + 2,    ///< .
   KEY_TYPED        = KEY_EVENT + 3,    ///< .
   TIMER_TICKED     = TIMER_EVENT + 1,  ///< .
} EventType;


/**
 * This enumeration type defines a set of constants used to check whether
 * modifiers are in effect.
 */
enum ModifierCodes {
   SHIFT_DOWN     = 1 << 0, ///< Shift key
   CTRL_DOWN      = 1 << 1, ///< Control key
   META_DOWN      = 1 << 2, ///< Meta key
   ALT_DOWN       = 1 << 3, ///< Alt key
   ALT_GRAPH_DOWN = 1 << 4, ///< AltGr key
   BUTTON1_DOWN   = 1 << 5, ///< Left mouse button
   BUTTON2_DOWN   = 1 << 6, ///< Middle mouse button
   BUTTON3_DOWN   = 1 << 7  ///< Right mouse button
};


/**
 * This type defines the names of the key codes returned in a key event.
 */
enum KeyCodes {
   BACKSPACE_KEY = 8,       ///< Backspace
   TAB_KEY = 9,             ///< Tab
   ENTER_KEY = 10,          ///< Enter
   CLEAR_KEY = 12,          ///< Clear
   ESCAPE_KEY = 27,         ///< Esc
   PAGE_UP_KEY = 33,        ///< Page up
   PAGE_DOWN_KEY = 34,      ///< Page down
   END_KEY = 35,            ///< End
   HOME_KEY = 36,           ///< Home
   LEFT_ARROW_KEY = 37,     ///< Left arrow
   UP_ARROW_KEY = 38,       ///< Up arrow
   RIGHT_ARROW_KEY = 39,    ///< Right arrow
   DOWN_ARROW_KEY = 40,     ///< Down arrow
   F1_KEY = 112,            ///< F1
   F2_KEY = 113,            ///< F2
   F3_KEY = 114,            ///< F3
   F4_KEY = 115,            ///< F4
   F5_KEY = 116,            ///< F5
   F6_KEY = 117,            ///< F6
   F7_KEY = 118,            ///< F7
   F8_KEY = 119,            ///< F8
   F9_KEY = 120,            ///< F9
   F10_KEY = 121,           ///< F10
   F11_KEY = 122,           ///< F11
   F12_KEY = 123,           ///< F12
   DELETE_KEY = 127,        ///< Delete
   HELP_KEY = 156           ///< Help
};

/* Forward definitions */

class GWindowEvent;
class GActionEvent;
class GMouseEvent;
class GKeyEvent;
class GTimerEvent;
class GObject;


/**
 * \class GEvent
 *
 * This class is the root of the hierarchy for all events.
 *
 * The standard paradigm for using <code>GEvent</code> is illustrated
 * by the following program, which allows the user to draw lines on the
 * graphics window:
 *
 * ~~~
 *     int main() {
 *        GWindow gw;
 *        GLine *line;
 *        cout << "This program lets the user draw lines by dragging." << endl;
 *        while (true) {
 *           GMouseEvent e = waitForEvent(MOUSE_EVENT);
 *           if (e.getEventType() == MOUSE_PRESSED) {
 *              line = new GLine(e.getX(), e.getY(), e.getX(), e.getY());
 *              gw.add(line);
 *           } else if (e.getEventType() == MOUSE_DRAGGED) {
 *              line->setEndPoint(e.getX(), e.getY());
 *           }
 *        }
 *     }
 * ~~~
 */
class GEvent {

public:

/**
 * Ensures that an event is properly initialized to a <code>NULL</code> event.
 */
   GEvent();


/**
 * Returns the enumerated type constant indicating the general category of this
 * event.
 *
 * Sample usage:
 *
 * ~~~
 * EventClassType eventClass = e.getEventClass();
 * ~~~
 */
   EventClassType getEventClass() const;


/**
 * Returns the enumerated type constant corresponding to the specific
 * event type of this event.
 *
 * Sample usage:
 *
 *     EventType type = e.getEventType();
 */
   EventType getEventType() const;


/**
 * Returns the system time in milliseconds at which this event occurred.
 * To ensure portability among systems that represent time in different
 * ways, the StanfordCPPLib packages use type <code>double</code> to
 * represent time, which is always encoded as the number of milliseconds
 * that have elapsed since 00:00:00 UTC on January 1, 1970, which is
 * the conventional zero point for computer-based time systems.
 *
 * Sample usage:
 *
 *     double time = e.getEventTime();
 */
   double getEventTime() const;


/**
 * Returns an integer whose bits indicate what modifiers are in effect.
 * To check whether the shift key is down, for example, one could use
 * the following code:
 * ~~~
 * if (e.getModifiers() & SHIFT_DOWN) ...
 * ~~~
 */
   int getModifiers() const;


/**
 * Returns a human-readable string representation of this event.
 *
 * Sample usage:
 *
 *     string str = e.toString();
 */
   virtual std::string toString() const;


/**
 * Returns \c true if this event is valid.
 *
 * Sample usage:
 *
 *     if (e.isValid()) ...
 */
   bool isValid();


/* Private section */

/**********************************************************************/
/* Note: Everything below this point in the file is logically part    */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

/*
 * Sets the event time field for this event.  The event system needs
 * access to this method, but conventional clients don't.
 *
 * Sample usage:
 *
 *     e.setEventTime(time);
 */

   void setEventTime(double time);

/*
 * Sets the modifiers field for this event.  The event system needs
 * access to this method, but conventional clients don't.
 *
 * Sample usage:
 *
 *     e.setModifiers(modifiers);
 */

   void setModifiers(int modifiers);

private:

/*
 * Instance variables
 * ------------------
 * Implementation note: All the variables from the subclasses are included
 * in the outer class to make it possible to convert between general events
 * and the various subclasses.  By keeping all event classes the same size,
 * this design avoids any issues of slicing off parts of the data during
 * such conversions.
 */

/* General events */

   EventClassType eventClass;
   int eventType;
   int modifiers;
   double eventTime;
   bool valid;
   std::string sourceKey;  // NOT USED (JL)

/* Window, mouse, and key events */

   GWindowData *gwd;

/* Action events */

   GObject *source;
   std::string actionCommand;

/* Mouse events */

   double x;
   double y;

/* Key events */

   int keyChar;
   int keyCode;

/* Timer events */

   GTimerData *gtd;

/* Friend specifications */

friend class GWindowEvent;
friend class GActionEvent;
friend class GMouseEvent;
friend class GKeyEvent;
friend class GTimerEvent;

};


/**
 * Waits for a mouse click in any window, discarding any other events.
 *
 * Sample usage:
 *
 *     waitForClick();
 *
 */

void waitForClick();


/**
 * Dismisses the process until an event occurs whose type is covered by
 * the given event mask.  The \em mask parameter is a combination of the events of
 * interest.  For example, to wait for a mouse event or an action event,
 * clients can use the following call:
 *
 * ~~~
 * e = waitForEvent(MOUSE_EVENT + ACTION_EVENT);
 * ~~~
 *
 * The \em mask parameter is optional.  If it is missing,
 * <code>waitForEvent</code> accepts any event.
 *
 * As a more sophisticated example, the following code is the canonical
 * event loop for an animated application that needs to respond to mouse,
 * key, and timer events:
 *
 *
 *     GTimer timer(ANIMATION_DELAY_IN_MILLISECONDS);
 *     timer.start();
 *     while (true) {
 *        GEvent e = waitForEvent(TIMER_EVENT + MOUSE_EVENT + KEY_EVENT);
 *        switch (e.getEventClass()) {
 *         case TIMER_EVENT:
 *           takeAnimationStep();
 *           break;
 *         case MOUSE_EVENT:
 *           handleMouseEvent(GMouseEvent(e));
 *           break;
 *         case KEY_EVENT:
 *           handleKeyEvent(GKeyEvent(e));
 *           break;
 *        }
 *     }
 *
 */

GEvent waitForEvent(int mask = ANY_EVENT);


/**
 * Checks to see if there are any events of the desired type waiting on the
 * event queue.  If so, this function returns the event in exactly the same
 * fashion as \ref waitForEvent; if not, this function
 * returns an invalid event.  The \em mask parameter is optional.
 * If it is missing, this function checks for any type of event.
 *
 * Sample usage:
 *
 *     GEvent e = getNextEvent(MOUSE_EVENT);
 *
 */
GEvent getNextEvent(int mask = ANY_EVENT);


/**
 * This event subclass represents a window event.
 * Each <code>GWindowEvent</code> keeps track of the event type
 * (<code>WINDOW_CLOSED</code>, <code>WINDOW_RESIZED</code>) along
 * with the identity of the window that generated the event.
 */
class GWindowEvent : public GEvent {

public:

/**
 * Creates a <code>GWindowEvent</code> using the specified parameters.
 *
 * Sample usage:
 *
 *     GWindowEvent windowEvent(type, gw);
 */

   GWindowEvent(EventType type, const GWindow & gw);


/**
 * Returns the graphics window in which this event occurred.
 *
 * Sample usage:
 *
 *     GWindow gw = e.getGWindow();
 */
   GWindow getGWindow() const;


/**
 * Converts the event to a human-readable representation of the event.
 *
 * Sample usage:
 *
 *     cout << e.toString() << endl;
 */
   std::string toString() const;


/* Private section */

   GWindowEvent();
   GWindowEvent(GEvent e);

};


/**
 * \class GActionEvent
 *
 * This event subclass represents an action event.
 * Action events are generated by the classes in the GInteractor
 * hierarchy.  As an example, the following program displays
 * a button that, when pushed, generates the message
 * <em>Please do not press this button again</em>
 * (with thanks to Douglas Adams):
 *
 * ~~~
 *    int main() {
 *       GWindow gw;
 *       GButton *button = new GButton("RED");
 *       gw.addToRegion(button, "SOUTH");
 *       while (true) {
 *          GEvent e = waitForEvent(ACTION_EVENT | CLICK_EVENT);
 *          if (e.getEventType() == MOUSE_CLICKED) break;
 *          cout << "Please do not press this button again." << endl;
 *       }
 *       return 0;
 *    }
 * ~~~
 */
class GActionEvent : public GEvent {

public:

/**
 * Creates a `%GActionEvent` using the specified parameters.
 *
 * Sample usage:
 *
 *     GActionEvent actionEvent(type, source, actionCommand);
 */
   GActionEvent(EventType type, GObject *source, std::string actionCommand);


/**
 * Returns a pointer to the GObject that generated this event.
 *
 * Sample usage:
 *
 *     GObject *gobj = e.getSource();
 */
   GObject *getSource() const;


/**
 * Returns the action command string associated with this event.
 *
 * Sample usage:
 *
 *     string cmd = e.getActionCommand();
 */
   std::string getActionCommand() const;


/**
 * Returns a human-readable string representation of this event.
 *
 * Sample usage:
 *
 *     string str = e.toString();
 */
   std::string toString() const;


/* Private section */

   GActionEvent();
   GActionEvent(GEvent e);

};


/**
 * \class GMouseEvent
 *
 * This event subclass represents a mouse event.  Each mouse event
 * records the event type (<code>MOUSE_PRESSED</code>,
 * <code>MOUSE_RELEASED</code>, <code>MOUSE_CLICKED</code>,
 * <code>MOUSE_MOVED</code>, or <code>MOUSE_DRAGGED</code>) along
 * with the coordinates of the event.  Clicking the mouse generates
 * three events in the following order: <code>MOUSE_PRESSED</code>,
 * <code>MOUSE_RELEASED</code>, <code>MOUSE_CLICKED</code>.
 *
 * As an example, the following program uses mouse events to let
 * the user draw rectangles on the graphics window.  The only
 * complexity in this code is the use of the library functions
 * <code>min</code> and <code>abs</code> to ensure that the
 * dimensions of the rectangle are positive.
 *
 * ~~~
 *    int main() {
 *       GWindow gw;
 *       cout << "This program lets the user draw rectangles." << endl;
 *       GRect *rect;
 *       double startX;
 *       double startY;
 *       while (true) {
 *          GMouseEvent e = waitForEvent();
 *          if (e.getEventType() == MOUSE_PRESSED) {
 *             startX = e.getX();
 *             startY = e.getY();
 *             rect = new GRect(startX, startY, 0, 0);
 *             rect->setFilled(true);
 *             gw.add(rect);
 *          } else if (e.getEventType() == MOUSE_DRAGGED) {
 *             double x = min(e.getX(), startX);
 *             double y = min(e.getY(), startY);
 *             double width = abs(e.getX() - startX);
 *             double height = abs(e.getY() - startY);
 *             rect->setBounds(x, y, width, height);
 *          }
 *       }
 *    }
 * ~~~
 */
class GMouseEvent : public GEvent {

public:

/**
 * Creates a `%GMouseEvent` using the specified parameters.
 *
 * Sample usage:
 *
 *     GMouseEvent mouseEvent(type, gw, x, y);
 */
   GMouseEvent(EventType type, const GWindow & gw, double x, double y);


/**
 * Returns the graphics window in which this event occurred.
 *
 * Sample usage:
 *
 *     GWindow gw = e.getGWindow();
 */
   GWindow getGWindow() const;


/**
 * Returns the  <i>x</i>-coordinate at which this event occurred, relative
 * to the window origin at the upper-left corner of the window.
 *
 * Sample usage:
 *
 *     double x = getX();
 */
   double getX() const;


/**
  * Returns the <i>y</i>-coordinate at which this event occurred, relative
  * to the window origin at the upper-left corner of the window.
  *
  * Sample usage:
  *
  *     double y = getY();
  */
   double getY() const;


/**
  * Returns a human-readable string representation of this event.
  *
  * Sample usage:
  *
  *     string str = e.toString();
  */
   std::string toString() const;


/* Private section */

   GMouseEvent();
   GMouseEvent(GEvent e);

};


/**
 * \class GKeyEvent
 *
 * This event subclass represents a key event.  Each key event records
 * the event type along with two representations of the key.  The
 * \ref getKeyChar function is more generally useful and
 * returns the character generated by the key, after taking into account modifier keys.
 * The \ref getKeyCode function returns an integer identifying
 * the physical key that was pressed,
 * which can be a function key as well as a standard key.
 * The codes return by \ref getKeyCode are listed in the
 * \ref KeyCodes enumeration.
 */
class GKeyEvent : public GEvent {

public:

/**
 * Creates a `%GKeyEvent` using the specified parameters.
 *
 * Sample usage:
 *
 *     GKeyEvent keyEvent(type, gw, keyChar, keyCode);
 */
   GKeyEvent(EventType type, const GWindow & gw, int keyChar, int keyCode);


/**
  * Returns the graphics window in which this event occurred.
  *
  * Sample usage:
  *
  *     GWindow gw = e.getGWindow();
  */
   GWindow getGWindow() const;


/**
 * Returns the character generated by the keystroke, taking modifier
 * keys into account.  For example, if the user typed the <code>'a'</code>
 * key with the shift key down, this method will return
 * <code>'A'</code>.  If the key code in the event does not correspond
 * to a character, this method returns the null character <code>'\0'</code>.
 *
 * Sample usage:
 *
 *     char ch = e.getKeyChar();
 */
   char getKeyChar() const;


/**
 * Returns the integer code associated with the physical key that
 * generated this event.
 *
 * Sample usage:
 *
 *     int code = e.getKeyCode();
 */
   int getKeyCode() const;


/**
 * Returns a human-readable string representation of this event.
 *
 * Sample usage:
 *
 *     cout << e.toString() << endl;
 */
   std::string toString() const;


/* Private section */

   GKeyEvent();
   GKeyEvent(GEvent e);

};


/**
 * \class GTimerEvent
 *
 * This event subclass represents a timer event.  Timer events are
 * generated by a `%GTimer` object, which produces a new event at
 * a fixed interval measured in milliseconds.  As an example, the
 * following program generates a timer event every two seconds,
 * stopping when the user clicks somewhere in the window:
 *
 * ~~~
 *    int main() {
 *       cout << "This program generates timer events." << endl;
 *       GTimer timer(2000);
 *       timer.start();
 *       while (true) {
 *          GEvent e = waitForEvent(CLICK_EVENT | TIMER_EVENT);
 *          if (e.getEventType() == MOUSE_CLICKED) break;
 *          cout << "Timer ticked" << endl;
 *       }
 *       return 0;
 *    }
 * ~~~
 */
class GTimerEvent : public GEvent {

public:

/**
 * Creates a `%GTimerEvent` for the specified timer.
 *
 * Sample usage:
 *
 *     GTimerEvent timerEvent(type, timer);
 */
   GTimerEvent(EventType type, const GTimer & timer);


/**
 * Returns the timer that generated this event.
 *
 * Sample usage:
 *
 *     GTimer timer = e.getGTimer();
 */
   GTimer getGTimer() const;


/**
 * Returns a human-readable string representation of this event.
 *
 * Sample usage:
 *
 *     cout << e.toString() << endl;
 */
   std::string toString() const;


/* Private section */

   GTimerEvent();
   GTimerEvent(GEvent e);

};

#endif
