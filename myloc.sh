#!/bin/sh
# 
# File:   myloc.sh
# Author: sriram
#
# Created on 7 Apr, 2014, 8:33:35 AM
#
FILES=./local/*
TOTAL_LOC=0
echo "Inital lines: $TOTAL_LOC"
for f in $FILES
do
  #echo "Processing $f file..."
  # take action on each file. $f store current file name
  LOC=$(wc -l < "$f")
  #$LOC := wc -l $f
  echo "Number of lines in file: $f is: $LOC"
  TOTAL_LOC=$(($TOTAL_LOC + $LOC))
done
echo "********************************************"
echo "--- Total Number of lines is: $TOTAL_LOC ---"
echo "********************************************"