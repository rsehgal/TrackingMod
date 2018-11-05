#!/bin/sh
rm -rf Par.txt
for material in Al Fe #Pb U
do
	for numOfEvents in 1000 5000 #10000 15000 20000 30000 50000 100000
	do
		#echo "NumOfEvents : "$numOfEvents" :: Material : "$material
		filename=$material"_"$numOfEvents".txt"
		#echo $filename
		sed "s/Material/$material/g" testMapping_Template.PAR | sed "s/NumOfEvents/$numOfEvents/g" > $filename
		mv $filename testMapping.txt
		rm -rf EventHelper* *RunHelper* Poca* 1234.root Fitted.root SampledFitted.root Exact.root Track* Hits.txt run.txt Stats* Actual* Hodoscope.gdml
		./mainNew $1 
		./OfflineRunEventHelper Exact	
	done
	echo "" >> Par.txt

done
