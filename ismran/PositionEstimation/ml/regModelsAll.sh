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
	python saveRegModel.py ../datasets/$eachBarName/$eachBarName\_-45_training.txt $eachBarName -45 4
	python saveRegModel.py ../datasets/$eachBarName/$eachBarName\_-40_training.txt $eachBarName -40 4
	python saveRegModel.py ../datasets/$eachBarName/$eachBarName\_-30_training.txt $eachBarName -30 4
	python saveRegModel.py ../datasets/$eachBarName/$eachBarName\_-20_training.txt $eachBarName -20 4
	python saveRegModel.py ../datasets/$eachBarName/$eachBarName\_-10_training.txt $eachBarName -10 4
	python saveRegModel.py ../datasets/$eachBarName/$eachBarName\_+0_training.txt $eachBarName +0 4
	python saveRegModel.py ../datasets/$eachBarName/$eachBarName\_+10_training.txt $eachBarName +10 4
	python saveRegModel.py ../datasets/$eachBarName/$eachBarName\_+20_training.txt $eachBarName +20 4
	python saveRegModel.py ../datasets/$eachBarName/$eachBarName\_+30_training.txt $eachBarName +30 4
	python saveRegModel.py ../datasets/$eachBarName/$eachBarName\_+40_training.txt $eachBarName +40 4
	python saveRegModel.py ../datasets/$eachBarName/$eachBarName\_+45_training.txt $eachBarName +45 4
	mv model_Regression*.sav $dir/$eachBarName/
done
