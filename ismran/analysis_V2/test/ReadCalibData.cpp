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
int main(){
	gStyle->SetOptStat(0);
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	//std::vector<int> posVec={-45,-40,-30,-20,-10,0,10,20,30,40,45};
	std::vector<int> posVec={-45,-30,0,30,45};
	new TCanvas();
	TLegend *legend = new TLegend();
	for(unsigned int i = 0 ; i < posVec.size() ; i++){
		std::string filename="";
		if(posVec[i] < 0)
			filename="/home/rsehgal/BackBoneSoftwares/ismranData/PS71_Cs137/Cs137_PS71_SF874_Couples_N1802V_Th6_F1939V_Th6_";
		else
			filename="/home/rsehgal/BackBoneSoftwares/ismranData/PS71_Cs137/Cs137_PS71_SF874_Couples_N1802V_Th6_F1939V_Th6_+";

		std::string location=std::to_string(posVec[i])+"cm";
		std::string secondHalf = location+".root";
		filename += secondHalf;
		std::cout <<"Processing file : " << filename << std::endl;

		TFile *fp = new TFile(filename.c_str(), "r");
		TTree *tr = (TTree*) fp->Get("ftree");

		int delt;

		tr->SetBranchAddress("fDelt", &delt);
		Long64_t nEntries = tr->GetEntries();
		Long64_t nb = 0;
		const int offset=0;
		TH1F *delTHist = new TH1F(location.c_str(),location.c_str(),100,-20,20);
		for (Long64_t iev = 0; iev < nEntries; iev++) {
			nb += tr->GetEntry(iev);
			delTHist->Fill(delt/1000.);
		}
		legend->AddEntry(delTHist);
		delTHist->Scale(1/delTHist->Integral());
		if(i==4)
			delTHist->SetLineColor(i+2);
		else
			delTHist->SetLineColor(i+1);
		delTHist->Draw("same");

	}
	legend->Draw("same");

	fApp->Run();
}

