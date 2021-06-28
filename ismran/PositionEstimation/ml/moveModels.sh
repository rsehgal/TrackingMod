#!/bin/sh
RED='\033[0;31m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m'
dir="../datasets"
#listOfBarNames="PS89_SF873" #`ls $dir | grep PS`
listOfBarNames=`ls $dir | grep PS`
for eachBarName in $listOfBarNames
do
	mv model_Regression_$eachBarName*.sav ../datasets/$eachBarName
	#if [[ -d $dir/$eachBarName ]]
	#then
	#	mv $dir/$eachBarName ../datasets/
	#fi
done

#for (( i=10; i <=90; i++))
#do
#	mv model_Regression_PS$i* ../datasets/PS$1*
#done
