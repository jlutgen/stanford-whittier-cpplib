/*
 * @(#)JBECommand.java   3.01.1 07/30/14
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

import edu.stanford.cs.java.graphics.G3DRect;
import edu.stanford.cs.java.graphics.GArc;
//import edu.stanford.cs.java.graphics.GCompound;
import edu.stanford.cs.java.graphics.GContainer;
import edu.stanford.cs.java.graphics.GDimension;
import edu.stanford.cs.java.graphics.GFillable;
import edu.stanford.cs.java.graphics.GImage;
import edu.stanford.cs.java.graphics.GLabel;
import edu.stanford.cs.java.graphics.GLine;
import edu.stanford.cs.java.graphics.GObject;
import edu.stanford.cs.java.graphics.GOval;
import edu.stanford.cs.java.graphics.GPolygon;
import edu.stanford.cs.java.graphics.GRect;
import edu.stanford.cs.java.graphics.GRectangle;
import edu.stanford.cs.java.graphics.GResizable;
import edu.stanford.cs.java.graphics.GRoundRect;
import edu.stanford.cs.java.tokenscanner.TokenScanner;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.Toolkit;
//import java.io.IOException;
import java.lang.reflect.Method;
import java.util.HashMap;

import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JSlider;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;
import javax.swing.Timer;

public abstract class JBECommand {
   public abstract void execute(TokenScanner scanner, JavaBackEnd jbe);

   public static HashMap<String,JBECommand> createCommandTable() {
      HashMap<String,JBECommand> cmdTable = new HashMap<String,JBECommand>();
      cmdTable.put("File.openFileDialog", new File_openFileDialog());
      cmdTable.put("G3DRect.create", new G3DRect_create());
      cmdTable.put("G3DRect.setRaised", new G3DRect_setRaised());
      cmdTable.put("GArc.create", new GArc_create());
      cmdTable.put("GArc.setFrameRectangle", new GArc_setFrameRectangle());
      cmdTable.put("GArc.setStartAngle", new GArc_setStartAngle());
      cmdTable.put("GArc.setSweepAngle", new GArc_setSweepAngle());
      cmdTable.put("GBufferedImage.create", new GBufferedImage_create());
      cmdTable.put("GBufferedImage.fill", new GBufferedImage_fill());
      cmdTable.put("GBufferedImage.fillRegion", new GBufferedImage_fillRegion());
      cmdTable.put("GBufferedImage.load", new GBufferedImage_load());
      cmdTable.put("GBufferedImage.resize", new GBufferedImage_resize());
      cmdTable.put("GBufferedImage.save", new GBufferedImage_save());
      cmdTable.put("GBufferedImage.setRGB", new GBufferedImage_setRGB());
      cmdTable.put("GButton.create", new GButton_create());
      cmdTable.put("GCheckBox.create", new GCheckBox_create());
      cmdTable.put("GCheckBox.isSelected", new GCheckBox_isSelected());
      cmdTable.put("GCheckBox.setSelected", new GCheckBox_setSelected());
      cmdTable.put("GCompound.add", new GCompound_add());
      cmdTable.put("GCompound.create", new GCompound_create());
      cmdTable.put("GEvent.getNextEvent", new GEvent_getNextEvent());
      cmdTable.put("GEvent.waitForEvent", new GEvent_waitForEvent());
      cmdTable.put("GImage.create", new GImage_create());
      cmdTable.put("GInteractor.setActionCommand",
                   new GInteractor_setActionCommand());
      cmdTable.put("GInteractor.getSize", new GInteractor_getSize());
      cmdTable.put("GLabel.create", new GLabel_create());
      cmdTable.put("GLabel.getFontAscent", new GLabel_getFontAscent());
      cmdTable.put("GLabel.getFontDescent", new GLabel_getFontDescent());
      cmdTable.put("GLabel.getGLabelSize", new GLabel_getGLabelSize());
      cmdTable.put("GLabel.setFont", new GLabel_setFont());
      cmdTable.put("GLabel.setLabel", new GLabel_setLabel());
      cmdTable.put("GLine.create", new GLine_create());
      cmdTable.put("GLine.setEndPoint", new GLine_setEndPoint());
      cmdTable.put("GLine.setStartPoint", new GLine_setStartPoint());
      cmdTable.put("GObject.contains", new GObject_contains());
      cmdTable.put("GObject.delete", new GObject_delete());
      cmdTable.put("GObject.getBounds", new GObject_getBounds());
      cmdTable.put("GObject.remove", new GObject_remove());
      cmdTable.put("GObject.rotate", new GObject_rotate());
      cmdTable.put("GObject.scale", new GObject_scale());
      cmdTable.put("GObject.sendBackward", new GObject_sendBackward());
      cmdTable.put("GObject.sendForward", new GObject_sendForward());
      cmdTable.put("GObject.sendToBack", new GObject_sendToBack());
      cmdTable.put("GObject.sendToFront", new GObject_sendToFront());
      cmdTable.put("GObject.setColor", new GObject_setColor());
      cmdTable.put("GObject.setFillColor", new GObject_setFillColor());
      cmdTable.put("GObject.setFilled", new GObject_setFilled());
      cmdTable.put("GObject.setLineWidth", new GObject_setLineWidth());
      cmdTable.put("GObject.setLocation", new GObject_setLocation());
      cmdTable.put("GObject.setSize", new GObject_setSize());
      cmdTable.put("GObject.setVisible", new GObject_setVisible());
      cmdTable.put("GOval.create", new GOval_create());
      cmdTable.put("GPolygon.addVertex", new GPolygon_addVertex());
      cmdTable.put("GPolygon.create", new GPolygon_create());
      cmdTable.put("GRect.create", new GRect_create());
      cmdTable.put("GRoundRect.create", new GRoundRect_create());
      cmdTable.put("GSlider.create", new GSlider_create());
      cmdTable.put("GSlider.getValue", new GSlider_getValue());
      cmdTable.put("GSlider.setValue", new GSlider_setValue());
      cmdTable.put("GTextField.create", new GTextField_create());
      cmdTable.put("GTextField.getText", new GTextField_getText());
      cmdTable.put("GTextField.setText", new GTextField_setText());
      cmdTable.put("GChooser.create", new GChooser_create());
      cmdTable.put("GChooser.addItem", new GChooser_addItem());
      cmdTable.put("GChooser.getSelectedItem", new GChooser_getSelectedItem());
      cmdTable.put("GChooser.setSelectedItem", new GChooser_setSelectedItem());
      cmdTable.put("GTimer.create", new GTimer_create());
      cmdTable.put("GTimer.deleteTimer", new GTimer_deleteTimer());
      cmdTable.put("GTimer.pause", new GTimer_pause());
      cmdTable.put("GTimer.startTimer", new GTimer_startTimer());
      cmdTable.put("GTimer.stopTimer", new GTimer_stopTimer());
      cmdTable.put("GWindow.addToRegion", new GWindow_addToRegion());
      cmdTable.put("GWindow.removeFromRegion", new GWindow_removeFromRegion());
      cmdTable.put("GWindow.setRegionAlignment",
                   new GWindow_setRegionAlignment());
      cmdTable.put("GWindow.clear", new GWindow_clear());
      cmdTable.put("GWindow.close", new GWindow_close());
      cmdTable.put("GWindow.create", new GWindow_create());
      cmdTable.put("GWindow.delete", new GWindow_delete());
      cmdTable.put("GWindow.draw", new GWindow_draw());
      cmdTable.put("GWindow.exitGraphics", new GWindow_exitGraphics());
      cmdTable.put("GWindow.getCanvasWidth", new GWindow_getCanvasWidth());
      cmdTable.put("GWindow.getCanvasHeight", new GWindow_getCanvasHeight());
      cmdTable.put("GWindow.getScreenHeight", new GWindow_getScreenHeight());
      cmdTable.put("GWindow.getScreenWidth", new GWindow_getScreenWidth());
      cmdTable.put("GWindow.repaint", new GWindow_repaint());
      cmdTable.put("GWindow.requestFocus", new GWindow_requestFocus());
      cmdTable.put("GWindow.setResizable", new GWindow_setResizable());
      cmdTable.put("GWindow.setTitle", new GWindow_setTitle());
      cmdTable.put("GWindow.setVisible", new GWindow_setVisible());
      cmdTable.put("TopCompound.create", new TopCompound_create());
      cmdTable.put("JBEConsole.clear", new JBEConsole_clear());
      cmdTable.put("JBEConsole.getLine", new JBEConsole_getLine());
      cmdTable.put("JBEConsole.print", new JBEConsole_print());
      cmdTable.put("JBEConsole.println", new JBEConsole_println());
      cmdTable.put("JBEConsole.setFont", new JBEConsole_setFont());
      cmdTable.put("JBEConsole.setLocation", new JBEConsole_setLocation());
      cmdTable.put("JBEConsole.setSize", new JBEConsole_setSize());
      cmdTable.put("Sound.create", new Sound_create());
      cmdTable.put("Sound.delete", new Sound_delete());
      cmdTable.put("Sound.play", new Sound_play());
      return cmdTable;
   }

   public int nextInt(TokenScanner scanner) {
      String token = scanner.nextToken();
      if (token.equals("-")) token += scanner.nextToken();
      return Integer.parseInt(token);
   }

   public double nextDouble(TokenScanner scanner) {
      String token = scanner.nextToken();
      if (token.equals("-")) token += scanner.nextToken();
      return Double.parseDouble(token);
   }

   public String nextString(TokenScanner scanner) {
      return scanner.getStringValue(scanner.nextToken());
   }

   public boolean nextBoolean(TokenScanner scanner) {
      return scanner.nextToken().startsWith("t");
   }
}

class GWindow_create extends JBECommand {
   class Runner implements Runnable {
	   public Runner(String id, String top, int width, int height, JavaBackEnd jbe) {
		   this.id = id;
		   this.top = top;
		   this.width = width;
		   this.height = height;
		   this.jbe = jbe;
	   }
	   public void run() {
		   jbe.createWindow(id, width, height,
                   (JBETopCompound) jbe.getGObject(top));
	   }
	   private String id;
	   private String top;
	   private int width;
	   private int height;
	   private JavaBackEnd jbe;
   }
   
   public void execute(TokenScanner scanner, JavaBackEnd jbe) { 
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      int width = nextInt(scanner);
      scanner.verifyToken(",");
      int height = nextInt(scanner);
      scanner.verifyToken(",");
      String top = nextString(scanner);
      scanner.verifyToken(")");
      try {
    	  SwingUtilities.invokeAndWait(new Runner(id, top, width, height, jbe));
      }
      catch (Exception e) {
    	  /* empty */
      }
      jbe.println("result:ok");
   }
}

class GWindow_delete extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      jbe.deleteWindow(id);
   }
}

class GWindow_close extends JBECommand {
	class Runner implements Runnable {
		   public Runner(JBEWindow jw) {
			   this.jw = jw;
		   }
		   public void run() {
			   jw.close();
		   }
		   private JBEWindow jw;
	}
	
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      JBEWindow jw = jbe.getWindow(id);
      scanner.verifyToken(")");
      if (jw != null) {
    	  try {
    		  SwingUtilities.invokeLater(new Runner(jw));
    	  }
    	  catch (Exception e) {
    		  /* empty */
    	  }
      }
   }
}

class GWindow_addToRegion extends JBECommand {
	class Runner implements Runnable {
		   public Runner(JBEWindow jw, GObject gobj, String region, JavaBackEnd jbe) {
			   this.jw = jw;
			   this.gobj = gobj;
			   this.region = region;
			   this.jbe = jbe;
		   }
		   public void run() {
			   jw.addToRegion(jbe.getInteractor(gobj), region);
		   }
		   private JBEWindow jw;
		   private GObject gobj;
		   private String region;
		   private JavaBackEnd jbe;
	}
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id1 = nextString(scanner);
      scanner.verifyToken(",");
      String id2 = nextString(scanner);
      scanner.verifyToken(",");
      String region = nextString(scanner);
      scanner.verifyToken(")");
      JBEWindow jw = jbe.getWindow(id1);
      GObject gobj = jbe.getGObject(id2);
      if (jw != null && gobj != null) {
    	  try {
    		  SwingUtilities.invokeLater(new Runner(jw, gobj, region, jbe));
    	  }
    	  catch (Exception e) {
    		  /* empty */
    	  }
      }
   }
}

class GWindow_removeFromRegion extends JBECommand {
	class Runner implements Runnable {
		   public Runner(JBEWindow jw, GObject gobj, String region, JavaBackEnd jbe) {
			   this.jw = jw;
			   this.gobj = gobj;
			   this.region = region;
			   this.jbe = jbe;
		   }
		   public void run() {
			   jw.removeFromRegion(jbe.getInteractor(gobj), region);
		   }
		   private JBEWindow jw;
		   private GObject gobj;
		   private String region;
		   private JavaBackEnd jbe;
	}
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id1 = nextString(scanner);
      scanner.verifyToken(",");
      String id2 = nextString(scanner);
      scanner.verifyToken(",");
      String region = nextString(scanner);
      scanner.verifyToken(")");
      JBEWindow jw = jbe.getWindow(id1);
      GObject gobj = jbe.getGObject(id2);
      if (jw != null && gobj != null) {
    	  try {
    		  SwingUtilities.invokeLater(new Runner(jw, gobj, region, jbe));
    	  }
    	  catch (Exception e) {
    		  /* empty */
    	  }
      }
   }
}

class GWindow_clear extends JBECommand {
	class Runner implements Runnable {
		   public Runner(JBEWindow jw) {
			   this.jw = jw;
		   }
		   public void run() {
			   jw.clear();
		   }
		   private JBEWindow jw;
	}
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      JBEWindow jw = jbe.getWindow(id);
      scanner.verifyToken(")");
      if (jw != null) {
    	  try {
    		  SwingUtilities.invokeLater(new Runner(jw));
    	  }
    	  catch (Exception e) {
    		  /* empty */
    	  }
      }
   }
}

class GWindow_repaint extends JBECommand {
	class Runner implements Runnable {
		public Runner(JBEWindow jw) {
			this.jw = jw;
		}
		public void run() {
			jw.getCanvas().repaint();
		}
		private JBEWindow jw;
	}
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String id = nextString(scanner);
		JBEWindow jw = jbe.getWindow(id);
		scanner.verifyToken(")");
		if (jw != null) {
			try {
				SwingUtilities.invokeLater(new Runner(jw));
			}
			catch (Exception e) {
				/* empty */
			}
		}
	}
}

class GWindow_requestFocus extends JBECommand {
	class Runner implements Runnable {
		public Runner(JBEWindow jw) {
			this.jw = jw;
		}
		public void run() {
			jw.toFront();
			jw.getCanvas().requestFocus();
		}
		private JBEWindow jw;
	}
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String id = nextString(scanner);
		JBEWindow jw = jbe.getWindow(id);
		scanner.verifyToken(")");
		if (jw != null) {
			try {
				SwingUtilities.invokeLater(new Runner(jw));
			}
			catch (Exception e) {
				/* empty */
			} 
		}
	}
}

class GWindow_setResizable extends JBECommand {
	class Runner implements Runnable {
		public Runner(JBEWindow jw, boolean flag) {
			this.jw = jw;
			this.flag = flag;
		}
		public void run() {
			jw.setResizable(flag);
		}
		private JBEWindow jw;
		private boolean flag;
	}
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String id = nextString(scanner);
		JBEWindow jw = jbe.getWindow(id);
		scanner.verifyToken(",");
		boolean flag = scanner.nextToken().equals("true");
		scanner.verifyToken(")");
		if (jw != null) {
			try {
				SwingUtilities.invokeLater(new Runner(jw, flag));
			}
			catch (Exception e) {
				/* empty */
			} 
		}
	}
}

class GWindow_setTitle extends JBECommand {
	class Runner implements Runnable {
		public Runner(JBEWindow jw, String title) {
			this.jw = jw;
			this.title = title;
		}
		public void run() {
			jw.setTitle(title);
		}
		private JBEWindow jw;
		private String title;
	}
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String id = nextString(scanner);
		JBEWindow jw = jbe.getWindow(id);
		scanner.verifyToken(",");
		String title = nextString(scanner);
		scanner.verifyToken(")");
		if (jw != null) {
			try {
				SwingUtilities.invokeLater(new Runner(jw, title));
			}
			catch (Exception e) {
				/* empty */
			} 
		}
	}
}

class GWindow_setVisible extends JBECommand {
	class Runner implements Runnable {
		public Runner(JBEWindow jw, boolean flag) {
			this.jw = jw;
			this.flag = flag;
		}
		public void run() {
			jw.setVisible(flag);
		}
		private JBEWindow jw;
		private boolean flag;
	}
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String id = nextString(scanner);
		JBEWindow jw = jbe.getWindow(id);
		scanner.verifyToken(",");
		boolean flag = scanner.nextToken().equals("true");
		scanner.verifyToken(")");
		if (jw != null)  {
			try {
				SwingUtilities.invokeLater(new Runner(jw, flag));
			}
			catch (Exception e) {
				/* empty */
			} 
		}
	}
}

class GTimer_pause extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      double milliseconds = nextDouble(scanner);
      scanner.verifyToken(")");
      try {
         Thread.sleep((int) Math.round(milliseconds));
      } catch (Exception ex) {
         /* Empty */
      }
      jbe.println("result:ok");
   }
}

class GWindow_getCanvasWidth extends JBECommand {
	class Runner implements Runnable {
		public Runner(JBEWindow jw) {
			this.jw = jw;
		}
		public void run() {
			result = jw.getCanvas().getWidth();
		}
		private JBEWindow jw;
		public int result;
	}
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      JBEWindow jw = jbe.getWindow(id);
      scanner.verifyToken(")");
      if (jw == null) {
          jbe.println("error:null window");
      } else {
    	  Runner runner = new Runner(jw);
    	  try {
				SwingUtilities.invokeAndWait(runner);
			}
			catch (Exception e) {
				/* empty */
		  } 
          jbe.println("result:" + runner.result);
      }
   }
}

class GWindow_getCanvasHeight extends JBECommand {
	class Runner implements Runnable {
		public Runner(JBEWindow jw) {
			this.jw = jw;
		}
		public void run() {
			result = jw.getCanvas().getHeight();
		}
		private JBEWindow jw;
		public int result;
	}
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      JBEWindow jw = jbe.getWindow(id);
      scanner.verifyToken(")");
      if (jw == null) {
          jbe.println("error:null window");
      } else {
    	  Runner runner = new Runner(jw);
    	  try {
				SwingUtilities.invokeAndWait(runner);
			}
			catch (Exception e) {
				/* empty */
		  } 
          jbe.println("result:" + runner.result);
      }
   }
}

class GWindow_getScreenWidth extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      scanner.verifyToken(")");
      Dimension size = Toolkit.getDefaultToolkit().getScreenSize();
      jbe.println("result:" + size.width);
   }
}

class GWindow_getScreenHeight extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      scanner.verifyToken(")");
      Dimension size = Toolkit.getDefaultToolkit().getScreenSize();
      jbe.println("result:" + size.height);
   }
}

class JBEConsole_clear extends JBECommand {
	class Runner implements Runnable {
		public Runner(JavaBackEnd jbe) {
			this.jbe = jbe;
		}
		public void run() {
			jbe.clearConsole();
		}
		private JavaBackEnd jbe;
	}
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		scanner.verifyToken(")");
		try {
			SwingUtilities.invokeLater(new Runner(jbe));
		}
		catch (Exception e) {
			/* empty */
		} 
	}
}

class JBEConsole_setFont extends JBECommand {
	class Runner implements Runnable {
		public Runner(String font, JavaBackEnd jbe) {
			this.font = font;
			this.jbe = jbe;
		}
		public void run() {
			jbe.setConsoleFont(font);
		}
		private String font;
		private JavaBackEnd jbe;
	}
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String font = nextString(scanner);
		scanner.verifyToken(")");
		try {
			SwingUtilities.invokeLater(new Runner(font, jbe));
		}
		catch (Exception e) {
			/* empty */
		} 
	}
}


class JBEConsole_setLocation extends JBECommand {
	class Runner implements Runnable {
		public Runner(int x, int y, JavaBackEnd jbe) {
			this.x = x;
			this.y = y;
			this.jbe = jbe;
		}
		public void run() {
			jbe.setConsoleLocation(x, y);
		}
		private int x;
		private int y;
		private JavaBackEnd jbe;
	}
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		int x = nextInt(scanner);
		scanner.verifyToken(",");
		int y = nextInt(scanner);
		scanner.verifyToken(")");
		try {
			SwingUtilities.invokeLater(new Runner(x, y, jbe));
		}
		catch (Exception e) {
			/* empty */
		} 
	}
}

class JBEConsole_setSize extends JBECommand {
	class Runner implements Runnable {
		public Runner(int width, int height, JavaBackEnd jbe) {
			this.width = width;
			this.height = height;
			this.jbe = jbe;
		}
		public void run() {
			jbe.setConsoleSize(width, height);
		}
		private int width;
		private int height;
		private JavaBackEnd jbe;
	}
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      int width = nextInt(scanner);
      scanner.verifyToken(",");
      int height = nextInt(scanner);
      scanner.verifyToken(")");
      try {
			SwingUtilities.invokeLater(new Runner(width, height, jbe));
		}
		catch (Exception e) {
			/* empty */
		} 
   }
}

// OK not on Event Dispatch Thread
class JBEConsole_getLine extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      scanner.verifyToken(")");
      jbe.println("result:" + jbe.getConsole());
   }
}

class JBEConsole_print extends JBECommand {
	class Runner implements Runnable {
		public Runner(String str, boolean isStdErr, JavaBackEnd jbe) {
			this.str = str;
			this.isStdErr = isStdErr;
			this.jbe = jbe;
		}
		public void run() {
			jbe.putConsole(str, isStdErr);
		}
		private String str;
		private boolean isStdErr;
		private JavaBackEnd jbe;
	}
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String str = nextString(scanner);
      String token = scanner.nextToken();
      boolean isStdErr = false;
      if (token.equals(",")) {
          isStdErr = nextBoolean(scanner);
      }
      //scanner.verifyToken(")");
      try {
			SwingUtilities.invokeLater(new Runner(str, isStdErr, jbe));
		}
		catch (Exception e) {
			/* empty */
		} 
   }
}

class JBEConsole_println extends JBECommand {
	class Runner implements Runnable {
		public Runner(JavaBackEnd jbe) {
			this.jbe = jbe;
		}
		public void run() {
			jbe.endLineConsole();
		}
		private JavaBackEnd jbe;
	}
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      scanner.verifyToken(")");
      //jbe.endLineConsole();
      try {
			SwingUtilities.invokeLater(new Runner(jbe));
		}
		catch (Exception e) {
			/* empty */
		} 
   }
}

class GEvent_getNextEvent extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      int mask = nextInt(scanner);
      scanner.verifyToken(")");
      jbe.getNextEvent(mask);
   }
}

class GEvent_waitForEvent extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      int mask = nextInt(scanner);
      scanner.verifyToken(")");
      jbe.waitForEvent(mask);
   }
}

class Sound_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String filename = nextString(scanner);
      scanner.verifyToken(")");
      String status = "ok";
      try {
         jbe.createSound(id, filename);
      } catch (Exception ex) {
         status = ex.getMessage();
      }
      jbe.println("result:" + status);
   }
}

class Sound_delete extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      jbe.deleteSound(id);
   }
}

class Sound_play extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      jbe.playSound(id);
   }
}

class GTimer_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double msec = nextDouble(scanner);
      scanner.verifyToken(")");
      jbe.createTimer(id, msec);
   }
}

class GTimer_deleteTimer extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      jbe.deleteTimer(id);
   }
}

class GTimer_startTimer extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      jbe.startTimer(id);
   }
}

class GTimer_stopTimer extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      jbe.stopTimer(id);
   }
}

// Don't need to call Timer constructor from Event Dispatch Thread.
class GTimer extends Timer {
   public GTimer(String id, double delay) {
      super((int) Math.round(delay), null);
      this.id = id;
   }

   public String getId() {
      return id;
   }

   private String id;
}

class GRect_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double width = nextDouble(scanner);
      scanner.verifyToken(",");
      double height = nextDouble(scanner);
      scanner.verifyToken(")");
      GRect gobj = new GRect(width, height);
      jbe.defineGObject(id, gobj);
   }
}

class GRoundRect_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double width = nextDouble(scanner);
      scanner.verifyToken(",");
      double height = nextDouble(scanner);
      scanner.verifyToken(",");
      double arc = nextDouble(scanner);
      scanner.verifyToken(")");
      GRoundRect gobj = new GRoundRect(0, 0, width, height, arc);
      jbe.defineGObject(id, gobj);
   }
}

class G3DRect_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double width = nextDouble(scanner);
      scanner.verifyToken(",");
      double height = nextDouble(scanner);
      scanner.verifyToken(",");
      boolean raised = scanner.nextToken().equals("true");
      scanner.verifyToken(")");
      G3DRect gobj = new G3DRect(0, 0, width, height, raised);
      jbe.defineGObject(id, gobj);
   }
}

class G3DRect_setRaised extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      boolean raised = nextBoolean(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) ((G3DRect) gobj).setRaised(raised);
   }
}

class GLine_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double x1 = nextDouble(scanner);
      scanner.verifyToken(",");
      double y1 = nextDouble(scanner);
      scanner.verifyToken(",");
      double x2 = nextDouble(scanner);
      scanner.verifyToken(",");
      double y2 = nextDouble(scanner);
      scanner.verifyToken(")");
      GLine gobj = new GLine(x1, y1, x2, y2);
      jbe.defineGObject(id, gobj);
   }
}

class GLine_setStartPoint extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double x = nextDouble(scanner);
      scanner.verifyToken(",");
      double y = nextDouble(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) ((GLine) gobj).setStartPoint(x, y);
   }
}

class GLine_setEndPoint extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double x = nextDouble(scanner);
      scanner.verifyToken(",");
      double y = nextDouble(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) ((GLine) gobj).setEndPoint(x, y);
   }
}

class GPolygon_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GPolygon gobj = new GPolygon();
      jbe.defineGObject(id, gobj);
   }
}

class GPolygon_addVertex extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double x = nextDouble(scanner);
      scanner.verifyToken(",");
      double y = nextDouble(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) ((GPolygon) gobj).addVertex(x, y);
   }
}

class GImage_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String filename = nextString(scanner);
      scanner.verifyToken(")");
      try {
         GImage gobj = new GImage(filename);
         jbe.defineGObject(id, gobj);
         jbe.println("result:GDimension(" + gobj.getWidth() +
                            ", " + gobj.getHeight() + ")");
      } catch (Exception ex) {
         jbe.println("result:" + ex.getMessage());
      }
   }
}

class GOval_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double width = nextDouble(scanner);
      scanner.verifyToken(",");
      double height = nextDouble(scanner);
      scanner.verifyToken(")");
      GOval gobj = new GOval(width, height);
      jbe.defineGObject(id, gobj);
   }
}

class GLabel_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String str = nextString(scanner);
      scanner.verifyToken(")");
      JBELabel gobj = new JBELabel(str);
      jbe.defineGObject(id, gobj);
   }
}

class GArc_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double width = nextDouble(scanner);
      scanner.verifyToken(",");
      double height = nextDouble(scanner);
      scanner.verifyToken(",");
      double start = nextDouble(scanner);
      scanner.verifyToken(",");
      double sweep = nextDouble(scanner);
      scanner.verifyToken(")");
      GArc gobj = new GArc(width, height, start, sweep);
      jbe.defineGObject(id, gobj);
   }
}

class GArc_setStartAngle extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double angle = nextDouble(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) ((GArc) gobj).setStartAngle(angle);
   }
}

class GArc_setSweepAngle extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double angle = nextDouble(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) ((GArc) gobj).setSweepAngle(angle);
   }
}

class GArc_setFrameRectangle extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double x = nextDouble(scanner);
      scanner.verifyToken(",");
      double y = nextDouble(scanner);
      scanner.verifyToken(",");
      double width = nextDouble(scanner);
      scanner.verifyToken(",");
      double height = nextDouble(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         ((GArc) gobj).setFrameRectangle(x, y, width, height);
      }
   }
}

class GCompound_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      JBETopCompound gobj = new JBETopCompound();
      jbe.defineGObject(id, gobj);
   }
}

class GCompound_add extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      //System.err.println("JBE DEBUG: add 1");
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj1 = jbe.getGObject(id);
      scanner.verifyToken(",");
      id = nextString(scanner);
      GObject gobj2 = jbe.getGObject(id);
      scanner.verifyToken(")");
      //System.err.println("JBE DEBUG: add 2");

      if (gobj1 != null && gobj2 != null) {
         //System.err.println("JBE DEBUG: add 3");
         ((JBETopCompound) gobj1).add(gobj2);
         jbe.println("result:ok");
         //System.err.println("JBE DEBUG: add 4");
      } else {
      	 jbe.println("error:GCompound_add: an object was null");
      }
   }
}

class TopCompound_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      JBETopCompound gobj = new JBETopCompound();
      jbe.defineGObject(id, gobj);
   }
}

class GObject_delete extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      jbe.deleteGObject(id);
   }
}

// Move the computation here into the client side

class GObject_contains extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double x = nextDouble(scanner);
      scanner.verifyToken(",");
      double y = nextDouble(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      jbe.println("result:" + (gobj != null && gobj.contains(x, y)));
   }
}

// Move the computation here into the client side

class GObject_getBounds extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj == null) {
         jbe.println("error: NULL object");
      } else {
         GRectangle bounds = gobj.getBounds();
         jbe.println("result:GRectangle(" + bounds.getX() + ", "
                            + bounds.getY() + ", " + bounds.getWidth() + ", "
                            + bounds.getHeight() + ")");
      }
   }
}

class GObject_remove extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(")");
      if (gobj != null) {
         GContainer parent = gobj.getParent();
         if (parent != null) {
            parent.remove(gobj);
         }
      }
   }
}

class GObject_rotate extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(",");
      double theta = nextDouble(scanner);
      scanner.verifyToken(")");
      if (gobj != null) gobj.rotate(theta);
   }
}

class GObject_scale extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(",");
      double sx = nextDouble(scanner);
      scanner.verifyToken(",");
      double sy = nextDouble(scanner);
      scanner.verifyToken(")");
      if (gobj != null) gobj.scale(sx, sy);
   }
}

class GObject_sendForward extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(")");
      if (gobj != null) gobj.sendForward();
   }
}

class GObject_sendToFront extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(")");
      if (gobj != null) gobj.sendToFront();
   }
}

class GObject_sendBackward extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(")");
      if (gobj != null) gobj.sendBackward();
   }
}

class GObject_sendToBack extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(")");
      if (gobj != null) gobj.sendToBack();
   }
}

class GObject_setLineWidth extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(",");
      double lineWidth = nextDouble(scanner);
      scanner.verifyToken(")");
      if (gobj != null) gobj.setLineWidth(lineWidth);
   }
}

class GObject_setColor extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(",");
      String color = nextString(scanner);
      scanner.verifyToken(")");
      if (gobj != null) {
         Color c = color.equals("") ? null : JavaBackEnd.decodeColor(color);
         gobj.setColor(c);
      }
   }
}

class GObject_setLocation extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double x = nextDouble(scanner);
      scanner.verifyToken(",");
      double y = nextDouble(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) gobj.setLocation(x, y);
   }
}

class GObject_setVisible extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(",");
      boolean flag = nextBoolean(scanner);
      scanner.verifyToken(")");
      if (gobj != null) gobj.setVisible(flag);
   }
}

class GWindow_draw extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      JBEWindow jw = jbe.getWindow(id);
      scanner.verifyToken(",");
      id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(")");
      if (jw != null && gobj != null && gobj.isVisible()) {
         JBECanvas jc = jw.getCanvas();
         Graphics2D osg = jc.getOSG();
         osg.setColor(gobj.getColor());
         gobj.paint(osg);
         jc.repaint();
      }
   }
}

class GObject_setSize extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double width = nextDouble(scanner);
      scanner.verifyToken(",");
      double height = nextDouble(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) ((GResizable) gobj).setSize(width, height);
   }
}

class GObject_setFilled extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      boolean flag = scanner.nextToken().equals("true");
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) ((GFillable) gobj).setFilled(flag);
   }
}

class GObject_setFillColor extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String color = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         Color c = (color.equals("")) ? null : JavaBackEnd.decodeColor(color);
         ((GFillable) gobj).setFillColor(c);
      }
   }
}

class GLabel_setFont extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String font = nextString(scanner);
      scanner.verifyToken(")");
      GLabel label = (GLabel) jbe.getGObject(id);
      if (label != null) label.setFont(font);
   }
}

class GLabel_setLabel extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String str = nextString(scanner);
      scanner.verifyToken(")");
      GLabel label = (GLabel) jbe.getGObject(id);
      if (label != null) label.setLabel(str);
   }
}

class GLabel_getFontAscent extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GLabel label = (GLabel) jbe.getGObject(id);
      jbe.println("result:" + label.getAscent());
   }
}

class GLabel_getFontDescent extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GLabel label = (GLabel) jbe.getGObject(id);
      jbe.println("result:" + label.getDescent());
   }
}

class GLabel_getGLabelSize extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GLabel label = (GLabel) jbe.getGObject(id);
      GDimension size = label.getSize();
      jbe.println("result:GDimension(" + size.getWidth()
                       + ", " + size.getHeight() + ")");
   }
}

class GInteractor_setActionCommand extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String cmd = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         ((GInteractor) gobj).setActionCommand(cmd);
      }
   }
}

class GInteractor_getSize extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JComponent jcomp = ((GInteractor) gobj).getInteractor();
         Dimension size = (jcomp.isShowing()) ? jcomp.getSize()
                                              : jcomp.getPreferredSize();
         jbe.println("result:GDimension(" + size.width + ", "
                                                 + size.height + ")");
      } else {
         jbe.println("result:GDimension(0, 0)");
      }
   }
}

class GButton_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String label = nextString(scanner);
      scanner.verifyToken(")");
      GButton gobj = new GButton(label, jbe);
      gobj.setActionCommand(label);
      jbe.defineGObject(id, gobj);
      jbe.defineSource(gobj.getInteractor(), id);
   }
}

class GCheckBox_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String label = nextString(scanner);
      scanner.verifyToken(")");
      GCheckBox gobj = new GCheckBox(label, jbe);
      jbe.defineGObject(id, gobj);
      jbe.defineSource(gobj.getInteractor(), id);
   }
}

class GCheckBox_isSelected extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JCheckBox chkbox = (JCheckBox) ((GCheckBox) gobj).getInteractor();
         jbe.println("result:" + chkbox.isSelected());
      } else {
         jbe.println("result:false");
      }
   }
}

class GCheckBox_setSelected extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      boolean state = nextBoolean(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JCheckBox chkbox = (JCheckBox) ((GCheckBox) gobj).getInteractor();
         chkbox.setSelected(state);
      }
   }
}

class GSlider_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      int min = nextInt(scanner);
      scanner.verifyToken(",");
      int max = nextInt(scanner);
      scanner.verifyToken(",");
      int value = nextInt(scanner);
      scanner.verifyToken(")");
      GSlider gobj = new GSlider(min, max, value, jbe);
      jbe.defineGObject(id, gobj);
      jbe.defineSource(gobj.getInteractor(), id);
   }
}

class GSlider_getValue extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JSlider slider = (JSlider) ((GSlider) gobj).getInteractor();
         jbe.println("result:" + slider.getValue());
      } else {
         jbe.println("result:0");
      }
   }
}

class GSlider_setValue extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      int value = nextInt(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JSlider slider = (JSlider) ((GSlider) gobj).getInteractor();
         slider.setValue(value);
      }
   }
}

class GTextField_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      int nChars = nextInt(scanner);
      scanner.verifyToken(")");
      GTextField gobj = new GTextField(nChars, jbe);
      jbe.defineGObject(id, gobj);
      jbe.defineSource(gobj.getInteractor(), id);
   }
}

class GTextField_getText extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JTextField field = (JTextField) ((GTextField) gobj).getInteractor();
         jbe.println("result:" + field.getText());
      } else {
         jbe.println("result:");
      }
   }
}

class GTextField_setText extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String str = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JTextField field = (JTextField) ((GTextField) gobj).getInteractor();
         field.setText(str);
      }
   }
}

class GChooser_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GChooser gobj = new GChooser(jbe);
      jbe.defineGObject(id, gobj);
      jbe.defineSource(gobj.getInteractor(), id);
   }
}

class GChooser_addItem extends JBECommand {
/*
 * Implementation note
 * -------------------
 * This implementation uses reflection to call the addItem method to
 * avoid generating warnings in Java 7 (which added a type parameter
 * to the JComboBox class) while still allowing compilation in Java 6.
 */

   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String item = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JComboBox chooser = (JComboBox) ((GChooser) gobj).getInteractor();
         Class<?> c = chooser.getClass();
         Class[] types = { Object.class };
         Object[] args = { item };
         try {
            Method addItem = c.getMethod("addItem", types);
            addItem.invoke(chooser, args);
         } catch (Exception ex) {
            System.out.println("error: " + ex);
         }
      }
   }
}

class GChooser_getSelectedItem extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JComboBox chooser = (JComboBox) ((GChooser) gobj).getInteractor();
         jbe.println("result:" + chooser.getSelectedItem());
      } else {
         jbe.println("result:");
      }
   }
}

class GChooser_setSelectedItem extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String item = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JComboBox chooser = (JComboBox) ((GChooser) gobj).getInteractor();
         chooser.setSelectedItem(item);
      }
   }
}

class GWindow_setRegionAlignment extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String region = nextString(scanner);
      scanner.verifyToken(",");
      String align = nextString(scanner);
      scanner.verifyToken(")");
      JBEWindow jw = jbe.getWindow(id);
      if (jw != null) jw.setRegionAlignment(region, align);
   }
}

class GWindow_exitGraphics extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      scanner.verifyToken(")");
      System.exit(0);
   }
}

/* JL added this class */
class fileDialogRunner implements Runnable {
    public fileDialogRunner(File_openFileDialog ofd, JavaBackEnd jbe) {
        this.ofd = ofd;
        this.jbe = jbe;
    }

    public void run() {
        ofd.result = jbe.openFileDialog(ofd.title, ofd.mode, ofd.path);
    }

    private File_openFileDialog ofd;
    private JavaBackEnd jbe;
}

class File_openFileDialog extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      title = nextString(scanner);
      scanner.verifyToken(",");
      mode = nextString(scanner);
      scanner.verifyToken(",");
      path = nextString(scanner).trim(); // JL added .trim(). Clients must add a space to end of real path since path can end with backslash,
                                         // so we must remove it to recover real path.
      scanner.verifyToken(")");
      try {
        javax.swing.SwingUtilities.invokeAndWait(new fileDialogRunner(this, jbe)); // runs in 'event dispatch thread'
      } catch (Exception e) {
        e.printStackTrace();
      }
      jbe.println("result:" + result);
   }

   public String result;
   public String title;
   public String mode;
   public String path;
}

class GBufferedImage_create extends JBECommand {
	// gbufferedimage = new GBufferedImage(x, y, width, height);
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd jbe) {
		paramTokenScanner.verifyToken("(");
		String id = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int x = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int y = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int w = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int h = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int rgb = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GBufferedImage img = new GBufferedImage(w, h, rgb);
		img.setLocation(x, y);
		jbe.defineGObject(id, img);
		jbe.defineSource(img.getInteractor(), id);
	}
}

class GBufferedImage_fill extends JBECommand {
	// gbufferedimage.setRGB(x, y, rgb);
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd jbe) {
		paramTokenScanner.verifyToken("(");
		String id = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int rgb = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject gobj = jbe.getGObject(id);
		if (gobj != null && gobj instanceof GBufferedImage) {
			GBufferedImage img = (GBufferedImage) gobj;
			img.fill(rgb);
		}
	}
}

class GBufferedImage_fillRegion extends JBECommand {
	// gbufferedimage.setRGB(x, y, rgb);
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd jbe) {
		paramTokenScanner.verifyToken("(");
		String id = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int x = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int y = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int w = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int h = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int rgb = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject gobj = jbe.getGObject(id);
		if (gobj != null && gobj instanceof GBufferedImage) {
			GBufferedImage img = (GBufferedImage) gobj;
			img.fillRegion(x, y, w, h, rgb);
		}
	}
}

class GBufferedImage_load extends JBECommand {
	// gbufferedimage.load("foobar.png");
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd jbe) {
		paramTokenScanner.verifyToken("(");
		String id = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String filename = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject gobj = jbe.getGObject(id);
		System.err.println("JBE DEBUG: execute load: " + gobj); // DELETE
		if (gobj != null && gobj instanceof GBufferedImage) {
			GBufferedImage img = (GBufferedImage) gobj;
			result = img.load(filename);
			jbe.println("result:" + result);
		} else {
		    jbe.println("result:");
		}
	}
	
	public String result;
}

class GBufferedImage_resize extends JBECommand {
	// gbufferedimage.setRGB(x, y, rgb);
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd jbe) {
		paramTokenScanner.verifyToken("(");
		String id = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int w = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int h = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String retain = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject gobj = jbe.getGObject(id);
		if (gobj != null && gobj instanceof GBufferedImage) {
			GBufferedImage img = (GBufferedImage) gobj;
			img.resize(w, h, retain.equals("true"));
		}
	}
}

class GBufferedImage_save extends JBECommand {
	// gbufferedimage.save("foobar.png");
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd jbe) {
		paramTokenScanner.verifyToken("(");
		String id = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String filename = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject gobj = jbe.getGObject(id);
		if (gobj != null && gobj instanceof GBufferedImage) {
			GBufferedImage img = (GBufferedImage) gobj;
			img.save(filename);
			jbe.println("result:ok");
		} else {
			jbe.println("result:");
		}
	}
}

class GBufferedImage_setRGB extends JBECommand {
	// gbufferedimage.setRGB(x, y, rgb);
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd jbe) {
		paramTokenScanner.verifyToken("(");
		String id = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int x = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int y = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int rgb = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject gobj = jbe.getGObject(id);
		if (gobj != null && gobj instanceof GBufferedImage) {
			GBufferedImage img = (GBufferedImage) gobj;
			img.setRGB(x, y, rgb);
		}
	}
}

