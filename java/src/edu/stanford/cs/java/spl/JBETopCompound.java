/*
 * JBETopCompound.java
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

import javax.swing.JComponent;

import edu.stanford.cs.java.graphics.GCompound;
import edu.stanford.cs.java.graphics.GMath;
import edu.stanford.cs.java.graphics.GObject;
import edu.stanford.cs.java.graphics.GContainer;
import edu.stanford.cs.java.graphics.GPoint;


public class JBETopCompound extends GCompound {
   public void setCanvas(JBECanvas jc) {
      owner = jc;
   }

   public JBECanvas getCanvas() {
      return owner;
   }
   
   @Override
   public void setParent(GContainer parent) {
	   super.setParent(parent);
	   if (parent == null && owner != null) {
		   
		   // BUGFIX (JL)
		   // This compound has a JBECanvas above it,
		   // but is being removed from its parent, so we
		   // must remove any interactors in this compound or
		   // compounds below this one from the canvas.
		   mapRemoveInteractorsFromCanvas(owner);
		   
		   owner = null;
	   } else if (parent instanceof JBETopCompound) {
		   JBECanvas jc = ((JBETopCompound) parent).getCanvas();
		   if (jc != null) {
			   
			   // BUGFIX (JL)
			   // This compound is being added to a compound that
			   // has a canvas above it, so we must add any interactors
			   // in this compound or compounds below this one to
			   // the canvas.
			   owner = jc;
			   mapAddInteractorsToCanvas(jc);
		   }
	   }
   }

   // Method added by JL
   public void mapAddInteractorsToCanvas(JBECanvas jc) {
	   int nElements = getElementCount();
		for (int i = 0; i < nElements; i++) {
			GObject gobj = getElement(i);
			if (gobj instanceof GInteractor) {
				JComponent interactor = ((GInteractor) gobj).getInteractor();
				((GInteractor) gobj).updateCanvasLocation();
				jc.add(interactor);
				jc.validate();
			} else if (gobj instanceof JBETopCompound) {
				((JBETopCompound) gobj).setCanvas(jc);
				((JBETopCompound) gobj).mapAddInteractorsToCanvas(jc);
			}
		}
   }
   
   // Method added by JL
   public void mapRemoveInteractorsFromCanvas(JBECanvas jc) {
	   int nElements = getElementCount();
	   for (int i = 0; i < nElements; i++) {
		   GObject gobj = getElement(i);
		   if (gobj instanceof GInteractor) {
			   jc.remove(((GInteractor) gobj).getInteractor());
			   jc.validate();
		   } else if (gobj instanceof JBETopCompound) {
			   ((JBETopCompound) gobj).setCanvas(null);
			   ((JBETopCompound) gobj).mapRemoveInteractorsFromCanvas(jc);
		   }
	   }
   }
   
   // Method added by JL
   // @Override
   public void setLocation(double x, double y) {
	   super.setLocation(x, y);
	   if (owner != null) {
		   mapUpdateCanvasLocations();
	   }
   }
   
   // Method added by JL
   public void mapUpdateCanvasLocations() {
	   int nElements = getElementCount();
	   for (int i = 0; i < nElements; i++) {
		   GObject gobj = getElement(i);
		   if (gobj instanceof GInteractor) {
			   ((GInteractor) gobj).updateCanvasLocation();
		   } else if (gobj instanceof JBETopCompound) {
			   ((JBETopCompound) gobj).mapUpdateCanvasLocations();
		   }
	   }
   }
   
   // Implementation note (JL): JBETopCompound is used for all GCompounds created
   // by front end, not just the top compound. We use the `owner` field to track
   // whether this JBETopCompound has a JBECanvas as an ancestor, in which case `owner`
   // is that JBECanvas, or doesn't, in which case `owner` is null.
   private JBECanvas owner;
}
