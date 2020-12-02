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
#include "Plotter.h"
#include "PsBar.h"
#include "TApplication.h"
int main(){
	//TFile *fp = new TFile("TestExactHit.root","RECREATE");
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	std::vector<double> *xVec = new std::vector<double>;
	std::vector<double> *yVec = new std::vector<double>;
	std::vector<double> *zVec = new std::vector<double>;
	double initialEnergy = 0.;
	double depositedEnergy  = 0.;
	unsigned int evNo = 0;
	TFile *exactHitFile = new TFile("exactHitData.root","READ");
	TTree *exactHitTree = (TTree*)exactHitFile->Get("ExactHitTree");
	exactHitTree->SetBranchAddress("xVec",&xVec);
	exactHitTree->SetBranchAddress("yVec",&yVec);
	exactHitTree->SetBranchAddress("zVec",&zVec);
	exactHitTree->SetBranchAddress("InitialEnergy",&initialEnergy);
	exactHitTree->SetBranchAddress("DepositedEnergy", &depositedEnergy);
	exactHitTree->SetBranchAddress("EvNo", &evNo);

	Long64_t nentries = exactHitTree->GetEntries();
	std::cout << "Num of Entries : " << nentries << std::endl;
	Long64_t nbytes = 0;
	std::vector<lite_interface::Point3D*> vecOfPoint3D;
	for(unsigned int i = 0 ; i < nentries ; i++){
		vecOfPoint3D.clear();
		nbytes += exactHitTree->GetEntry(i);
		//if(i<20){
		if(evNo<20){
				std::cout << "===== i : " << i <<" ========= Size : " << xVec->size() << " =======================" << std::endl;
		//std::cout << "Size of xVec : " << xVec->size() << std::endl;

		for(unsigned int j = 0 ; j < xVec->size() ; j++){
			std::cout << "Hit Point : (" << xVec->at(j) <<" , " << yVec->at(j) << " , " << zVec->at(j) << ")" << std::endl;
		}
        /*
		if(i < 10){
			new TCanvas();
			DrawGrid("Fitted Muon Track in XY plane; X axis ; Y axis", 9, 9);
			lite_interface::PlotMuonTrack(*xVec,*yVec)->Draw("p");
			fp->cd();
			lite_interface::PlotMuonTrack(*xVec,*yVec)->Write();
		}
		 */
		for(unsigned int j = 0 ; j < xVec->size() ; j++){
			vecOfPoint3D.push_back(new lite_interface::Point3D(xVec->at(j)/10.,yVec->at(j)/10.,zVec->at(j)/10.));
		}


			//fp->cd();
			//TCanvas *can = new TCanvas();
			std::string title = "Particle : "+std::to_string(evNo)+" : Initial Energy : "+std::to_string(initialEnergy)+" : DepositedEnergy : "+std::to_string(depositedEnergy);
			new TCanvas(title.c_str(),title.c_str());
			DrawGrid("Fitted Muon Track in XY plane; X axis ; Y axis", 9, 9);
			//PlotMuonTrackXY(CreateFittedTrack(vecOfPoint3D))->Write();
			PlotMuonTrackXY(vecOfPoint3D)->Draw("p");
			//PlotMuonTrackXY(vecOfPoint3D)->Write();
		}

	}

	//fp->Close();
	exactHitFile->Close();
	fApp->Run();
	return 0;

}

