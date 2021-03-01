/*
 * SystemHealthComparison.cpp
 *
 *  Created on: 11-Feb-2021
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

	std::vector<std::string> vecOfDates = {"14Jan - Without Scatterer","18Jan - Aluminium","22Jan - Iron","03Feb - Lead"};
	std::vector<std::string> vecOfFileNames={"Cosmic-SystemHealth-WithoutScatter-22Feb-14Jan.root", "Cosmic-SystemHealth-Al-22Feb-18Jan.root",
											 "Cosmic-SystemHealth-Fe-22Feb-22Jan.root","Cosmic-SystemHealth-Pb-22Feb-03Feb.root"};//,"SystemHealth-Fe-03Feb.root"};

	std::vector<TFile*> vecOfFilePointers;

	for(unsigned int i = 0 ; i < vecOfFileNames.size() ; i++){
		vecOfFilePointers.push_back(new TFile(vecOfFileNames[i].c_str(),"READ"));
	}
	std::string rmfiles="";
	for(unsigned int i = 0 ; i < vecOfBarsNamess.size() ; i++){
		TCanvas *can = new TCanvas(vecOfBarsNamess[i].c_str(),vecOfBarsNamess[i].c_str());
		can->Divide(2,3);

		{TLegend *leg = new TLegend();
		(can->cd(1))->SetLogy();
		for(unsigned int j = 0 ; j < vecOfFileNames.size() ; j++){
			std::string histName = vecOfBarsNamess[i]+"-Near";
			((TH1D*)vecOfFilePointers[j]->Get(histName.c_str()))->SetLineColor(j+1);
			((TH1D*)vecOfFilePointers[j]->Get(histName.c_str()))->Draw("same");
			leg->AddEntry(((TH1D*)vecOfFilePointers[j]->Get(histName.c_str())),vecOfDates[j].c_str(),"l");
			leg->Draw("same");
		}
		}

		{TLegend *leg = new TLegend();
		(can->cd(2))->SetLogy();
		for(unsigned int j = 0 ; j < vecOfFileNames.size() ; j++){
			std::string histName = vecOfBarsNamess[i]+"-Far";
			((TH1D*)vecOfFilePointers[j]->Get(histName.c_str()))->SetLineColor(j+1);
			((TH1D*)vecOfFilePointers[j]->Get(histName.c_str()))->Draw("same");
			leg->AddEntry(((TH1D*)vecOfFilePointers[j]->Get(histName.c_str())),vecOfDates[j].c_str(),"l");
			leg->Draw("same");
		}
		}

		{TLegend *leg = new TLegend();
		(can->cd(3))->SetLogy();
		for(unsigned int j = 0 ; j < vecOfFileNames.size() ; j++){
			std::string histName = vecOfBarsNamess[i]+"-QMean";
			((TH1D*)vecOfFilePointers[j]->Get(histName.c_str()))->SetLineColor(j+1);
			((TH1D*)vecOfFilePointers[j]->Get(histName.c_str()))->Draw("same");
			leg->AddEntry(((TH1D*)vecOfFilePointers[j]->Get(histName.c_str())),vecOfDates[j].c_str(),"l");
			leg->Draw("same");
		}
		}

		{TLegend *leg = new TLegend();
		(can->cd(4))->SetLogy();
		for(unsigned int j = 0 ; j < vecOfFileNames.size() ; j++){
			std::string histName = vecOfBarsNamess[i]+"-QMeanCorrected";
			((TH1D*)vecOfFilePointers[j]->Get(histName.c_str()))->SetLineColor(j+1);
			((TH1D*)vecOfFilePointers[j]->Get(histName.c_str()))->Draw("same");
			leg->AddEntry(((TH1D*)vecOfFilePointers[j]->Get(histName.c_str())),vecOfDates[j].c_str(),"l");
			leg->Draw("same");
		}
		}

		{TLegend *leg = new TLegend();
		(can->cd(5))->SetLogy();
		for(unsigned int j = 0 ; j < vecOfFileNames.size() ; j++){
			std::string histName = vecOfBarsNamess[i]+"-DelT";
			((TH1D*)vecOfFilePointers[j]->Get(histName.c_str()))->SetLineColor(j+1);
			((TH1D*)vecOfFilePointers[j]->Get(histName.c_str()))->Draw("same");
			leg->AddEntry(((TH1D*)vecOfFilePointers[j]->Get(histName.c_str())),vecOfDates[j].c_str(),"l");
			leg->Draw("same");
		}
		}

		{TLegend *leg = new TLegend();
		(can->cd(6))->SetLogy();
		for(unsigned int j = 0 ; j < vecOfFileNames.size() ; j++){
			std::string histName = vecOfBarsNamess[i]+"-DelTCorrected";
			((TH1D*)vecOfFilePointers[j]->Get(histName.c_str()))->SetLineColor(j+1);
			((TH1D*)vecOfFilePointers[j]->Get(histName.c_str()))->Draw("same");
			leg->AddEntry(((TH1D*)vecOfFilePointers[j]->Get(histName.c_str())),vecOfDates[j].c_str(),"l");
			leg->Draw("same");
		}
		}

		std::string canFileName = vecOfBarsNamess[i]+".pdf";
		rmfiles += (canFileName+" ");
		can->Print(canFileName.c_str());

		delete can;
	}

	std::string rmcommand="rm -rf "+rmfiles;
	std::string mergecommand="pdfunite "+rmfiles+"SystemHealth-Comparison.pdf";
	system(mergecommand.c_str());
	system(rmcommand.c_str());
}
