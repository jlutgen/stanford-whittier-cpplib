#!/bin/bash
# Usage: make-assignment project_name dir

PROJNAME=$1
DIR=$2
if [ -e $DIR ] 
then
	echo "$DIR exists. Exiting."
	exit
fi

set -x # echo commands
mkdir -p $2
cp assignment-skeleton.pro $DIR/$PROJNAME.pro
cp -R StanfordCPPLib $DIR
mkdir $DIR/resources
cp resources/spl.jar resources/EnglishWords.dat resources/addr2line.exe $DIR/resources
mkdir $DIR/extra
mkdir $DIR/src
rm $DIR/StanfordCPPLib/*.cpp
rm $DIR/StanfordCPPLib/*.orig
rm -rf $DIR/StanfordCPPLib/original\ headers