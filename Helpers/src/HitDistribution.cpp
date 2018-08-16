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

using Detector = Properties;
int numOfStrips = 32;
double stripWidth =  100*cm/numOfStrips ;

HitDistribution::HitDistribution() {
	//Initialize();
	//Initialize2();
	Initialize3();
}


void HitDistribution::Initialize(){
	// TODO Auto-generated constructor stub
	//int numOfStrips = 32;
	int numOfBinsInAPixel = 100;
//	double stripWidth =  100*cm/numOfStrips ;
	int numOfBinsForEachStrip = 30;
	for(int i = 0  ; i < numOfStrips ; i++){

		double low = -50*cm+(i+1)*stripWidth;
		double high = low + stripWidth;
		std::string index = std::to_string(i);
		std::string xHistName=fDetectorName+"X_Hist_"+ index;
		std::string yHistName=fDetectorName+"Y_Hist_"+ index;
		fXHistVector.push_back(new TH1F(xHistName.c_str(),xHistName.c_str(),numOfBinsForEachStrip,low,high));
		fYHistVector.push_back(new TH1F(yHistName.c_str(),yHistName.c_str(),numOfBinsForEachStrip,low,high));
	}
}

void HitDistribution::Initialize2(){
	// TODO Auto-generated constructor stub
	int numOfBinsForEachStrip = 30;
	for(int x = 0  ; x < numOfStrips ; x++){

		double xlow = -50*cm+(x+1)*stripWidth;
		double xhigh = xlow + stripWidth;

		for (int y = 0 ; y < numOfStrips ; y++) {

//			double xlow = -50*cm+(x+1)*stripWidth;
//			double xhigh = xlow + stripWidth;
			double ylow = -50*cm+(y+1)*stripWidth;
			double yhigh = ylow + stripWidth;
			//std::string index = std::to_string(i);
			std::string pixelIndex = std::to_string(numOfStrips*x+y);
//			std::string xHistName=fDetectorName+"X_Hist_"+ index;
//			std::string yHistName=fDetectorName+"Y_Hist_"+ index;
			std::string xHistName=fDetectorName+"X_Hist_"+ pixelIndex;
			std::string yHistName=fDetectorName+"Y_Hist_"+ pixelIndex;
			fXHistVector.push_back(new TH1F(xHistName.c_str(),xHistName.c_str(),numOfBinsForEachStrip,xlow,xhigh));
			fYHistVector.push_back(new TH1F(yHistName.c_str(),yHistName.c_str(),numOfBinsForEachStrip,ylow,yhigh));
		}
	}
}

void HitDistribution::Initialize3(){
	// TODO Auto-generated constructor stub
	int numOfBinsForEachStrip = 30;
	for(int x = 0  ; x < numOfStrips ; x++){

		double xlow = -50*cm+(x+1)*stripWidth;
		double xhigh = xlow + stripWidth;

		for (int y = 0 ; y < numOfStrips ; y++) {

			double ylow = -50*cm+(y+1)*stripWidth;
			double yhigh = ylow + stripWidth;
			int pixelNumber = numOfStrips*x+y;
			std::cout << "Creating Histogram for Pixel : " << pixelNumber << std::endl;
			fHistVector.push_back(new Hist(fDetectorName,pixelNumber,xlow,xhigh,ylow,yhigh));
		}
	}
}

void HitDistribution::Initialize4(){
	// TODO Auto-generated constructor stub
	int numOfBinsForEachStrip = 30;
	for(int x = 0  ; x < numOfStrips ; x++){

		double xlow = -50*cm + x*stripWidth;
		double xhigh = xlow + stripWidth;

		for (int y = 0 ; y < numOfStrips ; y++) {

			double ylow = -50*cm+ y*stripWidth;
			double yhigh = ylow + stripWidth;
			int pixelNumber = numOfStrips*x+y;
			std::cout << "Creating Histogram for Pixel : " << pixelNumber << " : Of Detector : "
					  << fDetectorName << std::endl;
			fHistVector.push_back(new Hist(fDetectorName,pixelNumber,xlow,xhigh,ylow,yhigh));
		}

	}

	//Creating Histogram of full Detector
	fFullDetectorHistX = new TH1F("FullDetectorHistogram-X","FullDetectorHistogram-X",32,-50*cm,50*cm);
	fFullDetectorHistY = new TH1F("FullDetectorHistogram-Y","FullDetectorHistogram-Y",32,-50*cm,50*cm);

}


HitDistribution::HitDistribution(std::string detectorName)  {
	fDetectorName = detectorName ;
	Initialize4();
}

HitDistribution::HitDistribution(Detector *detector):fDetector(detector) {
	HitDistribution(fDetector->GetName());
}



HitDistribution::~HitDistribution() {
	// TODO Auto-generated destructor stub
}

//void HitDistribution::Fill(int stripNum, double val, bool x){
void HitDistribution::Fill(int pixelNum, double val, bool x){
	if(x){
		fXHistVector[pixelNum]->Fill(val);
	}else{
		fYHistVector[pixelNum]->Fill(val);
	}
}

void HitDistribution::Fill(double xval, double yval){
/*
	int stripNum = (xval + 500)/stripWidth;
	if(stripNum < 32)
		Fill(stripNum,xval,true);

	stripNum = (yval + 500)/stripWidth;
	if(stripNum < 32)
		Fill(stripNum,yval,false);
*/
	if((xval > -500 && xval < 500) && (yval > -500 && yval < 500)){
	int xStripNum = (xval + 500)/stripWidth;
	int yStripNum = (yval + 500)/stripWidth;
//	xStripNum--;
//	yStripNum--;
	int pixelIndex = numOfStrips*xStripNum+yStripNum ;
	if(pixelIndex < 1024 && pixelIndex >= 0){
	std::cout << "PixelIndex : " << pixelIndex << std::endl;
//	Fill(pixelIndex,xval,true);
//	Fill(pixelIndex,yval,false);
	std::cout << "XMin : " << fHistVector[pixelIndex]->sXMin << " :: XMax : "<< fHistVector[pixelIndex]->sXMax << std::endl;

	//if(fHistVector[pixelIndex]->sXMin < xval && fHistVector[pixelIndex]->sXMax > xval)
	fHistVector[pixelIndex]->sXHist->Fill(xval);
	fHistVector[pixelIndex]->sYHist->Fill(yval);
	}

	//storing values for hit graph
	fXValueVector.push_back(xval);
	fYValueVector.push_back(yval);
	fFullDetectorHistX->Fill(xval);
	fFullDetectorHistY->Fill(yval);

	//fHistVector[pixelIndex]->sYHist->Fill(yval);
	}
}

/*void HitDistribution::Write(std::string fileName){

	TFile *rootFile = new TFile ( (fileName+".root").c_str(),"RECREATE","Pixel Hit Distribution") ;

	for(int i = 0  ; i < numOfStrips ; i++)
		fXHistVector[i]->Write();

	for(int i = 0  ; i < numOfStrips ; i++)
		fYHistVector[i]->Write();

	rootFile->Close();
}*/

void HitDistribution::Write(){

}

void HitDistribution::Write(std::string fileName){

	std::cout << "FileName : " << fileName << std::endl;
	//TFile *rootFile = new TFile ( (fFileName+".root").c_str(),"RECREATE","Pixel Hit Distribution") ;
	TFile *rootFile = new TFile ( (fileName+".root").c_str(),"RECREATE","Pixel Hit Distribution") ;

	fHitGraph = new TGraph(fXValueVector.size(),&fXValueVector[0],&fYValueVector[0]);
	fHitGraph->Draw();
	fHitGraph->Write("ap");
	fFullDetectorHistX->Draw();
	fFullDetectorHistY->Draw();
	fFullDetectorHistX->Write();
	fFullDetectorHistY->Write();
	for(int x = 0  ; x < numOfStrips ; x++){
		for (int y = 0 ; y < numOfStrips ; y++) {
				int pixelNumber = numOfStrips*x+y;
				fHistVector[pixelNumber]->sXHist->Write();
				fHistVector[pixelNumber]->sYHist->Write();
		}
	}
	/*for(int i = 0  ; i < numOfStrips ; i++)
		fXHistVector[i]->Write();

	for(int i = 0  ; i < numOfStrips ; i++)
		fYHistVector[i]->Write();*/

//	fHitGraph = new TGraph(fXValueVector.size(),&fXValueVector[0],&fYValueVector[0]);
//	fHitGraph->Write();
	rootFile->Close();
}

void HitDistribution::Delete(){
  for(int i = 0 ; i  < fHistVector.size() ; i++){
	  delete fHistVector[i];
  }
}



} /* namespace Tomography */
