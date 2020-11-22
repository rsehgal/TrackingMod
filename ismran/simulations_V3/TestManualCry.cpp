/*
 * TestManualCry.cpp
 *
 *  Created on: 21-Nov-2020
 *      Author: rsehgal
 */

#include "cry_manual_interface.cpp"
#include <iostream>
#include <TCanvas.h>
#include <TH1F.h>
#include <TApplication.h>

int main(){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	unsigned int n = 10000000;
	std::vector<Muon*> vecOfMuon = SampleMuonParticle(n);
	//CRYParticle *muon = SampleMuonParticle(n);
	//std::cout << "(" << muon->u() <<" , " << muon->v() << " , " << muon->w() << ")" << std::endl;
	/*for(unsigned int i = 0 ; i < vecOfMuon.size() ; i++ ){
		std::cout << "Event num : " << i << " : ";vecOfMuon[i]->Print();
	}*/

	new TCanvas("MuonAngularDistribution","CRY Muon angular distribution");
	TH1F *anghist = GetDistribution(vecOfMuon);
	anghist->Draw();

	Save(vecOfMuon);

	fApp->Run();
	return 0;
}

