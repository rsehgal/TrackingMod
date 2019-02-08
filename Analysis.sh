#!/bin/sh
analType=$1
material=$2
dataDir=Sim_$material

#creating directory
rm -rf $analType
cp -rf build $analType

cd $analType
if [ "$analType" = "Tpr" ]; then
rm -rf TprData
mkdir TprData
fi

rm -rf Par.txt Tpr.txt filteredTpr.txt
echo "numOfMaterials 1" > $analType.txt
echo $material >> $analType.txt
echo "numberOfSteps 6" >> $analyType.txt
echo "5000. 10000. 20000. 30000. 50000. 100000." >> $analType.txt
for numOfEvents in 5000 10000 20000 30000 50000 100000
do
	filename=$material"_"$numOfEvents".txt"

	#Creating testMapping.txt
       	sed "s/Material/$material/g" testMapping_Template.PAR | sed "s/NumOfEvents/$numOfEvents/g" > $filename
        mv $filename testMapping.txt

	#Deleting file that got created in last Run
       	rm -rf EventHelper* *RunHelper* Poca* 1234.root Fitted.root SampledFitted.root Exact.root Track* Hits.txt run.txt Stats* Actual* Hodoscope.gdml


	cp  "../$dataDir/SimData/TrackExact_"$filename TrackExact.txt
	#Running Offline Analysis
	./OfflineRunEventHelper Exact
	if [ "$analType" = "Tpr" ]; then
		mv TrackExact.txt "TprData/TrackExact_"$filename
	        mv Exact.root "TprData/Exact_"$filename".root"
		./TestTpr PocaPtRunHelper-Exact.txt 0
		./TestTpr filteredPocaPtRunHelper-Exact.txt 1
	fi

done
if [ "$analType" = "Tpr" ]; then
echo "" >> Tpr.txt
echo "" >> Fpr.txt
fi
