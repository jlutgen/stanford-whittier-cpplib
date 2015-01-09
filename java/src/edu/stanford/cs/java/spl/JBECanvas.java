/*
 * @(#)JBECanvas.java   3.01.1 07/30/14
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

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Shape;
import java.awt.Toolkit;
import java.awt.geom.Arc2D;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Line2D;
import java.awt.geom.Rectangle2D;

import javax.swing.SwingUtilities;

public class JBECanvas extends GCanvas {

	public JBECanvas(String id, int width, int height) {
		windowId = id;
		setBackground(Color.WHITE);
		setForeground(Color.BLACK);
		setOpaque(false);
		setLayout(null);
		preferredWidth = width;
		preferredHeight = height;
	}

	protected Graphics2D getOSG() {
		return osg;
	}

	protected void setTopCompound(JBETopCompound top) {
		topCompound = top;
		top.setCanvas(this);
		top.setParent(this);
	}

	protected JBETopCompound getTopCompound() {
		return topCompound;
	}

	protected void initOffscreenImage() {
		SwingUtilities.invokeLater(new Runnable() {
	        public void run() {
	        	Dimension size = Toolkit.getDefaultToolkit().getScreenSize();
	    		offscreenImage = createImage(size.width, size.height);
	    		osg = (Graphics2D) offscreenImage.getGraphics();
	    		osg.setColor(getBackground());
	    		osg.fillRect(0, 0, size.width, size.height);
	    		osg.setColor(getForeground());
	        }
	    });
	}

	public String getWindowId() {
		return windowId;
	}

	public Dimension getPreferredSize() {
		return new Dimension(preferredWidth, preferredHeight);
	}

	// called only by JBEWindow.clear
	public void clear() {
		Dimension size = Toolkit.getDefaultToolkit().getScreenSize();
		osg.setColor(Color.WHITE);
		osg.fillRect(0, 0, size.width, size.height);
		osg.setColor(Color.BLACK);
		repaint();	 
	}

	// UNUSED
	public void draw(Shape shape) {
		final Shape fshape = shape;
		SwingUtilities.invokeLater(new Runnable() {
	        public void run() {
	        	osg.draw(fshape);
	    		repaint();
	        }
	    });
	}

	// UNUSED
	public void fill(Shape shape) {
		final Shape fshape = shape;
		SwingUtilities.invokeLater(new Runnable() {
	        public void run() {
	        	osg.fill(fshape);
	    		repaint();
	        }
	    });
	}

	// UNUSED
	public void drawRect(double x, double y, double width, double height) {
		final Rectangle2D r = new Rectangle2D.Double(x, y, width, height);
		SwingUtilities.invokeLater(new Runnable() {
	        public void run() {
	        	osg.draw(r);
	    		repaint();
	        }
	    });
	}

	// UNUSED
	public void fillRect(double x, double y, double width, double height) {
		final Rectangle2D r = new Rectangle2D.Double(x, y, width, height);
		SwingUtilities.invokeLater(new Runnable() {
	        public void run() {
	        	osg.fill(r);
	    		osg.draw(r);
	    		repaint();
	        }
	    });
	}

	// UNUSED
	public void drawOval(double x, double y, double width, double height) {
		final Ellipse2D e = new Ellipse2D.Double(x, y, width, height);
		SwingUtilities.invokeLater(new Runnable() {
	        public void run() {
	        	osg.draw(e);
	    		repaint();	        }
	    });
	}

	// UNUSED
	public void fillOval(double x, double y, double width, double height) {
		final Ellipse2D e = new Ellipse2D.Double(x, y, width, height);
		SwingUtilities.invokeLater(new Runnable() {
	        public void run() {
	        	osg.fill(e);
	    		osg.draw(e);
	    		repaint();	        }
	    });
	}

	// UNUSED
	public void drawArc(double x, double y, double width, double height,
			double start, double sweep) {
		final Arc2D arc = new Arc2D.Double(x, y, width, height,
				start, sweep, Arc2D.OPEN);
		SwingUtilities.invokeLater(new Runnable() {
	        public void run() {
	        	osg.draw(arc);
	    		repaint();	        }
	    });
	}

	// UNUSED
	public void fillArc(double x, double y, double width, double height,
			double start, double sweep) {
		final Arc2D arc = new Arc2D.Double(x, y, width, height,
				start, sweep, Arc2D.PIE);
		SwingUtilities.invokeLater(new Runnable() {
	        public void run() {
	        	osg.fill(arc);
	    		osg.draw(arc);
	    		repaint();	        }
	    });
	}

	// UNUSED
	public void drawLine(double x0, double y0, double x1, double y1) {
		final Line2D line = new Line2D.Double(x0, y0, x1, y1);
		SwingUtilities.invokeLater(new Runnable() {
	        public void run() {
	        	osg.draw(line);
	    		repaint();	        }
	    });
	}

	// UNUSED
	public void setColor(int rgb) {
		final int frgb = rgb;
		SwingUtilities.invokeLater(new Runnable() {
	        public void run() {
	    		osg.setColor(new Color(frgb));
	        }
	    });
	}

	public void paint(Graphics g) {
		g.drawImage(offscreenImage, 0, 0, this);
		topCompound.paint(g);
		super.paint(g);
	}

	/* Instance variables */

	private String windowId;
	private JBETopCompound topCompound;
	private Graphics2D osg;
	private Image offscreenImage;
	private int preferredWidth;
	private int preferredHeight;

	/* Serial version UID */
	/**
	 * The serialization code for this class.  This value should be incremented
	 * whenever you change the structure of this class in an incompatible way,
	 * typically by adding a new instance variable.
	 */

	static final long serialVersionUID = 21L;
}
