#!/bin/sh
# 
# File:   myloc.sh
# Author: sriram
#
# Created on 7 Apr, 2014, 8:33:35 AM
#
FILES=./local/*
DEPRECATED_FILES=./local/deprecated/*
FILES="$FILES $DEPRECATED_FILES"
echo $DEPRECATED_FILES
TOTAL_LOC=0
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
NORMAL=$(tput sgr0)
#echo "Inital lines: $TOTAL_LOC"
echo "******************* LOC ****************"
printf " %-50s $GREEN[%10s]$NORMAL\n" "File Name" "LOC"
printf " %-50s   $GREEN%10s$NORMAL\n" "==================" "========= "
for f in $FILES
do

if [ -f "${f}" ] ; then
  #echo "Processing $f file..."
  # take action on each file. $f store current file name

  LOC=$(wc -l < "$f")
  #$LOC := wc -l $f
  #echo "Number of lines in file: $f is: $LOC"
  printf " %-50s $GREEN[%10d]$NORMAL\n" $f $LOC
  TOTAL_LOC=$(($TOTAL_LOC + $LOC))
fi
done
echo "********************************************"
#echo "--- Total LOC: $TOTAL_LOC ---"
printf "Total LOC: *** $GREEN%s$NORMAL ***\n" $TOTAL_LOC
echo "********************************************"
