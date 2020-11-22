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
#include "MuonReader.h"
int main(int argc, char *argv[]){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	std::string inputfileName = argv[1];// std::string(argv[1]);

	lite_interface::MuonReader::instance(inputfileName);
	std::vector<Muon*> muonVec;
	for(unsigned int i = 0 ; i < std::atoi(argv[2]) ; i++){
		Muon *muon = new Muon(*lite_interface::MuonReader::instance()->GetMuon());
		//std::cout << "From MuonReader_V2 : " ;muon->Print();
		muonVec.push_back(muon);
	}

	new TCanvas("MuonAngularDistribution","CRY Muon angular distribution");
	TH1F *anghist = GetDistribution(muonVec);
	anghist->Draw();

	fApp->Run();
	return 0;
}