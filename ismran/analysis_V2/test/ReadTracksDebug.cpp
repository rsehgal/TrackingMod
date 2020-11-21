/*
 * ReadTracksDebug.cpp
 *
 *  Created on: 21-Nov-2020
 *      Author: rsehgal
 */

#include "SingleMuonTrack.h"
#include <iostream>
#include <TTree.h>
#include <TFile.h>
#include "ScintillatorBar_V2.h"
#include "Plotter.h"
#include <vector>
#include<iterator>
#include <TH1F.h>
#include <TApplication.h>
#include "Calibration.h"
#include "HardwareNomenclature.h"
#include <TGraphErrors.h>
#include "Analyzer.h"
int main(int argc,char *argv[]){
	lite_interface::IsSimulation = true;
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	GenerateScintMatrixXYCenters();
	lite_interface::SingleMuonTrack *smt = new lite_interface::SingleMuonTrack;
	//std::vector<lite_interface::ScintillatorBar_V2*> fSingleMuonTrack;

	//std::vector<lite_interface::ScintillatorBar_V2*> *smt = new std::vector<lite_interface::ScintillatorBar_V2*>;
	//lite_interface::Calibration *calib = lite_interface::Calibration::instance("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib.root");
	std::string inputFileName=argv[1];
	TFile *trackFile = new TFile(inputFileName.c_str(),"READ");
	TTree *trackTree = (TTree*)trackFile->Get("TracksTree");
	trackTree->SetBranchAddress("MuonTracks",&smt);
	//trackTree->SetBranchAddress("fSingleMuonTrack",&fSingleMuonTrack);

	Long64_t nentries = trackTree->GetEntries();

	std::cout << "Num Of Entries : "  << nentries << std::endl;

	Long64_t nbytes = 0;
	for (Long64_t i=0; i<nentries;i++) {
		std::cout << "===============================" << std::endl;
			nbytes += trackTree->GetEntry(i);
			smt->Print();
			/*std::cout << "===============================" << std::endl;
			for(unsigned int j = 0 ; j < fSingleMuonTrack.size() ; j++){
				fSingleMuonTrack[j]->Print();
			}*/
	}

	return 0;

}
