#!/bin/sh
material=$2
rm -rf Sim_$material

#creating directory
cp -rf build Sim_$material
cd Sim_$material

rm -rf Tpr.txt Fpr.txt
rm -rf SimData
mkdir SimData
echo "numOfMaterials 1" > Tpr.txt
echo $material >> Tpr.txt
echo "numberOfSteps 6" >> Tpr.txt
echo "5000. 10000. 20000. 30000. 50000. 100000." >> Tpr.txt
cp Tpr.txt Fpr.txt

for numOfEvents in 5000 10000 20000 30000 50000 100000
do
	filename=$material"_"$numOfEvents".txt"

	#Creating testMapping.txt
       	sed "s/Material/$material/g" testMapping_Template.PAR | sed "s/NumOfEvents/$numOfEvents/g" > $filename
        mv $filename testMapping.txt

	#Deleting file that got created in last Run
       	rm -rf EventHelper* *RunHelper* Poca* 1234.root Fitted.root SampledFitted.root Exact.root Track* Hits.txt run.txt Stats* Actual* Hodoscope.gdml

	#Runing the Simulation
       	./mainNew $1 

	mv TrackExact.txt "SimData/TrackExact_"$filename
done
echo "" >> Tpr.txt
echo "" >> Fpr.txt

