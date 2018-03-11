#!/bin/bash
#arg 1 is the folder in which all csv files from the flights reside
#arg 2 is the destination csv which will be a concatenation of all
#files
cat $1/*.csv >> $2
