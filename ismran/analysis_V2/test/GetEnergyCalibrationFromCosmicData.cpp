/*
 * GetEnergyCalibrationFromCosmicData.cpp
 *
 *  Created on: 16-Feb-2021
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
	unsigned int barIndex = 4;

	/*lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
	std::string filename = argv[1];
	lite_interface::Analyzer analyzerObj(filename);
	std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec = analyzerObj.GetVectorOfScintillators();

	TH1F *histQMean = lite_interface::PlotQMean(scintBarVec,barIndex);

	int npeaks = 2;
	TSpectrum *s = new TSpectrum(2*npeaks);


	(new TCanvas())->SetLogy();
	histQMean->Draw();
	TFile *fp = new TFile("QmeanPeakTest.root","RECREATE");
	fp->cd();
	histQMean->Write();
	fp->Close();
	Int_t nfound = s->Search(histQMean,2,"",0.10);
*/
	TFile *enerFile = new TFile("EnergyCalib.root","RECREATE");
	std::string filename = argv[1];
	lite_interface::Analyzer analyzerObj(filename);
	std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec = analyzerObj.GetVectorOfScintillators();

	unsigned int barVecSize = vecOfBarsNamess.size();
	//barVecSize = 1;
	for(unsigned int i = 0 ; i < barVecSize ; i++){
	//(new TCanvas())->SetLogy();
	//TFile *fp = new TFile("QmeanPeakTest.root","READ");
	//TH1F *hist = ((TH1F*)fp->Get("PS41_QMean"));
	TH1F *hist = lite_interface::PlotQMean(scintBarVec,i,true);

	/*TF1 *g1    = new TF1("g1","gaus",0.3,0.6);
	TF1 *g2    = new TF1("g2","gaus",0.7,0.9);
	TF1 *g3    = new TF1("g3","gaus",5,12);*/

	TF1 *g1    = new TF1("g1","gaus",300,600);
	TF1 *g2    = new TF1("g2","gaus",700,900);
	TF1 *g3    = new TF1("g3","gaus",5000,12000);

	hist->Fit(g1,"R");
	double posK = g1->GetParameter(1);
	hist->Fit(g2,"R+");
	double posTl = g2->GetParameter(1);
	hist->Fit(g3,"R+");
	double posMu = g3->GetParameter(1);

	hist->Draw();

	std::vector<double> peakPos={posK,posTl,posMu};
	std::vector<double> peakEner={1.46,2.61,20.0};

	TGraph *gr = new TGraph(peakPos.size(),&peakPos[0],&peakEner[0]);
	gr->SetMarkerStyle(20);
	//TF1 *enerParam = new TF1((vecOfBarsNamess[barIndex]+"_Energy_F").c_str(),"pol1",0,30);
	TF1 *enerParam = new TF1((vecOfBarsNamess[i]+"_Energy_F").c_str(),"pol1",0,30000);
	gr->Fit(enerParam);

/*
	new TCanvas();
	gr->Draw("ap");
	enerParam->Draw("same");
*/

	//TFile *enerFile = new TFile("EnergyCalib.root","RECREATE");

	enerFile->cd();
	hist->Write();
	enerParam->Write();

	//delete enerFile;
	delete enerParam;
	delete gr;
	delete hist;
	delete g1;
	delete g2;
	delete g3;
	}



	enerFile->Close();
	delete enerFile;







	fApp->Run();
	return 0;
}
