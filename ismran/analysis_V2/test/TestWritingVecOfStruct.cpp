/*
 * TestWritingVecOfStruct.cpp
 *
 *  Created on: 28-Oct-2020
 *      Author: rsehgal
 */

#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
#include <TRint.h>
#include <ctime>
#include <iostream>
#include "SingleMuonTrack.h"
#include "ScintillatorBar_V2.h"

using namespace lite_interface;
int main(){




	ScintillatorBar_V2 *scint1 = new ScintillatorBar_V2(1,589,590,1024,512);
	ScintillatorBar_V2 *scint2 = new ScintillatorBar_V2(2,591,592,1024,512);
	ScintillatorBar_V2 *scint3 = new ScintillatorBar_V2(3,593,594,1024,512);
	scint1->Print();


	SingleMuonTrack *smt = new SingleMuonTrack();
	//std::vector<ScintillatorBar_V2*> *smt = new std::vector<ScintillatorBar_V2*>;
	smt->push_back(scint1);
	smt->push_back(scint2);
	smt->push_back(scint3);
	std::cout << "======================" << std::endl;
	smt->Print();

	std::string outputFileName = "test.root";

	gROOT->Reset();
   	TFile *hvdata;// = (TFile*)gROOT->GetListOfFiles()->FindObject("HVData.root");
	std::ifstream f(outputFileName);
	TTree *HVData;
   	//if (!hvdata) {
		std::cout << "File does not exist, hence creating the new one...." << std::endl;
      	hvdata = new TFile(outputFileName.c_str(), "RECREATE");
		HVData = new TTree("TracksTree","TracksTree");
		HVData->Branch("MuonTracks","lite_interface::SingleMuonTrack", &smt);
		//HVData->Branch("MuonTrack","std::vector<ScintillatorBar_V2*>*", &smt);

		HVData->Fill();
	HVData->Write();
	hvdata->Close();

	return 0;
}

