cache()

TEMPLATE = app

# Make sure we do not accidentally #include files placed in 'resources'

CONFIG -= qt
CONFIG += debug
CONFIG += console
#CONFIG += no_include_pwd

SOURCES = $$files($$PWD/StanfordCPPLibOld/src/*.cpp)
SOURCES += $$PWD/src/test-stanford-old.cpp
HEADERS = $$files($$PWD/StanfordCPPLibOld/include/*.h)
HEADERS += $$files($$PWD/StanfordCPPLibOld/include/private/*.h)

#win32:LIBS += -L$$PWD/StanfordCPPLib/lib/win32 -lStanfordCPPLib
#unix:LIBS += -L$$PWD/StanfordCPPLib/lib/x86-64 -lStanfordCPPLib

QMAKE_CXXFLAGS += -std=c++11 -Wno-unused-parameter -Wno-sign-compare
#win32:QMAKE_CXXFLAGS += -static
QMAKE_CXXFLAGS += -fvisibility-inlines-hidden
!win32:QMAKE_LFLAGS += -static-libstdc++ -pthread
win32: QMAKE_LFLAGS += -static

INCLUDEPATH += $$PWD/StanfordCPPLibOld/include
INCLUDEPATH += $$PWD/src

DESTDIR = $$PWD/out
win32:OBJECTS_DIR = $$OUT_PWD/obj/win
macx:OBJECTS_DIR = $$OUT_PWD/obj/mac
unix:OBJECTS_DIR = $$OUT_PWD/obj/linux

MOC_DIR = $$OUT_PWD/tmp

# Copies the given files to the destination directory
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
