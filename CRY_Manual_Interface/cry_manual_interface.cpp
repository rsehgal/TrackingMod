/*
 * cry_manual_interface.h
 *
 *  Created on: 21-Nov-2020
 *      Author: rsehgal
 */

#include "CRYGenerator.h"
#include "CRYSetup.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>  // For Ubuntu Linux

#include <TH1F.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TTree.h>
#include <TFile.h>
#include "cry_manual_interface.h"


TH1F* GetDistribution(std::vector<Muon*> muonVec){
		int numbins = 40;
		TH1F *angHist = new TH1F("MuonAngularDistribution", "CRY Muons angular distribution",numbins,0,M_PI/2.);
		TVector3 ref(0.,-1.,0.);
		for(unsigned int i = 0 ; i < muonVec.size() ; i++){
			double angle = TVector3(muonVec[i]->angleX,muonVec[i]->angleY,muonVec[i]->angleZ).Angle(ref); 
			//std::cout << "Angle : " << angle << std::endl;
			angHist->Fill(angle);
		}
		/*new TCanvas();
		angHist->Draw();*/

		TF1 *cosSqr = new TF1("cosSqr", "[0]*sin(x)*cos(x)*pow(cos(x),[1])", 0.,M_PI/2.);
		angHist->Fit(cosSqr);
		return angHist;
}

void Save(std::vector<Muon*> muonVec){
	double anglex=0;
	double angley=0;
	double anglez=0;
	double energy=0;
	TFile *muonFile = new TFile("muons.root","RECREATE");
	TTree *muonTree = new TTree("MuonTree","MuonTree");
	muonTree->Branch("u",&anglex,"anglex/D");
	muonTree->Branch("v",&angley,"angley/D");
	muonTree->Branch("w",&anglez,"anglez/D");
	muonTree->Branch("e",&energy,"energy/D");
	for(unsigned int i = 0 ; i < muonVec.size() ; i++){
		anglex = muonVec[i]->angleX;
		angley = muonVec[i]->angleY;
		anglez = muonVec[i]->angleZ;
		energy = muonVec[i]->energy;
		muonTree->Fill();
	}
	muonTree->Write();
	muonFile->Close();
}

//CRYParticle* SampleMuonParticle(){
std::vector<Muon*> SampleMuonParticle(int n){

	// Read the setup file into setupString
	std::ifstream inputFile;
	inputFile.open("setup.file", std::ios::in);
	char buffer[1000];

	std::string setupString("");
	while (!inputFile.getline(buffer, 1000).eof()) {
		setupString.append(buffer);
		setupString.append(" ");
	}

	// Parse the contents of the setup file
	CRYSetup *setup = new CRYSetup(setupString, "./data");

	// Setup the CRY event generator
	CRYGenerator gen(setup);

	// Generate the events
	std::vector<CRYParticle*> *ev=new std::vector<CRYParticle*>;

	//ev->clear();
	//gen.genEvent(ev);

	std::vector<Muon*> muonVec;
	for(unsigned int evNo = 0 ; evNo < n ; evNo++){
		ev->clear();
		gen.genEvent(ev);
		for ( unsigned j=0; j<ev->size(); j++) {
			  CRYParticle *p = (*ev)[j];
			  if(std::string(CRYUtils::partName(p->id())) == "muon")
				  muonVec.push_back( new Muon( p->u(),p->w(),p->v(),p->ke() ) );
		}
	}
	return muonVec;
}



