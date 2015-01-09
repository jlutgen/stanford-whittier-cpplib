
#QT       -= core gui

TARGET = StanfordCPPLib
TEMPLATE = lib
CONFIG -= qt
CONFIG += staticlib

INCLUDEPATH += $$PWD/StanfordCPPLib

SOURCES = $$files($$PWD/StanfordCPPLib/*.cpp)
SOURCES += $$files($$PWD/StanfordCPPLib/stacktrace/*.cpp)
!win32:SOURCES -= $$PWD/StanfordCPPLib/stacktrace/call_stack_windows.cpp

HEADERS = $$files($$PWD/StanfordCPPLib/*.h)
HEADERS += $$files($$PWD/StanfordCPPLib/stacktrace/*.h)
HEADERS += $$files($$PWD/StanfordCPPLib/private/*.h)

win32:LIBS += -lDbghelp

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -fvisibility-inlines-hidden
#QMAKE_CXXFLAGS += -fno-elide-constructors       # DEBUG

QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
QMAKE_CXXFLAGS_WARN_ON += -Wno-sign-compare
QMAKE_CXXFLAGS_WARN_ON += -Wno-missing-field-initializers

!win32:QMAKE_LFLAGS += -static-libstdc++ -pthread
win32: QMAKE_LFLAGS += -static

win32:DESTDIR = $$PWD/StanfordCPPLib/lib/win
unix:!macx:DESTDIR = $$PWD/StanfordCPPLib/lib/linux
macx:DESTDIR = $$PWD/StanfordCPPLib/lib/mac

OBJECTS_DIR = $$OUT_PWD/obj
#MOC_DIR = $$OUT_PWD/tmp

#DEFINES += SPL_CONSOLE_X=100
#DEFINES += SPL_CONSOLE_Y=10
DEFINES += SPL_CONSOLE_WIDTH=650
DEFINES += SPL_CONSOLE_HEIGHT=500
DEFINES += SPL_CONSOLE_FONTSIZE=12
DEFINES += SPL_CONSOLE_EXIT_ON_CLOSE
