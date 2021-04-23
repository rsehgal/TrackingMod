/*
 * MuonTrackAnalysis_InspectionOfPixelWithScatterer.cpp
 *
 *  Created on: 31-Mar-2021
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
	//gStyle->SetOptStat(0);
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

	TFile *trackFile = new TFile(filename.c_str(),"READ");
	TTree *trackTree = (TTree*)trackFile->Get("TracksTree");
	trackTree->SetBranchAddress("MuonTracks",&smt);

	Long64_t nentries = trackTree->GetEntries();

	Long64_t nbytes = 0;

	std::vector<lite_interface::SingleMuonTrack*> smtVec;

	int counter = 0;
	unsigned int testCounter = 0;
	for (Long64_t i=0; i<nentries;i++) {

		nbytes += trackTree->GetEntry(i);
		if(!(i % 1000000) && i!=0){
			std::cout << "Processed : " << i << " Tracks ........" << std::endl;
			testCounter++;
			/*if(testCounter==2)
				break;*/
		}
		testCounter++;
		if(testCounter==19500001)
			break;

		smtVec.push_back(new lite_interface::SingleMuonTrack(*smt));
	}
	TH2F *hist2D_Pixel_Layer3 = new TH2F("HitPointOnLayer_3","HitPointOnLayer_3",200,-50,50,200,-50,50);
	TH2F *hist2D_Pixel_Layer8 = new TH2F("HitPointOnLayer_8","HitPointOnLayer_8",200,-50,50,200,-50,50);
//	TH1F *histPixel_layer8 = new TH1F("HistOfPixelOnLayer8","HistOfPixelOnLayer8",numOfBarsInEachLayer*numOfBarsInEachLayer,0,numOfBarsInEachLayer*numOfBarsInEachLayer);

	unsigned int pixelCoincCounter = 0 ;

	for(unsigned int i = 0 ; i < smtVec.size() ; i++){

		//testCounter++;
		//if(testCounter==2)
			//break;
		bool layer3Pixel = false;
		bool layer8Pixel = false;
		lite_interface::Point3D *hitPointLayer8 = new lite_interface::Point3D();
		lite_interface::Point3D *hitPointLayer3 = new lite_interface::Point3D();
			//Block for layer 8
			{
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
					lite_interface::ScintillatorBar_V2 *scint = smtVec[i]->GetScintillator(hittBarIndex);

					if( (scint->GetBarIndexInLayer()==3 || scint->GetBarIndexInLayer()==4 || scint->GetBarIndexInLayer()==5) &&
						(scintLayer7->GetBarIndexInLayer()==3 || scintLayer7->GetBarIndexInLayer()==4 || scintLayer7->GetBarIndexInLayer()==5) &&
						(scintLayer9->GetBarIndexInLayer()==3 || scintLayer9->GetBarIndexInLayer()==4 || scintLayer9->GetBarIndexInLayer()==5)
					   ) {
						layer8Pixel = check;
						lite_interface::Point3D *temp = Get3DHitPointOnLayer(smtVec[i],8);
						hitPointLayer8->SetXYZ(temp->GetX(),temp->GetY(),temp->GetZ());

					}
				}
			}

			//Block for layer 3
			{
				unsigned int hittBarIndex = 10000;

				lite_interface::ScintillatorBar_V2 *scintLayer4;
				lite_interface::ScintillatorBar_V2 *scintLayer2;

				bool check = smtVec[i]->CheckTrackForLayerNum(4,hittBarIndex);
				if(check){
					scintLayer4 = smtVec[i]->GetScintillator(hittBarIndex);
				}
				check &= smtVec[i]->CheckTrackForLayerNum(2,hittBarIndex);
				if(check){
					scintLayer2 = smtVec[i]->GetScintillator(hittBarIndex);
				}
				check &= smtVec[i]->CheckTrackForLayerNum(3,hittBarIndex);

				if(check){
					lite_interface::ScintillatorBar_V2 *scint = smtVec[i]->GetScintillator(hittBarIndex);

					//if(scint->GetBarIndexInLayer()==4 && scintLayer2->GetBarIndexInLayer()==4 && scintLayer4->GetBarIndexInLayer()==4 ){
					if (
							(scint->GetBarIndexInLayer()==3 || scint->GetBarIndexInLayer()==4 || scint->GetBarIndexInLayer()==5) &&
							(scintLayer2->GetBarIndexInLayer()==3 || scintLayer2->GetBarIndexInLayer()==4 || scintLayer2->GetBarIndexInLayer()==5) &&
							(scintLayer4->GetBarIndexInLayer()==3 || scintLayer4->GetBarIndexInLayer()==4 || scintLayer4->GetBarIndexInLayer()==5)

					){
						layer3Pixel = check;
						lite_interface::Point3D *temp = Get3DHitPointOnLayer(smtVec[i],3);
						hitPointLayer3->SetXYZ(temp->GetX(),temp->GetY(),temp->GetZ());
						}
					}
			}

			if(layer3Pixel && layer8Pixel)
			{
				pixelCoincCounter++;
				if(hitPointLayer8->GetX() < 9000. && hitPointLayer8->GetZ() < 9000){
					hist2D_Pixel_Layer8->Fill(hitPointLayer8->GetX(),hitPointLayer8->GetZ());
				}
				if(hitPointLayer3->GetX() < 9000. && hitPointLayer3->GetZ() < 9000){
					hist2D_Pixel_Layer3->Fill(hitPointLayer3->GetX(),hitPointLayer3->GetZ());
				}
			}

	}

	TCanvas *canPixelLayer3 = new TCanvas("Can_PixelLayer3","Can_PixelLayer3");
	hist2D_Pixel_Layer3->Draw();

	TCanvas *canPixelLayer8 = new TCanvas("Can_PixelLayer8","Can_PixelLayer8");
	hist2D_Pixel_Layer8->Draw();


	TFile *fp = new TFile("Pixel_Coinc.root","RECREATE");
	fp->cd();
	hist2D_Pixel_Layer3->Write();
	hist2D_Pixel_Layer8->Write();
	fp->Close();

	std::cout << "Number of required Pixel Coinc Tracks : " << pixelCoincCounter << std::endl;

	fApp->Run();

}
