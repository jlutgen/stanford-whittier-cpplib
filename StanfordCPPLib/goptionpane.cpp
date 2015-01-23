/*
 * File: goptionpane.cpp
 * ---------------------
 * This file implements the operations declared in the goptionpane.h file.
 * See goptionpane.h for documentation of each member.
 * 
 * @author Marty Stepp
 * @author Jeff Lutgen
 * * @version 2015/01/17
 * - removed showTextFile Dialog
 * @version 2014/10/26
 * - added showTextFileDialog
 * @version 2014/10/09
 * - finished implementation of showOptionDialog; added dialog types and other fixes
 * - enum rearrangement for clearer client code and better error messages
 * - removed 'using namespace' statement
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

#include "goptionpane.h"
#include "platform.h"

static Platform* pp = getPlatform();

GOptionPane::GOptionPane() {
    // empty
}

GOptionPane::ConfirmResult GOptionPane::showConfirmDialog(GWindow *parent, std::string message, std::string title,
                                                          ConfirmType type) {
    if (type != GOptionPane::ConfirmType::YES_NO
            && type != GOptionPane::ConfirmType::YES_NO_CANCEL
            && type != GOptionPane::ConfirmType::OK_CANCEL) {
        error("GOptionPane::showConfirmDialog: Illegal dialog type");
    }
    if (title.empty()) {
        title = "Select an option";
    }
    
    int result = pp->goptionpane_showConfirmDialog(message, title, type, parent);
    if (result == OK_OPTION || result == YES_OPTION) {
        // this is weird code because JOptionPane thinks of OK and Yes as the same,
        // and differentiates based on context of whether this is an OK/Cancel or Yes/No dialog
        return (type == GOptionPane::ConfirmType::OK_CANCEL)
                ? GOptionPane::ConfirmResult::OK : GOptionPane::ConfirmResult::YES;
    } else if (result == NO_OPTION) {
        return GOptionPane::ConfirmResult::NO;
    } else {
        return GOptionPane::ConfirmResult::CANCEL;
    }
}

std::string GOptionPane::showInputDialog(GWindow *parent, std::string message, std::string title) {
    return pp->goptionpane_showInputDialog(message, title, parent);
}

void GOptionPane::showMessageDialog(GWindow *parent, std::string message, std::string title, MessageType type) {
    if (type != GOptionPane::MessageType::PLAIN_MESSAGE
            && type != GOptionPane::MessageType::INFORMATION_MESSAGE
            && type != GOptionPane::MessageType::ERROR_MESSAGE
            && type != GOptionPane::MessageType::WARNING_MESSAGE
            && type != GOptionPane::MessageType::QUESTION_MESSAGE) {
        error("GOptionPane::showMessageDialog: Illegal dialog type");
    }
    if (title.empty()) {
        title = "Message";
    }
    pp->goptionpane_showMessageDialog(message, title, type, parent);
}

std::string GOptionPane::showOptionDialog(GWindow *parent, std::string message, const Vector<std::string>& options,
                                          std::string title, std::string initiallySelected) {
    if (title.empty()) {
        title = "Select an option";
    }
    int index = pp->goptionpane_showOptionDialog(message, title, options, initiallySelected, parent);
    if (index == GOptionPane::InternalResult::CLOSED_OPTION
            || index < 0 || index >= options.size()) {
        return "";
    } else {
        return options[index];
    }
}

