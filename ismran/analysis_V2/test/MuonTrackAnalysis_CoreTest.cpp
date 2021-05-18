/*
 * MuonTrackAnalysis_SimplestSetup.cpp
 *
 *  Created on: 04-May-2021
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
	std::cout << "NumOfMuonTracks Read : " << numOfEventsToRead << std::endl;
	std::vector<unsigned int> scintIndexVector={53,70,88};
	
	unsigned int counter = 0;
	for(unsigned int i = 0 ; i < smtVec.size() ; i++){
		if(smtVec[i]->CheckTrackForRequiredScintillators(scintIndexVector))
			counter++;
	}

	std::string scintIds="Scintillators_";
	for(unsigned int i = 0 ; i < scintIndexVector.size() ; i++){
		scintIds+=(std::to_string(i)+"_");
	}
	std::cout << scintIds << " : " << counter << std::endl; 

	//fApp->Run();
}
