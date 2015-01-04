!macx:cache()

TEMPLATE = app

# Make sure we do not accidentally #include files placed in 'resources'

CONFIG -= qt
CONFIG += debug
CONFIG += console
#CONFIG += no_include_pwd

SOURCES = $$files($$PWD/StanfordCPPLib/*.cpp)
SOURCES += $$files($$PWD/StanfordCPPLib/stacktrace/*.cpp)
SOURCES += $$PWD/src/assign-1-main.cpp
HEADERS = $$files($$PWD/StanfordCPPLib/*.h)
HEADERS += $$files($$PWD/StanfordCPPLib/stacktrace/*.h)
HEADERS += $$files($$PWD/StanfordCPPLib/private/*.h)

#win32:LIBS += -L$$PWD/StanfordCPPLib/lib/win32 -lStanfordCPPLib
#unix:LIBS += -L$$PWD/StanfordCPPLib/lib/x86-64 -lStanfordCPPLib

win32:LIBS += -lDbghelp

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-sign-compare
QMAKE_CXXFLAGS += -Wno-missing-field-initializers
#win32:QMAKE_CXXFLAGS += -static
QMAKE_CXXFLAGS += -fvisibility-inlines-hidden
!win32:QMAKE_LFLAGS += -static-libstdc++ -pthread
win32: QMAKE_LFLAGS += -static

unix:!macx {
    QMAKE_LFLAGS += -rdynamic
    #QMAKE_LFLAGS += -Wl,--export-dynamic
    #QMAKE_CXXFLAGS += -Wl,--export-dynamic
}
!win32 {
    #QMAKE_CXXFLAGS += -Wno-dangling-field
    #QMAKE_CXXFLAGS += -Wno-unused-const-variable
    LIBS += -ldl
}

INCLUDEPATH += $$PWD/StanfordCPPLib
INCLUDEPATH += $$PWD/src

DESTDIR = $$PWD/out
win32:OBJECTS_DIR = $$OUT_PWD/obj/win
macx:OBJECTS_DIR = $$OUT_PWD/obj/mac
unix:OBJECTS_DIR = $$OUT_PWD/obj/linux

MOC_DIR = $$OUT_PWD/tmp

# set up flags used internally by the Stanford C++ libraries
DEFINES += SPL_CONSOLE_X=100
DEFINES += SPL_CONSOLE_Y=10
DEFINES += SPL_CONSOLE_WIDTH=650
DEFINES += SPL_CONSOLE_HEIGHT=500
DEFINES += SPL_CONSOLE_FONTSIZE=12
DEFINES += SPL_CONSOLE_EXIT_ON_CLOSE

# Function that copies the given files to the destination directory
defineTest(copyToDestdir) {
    files = $$1

    for(FILE, files) {
        DDIR = $$OUT_PWD

        # Replace slashes in paths with backslashes for Windows
        win32:FILE ~= s,/,\\,g
        win32:DDIR ~= s,/,\\,g

        !win32 {
            QMAKE_POST_LINK += cp -r '"'$$FILE'"' '"'$$DDIR'"' $$escape_expand(\\n\\t)
        }
        win32 {
            QMAKE_POST_LINK += xcopy '"'$$FILE'"' '"'$$DDIR'"' /e /y $$escape_expand(\\n\\t)
        }
    }

    export(QMAKE_POST_LINK)
}
#!win32 {
#    copyToDestdir($$files($$PWD/resources/*))
#}
#win32 {
#    copyToDestdir($$PWD/resources)
#}
