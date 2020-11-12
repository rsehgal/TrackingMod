/*
 * ReadTracks.cpp
 *
 *  Created on: 10-Nov-2020
 *      Author: rsehgal
 */

#include "SingleMuonTrack.h"
#include <iostream>
#include <TTree.h>
#include <TFile.h>
#include "ScintillatorBar_V2.h"

int main(int argc,char *argv[]){
	lite_interface::SingleMuonTrack *smt = 0;
	//std::vector<lite_interface::ScintillatorBar_V2*> *smt = new std::vector<lite_interface::ScintillatorBar_V2*>;
	std::string outputFileName=argv[1];
	TFile *trackFile = new TFile(outputFileName.c_str(),"READ");
	TTree *trackTree = (TTree*)trackFile->Get("TracksTree");
	trackTree->SetBranchAddress("MuonTracks",&smt);

	Long64_t nentries = trackTree->GetEntries();

	Long64_t nbytes = 0;
	if(nentries > 5)
		nentries = 5;
	for (Long64_t i=0; i<nentries;i++) {
		nbytes += trackTree->GetEntry(i);
		std::cout << "============ Event Num : " << i << " ============" << std::endl;
		smt->Print();
		//(*smt)[0]->Print();
		//std::cout << "Vector Size : " << (smt->GetMuonTrack()).size() << std::endl;
		//smt->Print();
		//std::vector<lite_interface::ScintillatorBar_V2*> scintVec = *smt;//->GetMuonTrack();
		/*for(unsigned int j = 0 ; j < scintVec.size() ; j++){
			std::cout << "Bar Index : " << scintVec[j]->GetBarIndex() << std::endl;
		}*/
	}

	return 0;
}



