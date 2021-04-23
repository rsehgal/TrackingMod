/*
 * BoardRatesn.cpp
 *
 *  Created on: 22-April-2021
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

		TH1F *boardsHist = new TH1F ("BoardsHistogram","BoardsHistogram",15,0,15);

		int counter = 0;
		nbytes += dataTree->GetEntry(0);
		ULong64_t tstampStart = tstamp;
		UInt_t timeStart = time;
		for (Long64_t i=1; i<nentries;i++) {
				nbytes += dataTree->GetEntry(i);
				boardsHist->Fill(brch/16);

					/*Long64_t timeElapsedWall = time-timeStart;
					Long64_t timeElapsedDaq = tstamp-tstampStart;
					timeElapsedDaq/=pow(10,12);
					Long64_t diff = timeElapsedWall-timeElapsedDaq;
					//if(std::fabs(timeElapsedWall-timeElapsedDaq) < 5 )
					if(std::fabs(diff) < 5 )
					{
						//outfile << std::setw(10) << "OK : " << std::setw(8) << brch << " , " << std::setw(20) << tstamp << " , " << std::setw(8) <<  qlong << " , " << std::setw(15) << time
						//																  << " : Time Elapsed (Wall) : " << std::setw(10) << timeElapsedWall
						//																  << " : Time Elapsed (DAQ) : " << std::setw(10) <<  timeElapsedDaq << std::endl;
					}
					else{
						std::cout << std::setw(10) << "NOTOK : " << std::setw(8) << brch << " , " << std::setw(20) << tstamp << " , " << std::setw(8) <<  qlong << " , " << std::setw(15) << time
																												  << " : Time Elapsed (Wall) : " << std::setw(10) << timeElapsedWall
																												  << " : Time Elapsed (DAQ) : " << std::setw(10) <<  timeElapsedDaq << std::endl;
						return 1;

					}*/

		}

		new TCanvas("Can_Boards","Can_Boards");
		boardsHist->Draw();
		fApp->Run();
		return 0;

}
