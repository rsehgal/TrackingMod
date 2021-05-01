#!/bin/sh
dir=$1
mat=$2
dt=$3
inputfilenames=`ls $dir/*$mat*$dt*.root`
for eachfile in $inputfilenames
do
	./Rollover_Inspection $eachfile
	echo "Rollover returned value for "$eachfile" : "$? >> rollover_$dt.txt
done
