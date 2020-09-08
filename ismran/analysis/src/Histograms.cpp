/*
 * Histograms.cpp
 *
 *  Created on: 04-Sep-2020
 *      Author: rsehgal
 */

#include "Histograms.h"
#include "ScintillatorBar_V2.h"
#include "TPad.h"

Histograms::Histograms() {
	// TODO Auto-generated constructor stub
	padHeight = 150;
	padWidth = 200;

}

Histograms::Histograms(std::string barName,unsigned int barIndex){

	fBarName = barName;
	fBarIndex = barIndex;
	int lqstart=0;
	int lqend=32000;
	int lnbins=32000;
	unsigned short int numOfPlots = 4;
	gStyle->SetOptStat(0);
	/*fhistQNearFarPad = new TPad( (barName+"-NearFarPad").c_str(),(barName+"-NearFarPad").c_str(),
							   0,barIndex*padHeight,
							   padWidth,(barIndex+1)*padHeight);
	fhistQNearFarPad->Draw();*/

	fhistQNear = new TH1D((barName+"-Near").c_str(),(barName+"-Near").c_str(),nbins,qstart,qend);
	fhistQNear->SetLineColor(1);
	fhistQFar  = new TH1D((barName+"-Far").c_str(),(barName+"-Far").c_str(),nbins,qstart,qend);
	fhistQFar->SetLineColor(2);

	/*fhistQMeanPad = new TPad( (barName+"-MeanPad").c_str(),(barName+"-MeanPad").c_str(),
							   padWidth,barIndex*padHeight,
							   2*padWidth,(barIndex+1)*padHeight);
	fhistQMeanPad->Draw();*/
	fhistQMean  = new TH1D((barName+"-Mean").c_str(),(barName+"-Mean").c_str(),nbins,qstart,qend);


	/*fhistDelTPad = new TPad( (barName+"-DelTPad").c_str(),(barName+"-DelTPad").c_str(),
							   2*padWidth,barIndex*padHeight,
							   3*padWidth,(barIndex+1)*padHeight);
	fhistDelTPad->Draw();*/
	fhistDelT  = new TH1D((barName+"-DelT").c_str(),(barName+"-DelT").c_str(),200,-25000,25000);

	/*fhistDelTCorrectedPad = new TPad( (barName+"-DelTCorrectedPad").c_str(),(barName+"-DelTCorrectedPad").c_str(),
							  3*padWidth,barIndex*padHeight,
							  4*padWidth,(barIndex+1)*padHeight);
	fhistDelTCorrectedPad->Draw();*/
	fhistDelTCorrected  = new TH1D((barName+"-DelTCorrected").c_str(),(barName+"-DelTCorrected").c_str(),200,-25000,25000);

	fhistQMeanCorrected  = new TH1D((barName+"-QMeanCorrected").c_str(),(barName+"-QMeanCorrected").c_str(),nbins,12000,32000);


}

void Histograms::FillHistogram(ScintillatorBar_V2 *scintBar){
	fhistQNear->Fill(scintBar->qlongNear);
	fhistQFar->Fill(scintBar->qlongFar);
	fhistQMean->Fill(scintBar->qlongMean);
	fhistDelT->Fill(scintBar->deltaTstamp);
	fhistDelTCorrected->Fill(scintBar->deltaTstampCorrected);
	//fhistQMeanCorrected->Fill(fEnergyCalibrationFactor*scintBar->qlongMean);
}
#if(0)
//Working
void Histograms::FillCorrectedQMean(){
	fhistQMeanCorrected  = new TH1D((fBarName+"-QMeanCorrected").c_str(),(fBarName+"-QMeanCorrected").c_str(),nbins,qstart+fEnergyCalibrationFactor,qend+fEnergyCalibrationFactor);
	//fhistQMeanCorrected = fhistQMean*fEnergyCalibrationFactor;//->Fill(scintBar->qlongMeanCorrected);
	for (int i = 1; i<= fhistQMean->GetXaxis()->GetNbins(); i++) {

	   //double temp = fhistQMean->GetBinContent(i);
	   /*if(i==10){
	   			std::cout << "For Bar : " << fBarName <<" : UnCorrected : " << fhistQMean->GetBinContent(i) <<" : Corrected : " << temp << std::endl;
	   	}*/

	   double xval = fhistQMean->GetXaxis()->GetBinCenter(i);
	   //xval += fEnergyCalibrationFactor;
	   int binNum = fhistQMean->GetXaxis()->FindBin(xval);
	   fhistQMeanCorrected->SetBinContent(binNum, fhistQMean->GetBinContent(i));
	}
}
#endif



Histograms::~Histograms() {
	// TODO Auto-generated destructor stub
}

void Histograms::DoSinglePointEnergyCalibrationForMuon(){
	int binmax = fhistQMean->GetMaximumBin();
	double bin = fhistQMean->GetXaxis()->GetBinCenter(binmax);

	int revBin = fhistQMean->GetXaxis()->FindBin(bin);
	fEnergyCalibrationFactor =  (1.0*muonEnergyPeak) - bin; //(1.0*muonEnergyPeak)/(double)bin;
	std::cout << "Energy Calibration factor of " << fBarName <<" : PeakAt : " << bin  <<" : BinMax :" << binmax << " : RevBinMax : "<< revBin <<" : " << fEnergyCalibrationFactor << std::endl;
}
