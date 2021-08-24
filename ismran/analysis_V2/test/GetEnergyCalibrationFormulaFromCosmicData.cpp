/*
 * GetEnergyCalibrationFormulaFromCosmicData.cpp
 *
 *  Created on: 10-Feb-2021
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

int main(int argc, char *argv[]){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	std::string filename = argv[1];
	ushort barIndex = std::atoi(argv[2]);

	lite_interface::Analyzer analyzerObj(filename);
	std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec = analyzerObj.GetVectorOfScintillators();

	std::vector<lite_interface::ScintillatorBar_V2*>::iterator itr;
	TH1D *enerHist = new TH1D ("EnergyHist", "EnergyHist",1000,0,40000);
	TF1 *g1    = new TF1("g1","gaus",500,800);
	TF1 *g2    = new TF1("g2","gaus",800,1100);
	for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
		double qlongmean = (*itr)->GetQLongMean();

		if((*itr)->GetBarIndex() == barIndex){
			enerHist->Fill(qlongmean);
			std::cout << "QMean : " << (*itr)->GetQLongMean() << std::endl;
		}
	}

	unsigned int nbins = 1000;
	unsigned int xmin = 0;
	unsigned int xmax = 40000;
	 TH1F *d1 = new TH1F("d1","",nbins,xmin,xmax);
	 Double_t source[nbins];
	   TSpectrum *s = new TSpectrum();

	   for (unsigned int i = 0; i < nbins; i++) source[i]=enerHist->GetBinContent(i + 1);
	   s->Background(source,nbins,10,TSpectrum::kBackDecreasingWindow,
	                 TSpectrum::kBackOrder8,kTRUE,
	                 TSpectrum::kBackSmoothing5,kTRUE);
	   for (unsigned int i = 0; i < nbins; i++) d1->SetBinContent(i + 1,source[i]);
	   d1->SetLineColor(kRed);



	//enerHist->Fit(g1,"R");
	//enerHist->Fit(g2,"R+");
	//enerHist->Fit(g3,"R+");

	new TCanvas("EnerHist","EnerHist");
	enerHist->Draw();
	d1->Draw("SAME L");

	fApp->Run();


	return 0;

}

