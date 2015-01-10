/*
 * @GTextArea.java
 * 
 * @author Jeff Lutgen
 */

/*************************************************************************/
/* Stanford Portable Library (adapted from the ACM graphics library)     */
/* Copyright (C) 2014 by Eric Roberts <eroberts@cs.stanford.edu>         */
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

package edu.stanford.cs.java.graphics;

import java.awt.Component;
import java.awt.Font;
import java.awt.Graphics2D;
import java.awt.Shape;
import java.awt.TextArea;
import java.awt.geom.AffineTransform;
import java.awt.geom.Rectangle2D;

/**
 * The <code>GTextArea</code> class is a graphical object whose appearance
 * consists of a rectangular area that can display text.
 */

public class GTextArea extends GObject {

	/**
	 * The default font used to display text.  You can change the font by
	 * invoking the <a href="#setFont(Font)"><code>setFont</code></a> method.
	 */

	public static final Font DEFAULT_FONT = new Font("Default", Font.PLAIN, 12);

	/**
	 * Creates a new <code>GLabel</code> object initialized to contain the
	 * specified string.
	 *
	 * @param rows The number of rows displayed by this <code>GTextArea</code>
	 * @param columns The number of columns displayed by this <code>GTextArea</code>
	 */

	public GTextArea(int rows, int columns) {
		this(rows, columns, 0, 0);
	}


	/**
	 * Creates a new <code>GTextArea</code> object with its anchor point at the
	 * specified position.
	 *
	 * @param x The x-coordinate of the anchor point for this <code>GTextArea</code>
	 * @param y The y-coordinate of the anchor point for this <code>GTextArea</code>
	 * @param rows The number of rows displayed by this <code>GTextArea</code>
	 * @param columns The number of columns displayed by this <code>GTextArea</code>
	 */

	public GTextArea(int rows, int columns, double x, double y) {
		textArea = new TextArea(rows, columns);
		setFont(DEFAULT_FONT);
		setLocation(x, y);
	}

	/**
	 * Changes the font used in the <code>GTextArea</code>.
	 *
	 * @param font A <code>Font</code> object indicating the new font
	 */

	public void setFont(Font font) {
		textArea.setFont(font);
	}

	/**
	 * Changes the font used to display the <code>GTextArea</code> as specified by
	 * the string <code>str</code>, which is interpreted in the style of
	 * <code>Font.decode</code>.  The usual format of the font string is
	 *
	 *<pre>
	 *    family-style-size
	 *</pre>
	 *
	 * where both <code>style</code> and <code>size</code> are optional.
	 * If any of these parts are specified as an asterisk, the existing
	 * value is retained.
	 *
	 * @param str A <code>String</code> specifying the new font
	 */

	public void setFont(String str) {
		textArea.setFont(Font.decode(str));
	}

	/**
	 * Returns the font used by this <code>GTextArea</code>.
	 *
	 * @return The font in use by this object
	 */

	public Font getFont() {
		return textArea.getFont();
	}

	/**
	 * Changes the text displayed by this <code>GTextArea</code> object.
	 *
	 * @param str The new string to display
	 */

	public void setText(String str) {
		textArea.setText(str);
	}

	/**
	 * Returns the text displayed by this object.
	 *
	 * @return The text displayed by this object
	 */

	public String getText() {
		return textArea.getText();
	}

	/**
	 * Implements the <code>paint2d</code> operation for this graphical object.
	 * This method is not called directly by clients.
	 * @noshow
	 */

	protected void paint2d(Graphics2D g) {
		/* empty */
	}

	/**
	 * Returns a <code>GRectangle</code> that specifies the bounding box for
	 * the string.
	 *
	 * @return The bounding box for this object
	 */

	public GRectangle getBounds() {
		Rectangle2D rect = textArea.getBounds();
		return new GRectangle(getX() + rect.getX(), getY() + rect.getY(),
				rect.getWidth(), rect.getHeight());
	}

	/**
	 * Checks to see whether a point is inside the object.
	 *
	 * @param x The x-coordinate of the point being tested
	 * @param y The y-coordinate of the point being tested
	 * @return <code>true</code> if the point (<code>x</code>,&nbsp;<code>y</code>)
	 *         is inside the object, and <code>false</code> otherwise
	 */

	public boolean contains(double x, double y) {
		return textArea.contains((int) (x - getX()), (int) (y - getY()));
	}

	/* Private instance variables */

	private TextArea textArea;

	/* Serial version UID */
	/**
	 * The serialization code for this class.  This value should be incremented
	 * whenever you change the structure of this class in an incompatible way,
	 * typically by adding a new instance variable.
	 */

	static final long serialVersionUID = 21L;

}
