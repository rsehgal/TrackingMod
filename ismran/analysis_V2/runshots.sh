#!/bin/sh
noOfShots=$1
#shotNo=0
for shotNo in $(seq 1 $noOfShots)
do
  echo "Currently running shot number $shotNo"
  filename="ProcessedData_"$shotNo".root"
  echo "Output Filename : $filename"
  ./AnalyzeData $2 $filename $noOfShots $shotNo
done

