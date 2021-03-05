#!/bin/sh
noOfShots=$1
#shotNo=0
counter=0
dir=$2
mat=$3
dt=$4
inputfilenames=`ls $dir/*$dt*.root`
finalProcessedFile="ProcessedData_$mat-$dt.root"
allProcessedFileNames=""
for eachfile in $inputfilenames
do
	counter=$((counter+1))	
	processedFileName="ProcessedData_$mat-$dt-$counter.root"
	justFileName=$(basename $eachfile .root)
	echo $justFileName
	allFileNames=""
	for shotNo in $(seq 1 $noOfShots)
	do
	  echo "Currently running shot number $shotNo"
	  filename="ProcessedData_$justFileName-$shotNo.root"
	  allFileNames+=" $filename"
	  echo "Output Filename : $filename"
	  ./AnalyzeData $eachfile $filename $noOfShots $shotNo
	done
	#command="hadd ProcessedData_$justFileName.root $allFileNames"

	#command="hadd ProcessedData_$mat-$dt-$counter.root $allFileNames"
	command="hadd $processedFileName $allFileNames"
	$command
	allProcessedFileNames+=" $processedFileName"
	rm -rf $allFileNames
done
hadd $finalProcessedFile $allProcessedFileNames
rm -rf $allProcessedFileNames
