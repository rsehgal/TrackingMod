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

	//lite_interface::Analyzer analyzerObj(filename);
	//analyzerObj->ReconstructMuonTrack();

	TFile *trackFile = new TFile(filename.c_str(),"READ");
	TTree *trackTree = (TTree*)trackFile->Get("TracksTree");
	trackTree->SetBranchAddress("MuonTracks",&smt);

	Long64_t nentries = trackTree->GetEntries();

	Long64_t nbytes = 0;

	std::vector<lite_interface::SingleMuonTrack*> smtVec;

	int counter = 0;
	for (Long64_t i=0; i<nentries;i++) {

		//std::cout << "Fetching Entry : " << i << std::endl;
		nbytes += trackTree->GetEntry(i);
		if(!(i % 100000) && i!=0)
			std::cout << "Processed : " << i << " Tracks ........" << std::endl;

		//if(smt->SingleHitInEachLayer())
		smtVec.push_back(new lite_interface::SingleMuonTrack(*smt));
	}

	std::cout << "Size of SMTVec : " << smtVec.size() << std::endl;
	//Concerntrating on layer number 9, 7, 0
	bool layer9Found = false;
	bool layer7Found = false;
	bool layer0Found = false;
	TH1F *layer9TimingHist = new TH1F("Layer_9_Timing_Hist","Layer_9_Timing_Hist",100,-20000,20000);
	TH1F *layer7TimingHist = new TH1F("Layer_7_Timing_Hist","Layer_7_Timing_Hist",100,-20000,20000);
	TH1F *layer0TimingHist = new TH1F("Layer_0_Timing_Hist","Layer_0_Timing_Hist",100,-20000,20000);

	Long_t delTCorr_Layer9=0;
	Long_t delTCorr_Layer7=0;
	Long_t delTCorr_Layer0=0;

	unsigned int counterSingleHitInEachLayer = 0;
	unsigned int counterSingleHitInEachLayerAndHittedIn3Layers = 0;
	Long_t timeWinLower = -1000;
	Long_t timeWinUpper = 1000;
	unsigned int barIndexInLayer = 4;
	for(unsigned int i = 0 ; i < smtVec.size() ; i++){
		layer9Found = false;
		layer7Found = false;
		layer0Found = false;
		if(smtVec[i]->SingleHitInEachLayer())
		{
			counterSingleHitInEachLayer++;
			//Long_t belowLayerDelTCorr=500000.;
			for(unsigned int j = 0 ; j < smtVec[i]->size() ; j++){
				std::vector<lite_interface::ScintillatorBar_V2*> smt = smtVec[i]->GetMuonTrack();
				lite_interface::ScintillatorBar_V2 *scint = smt[j];
				if(scint->GetLayerIndex() == 9){
					layer9Found = true;
					//Long_t delTCorr
					delTCorr_Layer9 = scint->GetDelTCorrected();// /1000.;
					layer9Found &= (scint->GetBarIndexInLayer()==barIndexInLayer);
					layer9Found &= (delTCorr_Layer9 > timeWinLower && delTCorr_Layer9 < timeWinUpper);
				}
				if(scint->GetLayerIndex() == 7){
					layer7Found = true;
					//Long_t delTCorr
					delTCorr_Layer7 = scint->GetDelTCorrected();// /1000.;
					layer7Found &= (scint->GetBarIndexInLayer()==barIndexInLayer);
					layer7Found &= (delTCorr_Layer7 > timeWinLower && delTCorr_Layer7 < timeWinUpper);
				}
				if(scint->GetLayerIndex() == 0){
					layer0Found = true;
					layer0Found &= (scint->GetBarIndexInLayer()==barIndexInLayer);
					//Long_t delTCorr
					delTCorr_Layer0 = scint->GetDelTCorrected();// /1000.;
					//layer0Found &= (delTCorr > -500 && delTCorr < 500);
				}
			}

			if(layer9Found && layer7Found && layer0Found){
				counterSingleHitInEachLayerAndHittedIn3Layers++;
				layer9TimingHist->Fill(delTCorr_Layer9);
				layer7TimingHist->Fill(delTCorr_Layer7);
				layer0TimingHist->Fill(delTCorr_Layer0);
			}


		}
	}
	std::cout << "Number of Tracks with Single hit in each layer : " << counterSingleHitInEachLayer << std::endl;
	std::cout << "Number of Tracks with Single hit in each layer and hitted in 3Layers : " << counterSingleHitInEachLayerAndHittedIn3Layers << std::endl;
	new TCanvas();
	layer9TimingHist->Draw();

	new TCanvas();
	layer7TimingHist->Draw();

	new TCanvas();
	layer0TimingHist->Draw();
	fApp->Run();


	return 0;
}
