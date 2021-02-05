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
	//nentries = 785000;
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

	TH1F *pixelHist = lite_interface::PlotPixelDelTCorrected(smtVec,3,12);
	TFile *outfile = new TFile(argv[2],"RECREATE");
	outfile->cd();
	pixelHist->Write();
	outfile->Close();

	TGraph *gr = lite_interface::PlotHitPointsOnBar(smtVec,2,9);
	gr->Draw("ap");

	TH2F *hist2D = lite_interface::PlotHitPointsOnBarHist(smtVec,4,13);
	new TCanvas();
	hist2D->Draw("colz");


	TH1F *histInterBarPixel3_9 = lite_interface::PlotPixelDelTBetweenBars(smtVec,3,9);
	new TCanvas();
	histInterBarPixel3_9->Draw();

	TH1F *histInterBarPixel3_12 = lite_interface::PlotPixelDelTBetweenBars(smtVec,3,12);
	new TCanvas();
	histInterBarPixel3_12->Draw();

	TH1F *histInterBarPixel3_17 = lite_interface::PlotPixelDelTBetweenBars(smtVec,3,17);
	new TCanvas();
	histInterBarPixel3_17->Draw();

	TH1F *ener9 = lite_interface::PlotQMeanCorrectedOfFirstBarWithRespectToSecond(smtVec,3,9);
	TH1F *ener12 = lite_interface::PlotQMeanCorrectedOfFirstBarWithRespectToSecond(smtVec,3,12);
	TH1F *ener17 = lite_interface::PlotQMeanCorrectedOfFirstBarWithRespectToSecond(smtVec,3,17);
	ener9->SetLineColor(1);
	ener12->SetLineColor(2);
	ener17->SetLineColor(4);
	new TCanvas("9","9");
	ener9->Draw();
	new TCanvas("12","12");
	ener12->Draw();
	new TCanvas("17","17");
	ener17->Draw();

	/*TH1F *enerRatio9_12 = new TH1F(*ener9);
	enerRatio9_12->Divide(ener12);
	new TCanvas("9-12","9-12");
	enerRatio9_12->Draw();

	TH1F *enerRatio9_17 = new TH1F(*ener9);
	enerRatio9_17->Divide(ener17);
	new TCanvas("9-17","9-17");
	enerRatio9_17->Draw();
*/
/*
	TH2F *hist2DLayer = lite_interface::PlotHitPointsOnLayerHist(smtVec,3);
	new TCanvas();
	hist2DLayer->Draw();

	TH2F *hist2DBar = lite_interface::PlotHitPointsOnBar(smtVec,3);
	new TCanvas();
	hist2DBar->Draw();

	TH1F *stripProfile = lite_interface::PlotStripProfileOfLayer(smtVec,0);
	new TCanvas();
	stripProfile->Draw();


	TH1F *stripProfile1 = lite_interface::PlotStripProfileOfLayer(smtVec,1);
	new TCanvas();
	stripProfile1->Draw();

	TH1F *stripProfile2 = lite_interface::PlotStripProfileOfLayer(smtVec,2);
	new TCanvas();
	stripProfile2->Draw();

	TH1F *stripProfile3 = lite_interface::PlotStripProfileOfLayer(smtVec,3);
	new TCanvas();
	stripProfile3->Draw();

	TH1F *stripProfile4 = lite_interface::PlotStripProfileOfLayer(smtVec,4);
	new TCanvas();
	stripProfile4->Draw();

	TH1F *stripProfile5 = lite_interface::PlotStripProfileOfLayer(smtVec,5);
	new TCanvas();
	stripProfile5->Draw();

	TH1F *stripProfile6 = lite_interface::PlotStripProfileOfLayer(smtVec,6);
	new TCanvas();
	stripProfile6->Draw();

	TH1F *stripProfile7 = lite_interface::PlotStripProfileOfLayer(smtVec,7);
	new TCanvas();
	stripProfile7->Draw();
*/



	std::cout << "Output file closed........" << std::endl;

	//pixelHist->Draw();
#if(0)
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
#endif

	fApp->Run();

	return 0;
}



