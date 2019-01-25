#!/bin/sh
rm -rf Par.txt
echo "numOfMaterials 4" > Par.txt
echo "Al Fe Pb U" >> Par.txt
echo "numOfSteps 6" >> Par.txt
echo "5000. 10000. 20000. 30000. 50000. 100000." >> Par.txt
numOfEvents=10000
for material in Al Fe Pb U
do
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


done

#Creating the plots
#./PlotPAR
