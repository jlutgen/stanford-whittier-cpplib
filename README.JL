
This folder contains my customizations and bugfixes for the StanfordCPPLib.


========= NOTES =====================================

private/main.h: included at end of console.h, error.h, filelib.h, gwindow.h.
	If no console or graphics flag set, redefine main() to call mainWrapper(), defined in startup.cpp.
        If console or graphics, redefine main() to call startupMain(), defined in platform.cpp, and 
          catch error exception.
	In either case, user main() is renamed to Main() (no arguments), argc and argv are stored in 
          static globals declared in startup.cpp, and these can be accessed if needed in Main() by
          calling getArgumentCount()/getArguments(), defined in startup.cpp (prototypes in private/main.h).


startup.cpp
mainWrapper(): just call Main() and catch error exception.


platform.cpp
startupMain(): initialize pipe to/from JBE, redirect cout/cin to a streambuf, call Main().


CONSOLE_FLAG turned on in main.h if console.h has been included.
GRAPHICS_FLAG turned on in main.h if gwindow.h has been included.
gwindow.h includes console.h.
These flags aren't used elsewhere in library.


Global int _mainFlags declared in startup.cpp, set to CONSOLE_FLAG+GRAPHICS_FLAG in main.h at beginning of
redefined main(), and declared extern in main.h. Not used in library, but user can access if needed.


CPPLib defines hashCode() for the following types:
GPoint, GDimension, GRectangle, string, int, char, long, double
Thus one may construct HashSet of those types and HashMap with those key types.
Added hashCode for Point.


openFileDialog(ifs, "title", "e:\\dir\\otherdir\\") crashes on windows (in jbe tokenscanner)
openFileDialog(ifs, "Pick a file", getCurrentDirectory()+"/" ) is how you get dialog to start in current working
directory (linux/mac)
openFileDialog(ifs, "Pick a file", getCurrentDirectory()+"/*.wav" ) gives you a filter (linux/mac)


Bug in acm-based 2014 "big" spl.jar: 
printEvent(String paramString, KeyEvent paramKeyEvent): 
e.getKeyChar() needs to be 
(int) e.getKeyChar())
Otherwise the scanning of the event string in platform.cpp crashes.
It's fixed in new spl.jar.


JBE uses JBEWindow for windows. Looks as if stanford java GWindow not used.

How action commands work:
Basically, a GInteractor that can generate an action event
(GButton, GSlider, GCheckbox, GChooser, GTextField) must have a non-empty
"action command" (a string) in order for action events to be generated.
When an action event e is received, can call e.getActionCommand() to 
figure out the source of the event. Can be non-unique, I guess. If you
must know precisely which GObject is the source, can call e.getSource()
to get a pointer to it. 

JBE Hierarchy (partial)
=============
GObject (has get/setParent to be called by its transient/compound GContainer parent, either a GCanvas or GCompound, when it is added to a GContainer. When front end
calls gwindow->add(gobject), the GObject is actually added to the window's canvas's
JBETopCompound.
	GArc
	GImage
	GLabel
		JBELabel (has private JLabel jlabel)
	GLine
	GOval
	GPolygon
	GRect
		G3DRect
		GRoundRect
	GInteractor (has JComponent `interactor`)
		GBufferedImage
		GButton
		GCheckbox
		GChooser
		GSlider
		GTextField
	GCompound
		JBETopCompound (this is the only sort of GCompound actually created)
			has a private JBECanvas owner
	
GContainer (interface)
	GCanvas
		JBECanvas
	GCompound
		add(GObject gobj): sets gobj's parent to be this GCanvas/GCompound
	These both have a private GObjectList contents. Front end never creates
	a canvas directly. When front end creates a GWindow, a JBECanvas is created
	as the GWindow's private 'canvas'.	
	
GObjectList (has private GContainer parent)
	
JComponent
	GCanvas
		JBECanvas
			has a private JBETopCompound top

Frame
	GWindow
		JBEWindow
			has 4 JPanels and a private JBECanvas canvas.

gw->add(GOval): sets GOval's t/c parent to JBETopCompound. T/c parent of JBETopCompound
is JBECanvas, whose (Swing) parent is JPanel. A JBECanvas is not a GObject, so 
does not have a transient/compound parent.

So the root of any (visible) GContainer containment hierarchy is a JBECanvas.

gw->addToRegion(jbelabel/ginteractor, ...) simply adds the label/interactor's private
'interactor' (a JComponent like a JLabel or JButton) to one of the GWindow's private
JPanels, so the JBELabel/GInteractor has null t/c parent and doesn't participate in
any GContainer containment hierarchy.

JBETopCompound is currently being abused, as every GCompound created by front end
is really JBETopCompound. This doesn't seem right, since a JBETC has a JBECanvas
'owner' field. One way to detect whether a compound is a top compound is to use JBETopCompound.getCanvas() and check for null, I suppose. Or I could just bite the bullet and use GCompound for non-top compounds, then check for JBETopCompound as necessary.
Really, front-end users shouldn't be able to add interactors to a GCompound. I could live with that, though some sort of grouping mechanism for interactors (besides addToRegion)
would be a nice feature. The problem now is that when interactors are added to a GCompound
in the front end, things like sendToBack don't have the expected effect, since the 
interactors are always drawn on top of other GObjects in the compound, no matter what
the internal ordering. But his is true even when interactors are added directly to the 
window, since they are really just added to the top compound.



CPP GObject
===========
GObject has protected GCompound *parent. Adding a GObject to GWindow really adds
it to top-level GCompound (whose parent is NULL).

JBE Threads and listeners
=========================

Best article on threads and Swing: 
	http://www.informit.com/articles/article.aspx?p=26326&seqNum=9
	
Thread-safe Swing methods (don't need to be called from Event Dispatch Thread):
some methods of JComponent: 
	- repaint, revalidate, invalidate
	- all addXXXListener and removeXXXListener methods
	- all methods explicitly documented as thread-safe
	- JTextComponent.setText
	- JTextArea.insert
	- JTextArea.append
	- JTextArea.replaceRange
	- JTextComponent: replaceSelection (overridden in JEditorPane, JTextPane)
	- JTextComponent: setText (overridden in JEditorPane)
	- JTextArea: insert, append, replaceRange
	- JTextPane: insertComponent, insertIcon, setLogicalStyle, 
	             setCharacterAttributes, setParagraphAttributes
	- AbstractDocument: render, remove, insertString (overridden 
	                    in PlainDocument, DefaultStyledDocument), createPosition
	- DefaultStyledDocument: setLogicalStyle, setCharacterAttributes, 
	                         setParagraphAttributes (overridden in HTMLDocument)
	- StyleContext: several
	
GButton, GCheckBox, GChooser, GTextField, GTimer: the jbe is action listener
Simply creating one of the first 4 of these (the interactors) sets the action
listener to be the jbe. 

GSlider: the jbe is change listener; this is set upon creation.

JFrame consoleFrame in JBE: jbe is window listener; 
consoleFrame contains a JBEConsole (JScrollPane) that has a ConsoleTextPane ctp.
The ctp is its own key listener.

JBEWindow (JFrame) has a JBECanvas. The JBECanvas has a GCanvasListener, which is a
mouse listener and mouse motion listener for the JBECanvas. It redispatches these
events, with source being an appropriate GObject rather than the canvas.
The jbe is a component listener, mouse listener, mouse motion
listener, and key listener for the JBECanvas of a JBEWindow.
The jbe is a window listener for a JBEWindow.

GRect_create calls GRect constructor calls GObject::setLocation calls GObject::repaint
calls GCanvas::conditionalRepaint, but parent is null so AWT repaint not called. In any
case, AWT repaint is thread-safe.

GCompound_add calls GCompound::add, which may call GObjectList::add


========== TODO LIST ========================================

The JBE pipe is needed for console and gwindow/gobjects/gevent/ginteractors, clearly, but also for gtimer, filelib (openFileDialog), and sound.
gtimer.cpp includes platform.h, which includes gwindow.h. But if I just include gtimer.h, how does initPipe get called??? Ah, I see: the only
way to use a GTimer is to use GEvent, but gevents.h includes gwindow.h.
Same question for sound.h. Answer: it doesn't, so trying to play a sound will hang the program forever.Same problem with filelib.h (openFileDialog).
BOTH FIXED (see JBEMISC stuff in private/main.h)
DONE


Can't play sound from command line (java -jar spl.jar), and can play a loaded sound only once from 
C++ program. See http://stackoverflow.com/questions/5529754/java-io-ioexception-mark-reset-not-supported.
FIXED. See changes in getClip() in JavaBackEnd.java. Still can't reliably play sound repeatedly unless
at least a 1-second pause is inserted between plays (on virtualized win7, anyway; works fine on linux).
DONE


Console: implement copy menu item, at least.
DONE (copy, cut, paste, select all)


Console: Implement file menu with Quit?
TODO


GWindow: removeFromRegion is in CPPLib, not in JBE. 
DONE


GImageTools (java): has many nice low-level methods. Most not
available in CPPLib. Add some of these?
TODO


Eliminate unused variable 'xch' in tokenscanner.cpp.
DONE


Eliminate unused parameters in CPPLib, turn warnings back on.
WON'T FIX: too many are unavoidable.


Implement closeConsoleAndExit.
DONE


Implement setConsolePrintExceptions? 
DONE


Implement setConsoleEcho?
TODO


Figure out how to regenerate CPPLib docs.
DONE: they seem to have used a customized version of cppdoc. That
program appears to be unmaintained. Switching to doxygen.


Figure out how to position console & gwindows so they don't overlap.
setConsoleSize() seems to have no effect.
BOTH FIXED (mostly): Must create GWindow before first cout/cin statement
in order for console to be placed beneath gwindow. Then can call setConsoleSize()
to resize console. See changes in JavaBackEnd.java setConsoleSize().
DONE


Add code to platform.cpp so in Windows so can set environment variable
JBETRACE to "true" to get comm trace logged to JBElog.txt. There is already
the capability in non-Windows systems to log to /dev/tty when JBETRACE is
"true". 
DONE


Develop a way for clients to get width and height of the canvas in a GWindow. 
When control strips are present, gw.getWidth() and gw.getHeight() aren't very
useful.
DONE: added GWindow.getCanvasHeight/Width() to JBE and CPPLib.


Implement SetConsoleExitOnClose.
DONE


Add code to terminate JBE harshly if SetConsoleOnExit is true and the user
closes the console window while it's waiting for console input, since there's
no way to successfully send "GWindow.exitGraphics" under those conditions.
DONE


Improve error messages for Vector, etc.
DONE


Implement overloaded readEntireFile() functions from old CPPLib?
DONE


Find all extra functions in old lib and decide whether to implement.
Same for Stepp version.
TODO


Implement printing of stack trace in exception handler (from Stepp).
DONE


Convert docstrings to Doxygen format.
DONE


Adapt Stepp's test to my project.
TODO


Possible things to add:
grid.fill(42)
DONE


for (Node* node : graph) {...} (iterator for Graph)
TODO


cout << lex (overload << for Lexicon)
TODO


less for Vector<>, Set<>, Queue<>, etc.,  so can make Set<Vector<int>>, Set<Set<int>>, etc.
TODO


Doxygen: 
Get rid of internal GraphComparator class docs
Get rid of internal Grid<>::GridRow class docs
And GTimerData struct
DONE


Remove -fno-elide-constructors when done debugging.
TODO


Write code to strip #include statements from doxygen file docs
(leave them in class docs, however).
DONE - edited doxygen.xml to accomplish this


Test everything carefully on Mac and Windows.
TODO


Fix parent handling in JBE for GInteractors so can remove a GInteractor
that was added directly with GWindow.add() instead of addToRegion.
DONE


Print cerr to console in red.
DONE on Mac. CHECK on Windows


Regenerate comments in graph.h
DONE


Fix readBinaryFile in Lexicon (segfault)
Doesn't crash on win32+mingw32, where sizeof(Edge) is 4, but crashes on Mac,
where it's 8 (!). Fixed by changing 'unsigned long' fields to uint32_t
in definition of Edge struct (to coax compiler into using only 32 bytes
for an Edge) and in byte-swapping routine my_ntohl. Now I see Stepp fixed
the Edge struct by preceding it with "#pragma pack(1)". Fine. So will I.
DONE


Fix findOnPath in filelib.cpp (doesn't seem to work if path is ".:~/lib", for example.
Bug was in splitPath function. 
DONE

Fixed other bugs in filelib.cpp and platform.cpp (marked BUGFIX)
DONE


createDirectoryPath: passing "~", "~/a/b", etc. doesn't work (Mac)
"." and ".." work on Mac but not Windows. Indeed, on Windows, if some
intermediate component of path already exists, createDirectoryPath(path) fails!
DONE


Check diffPixel thingy in GBufferedImage.
DONE


openFileDialog doesn't appear on Mac. Happens unpredictably. Fixed by
modifying JBECommand.java so file dialog stuff runs in "event dispatch thread".
DONE


Fix backend bugs in file dialog stuff.
DONE


Test thread.h on both platforms.
Fix serious bugs in thread library.
See GTimer for possible way to handle refCount.
DONE - I think. See thread-test.cpp.


Add red text div to css.
DONE


Add @brief tag to .h files descriptions.
DONE


Doxygen: images for GSlider, etc. are broken.
DONE


Insert doxygen comments for << (and maybe >>) operators in all classes
that have them. See point.h for model.
TODO


Script to change 'std::string' to 'string' in all doxygen-generated html?
Or maybe delete 'std::' everywhere, since 'std::istream' and other such
things also occur.
DONE: see doxygen-run.sh


Test sound library again.
TODO


Test GObject::sendForward, sendToBack, etc.
DONE


Rotating GLabel doesn't work. 
WON'T CHANGE. It does work if label is added to window canvas, but not
if added to side panel as an "interactor". Back end uses custom-drawn
string in former case (java GLabel) and uses a Swing JLabel (in JBELabel)
in the latter case. 

Asterisk in font string (setConsoleFont) doesn't work?
See Java Font.decode docs.
TODO

Press return to close window in mac terminal doesn't work
TODO

Add a consoleFocus command?
TODO

Write better docs for rotate and scale (gobjects.h). What these really
do is to concatenate transformations with the transformation matrix
of the object. The upshot is that a sequence of calls to rotate and scale
must be given in the reverse of the order in which you want them applied.
TODO

Add a translate command and a rotateAbout(x, y) command for gobjects.
TODO

Give console the focus and keyboard focus when getting input.
DONE

Tweak console position under graphics window on mac.
DONE

Java GBufferedImage.load: remove debug statements, look into weird bounds
for GIF image load (256x256 reported as 55x16 or 60x16).
TODO

Stack traces on Mac aren't very helpful:
 *** Stack trace (line numbers are approximate):
 *** call_stack_gcc.cpp:275  main
 *** exceptions.cpp:183      start
Fixed: problem was new version of atos on Mac.
DONE

Test creating/destroying windows repeatedly.
DONE

Calling glabel->setVisible(false) *before* adding label to region does not work.
TODO

Make GWindow::GWindow(w, h, visible) part of the documented interface and fix
back end so initially invisible windows don't briefly pop up.
TODO


Named type for file dialog filter list (dialog says "Image files" instead of "*.bmp;*.gif...")
TODO


Buffered image scaling
DONE


Button disabling
DONE


Option pane
DONE

Stepp's menus
TODO

Option to set parent window for GOptionPane
TODO


GRect--setFillColor(0x0) doesn't work but setFillColor("black") does??
TODO