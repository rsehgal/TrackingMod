/*
 * ExactHitDataTree.cpp
 *
 *  Created on: 01-Dec-2020
 *      Author: rsehgal
 */

#include "ExactHitDataTree.h"
#include <TFile.h>
#include <TTree.h>

namespace lite_interface {

ExactHitDataTree::ExactHitDataTree() {
	// TODO Auto-generated constructor stub
	fp = new TFile("exactHitData.root","RECREATE");
	/*Create the TTree and attach all the brances */
	fTree = new TTree("ExactHitTree","Tree to store exact entry hit point");
   	fTree->Branch("xVec",&fXVec);
   	fTree->Branch("yVec",&fYVec);
   	fTree->Branch("zVec",&fZVec);
   	fTree->Branch("InitialEnergy",&fInitialEnergy);
   	fTree->Branch("DepositedEnergy", &fDepositedEnergy);
   	fTree->Branch("EvNo", &fEvNo);

}

ExactHitDataTree::~ExactHitDataTree() {
	// TODO Auto-generated destructor stub
	delete fTree;
	delete fp;
}


void ExactHitDataTree::Fill(std::vector<double> xvec, std::vector<double> yvec, std::vector<double> zvec){
	fXVec = xvec;
	fYVec = yvec;
	fZVec = zvec;
	fTree->Fill();

}

void ExactHitDataTree::Fill(std::vector<double> xvec, std::vector<double> yvec, std::vector<double> zvec, double initialEnergy, double depositedEnergy){
	fXVec = xvec;
	fYVec = yvec;
	fZVec = zvec;
	fInitialEnergy = initialEnergy;
	fDepositedEnergy = depositedEnergy;
	fTree->Fill();
}

void ExactHitDataTree::Fill(std::vector<double> xvec, std::vector<double> yvec, std::vector<double> zvec, double initialEnergy, double depositedEnergy,unsigned int evNo){
	fXVec = xvec;
	fYVec = yvec;
	fZVec = zvec;
	fInitialEnergy = initialEnergy;
	fDepositedEnergy = depositedEnergy;
	fEvNo = evNo;
	fTree->Fill();
}

void ExactHitDataTree::Write(){
	fp->cd();
	fTree->Write();
}
void ExactHitDataTree::Close(){
	fp->Close();
}
void ExactHitDataTree::GetEntry(unsigned int entry){
	fTree->GetEntry(entry);
}


} /* namespace lite_interface */
