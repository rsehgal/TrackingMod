/*
 * TestScintillatorHealth.cpp
 *
 *  Created on: 03-Feb-2021
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

	ushort barIndex = std::atoi(argv[2]);
	lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
	lite_interface::Analyzer::numOfShots = 1;
	lite_interface::Analyzer::shotNo = 1;
	lite_interface::PairFinder::numOfShots = lite_interface::Analyzer::numOfShots;
	lite_interface::PairFinder::shotNo = lite_interface::Analyzer::shotNo;

	//lite_interface::PairFinder pairFinder =  new lite_interface::PairFinder(argv[1]);
	lite_interface::Analyzer analyzerObj(argv[1]);
	std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec = analyzerObj.GetVectorOfScintillators();


	TH1F *qNear = lite_interface::PlotQNear(scintBarVec, barIndex);
	TH1F *qFar = lite_interface::PlotQFar(scintBarVec, barIndex);
	TH1F *qMean = lite_interface::PlotQMean(scintBarVec, barIndex);
	TH1F *qMeanCorrected = lite_interface::PlotQMeanCorrected(scintBarVec, barIndex);

	(new TCanvas("Q","Q"))->SetLogy();
	qNear->Draw();
	qFar->SetLineColor(6);
	qFar->Draw("same");
	qMean->SetLineColor(kGreen);
	qMean->Draw("same");
	qMeanCorrected->SetLineColor(1);
	qMeanCorrected->Draw("same");

	new TCanvas("Corrected DelT","Corrected DelT");
	TH1F *delTCorrected = lite_interface::PlotDelTCorrected(scintBarVec, barIndex);
	delTCorrected->Draw();

	new TCanvas("DelTvsZ","DelTvsZ");
	//TGraph *delTvsZ = lite_interface::PlotDelTvsZ(scintBarVec, barIndex);
	TH2F *delTvsZ = lite_interface::PlotDelTvsZ(scintBarVec, barIndex);
	delTvsZ->Draw("colz");


	/*
	 * CHECKING STRIP) PROFILE
	 */
	TH1F *stripProfile = lite_interface::PlotStripProfileOfLayer(scintBarVec,0);
	new TCanvas();
	stripProfile->Draw();


	TH1F *stripProfile1 = lite_interface::PlotStripProfileOfLayer(scintBarVec,1);
	new TCanvas();
	stripProfile1->Draw();

	TH1F *stripProfile2 = lite_interface::PlotStripProfileOfLayer(scintBarVec,2);
	new TCanvas();
	stripProfile2->Draw();

	TH1F *stripProfile3 = lite_interface::PlotStripProfileOfLayer(scintBarVec,3);
	new TCanvas();
	stripProfile3->Draw();

/*	TH1F *stripProfile4 = lite_interface::PlotStripProfileOfLayer(smtVec,4);
	new TCanvas();
	stripProfile4->Draw();

	TH1F *stripProfile5 = lite_interface::PlotStripProfileOfLayer(smtVec,5);
	new TCanvas();
	stripProfile5->Draw();

	TH1F *stripProfile6 = lite_interface::PlotStripProfileOfLayer(smtVec,6);
	new TCanvas();
	stripProfile6->Draw();

	TH1F *stripProfile7 = lite_interface::PlotStripProfileOfLayer(smtVec,7);
	new TCanvas();
	stripProfile7->Draw();*/
	fApp->Run();


	return 0;
}
