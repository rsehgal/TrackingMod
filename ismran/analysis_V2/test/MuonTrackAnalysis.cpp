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

		if(smt->size() > 7)
			histTraversalTime->Fill(smt->GetTraversalTime());
		//smtVec.push_back(new lite_interface::SingleMuonTrack(*smt));
		energySumHist->Fill(smt->GetEnergySum());
		multiplictyHist->Fill(smt->size());
		//if(smt->SingleHitInEachLayer())
			smtVec.push_back(new lite_interface::SingleMuonTrack(*smt));
	}

	std::cout << "Size of SMTVec : " << smtVec.size() << std::endl;
	new TCanvas("MuonTrackSize","MuonTrackSize");
	muonTrackSizeHist->Draw();
	new TCanvas("TraversalTime","TraversalTime");
	histTraversalTime->Draw();
	std::vector<TH1D*> vecOfHist  = lite_interface::PlotEnergyDistributionWithMultiplicity(smtVec);
	std::cout << "Size of SMTVec2 : " << smtVec.size() << std::endl;
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


	/*unsigned int Counter = 0;
	for(unsigned int i = 0 ; i < smtVec.size() ; i++){
		Counter++;
		//if(!i)
			//std::cout << "Counter : " << Counter << std::endl;
		if(!(Counter%1000000))
		std::cout << "Counter : " << Counter << std::endl;
	}*/

	unsigned int genuineTrackCounter = 0 ;
	TH1F *energySumHistUsefulTracks = new TH1F("energySumHist_UsefulTracks","energySumHist_UsefulTracks",250,0,250);
	std::vector<lite_interface::SingleMuonTrack*> smtVecUsefulTracks;
	for(unsigned int i = 0 ; i < smtVec.size() ; i++){

		if(smtVec[i]->size() > 7)
		{
		//std::cout <<"Size of MuonTrack : " << i <<" : " << smtVec[i]->size() << std::endl;
		//For Debugging
		/*std::cout <<"=================== Track Num : " << i << " : Track Size : " << smtVec[i]->size() << " : Energy Sum : " << smtVec[i]->GetEnergySum() << " ============================" << std::endl;
		for(unsigned int j = 0 ; j < smtVec[i]->size() ; j++){

			//if(smtVec[i]->NumOfHitsInLayer(7)==1 )
			if(smtVec[i]->SingleHitInEachLayer())
			{
				//if((smtVec[i]->GetMuonTrack())[j]->GetLayerIndex() == 6 || (smtVec[i]->GetMuonTrack())[j]->GetLayerIndex() == 7)
				{

					std::cout << "Energy : " << (smtVec[i]->GetMuonTrack())[j]->GetQMeanCorrected() << " : "; (smtVec[i]->GetMuonTrack())[j]->EstimateHitPosition_Param()->Print();
				}
			}

		}*/

		//std::cout << "++++++++++++++++++++++++++++++++ Incoming Track ++++++++++++++++++++++++++++++++++" << std::endl;
		lite_interface::SingleMuonTrack *trk = smtVec[i]->GetIncomingTrack();
		if(trk){
			genuineTrackCounter++;
			smtVecUsefulTracks.push_back(new lite_interface::SingleMuonTrack(*(smtVec[i])) );
			energySumHistUsefulTracks->Fill(smtVec[i]->GetEnergySum());

		//For Debugging
		/*for(unsigned int j = 0 ; j < trk->size() ; j++){
			std::cout << "Energy : " << (trk->GetMuonTrack())[j]->GetQMeanCorrected() << " : "; (trk->GetMuonTrack())[j]->EstimateHitPosition_Param()->Print();
		}*/
		}
	}
	}


	TH1F *stripProfile7 = lite_interface::PlotStripProfileOfLayer(smtVec,7);
	TH1F *stripProfile6 = lite_interface::PlotStripProfileOfLayer(smtVec,6);
	TH1F *stripProfile5 = lite_interface::PlotStripProfileOfLayer(smtVec,5);

	/*TH1F *stripProfile7 = lite_interface::PlotStripProfileOfLayer(smtVecUsefulTracks,7);
		TH1F *stripProfile6 = lite_interface::PlotStripProfileOfLayer(smtVecUsefulTracks,6);
		TH1F *stripProfile5 = lite_interface::PlotStripProfileOfLayer(smtVecUsefulTracks,5);*/

	new TCanvas("StripProfile_Layer7","StripProfile_Layer7");
	stripProfile7->Draw();

	new TCanvas("StripProfile_Layer6","StripProfile_Layer6");
	stripProfile6->Draw();

	new TCanvas("StripProfile_Layer5","StripProfile_Layer5");
	stripProfile5->Draw();

	new TCanvas("EnergyDeposition_UsefulTracks","EnergyDeposition_UsefulTracks");
	energySumHistUsefulTracks->Draw();

	std::cout << std::endl << std::endl;
	std::cout <<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
	std::cout <<"Number of Useful Tracks : " << genuineTrackCounter << std::endl;
	std::cout <<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;

	fApp->Run();


	return 0;
}

