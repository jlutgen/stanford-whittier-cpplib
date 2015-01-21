/**
 * @file goptionpane.h
 *
 * @brief
 * This file defines the GOptionPane class, which supports
 * pop-up graphical dialog boxes for messages and user input.
 * 
 * This class is inspired by, extremely similar to, and implemented on
 * the back-end by, Java's JOptionPane class, so you may wish to consult
 * that class's documentation in the Java API Specification for more
 * information.
 */

/*
 * @author Marty Stepp
 * @author Jeff Lutgen
 * @version 2015/01/17
 * - removed showTextFile Dialog
 * @version 2014/10/26
 * - added showTextFileDialog
 * @version 2014/10/09
 * - finished implementation of showOptionDialog; added dialog types and other fixes
 * - enum rearrangement for clearer client code and better error messages
 * @since 2014/07/09
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

#ifndef _goptionpane_h
#define _goptionpane_h

#include <string>
#include "vector.h"
#include "gwindow.h"

/**
 * \class GOptionPane
 *
 * This class provides static methods that support pop-up graphical dialog
 * boxes for messages and user input.
 */
class GOptionPane {
public:
    /**
     * Constants for \ref showConfirmDialog types, taken from Java's JOptionPane.
     *
     * The three types of confirm dialogs are Yes/No, Yes/No/Cancel, and OK/Cancel.
     */
    enum ConfirmType {
        OK_CANCEL = 2,    ///<.
        YES_NO = 0,       ///<.
        YES_NO_CANCEL = 1 ///<.
    };

    /**
     * Constants for the various results that can be returned
     * from some option dialogs.
     */
    enum ConfirmResult {
        CANCEL = -1,   ///<.
        NO = 0,        ///<.
        OK = 2,        ///<.
        YES = 1        ///<.
    };
    
    /**
     * Constants for \ref showMessageDialog types, taken from Java's JOptionPane.
     */
    enum MessageType {
        ERROR = 0,          ///<.
        INFORMATION = 1,    ///<.
        PLAIN = -1,         ///<.
        QUESTION = 3,       ///<.
        WARNING = 2         ///<.
    };

    /**
     * Pops up a yes/no confirmation box.
     * Once the user clicks a button to close the box, one of the
     * \ref GOptionPaneResult enumeration constants is returned.
     * The caller can supply an optional window title; if none is passed, a default is used.
     *
     * Sample usages:
     *
     *     int result = GOptionPane::showConfirmDialog(message);
     *     int result = GOptionPane::showConfirmDialog(message, title);
     *     int result = GOptionPane::showConfirmDialog(message, title, type);
     */
    static ConfirmResult showConfirmDialog(std::string message, std::string title = "",
                                           ConfirmType type = YES_NO, GWindow *parent = NULL);
    
    /**
     * Pops up an input box with a text field where the user can type a
     * response, which is returned.
     * The caller can supply an optional window title; if none is passed, a default is used.
     * If the user cancels the box, an empty string is returned.
     *
     * Sample usages:
     *
     *     string response = GOptionPane::showInputDialog(message);
     *     string response = GOptionPane::showInputDialog(message, title);
     */
    static std::string showInputDialog(std::string message, std::string title = "", GWindow *parent = NULL);
    
    /**
     * Displays an output message dialog to the user.
     * The user must click the 'OK' button to close the dialog.
     * The caller can supply an optional window title; if none is passed, a default is used.
     * The optional 'type' parameter must be one of \ref ERROR, \ref INFORMATION, \ref PLAIN,
     * \ref QUESTION, or \ref WARNING; this slightly affects the dialog's
     * appearance.  The default is \ref PLAIN.
     *
     * Sample usages:
     *
     *     GOptionPane::showMessageDialog(message);
     *     GOptionPane::showMessageDialog(message, title);
     *     GOptionPane::showMessageDialog(message, title, type);
     */
    static void showMessageDialog(std::string message, std::string title = "",
                                  MessageType type = PLAIN, GWindow *parent = NULL);
    
    /**
     * Shows a general input box with a set of buttons from which the user may
     * choose one option.  The button names are taken from the \em options parameter.
     * The name of the button the user clicks is returned as a string.
     * If the user cancels the box, an empty string is returned.
     * The caller can supply an optional window title; if none is passed, a default is used.
     * The caller can supply an optional initially selected value from the list.
     *
     * Sample usages:
     *
     *     string choice = GOptionPane::showInputDialog(message);
     *     string choice = GOptionPane::showInputDialog(message, options);
     *     string choice = GOptionPane::showInputDialog(message, options, title);
     *     string choice = GOptionPane::showInputDialog(message, options, title, initiallySelected);
     */
    static std::string showOptionDialog(std::string message, const Vector<std::string>& options,
                                        std::string title = "", std::string initiallySelected = "",
                                        GWindow *parent = NULL);


private:
    /*
     * Private constructor so that clients don't try to construct
     * GOptionPane objects.  The class is meant to be used via its
     * static methods, not by instantiation.
     */
    GOptionPane();
    
    enum InternalResult {
        /*
         * The results that can come back from showConfirmDialog.
         * These are converted into Result enum values.
         */
        CANCEL_OPTION = 2,
        CLOSED_OPTION = -1,
        NO_OPTION = 1,
        OK_OPTION = 0,
        YES_OPTION = 0
    };
};

#endif
