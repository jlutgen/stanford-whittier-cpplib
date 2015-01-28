/*
 * JBEConsole.java
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

import java.awt.Color;
import java.awt.Font;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;
import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultStyledDocument;
import javax.swing.text.Document;
import javax.swing.text.SimpleAttributeSet;
import javax.swing.text.StyleConstants;

/**
 * This class simulates a console for the Stanford Portable Library.
 */

public class JBEConsole extends JScrollPane {

	/**
	 * Creates a new <code>JBEConsole</code> object.
	 */

	public JBEConsole() {
		//listeners = new ArrayList<ActionListener>();
		ctp = new ConsoleTextPane(this);
		ctp.setFont(DEFAULT_FONT);
		ctp.setBackground(Color.WHITE);
		ctp.setInputColor(Color.BLUE);
		ctp.setErrorColor(Color.RED);
		setViewportView(ctp);
		setMargin(DEFAULT_MARGIN);
		setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
		setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
		setBorder(BorderFactory.createLineBorder(Color.BLACK));
	}

	public JMenuBar createMenuBar() {
		return ctp.createMenuBar();
	}

	public void setMargin(int pixels) {
		setMargin(new Insets(pixels, pixels, pixels, pixels));
	}

	public void setMargin(Insets insets) {
		ctp.setMargin(insets);
	}

	public void clear() {
		ctp.clear();
	}

	public void print(Object x, boolean isStdErr) {
		if (isStdErr) {
			ctp.print(x.toString(), ConsoleTextPane.ERROR_STYLE);
		} else {
			ctp.print(x.toString(), ConsoleTextPane.OUTPUT_STYLE);
		}
	}

	public void println() {
		ctp.print("\n", ConsoleTextPane.OUTPUT_STYLE);
	}

	public void println(Object x, boolean isStdErr) {
		print(x, isStdErr);
		println();
	}

	public void printf(String format, Object... args) {
		print(String.format(format, args), false);
	}

	public void setFont(String str) {
		setFont(Font.decode(str));
	}

	public void setFont(final Font font) {
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				JBEConsole.super.setFont(font);
				if (ctp != null)
					ctp.setFont(font);
			}
		});
	}

	public String getLine() {
		ctp.requestFocusInWindow(); // JL added
		inputComplete = false;
		synchronized (this) {
			while (!inputComplete) {
				try {
					wait();
				} catch (InterruptedException ex) {
					/* Empty */
				}
			}
		}
		return inputLine;
	}

	public void processLine(String str) {
		synchronized (this) {
			inputComplete = true;
			inputLine = str;
			notifyAll();
		}
	}

	/* Constants */

	public static final Font DEFAULT_FONT = Font.decode("Monospaced-12");
	public static final int DEFAULT_MARGIN = 2;

	/* Private instance variables */

	//private ArrayList<ActionListener> listeners;
	private ConsoleTextPane ctp;
	private String inputLine;
	private boolean inputComplete;

	/* Serial version UID */
	/**
	 * The serialization code for this class.  This value should be incremented
	 * whenever you change the structure of this class in an incompatible way,
	 * typically by adding a new instance variable.
	 */

	static final long serialVersionUID = 21L;
}

class ConsoleTextPane extends JTextPane implements KeyListener {

	public ConsoleTextPane(JBEConsole console) {
		this.console = console;
		addKeyListener(this);
		document = getDocument(); // a DefaultStyledDocument
		//lineSeparator = System.getProperty("line.separator");
		outputAttributes = new SimpleAttributeSet();
		inputAttributes = new SimpleAttributeSet();
		errorAttributes = new SimpleAttributeSet();
		base = 0;
		lastChar = -1;
	}

	/**
	 * Create an Edit menu to support cut/copy/paste.
	 */
	// Called from Event Dispatch Thread
	public JMenuBar createMenuBar () {
		JMenuItem menuItem = null;
		JMenuBar menuBar = new JMenuBar();
		JMenu mainMenu = new JMenu("Edit");

		menuItem = new JMenuItem(new cutAction());
		menuItem.setText("Cut");
		menuItem.setAccelerator(KeyStroke.getKeyStroke("control X"));
		mainMenu.add(menuItem);

		menuItem = new JMenuItem(new copyAction());
		menuItem.setText("Copy");
		menuItem.setAccelerator(KeyStroke.getKeyStroke("control C"));
		mainMenu.add(menuItem);

		menuItem = new JMenuItem(new pasteAction());
		menuItem.setText("Paste");
		menuItem.setAccelerator(KeyStroke.getKeyStroke("control V"));
		mainMenu.add(menuItem);

		menuItem = new JMenuItem(new selectAllAction());
		menuItem.setText("Select All");
		menuItem.setAccelerator(KeyStroke.getKeyStroke("control A"));
		mainMenu.add(menuItem);

		menuBar.add(mainMenu);
		return menuBar;
	}

	class copyAction extends AbstractAction {
		public copyAction() {
			setEnabled(true);
		}

		public void actionPerformed(ActionEvent e) {
			copy();
		}

		/* Serial version UID */
		/**
		 * The serialization code for this class.  This value should be incremented
		 * whenever you change the structure of this class in an incompatible way,
		 * typically by adding a new instance variable.
		 */

		static final long serialVersionUID = 21L;
	}

	class cutAction extends AbstractAction {
		public cutAction() {
			setEnabled(true);
		}

		public void actionPerformed(ActionEvent e) {
			cut();
		}
		/* Serial version UID */
		/**
		 * The serialization code for this class.  This value should be incremented
		 * whenever you change the structure of this class in an incompatible way,
		 * typically by adding a new instance variable.
		 */

		static final long serialVersionUID = 21L;
	}

	class pasteAction extends AbstractAction {
		public pasteAction() {
			setEnabled(true);
		}

		public void actionPerformed(ActionEvent e) {
			paste();
		}
		/* Serial version UID */
		/**
		 * The serialization code for this class.  This value should be incremented
		 * whenever you change the structure of this class in an incompatible way,
		 * typically by adding a new instance variable.
		 */

		static final long serialVersionUID = 21L;
	}

	class selectAllAction extends AbstractAction {
		public selectAllAction() {
			setEnabled(true);
		}

		public void actionPerformed(ActionEvent e) {
			selectAll();
		}
		/* Serial version UID */
		/**
		 * The serialization code for this class.  This value should be incremented
		 * whenever you change the structure of this class in an incompatible way,
		 * typically by adding a new instance variable.
		 */

		static final long serialVersionUID = 21L;
	}

	/**
	 * Prints the string to the console.
	 */

	public void print(String str, int style) {
		final String fstr = str;
		final int fstyle = style;
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				insert(fstr, base, fstyle);
				base += fstr.length();
				setCaretPosition(base);
			}
		});	
	}

	/**
	 * Clears the console pane.
	 */

	public void clear() {
		SwingUtilities.invokeLater(new Runnable() {
	        public void run() {
	        	setText("");
	        	base = 0;
	        }
	    });
	}

	/**
	 * Returns the text stored in the console model.
	 */

	public String getText() {
		return getText();
	}

	/**
	 * Returns a substring from the text using endpoints defined as in
	 * <code>substring</code> for strings.
	 */
	// Called from Event Dispatch Thread
	public String getText(int start, int end) {
		try {
			return document.getText(start, end - start);
		} catch (BadLocationException ex) {
			throw new RuntimeException(ex);
		}
	}

	/**
	 * Returns the length of the text stored in the console model.
	 */
	// Called from Event Dispatch Thread
	public int getLength() {
		return document.getLength();
	}

	/**
	 * Implements the "cut" menu operation.
	 */

	@Override
	public void cut() {
		copy();
		deleteSelection();
	}

	/**
	 * Implements the "paste" menu operation.
	 */

	@Override
	public void paste() {
		if (getSelectionEnd() != document.getLength()) return;
		int start = deleteSelection();
		setSelectionStart(start);
		super.paste();
		select(document.getLength(), document.getLength());
		if (document instanceof DefaultStyledDocument) {
			DefaultStyledDocument doc = (DefaultStyledDocument) document;
			doc.setCharacterAttributes(start, getSelectionEnd() - start,
					inputAttributes, true);
		}
	}

	/**
	 * Returns true if the selection is a single point.
	 */

	public boolean isPointSelection() {
		return getSelectionStart() == getSelectionEnd();
	}

	/**
	 * Sets the style parameters for console input.  The style parameter
	 * is either <code>Font.PLAIN</code> or a sum of one or more of the attributes
	 * <code>Font.BOLD</code> and <code>Font.ITALIC</code>.
	 */

	public void setInputStyle(int style) {
		if (getLength() != 0) {
			throw new RuntimeException("Console styles and colors cannot be " +
					"changed after I/O has started.");
		}
		inputAttributes.addAttribute(StyleConstants.Bold,
				new Boolean((style & Font.BOLD) != 0));
		inputAttributes.addAttribute(StyleConstants.Italic,
				new Boolean((style & Font.ITALIC) != 0));
	}

	/**
	 * Sets the color used for console input.
	 */

	public void setInputColor(Color color) {
		if (getLength() != 0) {
			throw new RuntimeException("Console styles and colors cannot be " +
					"changed after I/O has started.");
		}
		inputAttributes.addAttribute(StyleConstants.Foreground, color);
	}

	/**
	 * Sets the style parameters for console error messages.  The style parameter
	 * is either <code>Font.PLAIN</code> or a sum of one or more of the attributes
	 * <code>Font.BOLD</code> and <code>Font.ITALIC</code>.
	 */
	public void setErrorStyle(int style) {
		if (getLength() != 0) {
			throw new RuntimeException("Console styles and colors cannot be " +
					"changed after I/O has started.");
		}
		errorAttributes.addAttribute(StyleConstants.Bold,
				new Boolean((style & Font.BOLD) != 0));
		errorAttributes.addAttribute(StyleConstants.Italic,
				new Boolean((style & Font.ITALIC) != 0));
	}

	/**
	 * Sets the color used for console error messages.
	 */

	public void setErrorColor(Color color) {
		if (getLength() != 0) {
			throw new RuntimeException("Console styles and colors cannot be " +
					"changed after I/O has started.");
		}
		errorAttributes.addAttribute(StyleConstants.Foreground, color);
	}

	/* KeyListener */

	@Override
	public void keyTyped(KeyEvent e) {
		if (!e.isMetaDown() && !e.isControlDown()) {
			processChar(e.getKeyChar());
			e.consume();
		}
	}

	@Override
	public void keyPressed(KeyEvent e) {
		switch (e.getKeyCode()) {
		case KeyEvent.VK_LEFT:
			processChar('B' - '@');
			e.consume();
			break;
		case KeyEvent.VK_RIGHT:
			processChar('F' - '@');
			e.consume();
			break;
		case KeyEvent.VK_X:
		case KeyEvent.VK_C:
		case KeyEvent.VK_V:
		case KeyEvent.VK_A:
			if (e.isControlDown()) {
				// Pass menu accelerator keystrokes upward.
				this.getParent().dispatchEvent(e);
			}
			else {
				e.consume();
			}
			break;
		default:
			e.consume();
		}
	}

	@Override
	public void keyReleased(KeyEvent e) {
		e.consume();
	}

	/**
	 * Process a single input character.
	 */

	private void processChar(int ch) {
		if (ch == '\n') {
			if (lastChar != '\r') signalEndOfLine();
		} else if (ch == '\r') {
			if (lastChar != '\n') signalEndOfLine();
		} else {
			if (getCaretPosition() < base) {
				setCaretPosition(getLength());
			}
			int dot = getSelectionStart();
			switch (ch) {
			case '\b': case '\177':
				if (dot == getSelectionEnd()) {
					if (dot > base) {
						delete(dot - 1, dot);
						dot--;
					}
				} else {
					dot = deleteSelection();
				}
				break;
			case 'A'-'@':
				selectAll();
				dot = -1;
				break;
			case 'B'-'@':
				dot = Math.max(getSelectionStart() - 1, base);
				break;
			case 'C'-'@':
				copy();
				dot = -1;
				break;
			case 'F'-'@':
				dot = Math.min(getSelectionEnd() + 1, getLength());
				break;
			case 'V'-'@':
				paste();
				dot = -1;
				break;
			case 'X'-'@':
				cut();
				dot = -1;
				break;
			default:
				if (dot != getSelectionEnd()) {
					dot = deleteSelection();
				}
				insert("" + (char) ch, dot, INPUT_STYLE);
				dot++;
			}
			if (dot != -1) {
				select(dot, dot);
				setCaretPosition(dot);
			}
		}
		lastChar = ch;
	}

	private void signalEndOfLine() {
		int len = getLength() - base;
		String line = getText(base, base + len);
		insert("\n", base + len, OUTPUT_STYLE);
		base += len + 1;
		console.processLine(line);
	}

	/**
	 * Inserts a string into the text pane at the position specified by
	 * <code>dot</code>, using the specified style.
	 */
	
	private void insert(String str, int dot, int style) {
		try {
			SimpleAttributeSet attributes = outputAttributes;
			switch (style) {
			case INPUT_STYLE: attributes = inputAttributes; break;
			case ERROR_STYLE: attributes = errorAttributes; break;
			}
			document.insertString(dot, str, attributes);
		} catch (BadLocationException ex) {
			/* Empty */
		}
	}

	/**
	 * Deletes text from the text pane beginning at position <code>p1</code> and
	 * continuing up to but not including <code>p2</code>.
	 */
	// Called from Event Dispatch Thread
	private void delete(int p1, int p2) {
		try {
			document.remove(p1, p2 - p1);
		} catch (BadLocationException ex) {
			throw new RuntimeException(ex);
		}
	}

	/**
	 * Deletes the current selection and returns the index of the deletion point.
	 */

	private int deleteSelection() {
		int start = Math.max(base, getSelectionStart());
		int end = getSelectionEnd();
		if (end <= base) return getLength();
		delete(start, end);
		return start;
	}

	/**
	 * Sets the relevant components of the graphics context from the attribute set.
	 */
	/*
   private void setStyleFromAttributes(Graphics g, AttributeSet attributes) {
      Font oldFont = getFont();
      int style = 0;
      if ((Boolean) attributes.getAttribute(StyleConstants.Bold)) {
         style |= Font.BOLD;
      }
      if ((Boolean) attributes.getAttribute(StyleConstants.Italic)) {
         style |= Font.ITALIC;
      }
      g.setFont(new Font(oldFont.getName(), style, oldFont.getSize()));
      Color color = (Color) attributes.getAttribute(StyleConstants.Foreground);
      if (color == null) color = getForeground();
      g.setColor(color);
   }
	 */
	/* Constants */

	public static final int OUTPUT_STYLE = 0;
	public static final int INPUT_STYLE = 1;
	public static final int ERROR_STYLE = 2;

	/* Private instance variables */

	//   private ActionListener actionListener;
	private Document document;
	private JBEConsole console;
	private SimpleAttributeSet errorAttributes;
	private SimpleAttributeSet inputAttributes;
	private SimpleAttributeSet outputAttributes;
	//private String lineSeparator;
	private int base;
	private int lastChar;

	/* Serial version UID */
	/**
	 * The serialization code for this class.  This value should be incremented
	 * whenever you change the structure of this class in an incompatible way,
	 * typically by adding a new instance variable.
	 */

	static final long serialVersionUID = 21L;
}
