/*
 * TestMuonEnergyCalib.cpp
 *
 *  Created on: 26-Dec-2020
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
#include "Histograms.h"
#include "PsBar.h"
#include "TFile.h"

int main(int argc, char *argv[]){
	GenerateScintMatrixXYCenters();
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	ushort barIndex = 25;
	lite_interface::Calibration *calib = lite_interface::Calibration::instance("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib2.root");

	lite_interface::Analyzer pf(argv[1]);
	std::vector<lite_interface::ScintillatorBar_V2*> vecOfScintillatorBars = pf.GetVectorOfScintillators();

	unsigned int numOfBars = numOfLayers*numOfBarsInEachLayer;

	TFile *fp = new TFile("QMean.root","RECREATE");
	//(new TCanvas("QMean","QMean"))->SetLogy();
	for(unsigned short int barIndex = 0 ; barIndex < numOfBars ; barIndex++ ){
		std::string barName = vecOfBarsNamess[barIndex];
		(new TCanvas(barName.c_str(),barName.c_str()))->SetLogy();
		TH1F *histQmeanCorrected = PlotQMeanCorrected(vecOfScintillatorBars,barIndex);
		histQmeanCorrected->SetLineColor(kGreen);
		//histQmeanCorrected->Draw();
		histQmeanCorrected->Write();
	}

	fp->Close();


	/*std::vector<Histograms*> histogramsVec;

	for(unsigned short int barIndex = 0 ; barIndex < numOfBars ; barIndex++ ){
		histogramsVec.push_back( new Histograms(vecOfBarsNamess[barIndex],barIndex ));
	}

	for(unsigned long int i = 0 ; i < vecOfScintillatorBars.size() ; i++){
		//std::cout << "ScintillatorBAR : " << vecOfScintillatorBars[i]->fBarIndex << std::endl;
		histogramsVec[vecOfScintillatorBars[i]->fBarIndex]->FillHistogram(vecOfScintillatorBars[i]);
	}


	std::cout << "@@@@@@@@@@@ Size of HIST-VECTOR : " << histogramsVec.size() <<" @@@@@@@@@@" << std::endl;

	for(unsigned int i = 0 ; i < histogramsVec.size() ; i++){

		new TCanvas( (histogramsVec[i]->fBarName).c_str(),(histogramsVec[i]->fBarName).c_str());
		histogramsVec[barIndex]->fhistQMeanCorrected->Draw();
	}*/

	fApp->Run();
}

