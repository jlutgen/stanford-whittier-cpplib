
MAKEFILE      = Makefile

####### Compiler, tools and options

CXX           = g++
DEFINES       = -DSPL_CONSOLE_WIDTH=650 -DSPL_CONSOLE_HEIGHT=500 -DSPL_CONSOLE_FONTSIZE=12 -DSPL_CONSOLE_EXIT_ON_CLOSE
CXXFLAGS      = -pipe -std=c++11 -fvisibility-inlines-hidden -g -fPIC -Wall -W -Wno-unused-parameter -Wno-sign-compare -Wno-missing-field-initializers $(DEFINES)
INCPATH       = -IStanfordCPPLib -IStanfordCPPLib/stacktrace -IStanfordCPPLib/private
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
DEL_FILE      = rm -f
DEL_DIR       = rmdir
MOVE          = mv -f
DISTDIR = build
AR            = ar cq
RANLIB        = ranlib -s
SED           = sed
STRIP         = 

####### Output directory

OBJECTS_DIR   = obj/

####### Files

SOURCES       = StanfordCPPLib/base64.cpp \
		StanfordCPPLib/console.cpp \
		StanfordCPPLib/direction.cpp \
		StanfordCPPLib/error.cpp \
		StanfordCPPLib/exceptions.cpp \
		StanfordCPPLib/filelib.cpp \
		StanfordCPPLib/gbufferedimage.cpp \
		StanfordCPPLib/gevents.cpp \
		StanfordCPPLib/ginteractors.cpp \
		StanfordCPPLib/gmath.cpp \
		StanfordCPPLib/gobjects.cpp \
		StanfordCPPLib/goptionpane.cpp \
		StanfordCPPLib/gtextarea.cpp \
		StanfordCPPLib/gtimer.cpp \
		StanfordCPPLib/gtypes.cpp \
		StanfordCPPLib/gwindow.cpp \
		StanfordCPPLib/hashcode.cpp \
		StanfordCPPLib/lexicon.cpp \
		StanfordCPPLib/main.cpp \
		StanfordCPPLib/platform.cpp \
		StanfordCPPLib/point.cpp \
		StanfordCPPLib/random.cpp \
		StanfordCPPLib/simpio.cpp \
		StanfordCPPLib/sound.cpp \
		StanfordCPPLib/startup.cpp \
		StanfordCPPLib/strlib.cpp \
		StanfordCPPLib/thread.cpp \
		StanfordCPPLib/tokenscanner.cpp \
		StanfordCPPLib/tplatform-posix.cpp \
		StanfordCPPLib/stacktrace/call_stack_gcc.cpp 
OBJECTS       = obj/base64.o \
		obj/console.o \
		obj/direction.o \
		obj/error.o \
		obj/exceptions.o \
		obj/filelib.o \
		obj/gbufferedimage.o \
		obj/gevents.o \
		obj/ginteractors.o \
		obj/gmath.o \
		obj/gobjects.o \
		obj/goptionpane.o \
		obj/gtextarea.o \
		obj/gtimer.o \
		obj/gtypes.o \
		obj/gwindow.o \
		obj/hashcode.o \
		obj/lexicon.o \
		obj/main.o \
		obj/platform.o \
		obj/point.o \
		obj/random.o \
		obj/simpio.o \
		obj/sound.o \
		obj/startup.o \
		obj/strlib.o \
		obj/thread.o \
		obj/tokenscanner.o \
		obj/tplatform-posix.o \
		obj/call_stack_gcc.o
DESTDIR       = build
TARGET        = libStanfordCPPLib.a

first: all
####### Implicit rules

.SUFFIXES: .o .cpp 

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

$(DESTDIR)/$(TARGET): $(OBJECTS_DIR) $(OBJECTS)
	test -d $(DESTDIR) || mkdir -p $(DESTDIR)
	-$(DEL_FILE) $(TARGET)
	$(AR) $(TARGET) $(OBJECTS)
	$(RANLIB) $(TARGET)
	-$(DEL_FILE) $(DESTDIR)/$(TARGET)
	-$(MOVE) $(TARGET) $(DESTDIR)

$(OBJECTS_DIR):
	test -d $(OBJECTS_DIR) || mkdir -p $(OBJECTS_DIR)

all: $(DESTDIR)/$(TARGET)

clean:
	-$(DEL_FILE) $(OBJECTS)

####### Compile

obj/base64.o: StanfordCPPLib/base64.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/base64.o StanfordCPPLib/base64.cpp

obj/console.o: StanfordCPPLib/console.cpp StanfordCPPLib/console.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/error.h \
		StanfordCPPLib/platform.h \
		StanfordCPPLib/gevents.h \
		StanfordCPPLib/gtimer.h \
		StanfordCPPLib/gwindow.h \
		StanfordCPPLib/gtypes.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/sound.h \
		StanfordCPPLib/exceptions.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/console.o StanfordCPPLib/console.cpp

obj/direction.o: StanfordCPPLib/direction.cpp StanfordCPPLib/error.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/direction.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/strlib.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/tokenscanner.h \
		StanfordCPPLib/private/tokenpatch.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/direction.o StanfordCPPLib/direction.cpp

obj/error.o: StanfordCPPLib/error.cpp StanfordCPPLib/error.h \
		StanfordCPPLib/private/main.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/error.o StanfordCPPLib/error.cpp

obj/exceptions.o: StanfordCPPLib/exceptions.cpp StanfordCPPLib/exceptions.h \
		StanfordCPPLib/error.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/filelib.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/strlib.h \
		StanfordCPPLib/stacktrace/call_stack.h \
		StanfordCPPLib/platform.h \
		StanfordCPPLib/gevents.h \
		StanfordCPPLib/gtimer.h \
		StanfordCPPLib/gwindow.h \
		StanfordCPPLib/gtypes.h \
		StanfordCPPLib/console.h \
		StanfordCPPLib/sound.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/exceptions.o StanfordCPPLib/exceptions.cpp

obj/filelib.o: StanfordCPPLib/filelib.cpp StanfordCPPLib/filelib.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/error.h \
		StanfordCPPLib/platform.h \
		StanfordCPPLib/gevents.h \
		StanfordCPPLib/gtimer.h \
		StanfordCPPLib/gwindow.h \
		StanfordCPPLib/gtypes.h \
		StanfordCPPLib/console.h \
		StanfordCPPLib/sound.h \
		StanfordCPPLib/strlib.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/filelib.o StanfordCPPLib/filelib.cpp

obj/gbufferedimage.o: StanfordCPPLib/gbufferedimage.cpp StanfordCPPLib/gbufferedimage.h \
		StanfordCPPLib/grid.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/strlib.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/ginteractors.h \
		StanfordCPPLib/gobjects.h \
		StanfordCPPLib/gtypes.h \
		StanfordCPPLib/gwindow.h \
		StanfordCPPLib/console.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/error.h \
		StanfordCPPLib/gmath.h \
		StanfordCPPLib/base64.h \
		StanfordCPPLib/filelib.h \
		StanfordCPPLib/platform.h \
		StanfordCPPLib/gevents.h \
		StanfordCPPLib/gtimer.h \
		StanfordCPPLib/sound.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/gbufferedimage.o StanfordCPPLib/gbufferedimage.cpp

obj/gevents.o: StanfordCPPLib/gevents.cpp StanfordCPPLib/error.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/gevents.h \
		StanfordCPPLib/gtimer.h \
		StanfordCPPLib/gwindow.h \
		StanfordCPPLib/gtypes.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/console.h \
		StanfordCPPLib/map.h \
		StanfordCPPLib/stack.h \
		StanfordCPPLib/platform.h \
		StanfordCPPLib/sound.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/gevents.o StanfordCPPLib/gevents.cpp

obj/ginteractors.o: StanfordCPPLib/ginteractors.cpp StanfordCPPLib/gevents.h \
		StanfordCPPLib/gtimer.h \
		StanfordCPPLib/gwindow.h \
		StanfordCPPLib/gtypes.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/console.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/error.h \
		StanfordCPPLib/ginteractors.h \
		StanfordCPPLib/gobjects.h \
		StanfordCPPLib/gmath.h \
		StanfordCPPLib/platform.h \
		StanfordCPPLib/sound.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/ginteractors.o StanfordCPPLib/ginteractors.cpp

obj/gmath.o: StanfordCPPLib/gmath.cpp StanfordCPPLib/gmath.h \
		StanfordCPPLib/gtypes.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/gmath.o StanfordCPPLib/gmath.cpp

obj/gobjects.o: StanfordCPPLib/gobjects.cpp StanfordCPPLib/gevents.h \
		StanfordCPPLib/gtimer.h \
		StanfordCPPLib/gwindow.h \
		StanfordCPPLib/gtypes.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/console.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/error.h \
		StanfordCPPLib/gmath.h \
		StanfordCPPLib/gobjects.h \
		StanfordCPPLib/platform.h \
		StanfordCPPLib/sound.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/gobjects.o StanfordCPPLib/gobjects.cpp

obj/goptionpane.o: StanfordCPPLib/goptionpane.cpp StanfordCPPLib/goptionpane.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/gwindow.h \
		StanfordCPPLib/gtypes.h \
		StanfordCPPLib/console.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/error.h \
		StanfordCPPLib/platform.h \
		StanfordCPPLib/gevents.h \
		StanfordCPPLib/gtimer.h \
		StanfordCPPLib/sound.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/goptionpane.o StanfordCPPLib/goptionpane.cpp

obj/gtextarea.o: StanfordCPPLib/gtextarea.cpp StanfordCPPLib/gtextarea.h \
		StanfordCPPLib/ginteractors.h \
		StanfordCPPLib/gobjects.h \
		StanfordCPPLib/gtypes.h \
		StanfordCPPLib/gwindow.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/console.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/error.h \
		StanfordCPPLib/gmath.h \
		StanfordCPPLib/platform.h \
		StanfordCPPLib/gevents.h \
		StanfordCPPLib/gtimer.h \
		StanfordCPPLib/sound.h \
		StanfordCPPLib/base64.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/gtextarea.o StanfordCPPLib/gtextarea.cpp

obj/gtimer.o: StanfordCPPLib/gtimer.cpp StanfordCPPLib/platform.h \
		StanfordCPPLib/gevents.h \
		StanfordCPPLib/gtimer.h \
		StanfordCPPLib/gwindow.h \
		StanfordCPPLib/gtypes.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/console.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/error.h \
		StanfordCPPLib/sound.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/gtimer.o StanfordCPPLib/gtimer.cpp

obj/gtypes.o: StanfordCPPLib/gtypes.cpp StanfordCPPLib/error.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/gtypes.h \
		StanfordCPPLib/strlib.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/hashcode.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/gtypes.o StanfordCPPLib/gtypes.cpp

obj/gwindow.o: StanfordCPPLib/gwindow.cpp StanfordCPPLib/gevents.h \
		StanfordCPPLib/gtimer.h \
		StanfordCPPLib/gwindow.h \
		StanfordCPPLib/gtypes.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/console.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/error.h \
		StanfordCPPLib/gobjects.h \
		StanfordCPPLib/gmath.h \
		StanfordCPPLib/map.h \
		StanfordCPPLib/stack.h \
		StanfordCPPLib/strlib.h \
		StanfordCPPLib/platform.h \
		StanfordCPPLib/sound.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/gwindow.o StanfordCPPLib/gwindow.cpp

obj/hashcode.o: StanfordCPPLib/hashcode.cpp StanfordCPPLib/hashcode.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/hashcode.o StanfordCPPLib/hashcode.cpp

obj/lexicon.o: StanfordCPPLib/lexicon.cpp StanfordCPPLib/error.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/lexicon.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/set.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/map.h \
		StanfordCPPLib/stack.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/strlib.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/lexicon.o StanfordCPPLib/lexicon.cpp

obj/main.o: StanfordCPPLib/main.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/main.o StanfordCPPLib/main.cpp

obj/platform.o: StanfordCPPLib/platform.cpp StanfordCPPLib/exceptions.h \
		StanfordCPPLib/error.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/filelib.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/gevents.h \
		StanfordCPPLib/gtimer.h \
		StanfordCPPLib/gwindow.h \
		StanfordCPPLib/gtypes.h \
		StanfordCPPLib/console.h \
		StanfordCPPLib/hashmap.h \
		StanfordCPPLib/queue.h \
		StanfordCPPLib/platform.h \
		StanfordCPPLib/sound.h \
		StanfordCPPLib/stack.h \
		StanfordCPPLib/strlib.h \
		StanfordCPPLib/tokenscanner.h \
		StanfordCPPLib/private/tokenpatch.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/platform.o StanfordCPPLib/platform.cpp

obj/point.o: StanfordCPPLib/point.cpp StanfordCPPLib/point.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/strlib.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/foreach.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/point.o StanfordCPPLib/point.cpp

obj/random.o: StanfordCPPLib/random.cpp StanfordCPPLib/random.h \
		StanfordCPPLib/private/randompatch.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/random.o StanfordCPPLib/random.cpp

obj/simpio.o: StanfordCPPLib/simpio.cpp StanfordCPPLib/simpio.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/simpio.o StanfordCPPLib/simpio.cpp

obj/sound.o: StanfordCPPLib/sound.cpp StanfordCPPLib/gevents.h \
		StanfordCPPLib/gtimer.h \
		StanfordCPPLib/gwindow.h \
		StanfordCPPLib/gtypes.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/console.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/error.h \
		StanfordCPPLib/sound.h \
		StanfordCPPLib/platform.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/sound.o StanfordCPPLib/sound.cpp

obj/startup.o: StanfordCPPLib/startup.cpp StanfordCPPLib/error.h \
		StanfordCPPLib/private/main.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/startup.o StanfordCPPLib/startup.cpp

obj/strlib.o: StanfordCPPLib/strlib.cpp StanfordCPPLib/error.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/strlib.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/hashcode.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/strlib.o StanfordCPPLib/strlib.cpp

obj/thread.o: StanfordCPPLib/thread.cpp StanfordCPPLib/thread.h \
		StanfordCPPLib/private/tplatform.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/thread.o StanfordCPPLib/thread.cpp

obj/tokenscanner.o: StanfordCPPLib/tokenscanner.cpp StanfordCPPLib/error.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/tokenscanner.h \
		StanfordCPPLib/private/tokenpatch.h \
		StanfordCPPLib/strlib.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/stack.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/tokenscanner.o StanfordCPPLib/tokenscanner.cpp

obj/tplatform-posix.o: StanfordCPPLib/tplatform-posix.cpp StanfordCPPLib/strlib.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/error.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/map.h \
		StanfordCPPLib/stack.h \
		StanfordCPPLib/private/tplatform.h \
		StanfordCPPLib/thread.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/tplatform-posix.o StanfordCPPLib/tplatform-posix.cpp

obj/call_stack_gcc.o: StanfordCPPLib/stacktrace/call_stack_gcc.cpp StanfordCPPLib/stacktrace/call_stack.h \
		StanfordCPPLib/exceptions.h \
		StanfordCPPLib/strlib.h \
		StanfordCPPLib/private/genericio.h \
		StanfordCPPLib/vector.h \
		StanfordCPPLib/foreach.h \
		StanfordCPPLib/hashcode.h \
		StanfordCPPLib/platform.h \
		StanfordCPPLib/gevents.h \
		StanfordCPPLib/gtimer.h \
		StanfordCPPLib/gwindow.h \
		StanfordCPPLib/gtypes.h \
		StanfordCPPLib/console.h \
		StanfordCPPLib/private/main.h \
		StanfordCPPLib/error.h \
		StanfordCPPLib/sound.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/call_stack_gcc.o StanfordCPPLib/stacktrace/call_stack_gcc.cpp

