/*
 * MyHit.cpp
 *
 *  Created on: October 3, 2020
 *      Author: rsehgal
 */

#include "DataTree.h"
#include <TTree.h>
#include <TFile.h>
#include "includes.hh"
DataTree::DataTree() {
	// TODO Auto-generated constructor stub

	/*Opening the file for storing the TTree*/
	fp = new TFile("simulatedData.root","RECREATE");

	/*Create the TTree and attach all the brances */
	fTree = new TTree("ftree","Tree to store ISMRAN simulated data");
	//fTree->Branch("scintName",&scintName,"scintName/C");
   	fTree->Branch("qlongNear",&qlongNear,"qlongNear/i");
   	fTree->Branch("qlongFar",&qlongFar,"qlongFar/i");
   	fTree->Branch("qlongMean",&qlongMean,"qlongMean/D");
   	fTree->Branch("qlongMeanCorrected",&qlongMeanCorrected,"qlongMeanCorrected/D");
   	fTree->Branch("tstampNear",&tstampNear,"tstampNear/l");
   	fTree->Branch("tstampFar",&tstampFar,"tstampFar/l");
   	fTree->Branch("tsmallTimeStamp",&tsmallTimeStamp,"tsmallTimeStamp/l");
   	fTree->Branch("deltaTstamp",&deltaTstamp,"deltaTstamp/L");
   	fTree->Branch("deltaTstampCorrected",&deltaTstampCorrected,"deltaTstampCorrected/L");
   	fTree->Branch("barIndex",&barIndex,"barIndex/s");
   	fTree->Branch("layerIndex",&layerIndex,"layerIndex/s");
   	
}

DataTree::DataTree(std::string filename) {
	// TODO Auto-generated constructor stub

	/*Opening the file for storing the TTree*/
	fp = new TFile(filename.c_str());

	/*Create the TTree and attach all the brances */
	/*fTree = new TTree("ftree","Tree to store ISMRAN simulated data");
	fTree->Branch("scintName",&scintName,"scintName/C");
   	fTree->Branch("qlongNear",&qlongNear,"qlongNear/i");
   	fTree->Branch("qlongFar",&qlongFar,"qlongFar/i");
   	fTree->Branch("qlongMean",&qlongMean,"qlongMean/D");
   	fTree->Branch("qlongMeanCorrected",&qlongMeanCorrected,"qlongMeanCorrected/D");
   	fTree->Branch("tstampNear",&tstampNear,"tstampNear/l");
   	fTree->Branch("tstampFar",&tstampFar,"tstampFar/l");
   	fTree->Branch("tsmallTimeStamp",&tsmallTimeStamp,"tsmallTimeStamp/l");
   	fTree->Branch("deltaTstamp",&deltaTstamp,"deltaTstamp/L");
   	fTree->Branch("deltaTstampCorrected",&deltaTstampCorrected,"deltaTstampCorrected/L");
   	fTree->Branch("barIndex",&barIndex,"barIndex/s");
   	fTree->Branch("layerIndex",&layerIndex,"layerIndex/s");*/

   	TTree* fTree = (TTree*) fp->Get("ftree");
	//float mass, charge, ...;
	//fTree->SetBranchAddress("scintName", &scintName);
	fTree->SetBranchAddress("qlongNear", &qlongNear);
	fTree->SetBranchAddress("qlongFar", &qlongFar);
	fTree->SetBranchAddress("qlongMean", &qlongMean);
	fTree->SetBranchAddress("qlongMeanCorrected", &qlongMeanCorrected);
	fTree->SetBranchAddress("tstampNear", &tstampNear);
	fTree->SetBranchAddress("tstampFar", &tstampFar);
	fTree->SetBranchAddress("tsmallTimeStamp", &tsmallTimeStamp);
	fTree->SetBranchAddress("deltaTstamp", &deltaTstamp);
	fTree->SetBranchAddress("deltaTstampCorrected", &deltaTstampCorrected);
	fTree->SetBranchAddress("barIndex", &barIndex);
	fTree->SetBranchAddress("layerIndex", &layerIndex);

	fNumOfEntries = fTree->GetEntries();
   	
}

void DataTree::GetEntry(unsigned int entry){
	fTree->GetEntry(entry);
}

DataTree::~DataTree() {
	// TODO Auto-generated destructor stub
	delete fTree;
	delete fp;
}

void DataTree::Fill(const char *scintname,
					UInt_t qlongnear,UInt_t qlongfar, Double_t qlongmean,
					Double_t qlongmeancorrected, ULong64_t tstampnear, ULong64_t tstampfar,
					ULong64_t tsmalltimestamp, Long64_t deltatstamp, Long64_t deltatstampcorrected,
					unsigned short barindex, unsigned short layerindex){

	//std::cout << "Fill function called........" << std::endl;

	//scintName = scintname;
	//std::cout << "ScintName : " << scintName << std::endl;
	qlongFar = qlongfar;
	qlongNear = qlongnear;
	qlongMean = qlongmean;
	qlongMeanCorrected = qlongmeancorrected;
	tstampNear = tstampnear;
	tstampFar = tstampfar;
	tsmallTimeStamp = tsmalltimestamp;
	deltaTstamp = deltatstamp;
	deltaTstampCorrected = deltatstampcorrected;
	barIndex = barindex;
	layerIndex = layerindex;

	//std::cout << "Going to fill ROOT TTree...." << std::endl;
	fTree->Fill();
}

void DataTree::Write(){
	fp->cd();
	fTree->Write();
	//fp->Close();
}

void DataTree::Close(){
	fp->Close();
}
