/*
 * MuonTrackAnalysis_StripProfileAnalysis.cpp
 *
 *  Created on: 26-Apr-2021
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
	std::vector<unsigned short> layerIndexVector={0,1,2,3,4,7,8,9};

	/*for(unsigned int i = 0 ; i < layerIndexVector.size() ; i++){
		std::string canName = "Can_Layer_"+std::to_string(layerIndexVector[i]);
		new TCanvas(canName.c_str(),canName.c_str());
		lite_interface::PlotStripProfileOfLayer(smtVec,layerIndexVector[i])->Draw();
	}*/

	std::string canName = "Can_";
	for(unsigned int i = 0 ; i < layerIndexVector.size() ; i++){
		canName+=std::to_string(layerIndexVector[i]);
	}
	canName += "Layers";
	new TCanvas(canName.c_str(),canName.c_str());

	std::string outfileName = "StripProfile_";
	outfileName += filename.substr(13);
	TFile *fp=new TFile(outfileName.c_str(),"RECREATE");
	fp->cd();
	TH1F *histStripProfile = lite_interface::PlotStripProfileOfVectorOfLayer(smtVec,layerIndexVector);
	histStripProfile->Write();
	histStripProfile->Draw();
	fp->Close();
	std::cout << "File Written and closed........." << std::endl;


	fApp->Run();
}
