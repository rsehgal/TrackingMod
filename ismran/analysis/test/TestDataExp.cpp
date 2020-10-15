/*
 * TestDataExp.cpp
 *
 *  Created on: 12-Oct-2020
 *      Author: rsehgal
 */

#include "DataTree.h"
#include "HardwareNomenclature.h"
#include "ScintillatorBar_V2.h"
#include "Analyzer_V2.h"
#include <TApplication.h>
#include "SingleMuonTrack.h"
#include "Histograms.h"
#include "HelperFunctions.h"
#include "Calibration.h"
#include "colors.h"
#include "FittedTracks.h"
int main(){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	bool verbose = false;
	Calibration *cb = new Calibration("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib.root");
	//Analyzer_V2 av("/home/rsehgal/BackBoneSoftwares/ismranData/ISMRAN_81bars_Th10All_CosmicRun_15hrs42mins_26Aug2020_2.root",cb);
	Analyzer_V2 av("/home/rsehgal/BackBoneSoftwares/ismranData/ISMRAN_81bars_Th10All_CosmicRun_09hrs34mins_02Sep2020_0.root",cb,0,false);
	std::vector< SingleMuonTrack* > muonTrackVec = av.ReconstrutTrack_V2();

	av.PlotHistOfNumOfMuonHitsInMuonTracks_V2(muonTrackVec);
	av.CalculateTotalEnergyDepositionForMuonTracks(muonTrackVec);

	HistInitializer();
	av.FillCoincidenceHist_V2(muonTrackVec);
	av.PlotCoincidenceCountGraph();
	av.PlotEnergyDistributionWithMultiplicity(muonTrackVec,0);

	/* Dummy test
	 * Actually this will go to a function in Analyzer
	 *
	 * Uncomment the below block to plot some fitted tracks.
	 */

	/*unsigned int numOfTracksToPlot = 10;
	unsigned int counter = 0 ;
	for(unsigned int i = 0 ; i < muonTrackVec.size() ;i++){

		if(muonTrackVec[i]->size() == numOfLayers){
			counter++;
			muonTrackVec[i]->CreateFittedMuonTrack(1);
			muonTrackVec[i]->fFittedTrack->PlotTrack();
			if(counter==numOfTracksToPlot)
				break;
		}
	}*/

	for(unsigned int i = 0 ; i < muonTrackVec.size() ;i++){
		if(muonTrackVec[i]->size() >= 5){
			muonTrackVec[i]->CreateFittedMuonTrack(1);
			av.fittedMuonTracks.push_back(muonTrackVec[i]->fFittedTrack->GetFittedTrack());
		}
	}

	av.PlotZenithAngle();

	std::cout << "EXECUTED THE FINAL INSTRUCTION..............." << std::endl;

	fApp->Run();
}


