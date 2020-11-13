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
#include "Plotter.h"
#include <vector>
#include<iterator>
#include <TH1F.h>
#include <TApplication.h>
#include "Calibration.h"
#include "HardwareNomenclature.h"
#include <TGraphErrors.h>
int main(int argc,char *argv[]){

	TApplication *fApp = new TApplication("Test", NULL, NULL);
	std::string outputFileName=argv[1];
	TFile *hitPointFile = new TFile(outputFileName.c_str(),"READ");
	TTree *hitPointVecTree = (TTree*)hitPointFile->Get("HitPointVecTree");
	std::vector<lite_interface::Point3D*> *vecOfPoint3D = 0;
	hitPointVecTree->SetBranchAddress("HitPointVec",&vecOfPoint3D);

	Long64_t nentries = hitPointVecTree->GetEntries();

	Long64_t nbytes = 0;

	unsigned int count = 0;
	for (Long64_t i=0; i<nentries;i++) {
		nbytes += hitPointVecTree->GetEntry(i);
		if(i < 50){
			TCanvas *can = new TCanvas();
			can->Divide(2,1);
			can->cd(1);
			DrawGrid("Muon Track in XY plane; X axis ; Y axis", 9, 9);
			PlotMuonTrackXY(*vecOfPoint3D)->Draw("p");

			can->cd(2);
			DrawGrid("Muon Track in ZY plane; Z axis ; Y axis", 9, 9);
			PlotMuonTrackZY(*vecOfPoint3D)->Draw("p");
		}
	}

	fApp->Run();

	return 0;
}



