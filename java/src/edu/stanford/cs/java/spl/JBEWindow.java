/*
 * JBEWindow.java   
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

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.WindowEvent;

import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

public class JBEWindow extends JFrame {

	public JBEWindow(JavaBackEnd parent, String id, String appName,
			int width, int height) {
		super(appName);
		jbe = parent;
		windowId = id;
		setLayout(new BorderLayout());
		canvas = new JBECanvas(id, width, height);
		addWindowListener(jbe);
		canvas.addComponentListener(jbe);
		canvas.addMouseListener(jbe);
		canvas.addMouseMotionListener(jbe);
		canvas.addKeyListener(jbe);
		add(canvas, BorderLayout.CENTER);
		createSidePanels();
	}

	public JBECanvas getCanvas() {
		return canvas;
	}

	public String getWindowId() {
		return windowId;
	}

	public void close() {
		final WindowEvent e = new WindowEvent(this, WindowEvent.WINDOW_CLOSING);
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				processWindowEvent(e);
			}
		});		
	}

	public void clear() {
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				canvas.clear();
				boolean needsValidating = false;
				if (northPanel != null) {
					northPanel.removeAll();
					needsValidating = true;
				}
				if (eastPanel != null) {
					eastPanel.removeAll();
					needsValidating = true;
				}
				if (southPanel != null) {
					southPanel.removeAll();
					needsValidating = true;
				}
				if (westPanel != null) {
					westPanel.removeAll();
					needsValidating = true;
				}
				if (needsValidating) validate();        
			}
		});
	}

	public void addToRegion(JComponent comp, String region) {
		final JComponent fcomp = comp;
		final String fregion = region;
		if (gbc == null) {
			gbc = new GridBagConstraints();
			gbc.gridwidth = GridBagConstraints.REMAINDER;
		}
		final GridBagConstraints fgbc = gbc;

		class Runner implements Runnable {
			public void run() {
				JPanel panel = null;
				if (fregion.equalsIgnoreCase("NORTH")) {
					panel = northPanel;
					if (panel != null) panel.add(fcomp);
				} else if (fregion.equalsIgnoreCase("EAST")) {
					panel = eastPanel;
					if (panel != null) panel.add(fcomp, fgbc);
				} else if (fregion.equalsIgnoreCase("SOUTH")) {
					panel = southPanel;
					if (panel != null) panel.add(fcomp);
				} else if (fregion.equalsIgnoreCase("WEST")) {
					panel = westPanel;
					if (panel != null) panel.add(fcomp, fgbc);
				}
				if (panel != null) validate();
			}
		}

		SwingUtilities.invokeLater(new Runner());
	}

	public void removeFromRegion(JComponent comp, String region) {
		final JComponent fcomp = comp;
		final String fregion = region;

		class Runner implements Runnable {
			public void run() {
				JPanel panel = null;
				if (fregion.equalsIgnoreCase("NORTH")) {
					panel = northPanel;
					if (panel != null) panel.remove(fcomp);
				} else if (fregion.equalsIgnoreCase("EAST")) {
					panel = eastPanel;
					if (panel != null) panel.remove(fcomp);
				} else if (fregion.equalsIgnoreCase("SOUTH")) {
					panel = southPanel;
					if (panel != null) panel.remove(fcomp);
				} else if (fregion.equalsIgnoreCase("WEST")) {
					panel = westPanel;
					if (panel != null) panel.remove(fcomp);
				}
				if (panel != null) validate();
			}
		}

		SwingUtilities.invokeLater(new Runner());
	}

	public void setRegionAlignment(String region, String align) {
		JPanel panel = null;
		if (region.equalsIgnoreCase("NORTH")) {
			panel = northPanel;
		} else if (region.equalsIgnoreCase("EAST")) {
			panel = eastPanel;
		} else if (region.equalsIgnoreCase("SOUTH")) {
			panel = southPanel;
		} else if (region.equalsIgnoreCase("WEST")) {
			panel = westPanel;
		}
		if (panel == southPanel || panel == northPanel) {
			int alignCode = FlowLayout.CENTER;
			if (align.equalsIgnoreCase("LEFT")) {
				alignCode = FlowLayout.LEFT;
			} else if (align.equalsIgnoreCase("RIGHT")) {
				alignCode = FlowLayout.RIGHT;
			}
			if (panel != null) {
				((FlowLayout) panel.getLayout()).setAlignment(alignCode);
				panel.revalidate(); // JL
				repaint(); // JL
			}
		}
	}

	private void createSidePanels() {
		northPanel = new JPanel();
		southPanel = new JPanel();
		eastPanel = new JPanel();
		westPanel = new JPanel();
		northPanel.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 0));
		southPanel.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 0));
		westPanel.setLayout(new GridBagLayout());
		eastPanel.setLayout(new GridBagLayout());
		add(northPanel, BorderLayout.NORTH);
		add(southPanel, BorderLayout.SOUTH);
		add(eastPanel, BorderLayout.EAST);
		add(westPanel, BorderLayout.WEST);
	}

	/* Private instance variables */

	private GridBagConstraints gbc;
	private JBECanvas canvas;
	private JPanel eastPanel;
	private JPanel northPanel;
	private JPanel southPanel;
	private JPanel westPanel;
	private JavaBackEnd jbe;
	private String windowId;

	/* Serial version UID */
	/**
	 * The serialization code for this class.  This value should be incremented
	 * whenever you change the structure of this class in an incompatible way,
	 * typically by adding a new instance variable.
	 */

	static final long serialVersionUID = 21L;
}
