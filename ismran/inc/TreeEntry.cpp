/*
 * TreeEntry.cpp
 *
 *  Created on: 21-Aug-2020
 *      Author: rsehgal
 */

#include "TreeEntry.h"
#include <vector>

using TreeEntryVector = std::vector<TreeEntry>;

void PrintEntryVector(TreeEntryVector treeEntVec){
	//int lenToPrint=treeEntVec.size();
	unsigned int lenToPrint=10;
	for(unsigned int i = 0 ; i < lenToPrint ; i++){
		treeEntVec[i].Print();
	}

	std::cout << "Printing timediff : " << treeEntVec[7].tstamp-treeEntVec[0].tstamp << std::endl;
}

void PrintScintillatorVector(std::vector<ScintillatorBar*> scintBarVector){
	//unsigned int n = scintBarVector.size();
	std::cout << "ScintBarVector size from  PrintScintillatorVector : " << scintBarVector.size() << std::endl;
	unsigned int n=20;
	std::cout << "======= Printing " << n <<" Scintillators entries===========" << std::endl;
	std::cout << "BarName" << " , " << "qlongNear" << " , " << "qlongFar" << " , " << "qlongMean" << " , "
					  << "tstampNear"  << " , " << "tstampFar" << " , " << "tsmallTimeStamp" << " , " << "deltaTstamp" << " , " << "ClockTime" << std::endl;
	for(unsigned int i=0 ; i < n ; i++){
		scintBarVector[i]->Print();
	}
}

void PrintMuonTrack(std::vector<ScintillatorBar*> singleMuonTrack){
	//unsigned int n = scintBarVector.size();

	for(unsigned int i=0 ; i < singleMuonTrack.size() ; i++){
		singleMuonTrack[i]->Print();
	}
}

void PrintMuonTrackVector(std::vector< std::vector<ScintillatorBar*> > muonTrackVec){
	//unsigned int muonTrackVecLength = muonTrackVec.size();
	unsigned int muonTrackVecLength = 20;
	for(unsigned int i = 0 ; i < muonTrackVecLength ; i++){
		std::cout << "====== Print Muon Track : " << i << " : Num of Layers Hitted : " << muonTrackVec[i].size()  << " ======" << std::endl;
		unsigned int muonHitLength = muonTrackVec[i].size();
		for(unsigned int j = 0 ; j < muonHitLength ; j++){
			muonTrackVec[i][j]->Print();
		}
	}
}

void PrintMuonTrackVectorAllLayers(std::vector< std::vector<ScintillatorBar*> > muonTrackVec){
	//unsigned int muonTrackVecLength = muonTrackVec.size();
	unsigned int muonTrackVecLength = 20;
	int i = 0 ;
	//for(unsigned int i = 0 ; i < muonTrackVecLength ; i++){
	while(muonTrackVecLength){


		if (muonTrackVec[i].size() >= numOfLayers) {
			muonTrackVecLength--;
			std::cout << "====== Print Muon Track : " << i << " : Num of Layers Hitted : " << muonTrackVec[i].size()  << " ======" << std::endl;

			//Sorting each track by barIndex,
			//std::sort(muonTrackVec[i].begin(),muonTrackVec[i].end(),CompareBarIndexInScintillator);

			unsigned int muonHitLength = muonTrackVec[i].size();
			for (unsigned int j = 0; j < muonHitLength; j++) {
				muonTrackVec[i][j]->Print();
			}

		}
		i++;
	}
}

/*
 * Function to detect the Muon Hit based on the energy info
 */
std::vector<ScintillatorBar*> DetectMuonHits(TreeEntryVector treeEntVec){
	std::cout <<"Trying to detect Muon Hits.........." << std::endl;
	std::vector<ScintillatorBar*> scintBarVec;
	int qstart = 4000;
	int qend = 18000;
	for(unsigned int i = 0 ; i < treeEntVec.size() ; ){
		std::string barId="";
		unsigned int barIndex=0;
		float qmean   = sqrt(treeEntVec[i].qlong*treeEntVec[i+1].qlong);
		ScintillatorBar *newScint ;
		if(treeEntVec[i].brch < treeEntVec[i+1].brch){
			newScint = new ScintillatorBar(treeEntVec[i].brch,treeEntVec[i+1].brch,
  	  	  	  	  	  	  	  	  	       treeEntVec[i].tstamp,treeEntVec[i+1].tstamp,
										   treeEntVec[i].qlong,treeEntVec[i+1].qlong,treeEntVec[i].time);
		}
		else{
			newScint = new ScintillatorBar(treeEntVec[i+1].brch,treeEntVec[i].brch,
					  	  	  	  	  	   treeEntVec[i+1].tstamp,treeEntVec[i].tstamp,
										   treeEntVec[i+1].qlong,treeEntVec[i].qlong,treeEntVec[i+1].time);
		}

		if(newScint->validPair && newScint->barIndex < numOfLayers*numOfBarsInEachLayer){
			scintBarVec.push_back(newScint);
			barIndex=newScint->barIndex;
			//vecOfEnergyHist[barIndex]->Fill(qmean);
		}
		i=i+2;
	}
	std::cout << "Length of Detected Muon Hits : " << scintBarVec.size() << std::endl;
	return scintBarVec;
}

bool CompareTimestampScintillator(ScintillatorBar *i1, ScintillatorBar *i2)
{
	//return (i1->tstampNear < i2->tstampNear);
	return (i1->tsmallTimeStamp < i2->tsmallTimeStamp);

}

bool CompareBarIndexInScintillator(ScintillatorBar *i1, ScintillatorBar *i2)
{
	//return (i1->tstampNear < i2->tstampNear);
	return (i1->barIndex > i2->barIndex);
}

/*
 * Function to fit straight line to muon hits
 */
double fitl(double *x, double *par){
	return par[0]+par[1]*x[0];
}

std::vector< std::vector<ScintillatorBar*> >  DetectMuonTracks(std::vector<ScintillatorBar*> muonHitVector){
	unsigned int hitLength = muonHitVector.size();
	std::cout << "HitLength from DetectMuonTracks : " << hitLength << std::endl;
	std::vector < std::vector<ScintillatorBar*> > muonTrackVec;
	std::vector<ScintillatorBar*> singleMuonTrack;
	singleMuonTrack.push_back(muonHitVector[0]);
	unsigned int hitInAllLayersCounter = 0;
	for (unsigned int i = 1; i < hitLength; i++) {

			if ((muonHitVector[i]->tsmallTimeStamp
				- muonHitVector[i - 1]->tsmallTimeStamp) < 20000) {
			//Within 20ns window
			singleMuonTrack.push_back(muonHitVector[i]);
		} else {
			//Outside 20ns window
			//Sorting the single muon track by BarIndex (in DESCENDING order), to form a logical track
			std::sort(singleMuonTrack.begin(),singleMuonTrack.end(),CompareBarIndexInScintillator);
			muonTrackVec.push_back(singleMuonTrack);
			//Just counting the number of muon where all the layer detected the muon
			//std::cout << "Track Finshed : Length : "  << singleMuonTrack.size() << std::endl;
			if (singleMuonTrack.size() == numOfLayers)
				hitInAllLayersCounter++;
			singleMuonTrack.clear();
			singleMuonTrack.push_back(muonHitVector[i]);
		}
	}

	std::cout
			<< "Number of Muon Tracks where all the layers detected the muon : "
			<< hitInAllLayersCounter << std::endl;
	return muonTrackVec;
}

std::vector<TreeEntry> CheckPairs(TreeEntryVector treeEntVec){
	TreeEntryVector smallTSEntVec;
	TreeEntryVector pairedEntVec;
	TreeEntryVector unpairedEntVec;
	int counter=110;
	for(unsigned int i = 0 ; i < treeEntVec.size() ; ){

		//treeEntVec[i].Print();

		if(abs(treeEntVec[i].brch - treeEntVec[i+1].brch) > 1){
			unpairedEntVec.push_back(treeEntVec[i]);


			if(0){
				//std::cout << "Unpaired entry found at index : " << i << std::endl;
				counter--;
				treeEntVec[i].Print();
				if(!counter)
					break;

			}

			i++;

		}else{
			pairedEntVec.push_back(treeEntVec[i]);
			pairedEntVec.push_back(treeEntVec[i+1]);
			if(treeEntVec[i].tstamp > treeEntVec[i+1].tstamp)
				smallTSEntVec.push_back(treeEntVec[i+1]);
			else
				smallTSEntVec.push_back(treeEntVec[i]);

			i = i+2;
		}

	}
	std::cout << "Length of Unpaired Entries Vector : " << unpairedEntVec.size() << std::endl;
	std::cout << "Length of Paired Entries Vector : " << pairedEntVec.size() << std::endl;
	std::cout << "Length of SmallTS Entries Vector : " << smallTSEntVec.size() << std::endl;
	std::cout << "Percentage of Unpaired Entries : " << (unpairedEntVec.size()*1.0/treeEntVec.size())*100 <<" %" << std::endl;

	std::cout <<"================== Printing few entries of Paired Entry Vec==================" << std::endl;
	PrintEntryVector(pairedEntVec);
	return pairedEntVec;

	/*
	std::cout <<"========== Trying to Sort SmallTS Entry Vector ==============" << std::endl;
		std::sort(smallTSEntVec.begin(),smallTSEntVec.end(),CompareTimestamp);
	std::cout <<"===================== Sorting Done........ ==================" << std::endl;

	std::cout <<"================== Printing few entries of SmallTS Entry Vec==================" << std::endl;
	PrintEntryVector(smallTSEntVec);

	//std::cout <<"================== Printing few entries ==================" << std::endl;
	//PrintEntryVector(smallTSEntVec);

	PlotHistOfTSDiff(pairedEntVec);
	//PlotHistOfTSDiff_AllBars(pairedEntVec);

	//Working
	//PlotHistOfQ(pairedEntVec);

	std::vector<ScintillatorBar*> scintBarVec = DetectMuonHits(pairedEntVec);
	PrintScintillatorVector(scintBarVec);
	PlotHistOfTSDiff_AllBars_V2(scintBarVec);

	std::cout <<"========== Trying to Sort ScintillatorBar Vector ==============" << std::endl;
	std::sort(scintBarVec.begin(),scintBarVec.end(),CompareTimestampScintillator);
	std::cout <<"===================== Sorting Done........ ==================" << std::endl;
	PrintScintillatorVector(scintBarVec);

	std::cout << std::endl;
	std::cout <<"==========================================================================================" << std::endl;
	std::cout <<"@@@@@@@@@@@@@@@@@@@@@@@@@@@ Trying to Get Vector of Muon tracks @@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
	std::cout <<"==========================================================================================" << std::endl;
	std::vector<std::vector<ScintillatorBar*>> muonTrackVec = DetectMuonTracks(scintBarVec);
	std::cout <<"=== Trying to get muon tracks where all the layer detecte the muon sorted by barIndex ====" << std::endl;
	std::vector<std::vector<ScintillatorBar*>> muonTrackVecAllLayersSorted = SortMuonTracksByBarIndex(muonTrackVec);
	PrintMuonTrackVector(muonTrackVecAllLayersSorted);
	FillCoincidenceHist(muonTrackVecAllLayersSorted);
	*/
	//FillTestPositionHist(muonTrackVecAllLayersSorted);
	//PrintMuonTrackVector(muonTrackVec);

	//std::sort(pairedEntVec.begin(),pairedEntVec.end(),CompareTimestamp);
	//PrintEntryVector(pairedEntVec);


	//PlotHistOfTS(smallTSEntVec);
	//return;
}



