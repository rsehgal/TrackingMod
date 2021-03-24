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
	TH2F *hist2D_Layer0 = new TH2F("HitPointOnLayer_0","HitPointOnLayer_0",200,-50,50,200,-50,50);
	TH2F *hist2D_Layer1 = new TH2F("HitPointOnLayer_1","HitPointOnLayer_1",200,-50,50,200,-50,50);
	TH2F *hist2D_Layer2 = new TH2F("HitPointOnLayer_2","HitPointOnLayer_2",200,-50,50,200,-50,50);
	TH2F *hist2D_Layer3 = new TH2F("HitPointOnLayer_3","HitPointOnLayer_3",200,-50,50,200,-50,50);
	TH2F *hist2D_Layer4 = new TH2F("HitPointOnLayer_4","HitPointOnLayer_4",200,-50,50,200,-50,50);
	TH2F *hist2D_Layer5 = new TH2F("HitPointOnLayer_5","HitPointOnLayer_5",200,-50,50,200,-50,50);
	TH2F *hist2D_Layer8 = new TH2F("HitPointOnLayer_8","HitPointOnLayer_8",200,-50,50,200,-50,50);

	for(unsigned int i = 0 ; i < smtVec.size() ; i++){
		{
		lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],0);
		hist2D_Layer0->Fill(hitPoint->GetX(),hitPoint->GetZ());
		}
		{
		lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],1);
		hist2D_Layer1->Fill(hitPoint->GetX(),hitPoint->GetZ());
		}
		{
		lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],2);
		hist2D_Layer2->Fill(hitPoint->GetX(),hitPoint->GetZ());
		}

		{
		lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],3);
		hist2D_Layer3->Fill(hitPoint->GetX(),hitPoint->GetZ());
		}
		{
		lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],4);
		hist2D_Layer4->Fill(hitPoint->GetX(),hitPoint->GetZ());
		}

		{
		lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],5);
		hist2D_Layer5->Fill(hitPoint->GetX(),hitPoint->GetZ());
		}
		{
		lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],8);
		hist2D_Layer8->Fill(hitPoint->GetX(),hitPoint->GetZ());
		}

	}

	new TCanvas("Layer0","Layer0");
	hist2D_Layer0->Draw("colz");

	new TCanvas("Layer1","Layer1");
	hist2D_Layer1->Draw("colz");

	new TCanvas("Layer2","Layer2");
	hist2D_Layer2->Draw("colz");

	new TCanvas("Layer3","Layer3");
	hist2D_Layer3->Draw("colz");

	new TCanvas("Layer4","Layer4");
	hist2D_Layer4->Draw();

	new TCanvas("Layer5","Layer5");
	hist2D_Layer5->Draw("colz");

	new TCanvas("Layer8","Layer8");
	hist2D_Layer8->Draw("colz");


	fApp->Run();


	return 0;
}

