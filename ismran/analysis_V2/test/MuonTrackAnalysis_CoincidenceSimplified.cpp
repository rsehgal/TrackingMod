/*
 * MuonTrackAnalysis.cpp
 *
 *  Created on: 01-Mar-2021
 *      Author: rsehgal
 */

#include <iostream>
#include <vector>
#include "PairFinder.h"
#include "Analyzer.h"
#include "Calibration.h"
#include <TApplication.h>
#include <TCanvas.h>
#include "Plotter.h"
#include "includes.hh"
#include "HardwareNomenclature.h"
#include <TSpectrum.h>
#include  "Histograms.h"

int main(int argc, char *argv[]){
	GenerateScintMatrixXYCenters();
	gStyle->SetOptStat(0);
	for(unsigned int i = 0 ; i < vecOfScintXYCenter.size() ; i++){
			if(!(i%9))
				std::cout <<"=================================" << std::endl;
			vecOfScintXYCenter[i].Print();
		}

	//return 0;


	TApplication *fApp = new TApplication("Test", NULL, NULL);
	lite_interface::SingleMuonTrack *smt = new lite_interface::SingleMuonTrack;

	lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
	std::string filename = argv[1];
	int counter = 0;

	std::vector<lite_interface::SingleMuonTrack*> smtVec = GetMuonTracksVector(filename);
	std::cout << "Size of SMTVec : " << smtVec.size() << std::endl;

	TH1F *hist_TopFirst_Bottom_All = new TH1F("Top_First_Bottom_All","Top_First_Bottom_All",9,0,9);
	TH1F *hist_TopMiddle_Bottom_All = new TH1F("Top_Middle_Bottom_All","Top_Middle_Bottom_All",9,0,9);

	unsigned short topLayerIndex = 9;
	unsigned short bottomLayerIndex = 0;
	for(unsigned int i = 0 ; i < smtVec.size() ; i++){
		unsigned int hittedIndex = 10000;
		lite_interface::ScintillatorBar_V2 *scintTop;
		lite_interface::ScintillatorBar_V2 *scintBottom;
		bool check = true;
		check = smtVec[i]->CheckTrackForLayerNum(topLayerIndex,hittedIndex);
		if(check && hittedIndex !=10000){
			scintTop = smtVec[i]->GetScintillator(hittedIndex);
		}
		hittedIndex = 10000;
		check &= smtVec[i]->CheckTrackForLayerNum(bottomLayerIndex,hittedIndex);
		if(check && hittedIndex!=10000){
			scintBottom = smtVec[i]->GetScintillator(hittedIndex);
			if(scintTop->GetBarIndexInLayer()==0){
				hist_TopFirst_Bottom_All->Fill(scintBottom->GetBarIndexInLayer());
			}
			if(scintTop->GetBarIndexInLayer()==4){
				hist_TopMiddle_Bottom_All->Fill(scintBottom->GetBarIndexInLayer());
			}
		}
	}

	TCanvas *can_TopFirst_Bottom_All = new TCanvas("Can_TopFirst_Bottom_All","Can_TopFirst_Bottom_All");
	hist_TopFirst_Bottom_All->Draw();

	
	TCanvas *can_TopMiddle_Bottom_All = new TCanvas("Can_TopMiddle_Bottom_All","Can_TopMiddle_Bottom_All");
	hist_TopMiddle_Bottom_All->Draw();

	std::string subFileName = filename.substr(13);
	TFile *fp = new TFile(("Coincidence_Top_"+std::to_string(topLayerIndex)+"_Bottom_"+std::to_string(bottomLayerIndex)+subFileName).c_str(),"RECREATE");
	fp->cd();
	hist_TopFirst_Bottom_All->Write();
	hist_TopMiddle_Bottom_All->Write();
	fp->Close();
		
	fApp->Run();

	return 0;
}

