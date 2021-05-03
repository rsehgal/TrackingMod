/*
 * MuonTrackAnalysis_SimplestSetup_Pixels.cpp
 *
 *  Created on: 30-Apr-2021
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
	//TApplication *fApp = new TApplication("Test", NULL, NULL);
	GenerateScintMatrixXYCenters();
	lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
	std::string filename = argv[1];
	std::string otherfile = argv[2];

	unsigned int numOfEventsToRead = NumOfEventsToRead(filename,otherfile,std::string("TracksTree")); 
	
	std::vector<lite_interface::SingleMuonTrack*> smtVec = GetMuonTracksVector(filename,numOfEventsToRead);
	
	TH2F *hist2D_Pixel_Layer8 = new TH2F("Hist_Pixel_Layer_8","Hist_Pixel_Layer8",200,-50.,50.,200, -50.,50.);
	TH2F *hist2D_Pixel_Layer8_Masked_With_54 = new TH2F("Hist_Pixel_Layer_8_Masked_With_54","Hist_Pixel_Layer8_Masked_With_54",200,-50.,50.,200, -50.,50.);
	TH2F *hist2D_Pixel_Layer3 = new TH2F("Hist_Pixel_Layer_3","Hist_Pixel_Layer3",200,-50.,50.,200, -50.,50.);
	TH1F *histBarsUsed = new TH1F("Hist_Bars_Used","Hist_Bars_Used",90,0,90);
	TH2F *hist2D_OneFullBar = new TH2F("One_Full_Bar","One_Full_Bar",200,-50.,50.,200, -50.,50.);
	TH2F *hist2D_Pixel_Layer3_Required = new TH2F("Hist_Pixel_Layer_3_Required","Hist_Pixel_Layer3_Required",200,-50.,50.,200, -50.,50.);
	unsigned int counter=0;
	//std::vector<unsigned int> vecOfReqScint={88,79,70,53,43,34};
	std::vector<unsigned int> vecOfReqScint={88,76,70,53,43,31,13,7};
	unsigned int counterReq=0;
	for(unsigned int i = 0 ; i < smtVec.size() ; i++){
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

				bool cond1 = (scint->GetBarIndexInLayer() == 4) && (scintLayer7->GetBarIndexInLayer()==1) && (scintLayer9->GetBarIndexInLayer()==1);
				bool cond2 = (scint->GetBarIndexInLayer() == 4) && (scintLayer7->GetBarIndexInLayer()==7) && (scintLayer9->GetBarIndexInLayer()==7);

				if(cond1 || cond2){
					lite_interface::Point3D *temp = Get3DHitPointOnLayer(smtVec[i],8);
					hitPointLayer8->SetXYZ(temp->GetX(),temp->GetY(),temp->GetZ());
					layer8Pixel = check;
					hist2D_Pixel_Layer8->Fill(hitPointLayer8->GetX(),hitPointLayer8->GetZ());
					histBarsUsed->Fill(scint->GetBarIndex());
					histBarsUsed->Fill(scintLayer7->GetBarIndex());
					histBarsUsed->Fill(scintLayer9->GetBarIndex());

					if(smtVec[i]->CheckTrackForScintillator(53)&& smtVec[i]->CheckTrackForScintillator(43) && smtVec[i]->CheckTrackForScintillator(34))
						hist2D_Pixel_Layer8_Masked_With_54->Fill(hitPointLayer8->GetX(),hitPointLayer8->GetZ());
				}
			

				if((scint->GetBarIndexInLayer() == 4 || scint->GetBarIndexInLayer() == 3) && (scintLayer7->GetBarIndexInLayer()==scintLayer9->GetBarIndexInLayer())){
					lite_interface::Point3D *temp = Get3DHitPointOnLayer(smtVec[i],8);
					hitPointLayer8->SetXYZ(temp->GetX(),temp->GetY(),temp->GetZ());
					hist2D_OneFullBar->Fill(hitPointLayer8->GetX(),hitPointLayer8->GetZ());
				}
			}

			
		}

		

		if(smtVec[i]->CheckTrackForRequiredScintillators(vecOfReqScint)){

			lite_interface::Point3D *temp = Get3DHitPointOnLayer(smtVec[i],8);
                        //hitPointLayer8->SetXYZ(temp->GetX(),temp->GetY(),temp->GetZ());
			if(std::fabs(temp->GetX()) < xyzLimit && std::fabs(temp->GetZ()) < xyzLimit){
				hist2D_Pixel_Layer3_Required->Fill(temp->GetX(),temp->GetZ());
                        	counterReq++;
			}
                }


	}
	std::cout << "Value of Required Counter : " << counterReq << std::endl;
	std::cout << "Number of Tracks that passes through required scintillators : "  << counter << std::endl;

	new TCanvas("Can_Pixel_Layer8_Masked_With_54","Can_Pixel_Layer8_Masked_With_54");
	hist2D_Pixel_Layer8_Masked_With_54->Draw();

	new TCanvas("Can_Pixel_Layer8_Required","Can_Pixel_Layer8_Required");
	hist2D_Pixel_Layer3_Required->Draw();


	new TCanvas("Can_OneFullBar","Can_OneFullBar");
        hist2D_OneFullBar->Draw();

	new TCanvas("Can_Bars-Used","Can_Bars_Used");
	histBarsUsed->Draw();

	new TCanvas("Can_Pixel_Layer8","Can_Pixel_Layer8");
	hist2D_Pixel_Layer8->Draw();

	std::string subFileName = filename.substr(13);

	TFile *fp = new TFile(("Pixel_Simplest"+subFileName).c_str(),"RECREATE");
	fp->cd();
	hist2D_Pixel_Layer8->Write();
	hist2D_Pixel_Layer8_Masked_With_54->Write();
	histBarsUsed->Write();
	hist2D_OneFullBar->Write();
	hist2D_Pixel_Layer3_Required->Write();
	fp->Close();
	
	//fApp->Run();
}
