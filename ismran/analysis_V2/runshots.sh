#!/bin/sh
noOfShots=$1
dt=$3
#shotNo=0
for shotNo in $(seq 1 $noOfShots)
do
  echo "Currently running shot number $shotNo"
  filename="ProcessedData_"$dt"_"$shotNo.root
  echo "Output Filename : $filename"
  ./AnalyzeData $2 $filename $noOfShots $shotNo
done

