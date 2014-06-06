#!/bin/sh
# 
# File:   lp_accuracy_checker.sh
# Author: Sriram Mahavadi
#
# Created on 5 Jun, 2014, 11:39:10 AM
#

SOPLEX_EXECUTABLE="/home/harsha/Desktop/sriram_thesis/softwares/soplex-2.0.0.linux.x86_64.gnu.opt"
EXTLP_EXECUTABLE="/home/harsha/Desktop/sriram_thesis/codebase/extlp/build/local/extlp"

SAVEIFS=$IFS
#IFS=$(echo -en "\n\b")
INPUT_DIR=/media/New\ Volume/sriram_thesis/miplib2010
INPUT_FILES=${INPUT_DIR}/*
for f in $INPUT_FILES
do
    echo ${f}
    ${SOPLEX_EXECUTABLE} "${f}"
    ${EXTLP_EXECUTABLE} "${f}"
done

IFS=$SAVEIFS


