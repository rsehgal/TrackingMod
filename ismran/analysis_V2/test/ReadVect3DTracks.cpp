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
#include "SingleMuonTrack.h"
int main(int argc,char *argv[]){
	lite_interface::Calibration *calib = lite_interface::Calibration::instance("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib.root");

	TApplication *fApp = new TApplication("Test", NULL, NULL);
	std::string outputFileName=argv[1];
	TFile *hitPointFile = new TFile(outputFileName.c_str(),"READ");
	TTree *hitPointVecTree = (TTree*)hitPointFile->Get("HitPointVecTree");
	TTree *hitPointVecTree_Param = (TTree*)hitPointFile->Get("HitPointVecTreeParam");
	std::vector<lite_interface::Point3D*> *vecOfPoint3D = 0;
	double energySum = 0;
	hitPointVecTree->SetBranchAddress("HitPointVec",&vecOfPoint3D);
	hitPointVecTree->SetBranchAddress("EnergySum",&energySum);

	std::vector<lite_interface::Point3D*> *vecOfPoint3D_Param = 0;
	hitPointVecTree_Param->SetBranchAddress("HitPointVec",&vecOfPoint3D_Param);

	Long64_t nentries = hitPointVecTree->GetEntries();

	Long64_t nbytes = 0;

	lite_interface::SingleMuonTrack *smt = 0;
	TTree *trackTree = (TTree*)hitPointFile->Get("TracksTree");
	trackTree->SetBranchAddress("MuonTracks",&smt);


	unsigned int count = 0;
	TH1F *histDiff = new TH1F("EstimateDiffHist","EstimateDiffHist",100,-1.,1.);
	for (Long64_t i=0; i<nentries;i++) {
		nbytes += hitPointVecTree->GetEntry(i);
		nbytes += trackTree->GetEntry(i);
		nbytes += hitPointVecTree_Param->GetEntry(i);
		histDiff->Fill((vecOfPoint3D->at(0)->GetZ() - vecOfPoint3D_Param->at(0)->GetZ()));
		if(energySum > 400. || energySum < -400.)
			continue;
		if(i >90 && i < 130){

			std::cout << "------------ Event Num : " << i << " --------------------" << std::endl;
			std::cout << "Energy sum : "  << energySum << std::endl;
			smt->Print();
			std::string title = "Event Num : "+std::to_string(i)+" : EnergySum : "+std::to_string(energySum);
			TCanvas *can = new TCanvas(title.c_str(),title.c_str());
			//TCanvas *can = new TCanvas();
			can->Divide(4,2);
			can->cd(1);
			DrawGrid("Muon Track in XY plane; X axis ; Y axis", 9, 9);
			PlotMuonTrackXY(*vecOfPoint3D)->Draw("p");

			can->cd(2);
			DrawGrid("Muon Track in ZY plane; Z axis ; Y axis", 9, 9);
			PlotMuonTrackZY(*vecOfPoint3D)->Draw("p");

			/*
			 * Trying to get Fitted Tracks
			 */

			can->cd(3);
			DrawGrid("Fitted Muon Track in XY plane; X axis ; Y axis", 9, 9);
			PlotMuonTrackXY(CreateFittedTrack(*vecOfPoint3D))->Draw("p");

			can->cd(4);
			DrawGrid("Fitted Muon Track in ZY plane; Z axis ; Y axis", 9, 9);
			PlotMuonTrackZY(CreateFittedTrack(*vecOfPoint3D))->Draw("p");


			//---------------- Parameterized stuff ------------------

			can->cd(5);
			DrawGrid("Param Muon Track in XY plane; X axis ; Y axis", 9, 9);
			PlotMuonTrackXY(*vecOfPoint3D_Param)->Draw("p");

			can->cd(6);
			DrawGrid("Param Muon Track in ZY plane; Z axis ; Y axis", 9, 9);
			PlotMuonTrackZY(*vecOfPoint3D_Param)->Draw("p");

			/*
			 * Trying to get Fitted Tracks
			 */

			can->cd(7);
			DrawGrid("Param Fitted Muon Track in XY plane; X axis ; Y axis", 9, 9);
			PlotMuonTrackXY(CreateFittedTrack(*vecOfPoint3D_Param))->Draw("p");

			can->cd(8);
			DrawGrid("Param Fitted Muon Track in ZY plane; Z axis ; Y axis", 9, 9);
			PlotMuonTrackZY(CreateFittedTrack(*vecOfPoint3D_Param))->Draw("p");
		}
	}

	new TCanvas();
	histDiff->Draw();
	fApp->Run();

	return 0;
}



