#!/bin/sh
noOfShots=$1
#shotNo=0
counter=0
dir=$2
mat=$3
dt=$4
helper=$5
inputfilenames=`ls $dir/*$mat*$dt*.root`
if [ -z "$helper" ]
then
	finalProcessedFile="ProcessedData_$mat-$dt.root"
else
	finalProcessedFile="ProcessedData_$mat-$helper-$dt.root"
fi

echo "Final Output File Name : "$finalProcessedFile

allProcessedFileNames=""
for eachfile in $inputfilenames
do
	echo "Processed File : "$eachfile >> processed.log 
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
	command="hadd -f $processedFileName $allFileNames"
	$command
	allProcessedFileNames+=" $processedFileName"
	rm -rf $allFileNames
done
hadd -f $finalProcessedFile $allProcessedFileNames
rm -rf $allProcessedFileNames
