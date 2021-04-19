#!/bin/sh
dir=$1
dt=$2
inputfilenames=`ls $dir/*$dt*.root`
for eachfile in $inputfilenames
do
	./Rollover_Inspection $eachfile
	echo "Rollover returned value for "$eachfile" : "$? >> rollover_$dt.txt
done
