/**
 * @file gtextarea.h
 *
 * @brief
 * This file exports the GTextArea class.
 *
 * @author Jeff Lutgen
 */

/*************************************************************************/
/* Stanford Portable Library                                             */
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

#ifndef _gtextarea_h
#define _gtextarea_h

#include "ginteractors.h"
#include "gobjects.h"
#include "gtypes.h"


/**
 * \class GTextArea
 */
class GTextArea : public GInteractor {
public:

    /** \_overload */
    GTextArea(double width, double height);
    /**
     * Constructs an text area with the specified width, height,
     * and initial location. If no location is passed, the default
     * of `(0, 0)` is used.
     *
     * Sample usages:
     *
     */
    /** \_overload */
    GTextArea(double x, double y, double width, double height);
    
    /* Prototypes for the virtual methods */
    virtual GRectangle getBounds() const;
    virtual std::string getType() const;
    virtual std::string toString() const;

    /* unique GTextArea behavior */

     /**
      * Sets the font used by this text area.
      *
      * Sample usage:
      *
      *     textarea->setFont();
      */
      void setFont(std::string font);


      /**
       * Returns the current text displayed by this text area.
       *
       * Sample usage:
       *
       *     string font = textarea->getText();
       */
       std::string getText() const;


       /**
        * Sets the text displayed by this text area.
        *
        * Sample usage:
        *
        *     textarea->setText();
        */
        void setText(std::string text);


        /**
         * Sets whether this text area is editable.
         *
         * Sample usage:
         *
         *     textarea->setText(isEditable);
         */
         void setEditable(bool isEditable);


        /** \_overload */
        void setBackgroundColor(std::string color);
        /**
         * Sets the background color used by this text area. The color
         * may be specified as a string or an integer. See
         * the documentation for \ref GObject::setColor() for the
         * more information on color specifications.
         *
         * To set the color of the text displayed in a text area, call
         * the text area's \c setColor method.
         *
         * Sample usage:
         *
         *     textarea->setBackgroundColor(color);
         */
         void setBackgroundColor(int rgb);

private:
         std::string backgroundColor;
         double width;
         double height;

};

#endif
