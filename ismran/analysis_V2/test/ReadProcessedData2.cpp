/*
 * ReadTracks.cpp
 *
 *  Created on: 10-Nov-2020
 *      Author: rsehgal
 */

#include "SingleMuonTrack.h"
#include <iostream>
#include <TTree.h>
#include <TFile.h>
#include "ScintillatorBar_V2.h"
#include "Plotter.h"
#include <vector>
#include<iterator>
#include <TH1F.h>
#include <TApplication.h>
#include "Calibration.h"
#include "HardwareNomenclature.h"
#include <TGraphErrors.h>
#include "SingleMuonTrack.h"
#include "Analyzer.h"
int main(int argc,char *argv[]){
	GenerateScintMatrixXYCenters();
	/*
	 * Set lite_interface::IsSimulation = true, if need to process simulated data file
	 * else for experimental data set it to false
	 */
	lite_interface::IsSimulation = true;
	lite_interface::Calibration *calib = lite_interface::Calibration::instance("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib2.root");

	TApplication *fApp = new TApplication("Test", NULL, NULL);
	std::string outputFileName=argv[1];
	TFile *hitPointFile = new TFile(outputFileName.c_str(),"READ");
	TTree *hitPointVecTree = (TTree*)hitPointFile->Get("HitPointVecTree");
	//TTree *hitPointVecTree_Param = (TTree*)hitPointFile->Get("HitPointVecTreeParam");
	std::vector<lite_interface::Point3D*> *vecOfPoint3D = 0;
	double energySum = 0;
	double zenithAngleLinear = 0.;
	double zenithAngleParam = 0.;

	Long64_t nbytes = 0;
	Long64_t nentries = hitPointVecTree->GetEntries();

	hitPointVecTree->SetBranchAddress("EnergySum",&energySum);
	hitPointVecTree->SetBranchAddress("zenithAngleLinear",&zenithAngleLinear);
	hitPointVecTree->SetBranchAddress("zenithAngleParam",&zenithAngleParam);
	std::vector<double> *energyVec = new std::vector<double>;
	hitPointVecTree->SetBranchAddress("EnergyVector",&energyVec);

	int nbins = 50;

	TH1F *enerSumHist = new TH1F("EnergySumHist","EnergySumHist",nbins,150,220);


		std::cout <<"Total entries to be processed : " << nentries << std::endl;

	std::vector<unsigned int> sizeVector;
	std::vector<double> energySumVector;

	std::vector<double> zenithAngleVectorLinear;
	std::vector<double> zenithAngleVectorParam;



	TH1F *histZen = new TH1F("NewZenHist","NewZenHist",nbins,0.05,M_PI/2.);

	std::string outfile=argv[2];
	TFile *fp = new TFile(outfile.c_str(),"RECREATE");
	for (Long64_t i=0; i<nentries;i++) {

		if(!(i%50000))
			std::cout << "Processed : " << i <<" entries...." << std::endl;

		nbytes += hitPointVecTree->GetEntry(i);

		if(energySum > 400. || energySum < 0.)
			continue;


		if(energySum > 140 && energySum < 220){
			//std::cout << "Energy Sum  : " << energySum << std::endl;
			enerSumHist->Fill(energySum);
		}



	} //end of Event loop

	fp->cd();
	enerSumHist->Write();

	fp->Close();
	std::cout <<"File Written and closed........." << std::endl;

	fApp->Run();
	return 0;
}



