/*
 * @(#)JBETopCompound.java   3.01.1 07/30/14
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

import edu.stanford.cs.java.graphics.GCompound;
import edu.stanford.cs.java.graphics.GObject;
import edu.stanford.cs.java.graphics.GContainer;


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
		   // This compound has a JBECanvas above it,
		   // but is being removed from its parent, so we
		   // must remove any interactors in this compound or
		   // compounds below this one from the canvas. (JL)
		   mapRemoveInteractorsFromCanvas(owner);
		   owner = null;
	   } else if (parent instanceof JBETopCompound) {
		   JBECanvas jc = ((JBETopCompound) parent).getCanvas();
		   if (jc != null) {
			   // This compound is being added to a compound that
			   // has a canvas above it, so we must add any interactors
			   // in this compound or compounds below this one to
			   // the canvas. (JL)
			   owner = jc;
			   mapAddInteractorsToCanvas(jc);
		   }
	   }
   }

   // JL
   public void mapAddInteractorsToCanvas(JBECanvas jc) {
	   int nElements = getElementCount();
		for (int i = 0; i < nElements; i++) {
			GObject gobj = getElement(i);
			if (gobj instanceof GInteractor) {
				jc.add(((GInteractor) gobj).getInteractor());
				jc.validate();
			} else if (gobj instanceof JBETopCompound) {
				((JBETopCompound) gobj).mapAddInteractorsToCanvas(jc);
			}
		}
   }
   
   // JL
   public void mapRemoveInteractorsFromCanvas(JBECanvas jc) {
	   int nElements = getElementCount();
		for (int i = 0; i < nElements; i++) {
			GObject gobj = getElement(i);
			if (gobj instanceof GInteractor) {
				jc.remove(((GInteractor) gobj).getInteractor());
				jc.validate();
			} else if (gobj instanceof JBETopCompound) {
				((JBETopCompound) gobj).mapRemoveInteractorsFromCanvas(jc);
			}
		}
   }
   
   private JBECanvas owner;
}
