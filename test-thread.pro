cache()

###################################################################
#  Project-specific sources and headers
#

SOURCES += $$PWD/src/tests-JL/thread-test.cpp

####################################################################
# Common configuration for all projects

# Mac users: change `10.9` to match your version of Mac OS X, if necessary.
QMAKE_MAC_SDK = macosx10.9

TEMPLATE = app
CONFIG -= qt
CONFIG -= debug_and_release
CONFIG += debug
win32:CONFIG += console

# StanfordCPPLib headers
HEADERS += $$files($$PWD/StanfordCPPLib/*.h)
HEADERS += $$files($$PWD/StanfordCPPLib/stacktrace/*.h)
HEADERS += $$files($$PWD/StanfordCPPLib/private/*.h)

# StanfordCPPLib library
win32 {
    LIBS += -L$$PWD/StanfordCPPLib/lib/win -lStanfordCPPLib
    PRE_TARGETDEPS = $$PWD/StanfordCPPLib/lib/win/libStanfordCPPLib.a
}
unix:!macx {
    LIBS += -L$$PWD/StanfordCPPLib/lib/linux -lStanfordCPPLib
    PRE_TARGETDEPS = $$PWD/StanfordCPPLib/lib/linux/libStanfordCPPLib.a
}
macx {
    LIBS += -L$$PWD/StanfordCPPLib/lib/mac -lStanfordCPPLib
    PRE_TARGETDEPS = $$PWD/StanfordCPPLib/lib/mac/libStanfordCPPLib.a
}

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -fvisibility-inlines-hidden

QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
QMAKE_CXXFLAGS_WARN_ON += -Wno-sign-compare
QMAKE_CXXFLAGS_WARN_ON += -Wno-missing-field-initializers

win32: QMAKE_LFLAGS += -static

unix:!macx {
    QMAKE_LFLAGS += -pthread
    QMAKE_LFLAGS += -rdynamic  # for backtraces
}

!win32 {
    LIBS += -ldl # for backtraces
}
win32:LIBS += -lDbghelp # for backtraces

INCLUDEPATH += $$PWD/StanfordCPPLib
INCLUDEPATH += $$PWD/src

OBJECTS_DIR = $$OUT_PWD/obj

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
!win32 {
    copyToDestdir($$files($$PWD/resources/*))
    copyToDestdir($$files($$PWD/extra/*))
}
win32 {
    copyToDestdir($$PWD/resources)
    copyToDestdir($$PWD/extra)
}
