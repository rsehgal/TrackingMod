#!/bin/sh
material=$2
rm -rf Tpr_$material

#creating directory
cp -rf build Tpr_$material
cd Tpr_$material

rm -rf Tpr.txt Fpr.txt
rm -rf TprData
mkdir TprData
echo "numOfMaterials 1" > Tpr.txt
echo "Pb" >> Tpr.txt
echo "numberOfSteps 6" >> Tpr.txt
echo "5000. 10000. 20000. 30000. 50000. 100000." >> Tpr.txt
cp Tpr.txt Fpr.txt
#numOfEvents=10000
#for material in Al Fe Pb U
#do
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

	#Running Offline Analysis
       	./OfflineRunEventHelper Exact
	./TestTpr PocaPtRunHelper-Exact.txt 0
	./TestTpr filteredPocaPtRunHelper-Exact.txt 1

	mv TrackExact.txt "TprData/TrackExact_"$filename
	mv Exact.root "TprData/Exact_"$filename".root"


		#
done
echo "" >> Tpr.txt
echo "" >> Fpr.txt



#done

#Creating the plots
#./PlotPAR
