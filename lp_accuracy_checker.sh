#!/bin/sh
# 
# File:   lp_accuracy_checker.sh
# Author: Sriram Mahavadi
#
# Created on 5 Jun, 2014, 11:39:10 AM
#

SOPLEX_EXECUTABLE="/home/sriram/Desktop/sriram_thesis_main/softwares/soplex-2.0.0.linux.x86_64.gnu.opt"
EXTLP_EXECUTABLE="/home/sriram/Desktop/sriram_thesis_main/codebase/extlp/build/local/extlp"

SAVEIFS=$IFS
#IFS=$(echo -en "\n\b")
INPUT_DIR=/home/sriram/Desktop/sriram_thesis_inputs/miplib2010_complete_max/max_general
${SOPLEX_EXECUTABLE} "${INPUT_DIR}/${1}"
#    ${EXTLP_EXECUTABLE} "${f}"
#INPUT_FILES=${INPUT_DIR}/*
#for f in $INPUT_FILES
#do
#   echo ${f}
    #${SOPLEX_EXECUTABLE} "${f}"
#    ${EXTLP_EXECUTABLE} "${f}"
#done

IFS=$SAVEIFS


