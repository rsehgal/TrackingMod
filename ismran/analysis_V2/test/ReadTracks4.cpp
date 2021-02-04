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
#include "Analyzer.h"
#include <TF1.h>
#include "PsBar.h"
#include "colors.h"
struct ScintData{
	ULong64_t sNear;
	ULong64_t sFar;
	ULong64_t sTAvg;
	ULong64_t sTSmall;
	float sZ;
	Long_t sDelT;
	float sEstimatedZ;


	ScintData(ULong64_t tavg,float z,Long_t delt){
		sTAvg = tavg;
		sZ = z;
		sDelT = delt;
	}

	ScintData(ULong64_t tnear, ULong64_t tfar,ULong64_t tavg,float z,Long_t delt){
		sNear = tnear;
		sFar = tfar;
		sTAvg = tavg;
        sZ = z;
        sDelT = delt;

        if(sNear < sFar)
        	sTSmall = sNear;
        else
        	sTSmall = sFar;
    }


	ScintData(float z,Long_t delt){
		sTAvg = 0.;
		sZ = z;
		sDelT = delt;
	}
};

bool IsCorrelatedEvent(std::vector<ScintData*> vecOfScintData,double startDelTInPs, double endDelTInPs, double zExtInCm){
	bool delTInRange = true;
	for(unsigned int i = 0 ; i < vecOfScintData.size() ; i++){
		delTInRange &= ( (vecOfScintData[i]->sDelT > startDelTInPs) && (vecOfScintData[i]->sDelT < endDelTInPs) );
	}

	bool zInReqExt = true;
	for(unsigned int i = 0 ; i < vecOfScintData.size()-1 ; i++){
		double zdiff = (vecOfScintData[i]->sZ - vecOfScintData[i+1]->sZ);
		//std::cout << "Zdiff : " << zdiff << std::endl;
		zInReqExt &= (std::fabs(zdiff) < zExtInCm);
	}

	zInReqExt &= (std::fabs(vecOfScintData[0]->sZ - vecOfScintData[vecOfScintData.size()-1]->sZ) < zExtInCm);

	return (delTInRange && zInReqExt);

}

struct Hist{
	TH1F *sDelTHist;
	TH1F *sZHist;
	TH1F *sTAvHist;
	//TH1F *energyHist;

	Hist(std::string name){
		sDelTHist = new TH1F((name+"_DelT").c_str(),(name+"_DelT").c_str(),100,-25000,25000);
		sZHist = new TH1F((name+"_Z").c_str(),(name+"_Z").c_str(),100,-100,100);
		//energyHist = new TH1F((name+"_Z").c_str(),(name+"_Z").c_str(),100,0,40);
	}
};


int main(int argc,char *argv[]){
	/*
	 * Set lite_interface::IsSimulation = true, if need to process simulated data file
	 * else for experimental data set it to false
	 */
	lite_interface::IsSimulation = false;

	//TFile *f=new TFile(argv[2],"RECREATE");

	TApplication *fApp = new TApplication("Test", NULL, NULL);
	GenerateScintMatrixXYCenters();
	lite_interface::SingleMuonTrack *smt = new lite_interface::SingleMuonTrack;
	lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
	std::string outputFileName=argv[1];
	TFile *trackFile = new TFile(outputFileName.c_str(),"READ");
	TTree *trackTree = (TTree*)trackFile->Get("TracksTree");
	trackTree->SetBranchAddress("MuonTracks",&smt);


	Long64_t nentries = trackTree->GetEntries();

	Long64_t nbytes = 0;
	unsigned int barIndex = 5;
	std::vector<lite_interface::ScintillatorBar_V2*> vectOfScintBars;
    std::vector<lite_interface::ScintillatorBar_V2*>::iterator itr;

	std::vector<lite_interface::SingleMuonTrack*> smtVec;

	std::vector<lite_interface::ScintillatorBar_V2*> scintBarVecForNewTrack;

	unsigned int count = 0;
	unsigned int count2 = 0;
	std::cout << "Total number of Entries : " << nentries << std::endl;

	unsigned int countVisTracks = 0;

	//nentries = 100;

	for (Long64_t i=0; i<nentries;i++) {
		//std::cout << "Fetching Entry : " << i << std::endl;
		nbytes += trackTree->GetEntry(i);
		if(!(i % 10000) && i!=0)
			std::cout << "Processed : " << i << " entries........" << std::endl;

		{
			std::vector<lite_interface::ScintillatorBar_V2*> scintBarVecOfATrack = smt->GetMuonTrack();
			scintBarVecForNewTrack.clear();

			for(unsigned int j = 0 ; j < scintBarVecOfATrack.size() ; j++){

				//(smt->GetMuonTrack())[j]->Print();
				if(scintBarVecOfATrack[j]->GetBarIndex() > 81){
					count++;

				}
				vectOfScintBars.push_back(new lite_interface::ScintillatorBar_V2(*scintBarVecOfATrack[j]));
				scintBarVecForNewTrack.push_back(new lite_interface::ScintillatorBar_V2(*scintBarVecOfATrack[j]));
			}
			smtVec.push_back(new lite_interface::SingleMuonTrack(scintBarVecForNewTrack));
		}

	}

	TFile *outfile = new TFile(argv[2],"RECREATE");
	{
		unsigned int countBuggy = 0 ;
		float allowedDeviation = 2;
		TCanvas *ok = new TCanvas("OK","OK");
		TCanvas *buggy = new TCanvas("BUGGY","BUGGY");
		for(unsigned int i = 0 ; i < numOfLayers*numOfBarsInEachLayer ; i++){
			std::cout <<"Processed DelT of Bar : " << vecOfBarsNamess[i] << std::endl;
			TH1F *histDelT = lite_interface::PlotDelTCorrected(smtVec,i);
			TH1F *histQMeanCorr = lite_interface::PlotQMeanCorrected(smtVec,i);
			histDelT->SetLineColor(i+1);
			histDelT->Scale(1/histDelT->Integral());
			if(std::fabs(histDelT->GetMean()) > allowedDeviation ){
				countBuggy++;
				buggy->cd();
				std::cout << RED << "Needs MORE correction for BAR : " << vecOfBarsNamess[i] << RESET << " : Mean : " << YELLOW << histDelT->GetMean() << RESET << std::endl;
				histDelT->Draw("same");
			}
			else{
				ok->cd();
				/*if(   i==58 || i==49
				   || i== 40 || i==31 || i==22 || i==13 || i==4)*/
				//if(   i==49 || i== 40 || i==4)
				if(i==3){
					histDelT->Draw("same");
					outfile->cd();
					histQMeanCorr->Write();
					new TCanvas("QMeanCorr","QMeanCorr");
					histQMeanCorr->Draw();

					histDelT->Write();
				}
			}
		}
		std::cout << GREEN << "Number of buggy detector where MEAN is deviated by more than : " << allowedDeviation << " :: " << countBuggy << RESET << std::endl;
		//histDelT->Draw();
	}
	outfile->Close();
	fApp->Run();

	return 0;
}



