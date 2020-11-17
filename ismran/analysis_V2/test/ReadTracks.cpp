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
	TFile *f=new TFile("test.root","RECREATE");
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	GenerateScintMatrixXYCenters();
	lite_interface::SingleMuonTrack *smt = 0;
	//std::vector<lite_interface::ScintillatorBar_V2*> *smt = new std::vector<lite_interface::ScintillatorBar_V2*>;
	lite_interface::Calibration *calib = lite_interface::Calibration::instance("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib.root");
	std::string outputFileName=argv[1];
	TFile *trackFile = new TFile(outputFileName.c_str(),"READ");
	TTree *trackTree = (TTree*)trackFile->Get("TracksTree");
	trackTree->SetBranchAddress("MuonTracks",&smt);

	Long64_t nentries = trackTree->GetEntries();

	Long64_t nbytes = 0;
	//if(nentries > 5)
		//nentries = 5;

	unsigned int barIndex = 5;
	std::vector<lite_interface::ScintillatorBar_V2*> vectOfScintBars;
	//std::vector<lite_interface::ScintillatorBar_V2*> vectOfScintBars2;
	//std::vector<lite_interface::ScintillatorBar_V2*> vectOfScintBarsProfile;
	//vectOfScintBars.reserve(nentries);

	  std::vector<lite_interface::ScintillatorBar_V2*>::iterator itr;

	  std::vector<lite_interface::SingleMuonTrack*> smtVec;

	  std::vector<lite_interface::ScintillatorBar_V2*> scintBarVecForNewTrack;

	unsigned int count = 0;
	for (Long64_t i=0; i<nentries;i++) {
		nbytes += trackTree->GetEntry(i);


		/*
		 * Trying to plot few tracks
		 */
		/*if(i<1){
			new TCanvas();
			TGraphErrors *grxy = lite_interface::PlotMuonTrack(smt,2);
			grxy->SetMarkerStyle(8);
			grxy->Draw("p");
			//DrawGrid("Muon Track in XY plane; X axis ; Y axis", 9, 9);
			f->cd();
			grxy->Write();
			f->Close();
		}*/

		//if(i < 5 )
		{
			//std::cout << "==============================" << std::endl;
			//if((smt->GetMuonTrack()).size() > 20 )
			//	smt->Print();
			std::vector<lite_interface::ScintillatorBar_V2*> scintBarVecOfATrack = smt->GetMuonTrack();

			scintBarVecForNewTrack.clear();

			for(unsigned int j = 0 ; j < scintBarVecOfATrack.size() ; j++){

				//(smt->GetMuonTrack())[j]->Print();
				if(scintBarVecOfATrack[j]->GetBarIndex() > 81){
					count++;
				}

				//vectOfScintBarsProfile.push_back(new lite_interface::ScintillatorBar_V2(*scintBarVecOfATrack[j]));
				vectOfScintBars.push_back(new lite_interface::ScintillatorBar_V2(*scintBarVecOfATrack[j]));
				scintBarVecForNewTrack.push_back(new lite_interface::ScintillatorBar_V2(*scintBarVecOfATrack[j]));
			}
			smtVec.push_back(new lite_interface::SingleMuonTrack(scintBarVecForNewTrack));
		}

	}

	std::cout << "Number of Scintillators with wrong BarIndex : " << count << std::endl;

	std::cout << "++++++++++++++++++++ VecOfScintBars Size : " << vectOfScintBars.size() << " ++++++++++++++++++++++++++++" << std::endl;
	for(unsigned int i = 0 ; i< vectOfScintBars.size() ; i++){
		if(i < 5){
		//	vectOfScintBars[i]->Print();
			//std::cout << vectOfScintBars[i]->GetBarIndex() << std::endl;
		}
	}

	TH1F *histQmeanCorrected = PlotQMeanCorrected(vectOfScintBars,5);
	histQmeanCorrected->SetLineColor(kGreen);
	TH1F *histQmeanCorrected2 = PlotQMeanCorrected(vectOfScintBars,15);
	histQmeanCorrected2->SetLineColor(kMagenta);
	new TCanvas();
	histQmeanCorrected->Draw();
	histQmeanCorrected2->Draw("same");

	new TCanvas();
	TH1F *histDelT = PlotDelT(vectOfScintBars,barIndex);
	TH1F *histDelTCorrected = PlotDelTCorrected(vectOfScintBars,barIndex);
	histDelTCorrected->SetLineColor(kMagenta);
	histDelT->Draw();
	histDelTCorrected->Draw("same");

	new TCanvas();
	TH1F *histBarProfile = PlotBarsProfile(vectOfScintBars);
	histBarProfile->Draw();

	new TCanvas();
	TH2F* hitPointsOnBar = PlotHitPointsOnBar(vectOfScintBars,5);
	hitPointsOnBar->Draw();

	new TCanvas();
	TH1F *energySumHist = PlotEnergySum(smtVec);
	energySumHist->Draw();

	new TCanvas();
	TH1F *zenithAngleHist = PlotZenithAngle(smtVec,1);
	zenithAngleHist->Draw();

	new TCanvas();
	TH1F *zenithAngleHist_Param = PlotZenithAngle(smtVec,2);
	zenithAngleHist_Param->Draw();

	fApp->Run();

	return 0;
}



