#!/bin/sh
dir=$1
mat=$2
dt=$3
haddcommand="hadd Cosmic-SystemHealth-$mat-$dt.root"
inputfilenames=`ls $dir/*$dt*.root`
rmcommand="rm -rf "
for eachfile in $inputfilenames
do
	echo "Processing file : $eachfile ....."
	justFileName=$(basename $eachfile .root)
	outputFileName="SystemHealth_$justFileName.root "
	haddcommand+=" $outputFileName"
	rmcommand+=$outputFileName
	./TestSystemHealth $eachfile
done
$haddcommand
$rmcommand
