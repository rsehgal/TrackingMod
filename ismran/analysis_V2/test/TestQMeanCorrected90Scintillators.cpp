/*
 * TestQMeanCorrected90Scintillators.cpp
 *
 *  Created on: 22-Mar-2021
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

	lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");

	lite_interface::Analyzer analyzerObj(argv[1]);
	std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec = analyzerObj.GetVectorOfScintillators();

	/*std::vector<Histograms*> vecOfHists;
	for(unsigned int i = 0 ; i < vecOfBarsNamess.size() ; i++){
		vecOfHists.push_back(new Histograms(vecOfBarsNamess[i],i));
	}*/

	std::vector<TH1F*> vecOfHists;
	for(unsigned int i = 0 ; i < vecOfBarsNamess.size() ; i++){
		vecOfHists.push_back(new TH1F(vecOfBarsNamess[i].c_str(),vecOfBarsNamess[i].c_str(),500,0,50));
	}


	std::vector<lite_interface::ScintillatorBar_V2*>::iterator itr;
	for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
		vecOfHists[(*itr)->GetBarIndex()]->Fill((*itr)->GetQMeanCorrected());
	}

	std::cout << "Required histograms filledd...." << std::endl;

	new TCanvas("QMean_90_Scintillators","QMean_90_Scintillators");
	for(unsigned int i = 0 ; i < vecOfBarsNamess.size() ; i++){
		vecOfHists[i]->Draw("same");
	}

	fApp->Run();
}
