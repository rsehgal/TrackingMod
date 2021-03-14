/*
 * ReadCalibData.cpp
 *
 *  Created on: 29-Dec-2020
 *      Author: rsehgal
 */


#include <iostream>
#include "includes.hh"
#include <TFitResult.h>
#include <TH1F.h>
#include <TTree.h>
#include <TFile.h>
#include <vector>
#include <TApplication.h>
#include <TStyle.h>
int main(int argc, char *argv[]){
	gStyle->SetOptStat(0);
	TApplication *fApp = new TApplication("Test", NULL, NULL);

	new TCanvas();
	//TLegend *legend = new TLegend();
	std::string filename = argv[1];
		TFile *fp = new TFile(filename.c_str(), "r");
		TTree *tr = (TTree*) fp->Get("ftree");

		int delt;
		//ULong64_t delt;

		tr->SetBranchAddress("fDelt", &delt);
		Long64_t nEntries = tr->GetEntries();
		Long64_t nb = 0;
		const int offset=0;
		TH1F *delTHist = new TH1F("delTHist","delTHist",100,-20,20);
		for (Long64_t iev = 0; iev < nEntries; iev++) {
			nb += tr->GetEntry(iev);
			delTHist->Fill(delt/1000.);
		}
		/*legend->AddEntry(delTHist);
		delTHist->Scale(1/delTHist->Integral());
		if(i==4)
			delTHist->SetLineColor(i+2);
		else
			delTHist->SetLineColor(i+1);*/
		delTHist->Draw("same");


	//legend->Draw("same");

	fApp->Run();
}

