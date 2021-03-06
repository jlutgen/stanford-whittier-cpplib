/*
 * GInteractor.java
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

import edu.stanford.cs.java.graphics.GCanvas;
import edu.stanford.cs.java.graphics.GContainer;
import edu.stanford.cs.java.graphics.GDimension;
import edu.stanford.cs.java.graphics.GMath;
import edu.stanford.cs.java.graphics.GObject;
import edu.stanford.cs.java.graphics.GPoint;
import edu.stanford.cs.java.graphics.GRectangle;
import edu.stanford.cs.java.graphics.GResizable;

import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.Graphics;
import java.awt.Point;

import javax.swing.JComponent;
import javax.swing.SwingUtilities;


public abstract class GInteractor extends GObject implements GResizable {

   public GInteractor(JComponent comp) {
      interactor = comp;
      actionCommand = "";
      Dimension size = comp.getPreferredSize();
      comp.setSize(size.width, size.height);
   }

   public JComponent getInteractor() {
      return interactor;
   }

   public GRectangle getBounds() {
      Point pt = interactor.getLocation();
      Dimension size = interactor.getPreferredSize();
      return new GRectangle(pt.x, pt.y, size.width, size.height);
   }

   public void setLocation(double x, double y) {
      super.setLocation(x, y);
      updateCanvasLocation();
   }

   public void updateCanvasLocation() {
	   GContainer parent = getParent();
	   if (parent instanceof JBETopCompound) {
		   GCanvas gc = ((JBETopCompound) parent).getCanvas();
		   if (gc != null) {
			   GPoint canvasLoc = ((JBETopCompound) parent).getCanvasPoint(getX(), getY());
			   interactor.setLocation(GMath.round(canvasLoc.getX()), 
					                  GMath.round(canvasLoc.getY()));
			   interactor.repaint();
		   }
	   }
   }

   public void setVisible(boolean visible) {
      super.setVisible(visible);
      interactor.setVisible(visible);
      interactor.repaint();
   }
   
   // JL added this method so front end can enable/disable buttons
   public void setEnabled(boolean enabled) {
	   interactor.setEnabled(enabled);
	   interactor.repaint();
   }
   
   // overrides GObject
   public void setParent(GContainer parent) {
	  super.setParent(parent);
      if (parent == null) {
         Container interactorParent = interactor.getParent(); // JComponent's parent (a GCanvas)
         if (interactorParent != null) {
         	interactorParent.remove(interactor);
         	interactorParent.validate();
        }
      } else if (parent instanceof JBETopCompound) {
         GCanvas gc = ((JBETopCompound) parent).getCanvas();
         if (gc != null) {
        	 updateCanvasLocation();
        	 gc.add(interactor);
        	 gc.validate();
         }
      }
   }

   public void setSize(double width, double height) {
      final int iw = GMath.round(width);
      final int ih = GMath.round(height);
      SwingUtilities.invokeLater(new Runnable() {
          public void run() {
        	  interactor.setPreferredSize(new Dimension(iw, ih));
              interactor.setSize(iw, ih);
              interactor.repaint();          }
      });
      
   }

   public void setSize(GDimension size) {
      setSize(size.getWidth(), size.getHeight());
   }

   public void setBounds(double x, double y, double width, double height) {
      setLocation(x, y);
      setSize(width, height);
   }

   public void setBounds(GRectangle bounds) {
      setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(),
                                              bounds.getHeight());
   }

   public void setActionCommand(String cmd) {
      actionCommand = cmd;
   }

   // Method added by JL so front end can set foreground color for interactors
   public void setColor(Color c) {
	   final Color fc = c;
	   SwingUtilities.invokeLater(new Runnable() {
		   public void run() {
			   interactor.setForeground(fc);
			   interactor.repaint();
		   }
	   });
   }
   
   // Method added by JL (but it's never called)
   public Color getColor() {
	   return interactor.getForeground();
   }
   
   public String getActionCommand() {
      return actionCommand;
   }

   public void repaint() {
      /* Empty */
   }

   public void paint2d(Graphics2D g) {
	   /* Empty */
   }

   protected void paintObject(Graphics g) {
      /* Empty */
   }

   private JComponent interactor;
   private String actionCommand;

}
