#!/bin/sh


if [ "$2" = '' ]
then
	echo "BreakLongLines.sh  file  pattern"
	exit 1
fi

FILE=$1
PATTERN=$2


sed "s/$PATTERN/\n$PATTERN/g" < $FILE

