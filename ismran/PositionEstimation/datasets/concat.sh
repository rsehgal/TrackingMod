#!/bin/sh
barname=$1
datatype=$2
inputfilenames=`ls $datatype/*.csv`
outfilename=$datatype"_"$barname".csv"
allfilename=""
for eachfile in $inputfilenames
do
	echo "Processing file : $eachfile ....."
	allfilename=$allfilename" "$eachfile
done
cat $allfilename > $datatype/$outfilename
#echo $allfilename
