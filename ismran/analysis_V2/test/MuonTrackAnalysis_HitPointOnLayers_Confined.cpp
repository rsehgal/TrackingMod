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
	TH2F *hist2D_Layer1 = new TH2F("HitPointOnLayer_1","HitPointOnLayer_1",200,-50,50,200,-50,50);
//	TH2F *hist2D_Layer2 = new TH2F("HitPointOnLayer_2","HitPointOnLayer_2",200,-50,50,200,-50,50);
	TH2F *hist2D_Layer3 = new TH2F("HitPointOnLayer_3","HitPointOnLayer_3",200,-50,50,200,-50,50);
//	TH2F *hist2D_Layer4 = new TH2F("HitPointOnLayer_4","HitPointOnLayer_4",200,-50,50,200,-50,50);
	TH2F *hist2D_Layer5 = new TH2F("HitPointOnLayer_5","HitPointOnLayer_5",200,-50,50,200,-50,50);
	TH2F *hist2D_Layer8 = new TH2F("HitPointOnLayer_8","HitPointOnLayer_8",200,-50,50,200,-50,50);

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
			if(smtVec[i]->CheckTrackForScintillator(59)){
				lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],3);
				if(hitPoint->GetX() < 9000. && hitPoint->GetZ() < 9000){
					hist2D_Layer3->Fill(hitPoint->GetX(),hitPoint->GetZ());
				}
			}
		}



	}

#if(0)
	for(unsigned int i = 0 ; i < smtVec.size() ; i++){
		{
		lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],0);
		if(hitPoint->GetX() < 9000. && hitPoint->GetZ() < 9000){
			hist2D_Layer0->Fill(hitPoint->GetX(),hitPoint->GetZ());
		}

		}

		bool checkLayer1 = false;
		bool checkLayer8 = false;
		lite_interface::Point3D *hitPointLayer1;
		//Block for Layer 1
		{

		lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],1);
		hitPointLayer1 = hitPoint;

		//hist2D_Layer1->Fill(hitPoint->GetX(),hitPoint->GetZ());

		unsigned int hittBarIndex = 10000;
				lite_interface::ScintillatorBar_V2 *scint0;
				lite_interface::ScintillatorBar_V2 *scint2;

				bool check = smtVec[i]->CheckTrackForLayerNum(0,hittBarIndex);
				if(check){
					scint0 = smtVec[i]->GetScintillator(hittBarIndex);
				}
				check &= smtVec[i]->CheckTrackForLayerNum(2,hittBarIndex);
				if(check){
					scint2 = smtVec[i]->GetScintillator(hittBarIndex);
				}
				//check &= smtVec[i]->CheckTrackForLayerNum(2,hittBarIndex);
				check &= smtVec[i]->CheckTrackForLayerNum(1,hittBarIndex);
				if(check){
					checkLayer1 = check;
					lite_interface::ScintillatorBar_V2 *scint = smtVec[i]->GetScintillator(hittBarIndex);
					/*if(scint->GetBarIndexInLayer()==3)
						zhist_Layer1->Fill(hitPoint->GetZ());*/
					//if(scint0->GetBarIndexInLayer() == scint2->GetBarIndexInLayer())
					if(hitPoint->GetX() < 9000. && hitPoint->GetZ() < 9000)
					{
						hist2D_Layer1->Fill(hitPoint->GetX(),hitPoint->GetZ());
					}

				}//check

		}
		//Block for layer 2
		{
		lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],2);
		if(hitPoint->GetX() < 9000. && hitPoint->GetZ() < 9000){
		hist2D_Layer2->Fill(hitPoint->GetX(),hitPoint->GetZ());
		}
		}

		//Block for layer 3
		{
		lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],3);
		if(hitPoint->GetX() < 9000. && hitPoint->GetZ() < 9000){
		hist2D_Layer3->Fill(hitPoint->GetX(),hitPoint->GetZ());
		}
		/*gr_Layer3->SetName("Layer3");
		gr_Layer3->SetTitle("Layer3");
		gr_Layer3->AddPoint(hitPoint->GetX(),hitPoint->GetZ());*/
		if(hitPoint->GetX() < 9000. && hitPoint->GetZ() < 9000){
		xvecLayer3.push_back(hitPoint->GetX());
		yvecLayer3.push_back(hitPoint->GetZ());
		}
		}
		//Block For layer 4
		{

		lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],4);
		if(hitPoint->GetX() < 9000. && hitPoint->GetZ() < 9000){
		hist2D_Layer4->Fill(hitPoint->GetX(),hitPoint->GetZ());
		}
		}

		//Block for layer 5
		{
		lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],5);
		if(hitPoint->GetX() < 9000. && hitPoint->GetZ() < 9000){
		hist2D_Layer5->Fill(hitPoint->GetX(),hitPoint->GetZ());
		}
		}

		//Block for Layer 8
		{
		lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],8);
		//hist2D_Layer8->Fill(hitPoint->GetX(),hitPoint->GetZ());

		unsigned int hittBarIndex = 10000;

		lite_interface::ScintillatorBar_V2 *scintLayer9;
		lite_interface::ScintillatorBar_V2 *scintLayer7;

		bool check = smtVec[i]->CheckTrackForLayerNum(9,hittBarIndex);
		if(check){
			scintLayer9 = smtVec[i]->GetScintillator(hittBarIndex);
		}
		check &= smtVec[i]->CheckTrackForLayerNum(7,hittBarIndex);
		if(check){
			scintLayer7 = smtVec[i]->GetScintillator(hittBarIndex);
		}
		check &= smtVec[i]->CheckTrackForLayerNum(8,hittBarIndex);

		if(check){
			checkLayer8 = check;
			lite_interface::ScintillatorBar_V2 *scint = smtVec[i]->GetScintillator(hittBarIndex);


			lite_interface::Point3D *hitPtLayer7 = scintLayer7->EstimateHitPosition_Param();
			lite_interface::Point3D *hitPtLayer9 = scintLayer9->EstimateHitPosition_Param();

			//if(scint->GetBarIndexInLayer()==3 && scintLayer7->GetBarIndexInLayer()==3 && scintLayer9->GetBarIndexInLayer()==3 ){
			//if(scintLayer7->GetBarIndexInLayer() == scintLayer9->GetBarIndexInLayer())
			{
				if(scint->GetBarIndexInLayer()==3 && scintLayer7->GetBarIndexInLayer()==3 && scintLayer9->GetBarIndexInLayer()==3 ){
					zhist_Layer8->Fill(hitPoint->GetZ());
					delZ_Layer_9_7->Fill(hitPtLayer7->GetZ()-hitPtLayer9->GetZ());
				}
				xhist_Layer8->Fill(hitPoint->GetX());
				if(hitPoint->GetX() < 9000. && hitPoint->GetZ() < 9000){
				hist2D_Layer8->Fill(hitPoint->GetX(),hitPoint->GetZ());
				}
			}




			/*if(checkLayer1 && checkLayer8){
				lite_interface::Point3D *hitPtLayer7 = scintLayer7->EstimateHitPosition_Param();
				lite_interface::Point3D *hitPtLayer9 = scintLayer9->EstimateHitPosition_Param();

				Tracking::Vector3D<double> start(hitPtLayer9->GetX(),hitPtLayer9->GetY(),hitPtLayer9->GetZ());
				Tracking::Vector3D<double> end(hitPtLayer7->GetX(),hitPtLayer7->GetY(),hitPtLayer7->GetZ());
				Tracking::Vector3D<double> dir = (end-start).Unit();
				double dist = (GetYOfLayer(1)-hitPoint->GetY())/dir.y();
				double xExtraPolated = hitPoint->GetX()+dir.x()*dist;
				double zExtraPolated = hitPoint->GetZ()+dir.z()*dist;
				delX_Hist_Layer1->Fill(xExtraPolated-hitPointLayer1->GetX());
				delZ_Hist_Layer1->Fill(zExtraPolated-hitPointLayer1->GetZ());
				dist_Hist_Layer8_1->Fill(dist);

			}*/
		}


		/*gr_Layer8->SetName("Layer8");
		gr_Layer8->SetTitle("Layer8");
		gr_Layer8->AddPoint(hitPoint->GetX(),hitPoint->GetZ());*/
		if(hitPoint->GetX() < 9000. && hitPoint->GetZ() < 9000){
		xvecLayer8.push_back(hitPoint->GetX());
		yvecLayer8.push_back(hitPoint->GetZ());

		}
		}

	}
#endif

/*
	new TCanvas("Layer0","Layer0");
	hist2D_Layer0->Draw("colz");

	new TCanvas("Layer1","Layer1");
	hist2D_Layer1->Draw("colz");

	new TCanvas("Layer2","Layer2");
	hist2D_Layer2->Draw("colz");
*/

	TCanvas *canLayer3 = new TCanvas("Layer3","Layer3");
	hist2D_Layer3->Draw("colz");

/*
	new TCanvas("Layer4","Layer4");
	hist2D_Layer4->Draw();
*/
/*
	new TCanvas("Layer5","Layer5");
	hist2D_Layer5->Draw("colz");
*/


/*
	TCanvas *canLayer8 = new TCanvas("Layer8","Layer8");
	hist2D_Layer8->Draw("colz");

	TCanvas *canLayer1 = new TCanvas("Layer1","Layer1");
	hist2D_Layer1->Draw("colz");
*/

/*
	new TCanvas("Hist_Of_Interpolated_Position_Layer1","Hist_Of_Interpolated_Position_Layer1");
	zhist_Layer1->Draw();

	new TCanvas("Hist_Of_Interpolated_Position_Layer8","Hist_Of_Interpolated_Position_Layer8");
	zhist_Layer8->Draw();

	new TCanvas("Hist_Of_X_Pos_From_Parameterization_Layer8","Hist_Of_X_Pos_From_Parameterization_Layer8");
	xhist_Layer8->Draw();


	new TCanvas("Hist_Of_DelX_Layer1","Hist_Of_DelX_Layer1");
	delX_Hist_Layer1->Draw();

	new TCanvas("Hist_Of_DelZ_Layer1","Hist_Of_DelZ_Layer1");
	delZ_Hist_Layer1->Draw();

	new TCanvas("Distance_Between_Layer8_And_Layer1","Distance_Between_Layer8_And_Layer1");
	dist_Hist_Layer8_1->Draw();

	new TCanvas("DelZ_Layer_9_7","DelZ_Layer_9_7");
	delZ_Layer_9_7->Draw();
*/

	std::string matWithExt = filename.substr(13);
	TFile *fp = new TFile(("Anti_Coincidence_HitPattern"+matWithExt).c_str(),"RECREATE");
	fp->cd();
/*
	hist2D_Layer0->Write();
	hist2D_Layer1->Write();
	hist2D_Layer2->Write();
*/
	hist2D_Layer3->Write();
/*
	hist2D_Layer4->Write();
*/
//	hist2D_Layer5->Write();

//	hist2D_Layer8->Write();

/*
	zhist_Layer1->Write();
	zhist_Layer8->Write();
	xhist_Layer8->Write();
	delX_Hist_Layer1->Write();
	delZ_Hist_Layer1->Write();
	dist_Hist_Layer8_1->Write();
	delZ_Layer_9_7->Write();
*/
	fp->Close();


	fApp->Run();


	return 0;
}


