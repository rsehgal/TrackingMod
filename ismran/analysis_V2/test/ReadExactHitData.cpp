/*
 * ReadExactHitData.cpp
 *
 *  Created on: 17-Dec-2020
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
#include <TH1D.h>
#include <TApplication.h>
#include "Calibration.h"
#include "HardwareNomenclature.h"
#include <TGraphErrors.h>
#include "SingleMuonTrack.h"
#include "Analyzer.h"
int main(int argc,char *argv[]){

	TApplication *fApp = new TApplication("Test", NULL, NULL);
	std::string outputFileName=argv[1];
	TFile *exactHitFile = new TFile(outputFileName.c_str(),"READ");
	TTree *exactHitTree = (TTree*)exactHitFile->Get("ExactHitTree");
	TFile *fp = new TFile("out.root","RECREATE");

	double angleCRY = 0.;
	double angleLinear = 0.;
	exactHitTree->SetBranchAddress("angleCRY",&angleCRY);
	exactHitTree->SetBranchAddress("angleLinear",&angleLinear);

	Long64_t nbytes = 0;
	Long64_t nentries = exactHitTree->GetEntries();

	int nbins = 100;
	//double start = cos(0.05);
	//double start = cos();
	TH1D *solidAngleHist = new TH1D("AngleCry","AngleCry",nbins,0.05,M_PI/2.);
	TH1D *solidAngleHistLinear = new TH1D("AngleCryLinear","AngleCryLinear",nbins,0.05,M_PI/2.);

	int counter = 0 ;
	for (Long64_t i=0; i<nentries;i++) {

			if(!(i%50000))
				std::cout << "Processed : " << i <<" entries...." << std::endl;

			nbytes += exactHitTree->GetEntry(i);

			if(angleCRY >= 0.){
				counter++;
				//std::cout << "Angle CRY : " << angleCRY << std::endl;
				solidAngleHist->Fill(angleCRY);

			}

			if(angleLinear > 0.02)
				solidAngleHistLinear->Fill(angleLinear);

	}

	TH1F *solidAngleCorrectedHist = new TH1F("Solid angle corrected AngularDistribution From CRY",
											 "Solid angle corrected AngularDistribution From CRY",
											 nbins,0.05,M_PI/2.);
	  solidAngleCorrectedHist->GetYaxis()->SetTitle("I_{#theta}  ( cm^{-2}sec^{-1}st^{-1})");
	  solidAngleCorrectedHist->GetXaxis()->SetTitle("#theta (radian)");
	      for(int i =0  ; i < nbins ; i++){
	      	double binCenter = solidAngleHist->GetXaxis()->GetBinCenter(i);
	      	double binContent = solidAngleHist->GetBinContent(i);
	      	//std::cout << "binContent : " << binContent << " : binCenter : " << binCenter << std::endl;
	      	solidAngleCorrectedHist->SetBinContent(i,binContent/(2*M_PI*std::sin(binCenter)*std::cos(binCenter)));
	      }


	  	TH1F *solidAngleCorrectedHistLinear = new TH1F("Solid angle corrected AngularDistribution From CRY LINEAR",
	  											 "Solid angle corrected AngularDistribution From CRY  LINEAR",
	  											 nbins,0.05,M_PI/2.);
	  	solidAngleCorrectedHistLinear->GetYaxis()->SetTitle("I_{#theta}  ( cm^{-2}sec^{-1}st^{-1})");
	  	solidAngleCorrectedHistLinear->GetXaxis()->SetTitle("#theta (radian)");
	  	      for(int i =0  ; i < nbins ; i++){
	  	      	double binCenter = solidAngleHistLinear->GetXaxis()->GetBinCenter(i);
	  	      	double binContent = solidAngleHistLinear->GetBinContent(i);
	  	      	//std::cout << "binContent : " << binContent << " : binCenter : " << binCenter << std::endl;
	  	      solidAngleCorrectedHistLinear->SetBinContent(i,binContent/(2*M_PI*std::sin(binCenter)*std::cos(binCenter)));
	  	      }


	      TH1F *solidAngleAndAcceptanceCorrectedHist_Linear = new TH1F("Solid angle and Acceptance corrected AngularDistribution From CRY LINEAR",
	    		                                   "Solid angle and Acceptance corrected AngularDistribution From CRY LINEAR",
												   nbins,0.05,M_PI/2.);

	      /*TH1F *solidAngleAndAcceptanceCorrectedHist_Div = new TH1F("Solid angle and Acceptance corrected AngularDistribution From CRY DIVIDE",
	      	    		                                   "Solid angle and Acceptance corrected AngularDistribution From CRY DIVIDE",
	      												   nbins,0.05,M_PI/2.);
	       */

	      TH1F *acceptanceHist = new TH1F("AcceptanceHist","AcceptanceHist",nbins,0.05,M_PI/2.);

	     // double startBinVal = solidAngleCorrectedHist->GetBinContent(0);
	      for(int i = 0  ; i < nbins ; i++){
	    	  int refVal = solidAngleCorrectedHist->GetBinContent(i);
	    	  int currentVal = solidAngleCorrectedHistLinear->GetBinContent(i);
	    	  if(refVal > 0. && currentVal > 0.){
	    	      double weightFactor = (1.0*currentVal) / (1.0*refVal);
	    		  //double weightFactor = (1.0*refVal) / (1.0*currentVal);
	    		  //double weightFactor2 = (1.0*currentVal) / (1.0*refVal);
	    	      acceptanceHist->SetBinContent(i,weightFactor);

	    	  std::cout << "Weight Factor : " << weightFactor << " : CurrentVal : " << currentVal << std::endl;

	    	  if(weightFactor >= 0.){
	    		  solidAngleAndAcceptanceCorrectedHist_Linear->SetBinContent(i,currentVal/weightFactor);
				  //solidAngleAndAcceptanceCorrectedHist_Div->SetBinContent(i,weightFactor2*currentVal);
	    	  }
	      }

	      }

	      TF1 *formula = new TF1("Cos Square","[0]*pow(cos(x),[1])",0,M_PI/2.);
	      //TF1 *formula = new TF1("Cos Square","[0]*pow(cos(x),[1])",0,M_PI/2.);
	      //solidAngleHist->Fit(formula,"r");
	      std::cout << "========= FITTING ACTUAL DISTRIBUTION===========" << std::endl;
	      solidAngleCorrectedHist->Fit(formula,"r");
	      std::cout << "========= FITTING ONLY SOLID ANGLE CORRECTION DISTRIBUTION===========" << std::endl;
	      solidAngleCorrectedHistLinear->Fit(formula,"r");
	      std::cout << "========= FITTING BOTH ACCEPTANCE and SOLID ANGLE CORRECTED DISTRIBUTION===========" << std::endl;
	      solidAngleAndAcceptanceCorrectedHist_Linear->Fit(formula,"r");


	      //solidAngleAndAcceptanceCorrectedHist_Div->Fit(formula,"r");

	std::cout << "Total number of positive values : " << counter << std::endl;

	new TCanvas();
	solidAngleHist->Draw();
	exactHitFile->Close();

	fp->cd();
	solidAngleHist->Write();
	solidAngleCorrectedHist->Write();
	solidAngleCorrectedHistLinear->Write();
	acceptanceHist->Write();
	solidAngleAndAcceptanceCorrectedHist_Linear->Write();
	//solidAngleAndAcceptanceCorrectedHist_Div->Write();

	fp->Close();



	fApp->Run();
	return 0;
}



