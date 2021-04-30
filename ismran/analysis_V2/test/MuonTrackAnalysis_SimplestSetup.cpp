/*
 * MuonTrackAnalysis_SimplestSetup.cpp
 *
 *  Created on: 27-Apr-2021
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
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	GenerateScintMatrixXYCenters();
	lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
	std::string filename = argv[1];
	std::vector<lite_interface::SingleMuonTrack*> smtVec = GetMuonTracksVector(filename);
	std::vector<unsigned int> scintIndexVector={53,70,88};
	std::string name = "Scint_";
	for(unsigned int i = 0 ; i < scintIndexVector.size() ; i++){
		name+=std::to_string(scintIndexVector[i]);
	}
	std::string histName2D=name+"Hist_2D";
	TH2F *hist2D = new TH2F(histName2D.c_str(),histName2D.c_str(),200,-50.,50.,200, -50.,50.);
	std::string histName1D=name+"Hist_1D";
	TH1F *hist1D = new TH1F(histName1D.c_str(),histName1D.c_str(),5,-50.,50.);
	unsigned int counter=0;
	for(unsigned int i = 0 ; i < smtVec.size() ; i++){
		//std::cout << "=========================================================== " << std::endl;
		//smtVec[i]->Print();
		if(smtVec[i]->CheckTrackForRequiredScintillators(scintIndexVector)){
			counter++;
			std::vector<lite_interface::ScintillatorBar_V2*> scintVect = smtVec[i]->GetMuonTrack();
			//if(std::fabs(scintVect[scintVect.size()-1]->GetDelTCorrected()-scintVect[0]->GetDelTCorrected()) < 2000){
			//if(std::fabs(scintVect[scintVect.size()-1]->GetTNearCorr()-scintVect[0]->GetTNearCorr()) < 2000)
			{
				for(unsigned int j = 0 ; j < scintVect.size() ; j++){
					if(scintVect[j]->GetBarIndex()==53){
						lite_interface::Point3D* hitPt = scintVect[j]->EstimateHitPosition_Param();
						//hitPt->Print();
						hist2D->Fill(hitPt->GetX(),hitPt->GetZ());
						hist1D->Fill(hitPt->GetZ());
					}

				}
			}
		}

	}

	std::cout << "Number of Tracks that passes through required scintillators : "  << counter << std::endl;
	
	std::string canName2D = name+"Can_2D";
	new TCanvas(canName2D.c_str(),canName2D.c_str());
	hist2D->Draw();
	std::string canName1D = name+"Can_1D";
	new TCanvas(canName1D.c_str(),canName1D.c_str());//	std::string canName = "Can_Scint_";
	hist1D->Draw();                              //	for(unsigned int i = 0 ; i < scintIndexVector.size() ; i++){
//		canName+=std::to_string(scintIndexVector[i]);
//	}
//	canName += "Indices";
//	new TCanvas(canName.c_str(),canName.c_str());
//	lite_interface::PlotStripProfileOfVectorOfLayer(smtVec,scintIndexVector)->Draw();
//
	fApp->Run();
}
