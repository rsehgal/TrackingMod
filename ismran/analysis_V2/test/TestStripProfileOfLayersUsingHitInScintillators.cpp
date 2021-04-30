/*
 * TestStripProfileOfLayersUsingHitInScintillators.cpp
 *
 *  Created on: 24-Mar-2021
 *      Author: rsehgal
 */

#include "PairFinder.h"
#include "Analyzer.h"
#include "Calibration.h"
#include <TApplication.h>
#include <TCanvas.h>
#include "Plotter.h"
#include "includes.hh"
#include "HardwareNomenclature.h"



int main(int argc, char *argv[]){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	GenerateScintMatrixXYCenters();

	//ushort barIndex = std::atoi(argv[2]);
	lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
	lite_interface::Analyzer::numOfShots = 1;
	lite_interface::Analyzer::shotNo = 1;
	lite_interface::PairFinder::numOfShots = lite_interface::Analyzer::numOfShots;
	lite_interface::PairFinder::shotNo = lite_interface::Analyzer::shotNo;
	//lite_interface::PairFinder pairFinder =  new lite_interface::PairFinder(argv[1]);
	lite_interface::Analyzer analyzerObj(argv[1],false);
	std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec = analyzerObj.GetVectorOfScintillators();

/*
	TH1F *stripProfile0 = lite_interface::PlotStripProfileOfLayer(scintBarVec,0);
	TH1F *stripProfile1 = lite_interface::PlotStripProfileOfLayer(scintBarVec,1);
	TH1F *stripProfile2 = lite_interface::PlotStripProfileOfLayer(scintBarVec,2);
	TH1F *stripProfile3 = lite_interface::PlotStripProfileOfLayer(scintBarVec,3);
	TH1F *stripProfile4 = lite_interface::PlotStripProfileOfLayer(scintBarVec,4);
*/


	/*TH1F *stripProfile7 = lite_interface::PlotStripProfileOfLayer(smtVecUsefulTracks,7);
		TH1F *stripProfile6 = lite_interface::PlotStripProfileOfLayer(smtVecUsefulTracks,6);
		TH1F *stripProfile5 = lite_interface::PlotStripProfileOfLayer(smtVecUsefulTracks,5);*/

/*
	new TCanvas("StripProfile_Layer0","StripProfile_Layer0");
	stripProfile0->Draw();

	new TCanvas("StripProfile_Layer1","StripProfile_Layer1");
	stripProfile1->Draw();

	new TCanvas("StripProfile_Layer2","StripProfile_Layer2");
	stripProfile2->Draw();

	new TCanvas("StripProfile_Layer3","StripProfile_Layer3");
	stripProfile3->Draw();

	new TCanvas("StripProfile_Layer4","StripProfile_Layer4");
	stripProfile4->Draw();
*/

	TH1F *stripHist = new TH1F("StipProfile_90_Detectors","StipProfile_90_Detectors",90,0,90);
	for(unsigned int i = 0 ; i < scintBarVec.size() ; i++){
		stripHist->Fill(scintBarVec[i]->GetBarIndex());
	}

	stripHist->Draw();
	fApp->Run();
}
