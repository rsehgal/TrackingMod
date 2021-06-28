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
	printf "${PURPLE}========================================================================${NC}\n"
	printf "${PURPLE}=================== Processing Bar : $eachBarName ======================${NC}\n"
	printf "${PURPLE}========================================================================${NC}\n"
	python saveClassModel.py $dir/$eachBarName/training_$eachBarName.txt 5 $eachBarName
	mv model_Classification_neighbors_5_$eachBarName.sav $dir/$eachBarName/
done
