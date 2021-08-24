/*
 * ReadCalibData.cpp
 *
 *  Created on: 29-Dec-2020
 *      Author: rsehgal
 */


#include <iostream>
#include "includes.hh"
#include <TFitResult.h>
#include <TH1F.h>
#include <TTree.h>
#include <TFile.h>
#include <vector>
#include <TApplication.h>
#include <TStyle.h>
int main(){
	gStyle->SetOptStat(0);
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	//std::vector<float> posVec={-45.,-40.,-30.,-20.,-10.,0.,10.,20.,30.,40.,45.};
	//std::vector<float> posVec={-40.,-30.,-20.,-10.,0.,10.,20.,30.,40.};
	//std::vector<float> posVec={-10.,0.,10.};
	std::vector<float> posVec={0.};
	std::vector<float> meanZVec;
	std::vector<float> meanTVec;
	//std::vector<int> posVec={-45,-30,0,30,45};
	//new TCanvas();
	TLegend *legend = new TLegend();

	TFile *formulaFile=new TFile("completeCalib2.root","READ");
	TF1 *param = (TF1*)formulaFile->Get("fzparam_PS71_SF874");
	for(unsigned int i = 0 ; i < posVec.size() ; i++){
		std::string filename="";
		if(posVec[i] < 0)
			//filename="/home/rsehgal/BackBoneSoftwares/ismranData/PS71_Cs137/Cs137_PS71_SF874_Couples_N1802V_Th6_F1939V_Th6_";
			filename="/home/rsehgal/BackBoneSoftwares/ismranData/PS89_Cs137/Cs137_PS89_SF873_Couples_N1862V_Th12_F1821V_Th12_";
		else
			filename="/home/rsehgal/BackBoneSoftwares/ismranData/PS89_Cs137/Cs137_PS89_SF873_Couples_N1862V_Th12_F1821V_Th12_+";

		std::string location=std::to_string((int)posVec[i])+"cm";
		std::string locationZ=location+"_Z";
		std::string locationT=location+"_T";
		std::string secondHalf = location+".root";
		filename += secondHalf;
		std::cout <<"Processing file : " << filename << std::endl;

		TFile *fp = new TFile(filename.c_str(), "r");
		TTree *tr = (TTree*) fp->Get("ftree");

		int delt;

		tr->SetBranchAddress("fDelt", &delt);
		Long64_t nEntries = tr->GetEntries();
		Long64_t nb = 0;
		const int offset=0;
		TH1F *delTHist = new TH1F(locationT.c_str(),locationT.c_str(),100,-20,20);
		TH1F *zHist = new TH1F(locationZ.c_str(),locationZ.c_str(),50,-50,50);
		for (Long64_t iev = 0; iev < nEntries; iev++) {
			nb += tr->GetEntry(iev);

			double delT_Corr =  1.0*delt/1000. + 0.698;
			float zval = param->Eval(delT_Corr);
			if(zval!=0.){
				delTHist->Fill(delT_Corr);
				zHist->Fill(zval);
			}
		}

		TF1 *formulaZ = new TF1("gaus","gaus",-50,50);
		formulaZ->SetLineColor(2);
		zHist->Fit(formulaZ,"r");
		float meanZ = formulaZ->GetParameter(1);
		float sigmaZ = formulaZ->GetParameter(2);
		std::cout <<"@@@@@@@@@@@ MeanZ: " << meanZ <<" : SigmaZ : " << sigmaZ << " @@@@@@@@@@@" << std::endl;
		meanZVec.push_back(meanZ);

		new TCanvas();
		zHist->Draw();

		TF1 *formulaT = new TF1("gaus","gaus",-20,20);
		delTHist->Fit(formulaT,"r");
		float meanT = formulaT->GetParameter(1);
		float sigmaT = formulaT->GetParameter(2);
		//std::cout <<"Mean : " << mean <<" : Sigma : " << sigma << std::endl;
		formulaT->SetLineColor(1);
		meanTVec.push_back(meanT);

		new TCanvas();
		delTHist->Draw();

		//Overlapping histogram
		new TCanvas();
		//zHist->Scale(1/zHist->Integral());
		//delTHist->Scale(1/delTHist->Integral());
		zHist->Draw();
		delTHist->Draw("same");

		legend->AddEntry(delTHist,"#Delta T_{LR}");
		legend->AddEntry(formulaT,"Fitted #Delta T_{LR}");
		legend->AddEntry(zHist,"Estimated Z");
		legend->AddEntry(formulaZ,"Fitted Estimated Z");
		legend->Draw();

		/*legend->AddEntry(delTHist);
		delTHist->Scale(1/delTHist->Integral());
		if(i==4)
			delTHist->SetLineColor(i+2);
		else
			delTHist->SetLineColor(i+1);
		delTHist->Draw("same");*/




	}

	TGraph *gr = new TGraph(posVec.size(),&meanTVec[0],&posVec[0]);
	TGraph *grZ = new TGraph(posVec.size(),&meanTVec[0],&meanZVec[0]);

	new TCanvas();
	gr->SetMarkerStyle(21);
	gr->SetMarkerColor(4);
	gr->SetMarkerSize(0.8);
	gr->Draw("ap");
	new TCanvas();
	grZ->SetMarkerStyle(20);
	grZ->SetMarkerColor(6);
	grZ->SetMarkerSize(0.8);
	grZ->Draw("ap");


	TMultiGraph *mg = new TMultiGraph();
	mg->Add(gr);
	mg->Add(grZ);
	new TCanvas();
	mg->Draw("ap");
	//legend->Draw("same");

	fApp->Run();
}

