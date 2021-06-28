#!/bin/sh
dir="../datasets"
listOfBarNames=`ls $dir | grep PS`
for eachBarName in $listOfBarNames
do
	echo "Processing BAR : "$eachBarName
	if [[ -d $dir/$eachBarName ]]
	then
		listOfFiles=`ls $dir/$eachBarName`
		for eachfile in $listOfFiles
		do
			echo "Processing file : $dir/$eachBarName/$eachfile"
			sed '/inf/d' -i $dir/$eachBarName/$eachfile
			sed '/nan/d' -i $dir/$eachBarName/$eachfile
		done
	fi
done
