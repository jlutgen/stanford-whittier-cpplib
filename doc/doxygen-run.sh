#!/bin/bash

# Run this script from this directory!

# Run doxygen and do some post-processing
cd ..
/Applications/Doxygen.app/Contents/Resources/doxygen doxygen.config

for f in ~/temp/doc/html/*.html ; do
	sed -e 's/std:://g' -i .bak $f  ; # strip out all occurrences of 'std::'
done

rm ~/temp/doc/html/*.bak

