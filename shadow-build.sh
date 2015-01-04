#!/bin/bash

CURRENTDIR=$(pwd)
PRO_FILE=$1
QMAKE=/usr/lib/x86_64-linux-gnu/qt5/bin/qmake
TEMPDIR=~/temp
BASE=$(basename -s .pro $1)
BUILDDIR=$TEMPDIR/$BASE
test -d $BUILDDIR || mkdir -p $BUILDDIR
cd $BUILDDIR
$QMAKE $CURRENTDIR/$PRO_FILE
