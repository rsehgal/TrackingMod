/*
 * HitDistribution.cpp
 *
 *  Created on: Apr 13, 2018
 *      Author: rsehgal
 */

#include "HitDistribution.h"
#include <G4SystemOfUnits.hh>
//#include <TH1F.h>
#include <TFile.h>

namespace Tomography {

int numOfStrips = 32;
double stripWidth =  100*cm/numOfStrips ;

HitDistribution::HitDistribution() {
	// TODO Auto-generated constructor stub
	//int numOfStrips = 32;
	int numOfBinsInAPixel = 100;
//	double stripWidth =  100*cm/numOfStrips ;
	int numOfBinsForEachStrip = 30;
	for(int i = 0  ; i < numOfStrips ; i++){

		double low = -50*cm+i*stripWidth;
		double high = low + stripWidth;
		std::string index = std::to_string(i);
		std::string xHistName="X_Hist_"+ index;
		std::string yHistName="Y_Hist_"+ index;
		fXHistVector.push_back(new TH1F(xHistName.c_str(),xHistName.c_str(),numOfBinsForEachStrip,low,high));
		fYHistVector.push_back(new TH1F(yHistName.c_str(),yHistName.c_str(),numOfBinsForEachStrip,low,high));

/*
		for(int j = 0 ; j < numOfStrips ; j++){
			std::string name = "";
			double xlow = 0., xhigh = 0.;
			double ylow = 0., yhigh = 0.;

			fXHistVector.push_back(new TH1F(name.c_tr(),name.c_str(),numOfBinsForEachStrip,xlow,xhigh));
			fYHistVector.push_back(new TH1F(name.c_tr(),name.c_str(),numOfBinsForEachStrip,ylow,yhigh));
		}
*/
	}


}

HitDistribution::~HitDistribution() {
	// TODO Auto-generated destructor stub
}

void HitDistribution::Fill(int stripNum, double val, bool x){
	if(x){
		fXHistVector[stripNum]->Fill(val);
	}else{
		fYHistVector[stripNum]->Fill(val);
	}
}

void HitDistribution::Write(std::string fileName){

	TFile *rootFile = new TFile ( (fileName+".root").c_str(),"RECREATE","Pixel Hit Distribution") ;

	for(int i = 0  ; i < numOfStrips ; i++)
		fXHistVector[i]->Write();

	for(int i = 0  ; i < numOfStrips ; i++)
		fYHistVector[i]->Write();

	rootFile->Close();
}

void HitDistribution::Fill(double xval, double yval){
	int stripNum = (xval + 500)/stripWidth;
	if(stripNum < 32)
		Fill(stripNum,xval,true);

	stripNum = (yval + 500)/stripWidth;
	if(stripNum < 32)
		Fill(stripNum,yval,false);
}

} /* namespace Tomography */
