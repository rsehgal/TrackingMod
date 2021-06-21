#!/bin/sh
dir=$1
inputfilenames=`ls $dir/*.root`
rmcommand="rm -rf "
for eachfile in $inputfilenames
do
	echo "Processing file : $eachfile ....."
	#justFileName=$(basename $eachfile .root)
	#outputFileName="SystemHealth_$justFileName.root "
	#haddcommand+=" $outputFileName"
	#rmcommand+=$outputFileName
	./TestReadCharacterizationData $eachfile &
done
