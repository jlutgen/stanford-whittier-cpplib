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
 * @class GOptionPane
 *
 * @brief This class provides static methods that support pop-up graphical dialog
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
        ERROR_MESSAGE = 0,          ///<.
        INFORMATION_MESSAGE = 1,    ///<.
        PLAIN_MESSAGE = -1,         ///<.
        QUESTION_MESSAGE = 3,       ///<.
        WARNING_MESSAGE = 2         ///<.
    };

    /**
     * Pops up a yes/no confirmation box.
     * Once the user clicks a button to close the box, one of the
     * \ref GOptionPaneResult enumeration constants is returned.
     * The caller can supply an optional window title; if none is passed, a default is used.
     *
     * The parameter \em parent specifies the GWindow in front of which the dialog box will
     * pop up. If \em parent is \code NULL, the dialog will pop up in front of the graphical
     * console window.
     *
     * Sample usages:
     *
     *     int result = GOptionPane::showConfirmDialog(parent, message);
     *     int result = GOptionPane::showConfirmDialog(parent, message, title);
     *     int result = GOptionPane::showConfirmDialog(parent, message, title, type);
     */
    static ConfirmResult showConfirmDialog(GWindow *parent, std::string message, std::string title = "",
                                           ConfirmType type = YES_NO);
    
    /**
     * Pops up an input box with a text field where the user can type a
     * response, which is returned.
     * The caller can supply an optional window title; if none is passed, a default is used.
     * If the user cancels the box, an empty string is returned.
     *
     * The parameter \em parent specifies the GWindow in front of which the dialog box will
     * pop up. If \em parent is \code NULL, the dialog will pop up in front of the graphical
     * console window.
     *
     * Sample usages:
     *
     *     string response = GOptionPane::showInputDialog(parent, message);
     *     string response = GOptionPane::showInputDialog(parent, message, title);
     */
    static std::string showInputDialog(GWindow *parent, std::string message, std::string title = "");
    
    /**
     * Displays an output message dialog to the user.
     * The user must click the 'OK' button to close the dialog.
     * The caller can supply an optional window title; if none is passed, a default is used.
     * The optional 'type' parameter must be one of \ref ERROR_MESSAGE, \ref INFORMATION_MESSAGE,
     * \ref PLAIN_MESSAGE, \ref QUESTION_MESSAGE, or \ref WARNING_MESSAGE; this slightly affects
     * the dialog's appearance.  The default is \ref PLAIN_MESSAGE.
     *
     * The parameter \em parent specifies the GWindow in front of which the dialog box will
     * pop up. If \em parent is \code NULL, the dialog will pop up in front of the graphical
     * console window.
     *
     * Sample usages:
     *
     *     GOptionPane::showMessageDialog(parent, message);
     *     GOptionPane::showMessageDialog(parent, message, title);
     *     GOptionPane::showMessageDialog(parent, message, title, type);
     */
    static void showMessageDialog(GWindow *parent, std::string message, std::string title = "",
                                  MessageType type = PLAIN_MESSAGE);
    
    /**
     * Shows a general input box with a set of buttons from which the user may
     * choose one option.  The button names are taken from the \em options parameter.
     * The name of the button the user clicks is returned as a string.
     * If the user cancels the box, an empty string is returned.
     * The caller can supply an optional window title; if none is passed, a default is used.
     * The caller can supply an optional initially selected value from the list.
     *
     * The parameter \em parent specifies the GWindow in front of which the dialog box will
     * pop up. If \em parent is \code NULL, the dialog will pop up in front of the graphical
     * console window.
     *
     * Sample usages:
     *
     *     string choice = GOptionPane::showInputDialog(parent, message);
     *     string choice = GOptionPane::showInputDialog(parent, message, options);
     *     string choice = GOptionPane::showInputDialog(parent, message, options, title);
     *     string choice = GOptionPane::showInputDialog(parent, message, options, title, initiallySelected);
     */
    static std::string showOptionDialog(GWindow *parent, std::string message, const Vector<std::string>& options,
                                        std::string title = "", std::string initiallySelected = "");


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
