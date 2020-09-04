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


	int qstart=0;
	int qend=30000;
	int nbins=500;
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
	fhistDelT  = new TH1D((barName+"-DelT").c_str(),(barName+"-DelT").c_str(),nbins,-25000,25000);

	/*fhistDelTCorrectedPad = new TPad( (barName+"-DelTCorrectedPad").c_str(),(barName+"-DelTCorrectedPad").c_str(),
							  3*padWidth,barIndex*padHeight,
							  4*padWidth,(barIndex+1)*padHeight);
	fhistDelTCorrectedPad->Draw();*/
	fhistDelTCorrected  = new TH1D((barName+"-DelTCorrected").c_str(),(barName+"-DelTCorrected").c_str(),nbins,-25000,25000);


}

void Histograms::FillHistogram(ScintillatorBar_V2 *scintBar){
	fhistQNear->Fill(scintBar->qlongNear);
	fhistQFar->Fill(scintBar->qlongFar);
	fhistQMean->Fill(scintBar->qlongMean);
	fhistDelT->Fill(scintBar->deltaTstamp);
	fhistDelTCorrected->Fill(scintBar->deltaTstampCorrected);
}

Histograms::~Histograms() {
	// TODO Auto-generated destructor stub
}

