/*
 * MuonTrackAnalysis.cpp
 *
 *  Created on: 01-Mar-2021
 *       Author: rsehgal
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
#include <TH2F.h>

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
//	TH2F *hist2D_Layer0 = new TH2F("HitPointOnLayer_0","HitPointOnLayer_0",200,-50,50,200,-50,50);
//	TH2F *hist2D_Layer1 = new TH2F("HitPointOnLayer_1","HitPointOnLayer_1",200,-50,50,200,-50,50);
//	TH2F *hist2D_Layer2 = new TH2F("HitPointOnLayer_2","HitPointOnLayer_2",200,-50,50,200,-50,50);
	TH2F *hist2D_Layer3 = new TH2F("HitPointOnLayer_3","HitPointOnLayer_3",200,-50,50,200,-50,50);
	TH2F *hist2D_Layer3_HitInAnyConfiningScints = new TH2F("HitPointOnLayer_3_HitInAnyConfiningScints","HitPointOnLayer_3_HitInAnyConfiningScints",200,-50,50,200,-50,50);
//	TH2F *hist2D_Layer4 = new TH2F("HitPointOnLayer_4","HitPointOnLayer_4",200,-50,50,200,-50,50);
//	TH2F *hist2D_Layer5 = new TH2F("HitPointOnLayer_5","HitPointOnLayer_5",200,-50,50,200,-50,50);
//	TH2F *hist2D_Layer8 = new TH2F("HitPointOnLayer_8","HitPointOnLayer_8",200,-50,50,200,-50,50);

	//TGraph *gr_Layer8 = new TGraph();
	//TGraph *gr_Layer3 = new TGraph();
/*
	std::vector<double> xvecLayer3, yvecLayer3;
	std::vector<double> xvecLayer8, yvecLayer8;

	TH1F *zhist_Layer1 = new TH1F("Hist_Z_InterPolated_Position_Layer1","Hist_Z_InterPolated_Position_Layer1",200,-50.,50.);
	TH1F *zhist_Layer8 = new TH1F("Hist_Z_InterPolated_Position_Layer8","Hist_Z_InterPolated_Position_Layer8",200,-50.,50.);
	TH1F *xhist_Layer8 = new TH1F("Hist_X_Pos_From_Parameterization_Layer8","Hist_X_Pos_From_Parameterization_Layer8",200,-50.,50.);
	TH1F *delX_Hist_Layer1 = new TH1F("DelX_Hist_Layer1","DelX_Hist_Layer1",200,-50.,50.);
	TH1F *delZ_Hist_Layer1 = new TH1F("DelZ_Hist_Layer1","DelZ_Hist_Layer1",200,-50.,50.);
	TH1F *dist_Hist_Layer8_1 = new TH1F("Distance_Between_Layer_8_1","Distance_Between_Layer_8_1",200,0,200.);
	TH1F *delZ_Layer_9_7 = new TH1F("delZ_Layer_9_7","delZ_Layer_9_7",200,-50,50.);
*/


	for(unsigned int i = 0 ; i < smtVec.size() ; i++){


		//Block for layer 3
		{
			if(smtVec[i]->HitInRequiredLayers() && smtVec[i]->NoHitInScintillators(vecOfAntiCoincideneScint)){
				lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],8);
				if(hitPoint->GetX() < 9000. && hitPoint->GetZ() < 9000){
					hist2D_Layer3->Fill(hitPoint->GetX(),hitPoint->GetZ());
				}
			}
		}

		//Block to  detect hit in Confining Scints
		{
			if(smtVec[i]->HitInRequiredLayers() && smtVec[i]->HitInAnyScintillators(vecOfAntiCoincideneScint)){
				lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],8);
				if(hitPoint->GetX() < 9000. && hitPoint->GetZ() < 9000){
					hist2D_Layer3_HitInAnyConfiningScints->Fill(hitPoint->GetX(),hitPoint->GetZ());
				}
			}

		}



	}


	TCanvas *canLayer3 = new TCanvas("Layer3","Layer3");
	hist2D_Layer3->Draw("colz");

	TCanvas *canLayer3_confiningScints = new TCanvas("Layer3_HitInConfiningScints","Layer3_HitInConfiningScints");
	hist2D_Layer3_HitInAnyConfiningScints->Draw("colz");

	std::string matWithExt = filename.substr(13);
	TFile *fp = new TFile(("AntiCoincidence_HitPattern"+matWithExt).c_str(),"RECREATE");
	fp->cd();
	hist2D_Layer3->Write();
	hist2D_Layer3_HitInAnyConfiningScints->Write();
	fp->Close();

	fApp->Run();


	return 0;
}


