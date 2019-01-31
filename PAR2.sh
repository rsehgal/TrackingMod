#!/bin/sh

#Shell script to run in batch mode
#usage : ./PAR2.sh cry.mac  <Material>
#        ./PAR2.sh cry.mac Fe

material=$2

#deleting the directory in case it exists
rm -rf $material

#creating directory
cp -rf build $material
cd $material
rm -rf Par.txt

echo "numOfMaterials 1" > Par.txt
echo $material >> Par.txt
echo "numOfSteps 6" >> Par.txt
echo "5000. 10000. 20000. 30000. 50000. 100000." >> Par.txt
numOfEvents=10000
for numOfEvents in 5000 10000 20000 30000 50000 100000
do
	filename=$material"_"$numOfEvents".txt"

	#Creating testMapping.txt
       	sed "s/Material/$material/g" testMapping_Template.PAR | sed "s/NumOfEvents/$numOfEvents/g" > $filename
       	cp $filename testMapping.txt

	#Deleting file that got created in last Run
       	rm -rf EventHelper* *RunHelper* Poca* 1234.root Fitted.root SampledFitted.root Exact.root Track* Hits.txt run.txt Stats* Actual* Hodoscope.gdml

	#Runing the Simulation
       	./mainNew $1 

	#Running Offline Analysis
       	./OfflineRunEventHelper Exact
done
echo "" >> Par.txt



#Creating the plots
#./PlotPAR
