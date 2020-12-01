/*
 * ExactHitRead.cpp
 *
 *  Created on: 01-Dec-2020
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
#include <vector>
int main(){
	std::vector<double> *xVec = new std::vector<double>;
	std::vector<double> *yVec = new std::vector<double>;
	std::vector<double> *zVec = new std::vector<double>;
	TFile *exactHitFile = new TFile("exactHitData.root","READ");
	TTree *exactHitTree = (TTree*)exactHitFile->Get("ExactHitTree");
	exactHitTree->SetBranchAddress("xVec",&xVec);
	exactHitTree->SetBranchAddress("yVec",&yVec);
	exactHitTree->SetBranchAddress("zVec",&zVec);


	Long64_t nentries = exactHitTree->GetEntries();
	std::cout << "Num of Entries : " << nentries << std::endl;
	Long64_t nbytes = 0;
	for(unsigned int i = 0 ; i < nentries ; i++){
		std::cout << "============== Size : " << xVec->size() << " =======================" << std::endl;
		nbytes += exactHitTree->GetEntry(i);
		//std::cout << "Size of xVec : " << xVec->size() << std::endl;
		for(unsigned int j = 0 ; j < xVec->size() ; j++){
			std::cout << "Hit Point : (" << xVec->at(j) <<" , " << yVec->at(j) << " , " << zVec->at(j) << ")" << std::endl;
		}

	}


	exactHitFile->Close();
	return 0;

}

