/*
 * MuonRead.cpp
 *
 *  Created on: 21-Nov-2020
 *      Author: rsehgal
 */

#include <TF1.h>
#include <TH1F.h>
#include <TTree.h>
#include <TFile.h>
#include <TApplication.h>
#include <TVector3.h>
#include <string>
#include <cstring>
#include "cry_manual_interface.h"

int main(int argc, char *argv[]){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	std::string inputfileName = argv[1];// std::string(argv[1]);
	TFile *muonFile = new TFile(inputfileName.c_str(),"READ");
	TTree *muonTree = (TTree*)muonFile->Get("MuonTree");

	double anglex = 0 ;
	double angley = 0 ;
	double anglez = 0 ;
	double energy = 0 ;

	muonTree->SetBranchAddress("u",&anglex);
	muonTree->SetBranchAddress("v",&angley);
	muonTree->SetBranchAddress("w",&anglez);
	muonTree->SetBranchAddress("e",&energy);

	Long64_t nentries = muonTree->GetEntries();

	Long64_t nbytes = 0;
	std::vector<Muon*> muonVec;
	for (Long64_t i=0; i<nentries;i++) {

		nbytes += muonTree->GetEntry(i);
		muonVec.push_back(new Muon(anglex,angley,anglez,energy));

	}

	new TCanvas("MuonAngularDistribution","CRY Muon angular distribution");
	TH1F *anghist = GetDistribution(muonVec);
	anghist->Draw();

	fApp->Run();
	return 0;
}


