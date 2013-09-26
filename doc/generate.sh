#!/bin/bash

PROGRAM="doxygen"

DOXYPATH=`which $PROGRAM 2&>/dev/null`
if [ $? == 1 ]; then
	$PROGRAM ./Doxyfile
	echo "END."
else
	echo "Cannot find doxygen!";
fi

