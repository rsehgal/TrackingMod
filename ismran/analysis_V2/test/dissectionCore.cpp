/*
 * dissection.cpp
 *
 *  Created on: 02-Mar-2021
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

int main(int argc, char *argv[]){
		TApplication *fApp = new TApplication("Test", NULL, NULL);
		lite_interface::SingleMuonTrack *smt = new lite_interface::SingleMuonTrack;

		lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
		std::string filename = argv[1];

		UShort_t brch; //! board #  and channel number ( its packed in as follows )	//! board*16 + chno.
		UInt_t qlong; //! integrated charge in long gate 88 nsec
		ULong64_t tstamp; //! time stamp in pico sec.
		UInt_t time; //! real computer time in sec

		TFile *dataFile = new TFile(filename.c_str(),"READ");
		TTree *dataTree = (TTree*)dataFile->Get("ftree");


		dataTree->SetBranchAddress("fBrCh", &brch);
		dataTree->SetBranchAddress("fQlong", &qlong);
		dataTree->SetBranchAddress("fTstamp", &tstamp);
		dataTree->SetBranchAddress("fTime", &time);



		Long64_t nentries = dataTree->GetEntries();
		std::cout << "Number of Entries in specified file : " << nentries << std::endl;
		Long64_t nbytes = 0;




		int counter = 0;
		for (Long64_t i=0; i<nentries;i++) {
				nbytes += dataTree->GetEntry(i);
				std::cout << brch << " , " << qlong << " , " << tstamp << " , " << time << std::endl;
		}

				fApp->Run();

		return 0;

}
