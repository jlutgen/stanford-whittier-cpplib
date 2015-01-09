#!/bin/bash
# Usage: update-assignment proj_name assign_dir

PROJNAME=$1
DIR=$2
if [ ! -d $DIR ] 
then
	echo "$DIR does not exist. Exiting."
	exit
fi

set -x # echo commands
cp -R StanfordCPPLib $DIR
rm $DIR/StanfordCPPLib/*.cpp
rm $DIR/StanfordCPPLib/*.orig
rm -rf $DIR/StanfordCPPLib/original\ headers
rm $DIR/*.pro
rm $DIR/*.pro.user*
cp assignment-skeleton.pro $DIR/$PROJNAME.pro

if [ ! -d $DIR/extra ]
then
	mkdir $DIR/extra
fi

if [ -d $DIR/out ] 
then
	rm $DIR/out/*.exe
	rm $DIR/out/spl.jar
	mv $DIR/out/* $DIR/extra
	rmdir $DIR/out
fi

if [ -d $DIR/resources ]
then
	cp resources/spl.jar resources/EnglishWords.dat resources/addr2line.exe $DIR/resources
else
	cp -R resources $DIR
fi
	


