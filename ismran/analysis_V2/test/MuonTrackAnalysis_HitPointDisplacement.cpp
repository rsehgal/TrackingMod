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
	lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
	std::string filename = argv[1];

	TFile *trackFile = new TFile(filename.c_str(),"READ");
	std::vector<lite_interface::SingleMuonTrack*> smtVec = GetMuonTracksVector(filename);//,2000000);

	std::cout << "Size of SMTVec : " << smtVec.size() << std::endl;
	unsigned int nbins = 400;
	float start=-100;
	float end = 100;
//	TH2F *hist2D_Layer0 = new TH2F("HitPointOnLayer_0","HitPointOnLayer_0",200,-50,50,200,-50,50);
	TH2F *hist2D_Layer1 = new TH2F("HitPointOnLayer_1","HitPointOnLayer_1",nbins,start,end,nbins,start,end);
//	TH2F *hist2D_Layer2 = new TH2F("HitPointOnLayer_2","HitPointOnLayer_2",200,-50,50,200,-50,50);
	TH2F *hist2D_Layer3 = new TH2F("HitPointOnLayer_3","HitPointOnLayer_3",nbins,start,end,nbins,start,end);
//	TH2F *hist2D_Layer4 = new TH2F("HitPointOnLayer_4","HitPointOnLayer_4",200,-50,50,200,-50,50);
	TH2F *hist2D_Layer5 = new TH2F("HitPointOnLayer_5","HitPointOnLayer_5",200,-50,50,200,-50,50);
	TH2F *hist2D_Layer7 = new TH2F("HitPointOnLayer_7","HitPointOnLayer_7",200,-50,50,200,-50,50);
	TH2F *hist2D_Layer8 = new TH2F("HitPointOnLayer_8","HitPointOnLayer_8",nbins,start,end,nbins,start,end);
	TH2F *hist2D_Layer9 = new TH2F("HitPointOnLayer_9","HitPointOnLayer_9",200,-50,50,200,-50,50);


	for(unsigned int i = 0 ; i < smtVec.size() ; i++){

				
		{
			unsigned int inspectedLayerIndex=8;
			unsigned int hittBarIndex = 10000;
			ushort startIndex = GetStartIndex(inspectedLayerIndex);
			ushort endIndex = GetEndIndex(inspectedLayerIndex);
			bool check = smtVec[i]->CheckTrackForLayerNum(startIndex,hittBarIndex);
			lite_interface::ScintillatorBar_V2 *scintStart;
			lite_interface::ScintillatorBar_V2 *scintEnd;
			if(check){
				scintStart = smtVec[i]->GetScintillator(hittBarIndex);
			}
			check &= smtVec[i]->CheckTrackForLayerNum(endIndex,hittBarIndex);
			if(check){
				scintEnd = smtVec[i]->GetScintillator(hittBarIndex);
			}
			check &= smtVec[i]->CheckTrackForLayerNum(inspectedLayerIndex,hittBarIndex);
			if(check){
				lite_interface::ScintillatorBar_V2 *scint = smtVec[i]->GetScintillator(hittBarIndex);
				lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer_Refined(smtVec[i],inspectedLayerIndex);
				if(hitPoint->GetX() < 9000. && hitPoint->GetZ() < 9000 
				&&  scintStart->GetBarIndexInLayer()==scintEnd->GetBarIndexInLayer()
				//&& (scintStart->GetBarIndexInLayer()==1 || scintStart->GetBarIndexInLayer()==7)
				&& scint->GetBarIndexInLayer()==4
				){
					   hist2D_Layer8->Fill(hitPoint->GetX(),hitPoint->GetZ());
			}
			}

		}
	



	}

	TCanvas *canLayer8 = new TCanvas("Layer8","Layer8");
	hist2D_Layer8->Draw("colz");
	std::string subExeName=std::string(argv[0]).substr(std::string(argv[0]).find("_"));
	std::string matWithExt = filename.substr(13);
	TFile *fp = new TFile(("HitPattern"+subExeName+matWithExt).c_str(),"RECREATE");
	fp->cd();
	hist2D_Layer8->Write();

	fp->Close();


	fApp->Run();


	return 0;
}


