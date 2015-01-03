/**
 * @file ginteractors.h
 *
 * This file exports a hierarchy of graphical interactors similar to those
 * provided in the Java Swing libraries.
 * <div class=inset>
 * <img src="../images/GInteractorHierarchy.png"
 *   width=558 alt="GInteractorHierarchy">
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

#ifndef _ginteractors_h
#define _ginteractors_h

#include "gobjects.h"
#include "gtypes.h"
#include "gwindow.h"
#include "vector.h"

/**
 * \class GInteractor
 *
 * This abstract class is the superclass for all graphical interactors.
 * In most applications, interactors will be added to a control strip
 * along one of the sides of the GWindow, but they can
 * also be placed in specific positions just like any other
 * GObject.
 */
class GInteractor : public GObject {

public:

/**
 * Sets the action command for this interactor to the indicated string.  If the string is not
 * empty, activating the interactor generates a GActionEvent.
 *
 * Sample usage:
 * ~~~
 * interactor.setActionCommand(cmd);
 * ~~~
 */
   void setActionCommand(std::string cmd);


/**
 * Returns the action command associated with this interactor.
 *
 * Sample usage:
 * ~~~
 * string cmd = interactor.getActionCommand();
 * ~~~
 */
   std::string getActionCommand();


/** \_overload */
   void setSize(const GDimension & size);
/**
 * Changes the size of this interactor to the specified size.
 *
 * Sample usages:
 * ~~~
 * interactor.setSize(size);
 * interactor.setSize(width, height);
 * ~~~
 */
   void setSize(double width, double height);


/** \_overload */
   void setBounds(const GRectangle & size);
/**
 * Changes the bounds of the interactor to the specified values.
 *
 * Sample usages:
 * ~~~
 * interactor.setBounds(rect);
 * interactor.setBounds(x, y, width, height);
 * ~~~
 */ 
   void setBounds(double x, double y, double width, double height);


/* Prototypes for the virtual methods */

   virtual GRectangle getBounds() const;

protected:

   GInteractor();

   std::string actionCommand;

};

/**
 * This interactor subclass represents an onscreen button.  The following
 * program displays a button that, when pressed, generates the message
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

class GButton : public GInteractor {

public:

/**
 * Creates a `%GButton` with the specified label.  This
 * constructor also sets the action command for the button to the
 * label string.
 *
 * Sample usage:
 * ~~~
 * GButton *button = new GButton(label);
 * ~~~
 */

   GButton(std::string label);

/* Prototypes for the virtual methods */

   virtual std::string getType() const;
   virtual std::string toString() const;

private:
   std::string label;

};

/**
 * This interactor subclass represents an onscreen check box.  Clicking
 * once on the check box selects it; clicking again removes the selection.
 * If a <code>GCheckBox</code> has an action command, clicking on the box
 * generates a <code>GActionEvent</code>.
 *
 * The appearance of a check box is machine-dependent, but the declaration
 * ~~~
 *  GCheckBox *traceBox = new GCheckBox("Trace");
 * ~~~
 * typically produces a check box that looks something like this:
 * <div class=inset>
 * <img src="../images/GCheckBox.png" alt="GCheckBox">
 * </div>
 */

class GCheckBox : public GInteractor {

public:

/**
 * Creates a <code>GCheckBox</code> with the specified label.  In contrast
 * to the <code>GButton</code> constructor, this constructor does not set
 * an action command.
 *
 * Sample usage:
 * ~~~
 * GCheckBox *chkbox = new GCheckBox(label);
 * ~~~
 */

   GCheckBox(std::string label);

/**
 * Sets the state of the check box.
 *
 * Sample usage:
 * ~~~
 * chkbox->setSelected(state);
 * ~~~
 */

   void setSelected(bool state);

/**
 * Returns <code>true</code> if the check box is selected.
 *
 * Sample usage:
 * ~~~
 * if (chkbox->isSelected()) ...
 * ~~~
 */

   bool isSelected();

/* Prototypes for the virtual methods */

   virtual std::string getType() const;
   virtual std::string toString() const;

private:
   std::string label;

};

/**
 * This interactor subclass represents an onscreen slider.  Dragging
 * the slider control generates an <code>ActionEvent</code> if the
 * slider has a nonempty action command.
 *
 * The display image of a slider is machine-dependent, but the declaration
 * ~~~
 *  GSlider *slider = new GSlider();
 * ~~~
 * typically produces a slider that looks something like this:
 * <div class=inset>
 * <img src="../images/GSlider.png" alt="GSlider">
 * </div>
 */

class GSlider : public GInteractor {

public:

    /** \_overload */
    GSlider();
/**
 * Creates a horizontal <code>GSlider</code>.  The second form allows
 * the client to specify the minimum value, maximum value, and current
 * value of the slider.  The first form is equivalent to calling
 * `%GSlider(0, 100, 50)`.  Assigning an action command
 * to the slider causes the slider to generate an action event whenever
 * the slider value changes.
 *
 * Sample usages:
 * ~~~
 * GSlider *slider = new GSlider();   // same as GSlider(0, 100, 50)
 * GSlider *slider = new GSlider(min, max, value);
 * ~~~
 */
   GSlider(int min, int max, int value);


/**
 * Sets the current value of the slider.
 *
 * Sample usage:
 * ~~~
 * slider->setValue(value);
 * ~~~
 */

   void setValue(int value);

/**
 * Returns the current value of the slider.
 *
 * Sample usage:
 * ~~~
 * int value = slider->getValue();
 * ~~~
 */

   int getValue();

/* Prototypes for the virtual methods */

   virtual std::string getType() const;
   virtual std::string toString() const;

private:
   void create(int min, int max, int value);
   int min;
   int max;

};

/**
 * This interactor subclass represents a text field for entering short
 * text strings.  Hitting the Enter key while in a text field generates a
 * <code>GActionEvent</code> if the text field has a nonempty action command.

 */

class GTextField : public GInteractor {

public:

    /** \_overload */
    GTextField();
/**
 * Creates a text field capable of holding \em nChars characters,
 * which defaults to 10.  Assigning an action command to the text field
 * causes it to generate an action event whenever the user types the
 * Enter key.
 *
 * Sample usages:
 * ~~~
 * GTextField *field = new GTextField();  // same as GTextField(10)
 * GTextField *field = new GTextField(nChars);
 * ~~~
 */
   GTextField(int nChars);


/**
 * Sets the text of the field to the specified string.
 *
 * Sample usage:
 * ~~~
 * field->setText(str);
 * ~~~
 */
   void setText(std::string str);


/**
 * Returns the contents of the text field.
 *
 * Sample usage:
 * ~~~
 * string str = field->getText();
 * ~~~
 */
   std::string getText();


/* Prototypes for the virtual methods */

   virtual std::string getType() const;
   virtual std::string toString() const;

};

/**
 * This interactor subclass represents a selectable list.  The
 * %GChooser constructor creates an empty chooser.
 * Once the chooser has been created, clients can use \ref addItem
 * to add the options.  For example, the following code creates a
 * %GChooser containing the four strings
 * <code>"Small"</code>, <code>"Medium"</code>, <code>"Large"</code>,
 * and <code>"X-Large"</code>:
 *
 * ~~~
 *    GChooser *sizeChooser = new GChooser();
 *    sizeChooser->addItem("Small");
 *    sizeChooser->addItem("Medium");
 *    sizeChooser->addItem("Large");
 *    sizeChooser->addItem("X-Large");
 * ~~~
 *
 * The appearance of a %GChooser is machine-dependent,
 * but the chooser generated by this code typically looks something like this:
 * <div class=inset>
 * <img src="../images/GChooser.png" alt="GChooser">
 * </div>
 */

class GChooser : public GInteractor {

public:

/**
 * Creates a chooser that initially contains no items, which are added
 * using the \ref addItem method.  Assigning an action command
 * to the chooser causes it to generate an action event whenever the
 * user selects an item.
 *
 * Sample usage:
 * ~~~
 * GChooser *chooser = new GChooser();
 * ~~~
 */
   GChooser();


/**
 * Adds a new item consisting of the specified string.
 *
 * Sample usage:
 * ~~~
 * chooser->addItem(item);
 * ~~~
 */
   void addItem(std::string item);


/**
 * Sets the chooser so that it shows the specified item.  If the item
 * does not exist in the chooser, no change occurs.
 *
 * Sample usage:
 * ~~~
 * chooser->setSelectedItem(item);
 * ~~~
 */
   void setSelectedItem(std::string item);


/**
 * Returns the current item selected in the chooser.
 *
 * Sample usage:
 * ~~~
 * string item = chooser->getSelectedItem();
 * ~~~
 */
   std::string getSelectedItem();


/* Prototypes for the virtual methods */

   virtual std::string getType() const;
   virtual std::string toString() const;

};

#endif
