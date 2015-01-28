/*
 * JBELabel.java
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

package edu.stanford.cs.java.spl;

import edu.stanford.cs.java.graphics.GLabel;

import java.awt.Color;
import java.awt.Font;

import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.SwingUtilities;

public class JBELabel extends GLabel {
   public JBELabel(String label) {
      super(label);
      jlabel = null;
   }

   public JComponent getInteractor() {
      if (jlabel == null) {
         jlabel = new JLabel(getLabel());
         jlabel.setFont(getFont());
         jlabel.setForeground(getColor());
      }
      return jlabel;
   }

   // overrides GLabel. Added by JL
   public void setFont(Font font) {
	   final Font ffont = font;
	   SwingUtilities.invokeLater(new Runnable() {
	        public void run() {
	        	JBELabel.super.setFont(ffont);
	            if (jlabel != null) jlabel.setFont(ffont);
	        }
	    });
      
   }

   // overrides GLabel
   public void setColor(Color color) {
	  final Color fcolor = color;
	  SwingUtilities.invokeLater(new Runnable() {
	        public void run() {
	        	JBELabel.super.setColor(fcolor);
	            if (jlabel != null) jlabel.setForeground(fcolor);
	        }
	    });
   }

   // overrides GLabel. Added by JL
   public void setLabel(String label) {
	  final String flabel = label;
	  SwingUtilities.invokeLater(new Runnable() {
	        public void run() {
	        	JBELabel.super.setLabel(flabel);
	            if (jlabel != null) jlabel.setText(flabel);
	        }
	    });
   }
   
   // overrides GLabel. Added by JL
   public void setVisible(boolean visible) {
	  final boolean fvisible = visible;
	  SwingUtilities.invokeLater(new Runnable() {
	        public void run() {
	        	if (jlabel != null) jlabel.setVisible(fvisible);
	        	JBELabel.super.setVisible(fvisible); 
	        }
	    });
   }
   
   private JLabel jlabel;
}
