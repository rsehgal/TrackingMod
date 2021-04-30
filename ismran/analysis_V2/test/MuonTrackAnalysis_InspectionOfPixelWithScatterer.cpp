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
	
	unsigned int numOfTracksToRead=0;//2000000;
	std::vector<lite_interface::SingleMuonTrack*> smtVec = GetMuonTracksVector(filename,numOfTracksToRead);	
	TH2F *hist2D_Layer3 = new TH2F("HitPointOnLayer_3","HitPointOnLayer_3",200,-50,50,200,-50,50);
	TH2F *hist2D_Layer8 = new TH2F("HitPointOnLayer_8","HitPointOnLayer_8",200,-50,50,200,-50,50);
	TH2F *hist2D_Pixel_Layer3 = new TH2F("Hist2D_Pixel_Count_Layer3","Hist2D_Pixel_Count_Layer3",numOfBarsInEachLayer,0,numOfBarsInEachLayer,numOfBarsInEachLayer,0,numOfBarsInEachLayer);
	TH1F *histPixel = new TH1F("HistOfPixelOnLayer","HistOfPixelOnLayer",numOfBarsInEachLayer*numOfBarsInEachLayer,0,numOfBarsInEachLayer*numOfBarsInEachLayer);

	for(unsigned int i = 0 ; i < smtVec.size() ; i++){

		bool layer3Pixel = false;
		bool layer8Pixel = false;
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

					if(scint->GetBarIndexInLayer()==4 && scintLayer7->GetBarIndexInLayer()==4 && scintLayer9->GetBarIndexInLayer()==4 ){
						layer8Pixel = check;
						lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],8);
						if(hitPoint->GetX() < 9000. && hitPoint->GetZ() < 9000){
							hist2D_Layer8->Fill(hitPoint->GetX(),hitPoint->GetZ());
						}
					}
					//lite_interface::Point3D *hitPtLayer7 = scintLayer7->EstimateHitPosition_Param();
					//lite_interface::Point3D *hitPtLayer9 = scintLayer9->EstimateHitPosition_Param();
				}


			}
			//Block for layer 3
			if(layer8Pixel)
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

								//if(scint->GetBarIndexInLayer()==4 && scintLayer2->GetBarIndexInLayer()==4 && scintLayer4->GetBarIndexInLayer()==4 )
								//if(layer8Pixel)
								{
									if(scint->GetBarIndexInLayer()==4 && scintLayer2->GetBarIndexInLayer()==4 && scintLayer4->GetBarIndexInLayer()==4 )
									{
									layer3Pixel = check;
									lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i],3);
									if(hitPoint->GetX() < 9000. && hitPoint->GetZ() < 9000){
										hist2D_Layer3->Fill(hitPoint->GetX(),hitPoint->GetZ());
									}
									}

									if(scintLayer2->GetBarIndexInLayer()==scintLayer4->GetBarIndexInLayer()){
										histPixel->Fill(scint->GetBarIndexInLayer()*numOfBarsInEachLayer+scintLayer2->GetBarIndexInLayer());
										hist2D_Pixel_Layer3->Fill(scint->GetBarIndexInLayer(),scintLayer2->GetBarIndexInLayer());
									}
								}
								//lite_interface::Point3D *hitPtLayer7 = scintLayer7->EstimateHitPosition_Param();
								//lite_interface::Point3D *hitPtLayer9 = scintLayer9->EstimateHitPosition_Param();
							}


						}



		}

	TCanvas *canLayer3 = new TCanvas("Layer3","Layer3");
	hist2D_Layer3->Draw("colz");


	TCanvas *canLayer8 = new TCanvas("Layer8","Layer8");
	hist2D_Layer8->Draw("colz");

	TCanvas *canPixel = new TCanvas("PixelHistLayer3","PixelHistLayer3");
	histPixel->Draw();

	TCanvas *canPixelCount = new TCanvas("Can_2D_PixelCount_Layer3","Can_2D_PixelCount_Layer3");
	hist2D_Pixel_Layer3->Draw();



	std::string matWithExt = filename.substr(13);
	TFile *fp = new TFile(("HitPattern_Pixel"+matWithExt).c_str(),"RECREATE");
	fp->cd();

	hist2D_Layer3->Write();
	hist2D_Layer8->Write();
	histPixel->Write();
	hist2D_Pixel_Layer3->Write();

	fp->Close();


	fApp->Run();



}
