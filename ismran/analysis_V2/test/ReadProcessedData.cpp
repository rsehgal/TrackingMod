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
	lite_interface::Calibration *calib = lite_interface::Calibration::instance("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib.root");

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

	int nbins = 100;

	TH1F *enerSumHist = new TH1F("EnergySumHist","EnergySumHist",nbins,0,300);


		std::cout <<"Total entries to be processed : " << nentries << std::endl;

	std::vector<unsigned int> sizeVector;
	std::vector<double> energySumVector;

	std::vector<double> zenithAngleVectorLinear;
	std::vector<double> zenithAngleVectorParam;



	TH1F *histZen = new TH1F("NewZenHist","NewZenHist",nbins,0.05,M_PI/2.);

	for (Long64_t i=0; i<nentries;i++) {

		if(!(i%50000))
			std::cout << "Processed : " << i <<" entries...." << std::endl;

		nbytes += hitPointVecTree->GetEntry(i);

		if(energySum > 400. || energySum < 0.)
			continue;

		//std::cout << "Energy Sum  : " << energySum << std::endl;
		enerSumHist->Fill(energySum);

		energySumVector.push_back(energySum);
		sizeVector.push_back(energyVec->size());

		zenithAngleVectorLinear.push_back(zenithAngleLinear);
		zenithAngleVectorParam.push_back(zenithAngleParam);

		histZen->Fill(zenithAngleLinear);


	} //end of Event loop

	new TCanvas();
	enerSumHist->Draw();

	new TCanvas();
	TH1F *zenithAngleLinearHist = lite_interface::GetSolidAngleCorrectedHist(lite_interface::PlotZenithAngle(zenithAngleVectorLinear,1));
	zenithAngleLinearHist->Draw();

	new TCanvas();
	lite_interface::GetSolidAngleCorrectedHist(lite_interface::PlotZenithAngle(zenithAngleVectorParam,2))->Draw();

	/*new TCanvas();
	lite_interface::Plot_Acc_Corr_ZenithAngle(zenithAngleVectorLinear,1)->Draw();
*/
	/*
	new TCanvas();
	lite_interface::Plot_Acc_Corr_ZenithAngle(zenithAngleVectorParam,2)->Draw();*/


	new TCanvas();
	std::vector<TH1D*> vecHist = lite_interface::PlotEnergyDistributionWithMultiplicity(sizeVector,energySumVector);
	new TCanvas("Energy with multiplicity..", "Energy with multiplicity..");
	for (unsigned int i = 0; i < vecHist.size(); i++) {
		//vecHist[i]->Draw("same");
		if (vecHist[i]->GetEntries() > 10) {
			vecHist[i]->Scale(1 / vecHist[i]->Integral());
			vecHist[i]->Draw("same");
		}
	}


	TFile *fp = new TFile("acceptance.root","READ");
	TH1F *acceptanceHist = (TH1F*)fp->Get("AcceptanceHist");
	TH1F *newHist = new TH1F("AcceptanceAppliedHist","AcceptanceAppliedHist",nbins,0.05,M_PI/2.);
	//double startBinVal = acceptanceHist->GetBinContent(0);
	for(unsigned int i = 0 ; i < zenithAngleLinearHist->GetNbinsX() ; i++){
		int currentVal = zenithAngleLinearHist->GetBinContent(i);
		//int refVal = acceptanceHist->GetBinContent(i);
		//double weightFactor = (1.0*currentVal) / (1.0*refVal);
		double weightFactor = acceptanceHist->GetBinContent(i);

		//int currZenVal = histZen->GetBinContent(i);
		//if(weightFactor > 0.)
		//newHist->SetBinContent(i,currZenVal*weightFactor);
		if(weightFactor > 0.)
			newHist->SetBinContent(i,currentVal/weightFactor);
	}

	new TCanvas("Acceptance applied hist");
	newHist->Draw();

	fApp->Run();
	return 0;
}



