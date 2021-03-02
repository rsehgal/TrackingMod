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

	TApplication *fApp = new TApplication("Test", NULL, NULL);
	lite_interface::SingleMuonTrack *smt = new lite_interface::SingleMuonTrack;

	lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
	std::string filename = argv[1];

	//lite_interface::Analyzer analyzerObj(filename);
	//analyzerObj->ReconstructMuonTrack();

	TH1F *histTraversalTime = new TH1F("Traversal_Time_Hist","Traversal_Time_Hist",100,0,25000);
	TH1F *muonTrackSizeHist = new TH1F("muonTrackSizeHist","muonTrackSizeHist",16,0,16);


	TFile *trackFile = new TFile(filename.c_str(),"READ");
	TTree *trackTree = (TTree*)trackFile->Get("TracksTree");
	trackTree->SetBranchAddress("MuonTracks",&smt);

	Long64_t nentries = trackTree->GetEntries();

	Long64_t nbytes = 0;

	std::vector<lite_interface::SingleMuonTrack*> smtVec;


	TH1F *energySumHist = new TH1F("energySumHist","energySumHist",250,0,250);

	TH1F *multiplictyHist = new TH1F("multiplictyHist","multiplictyHist",15,0,15);

	int counter = 0;
	for (Long64_t i=0; i<nentries;i++) {
		//std::cout << "Fetching Entry : " << i << std::endl;
		nbytes += trackTree->GetEntry(i);
		if(!(i % 100000) && i!=0)
			std::cout << "Processed : " << i << " Tracks ........" << std::endl;

		muonTrackSizeHist->Fill(smt->size());
		//if(i < 5)
		/*while(counter < 5){
			//if(smt->size() >= 8 && (smt->GetMuonTrack())[0]->GetLayerIndex()==(numOfLayers-1) && (smt->GetMuonTrack())[smt->size()-1]->GetLayerIndex()==0 ){
			if(smt->size() >=4){
				counter++;
				smt->Print();
			}
		}*/

		histTraversalTime->Fill(smt->GetTraversalTime());
		//smtVec.push_back(new lite_interface::SingleMuonTrack(*smt));
		energySumHist->Fill(smt->GetEnergySum());
		multiplictyHist->Fill(smt->size());
		smtVec.push_back(new lite_interface::SingleMuonTrack(*smt));
	}

	new TCanvas("MuonTrackSize","MuonTrackSize");
	muonTrackSizeHist->Draw();
	new TCanvas("TraversalTime","TraversalTime");
	histTraversalTime->Draw();
	std::vector<TH1D*> vecOfHist  = lite_interface::PlotEnergyDistributionWithMultiplicity(smtVec);
	new TCanvas("EnergySumWithMultiplicity","EnergySumWithMultiplicity");
	for(unsigned int i = 0; i < vecOfHist.size() ; i++){
		vecOfHist[i]->SetLineColor(i+2);
		//new TCanvas();
		if(vecOfHist[i]->GetEntries()  > 10)
		{

			vecOfHist[i]->Scale(1/vecOfHist[i]->Integral());
			vecOfHist[i]->Draw("same");
		}
	}


	new TCanvas("EnergySumHist","EnergySumHist");
	energySumHist->Draw();

	new TCanvas("MultiplicityHist","MultiplicityHist");
	multiplictyHist->Draw();

	fApp->Run();


	return 0;
}
