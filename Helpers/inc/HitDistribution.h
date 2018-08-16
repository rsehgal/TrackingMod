/*
 * HitDistribution.h
 *
 *  Created on: Apr 13, 2018
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_HITDISTRIBUTION_H_
#define HELPERS_INC_HITDISTRIBUTION_H_

#include <vector>
#include <iostream>
#include <TH1F.h>
#include <TGraph.h>
#include "Properties.h"
namespace Tomography {

using Detector = Properties;

struct Hist{

	TH1F *sXHist;
	TH1F *sYHist;
	double sXMin;
	double sXMax;
	double sYMin;
	double sYMax;
	int sNumOfBins = 30;
	int sPixelNumber;

	Hist(std::string detectorName, int pixelNumber, double xMin, double xMax, double yMin, double yMax) {
		sPixelNumber = pixelNumber;
		sXMin=xMin; sXMax=xMax;
        sYMin=yMin; sYMax=yMax;
        std::string xHistName = (detectorName+"XHist_"+std::to_string(pixelNumber));
        std::string yHistName = (detectorName+"YHist_"+std::to_string(pixelNumber));
		sXHist = new TH1F(xHistName.c_str(), xHistName.c_str(), sNumOfBins, sXMin, sXMax);
		sYHist = new TH1F(yHistName.c_str(), yHistName.c_str(), sNumOfBins, sYMin, sYMax);
	}
};

class HitDistribution {
	std::vector<TH1F*>fXHistVector;
	std::vector<TH1F*>fYHistVector;
	std::vector<Hist*> fHistVector;
	TGraph *fHitGraph;
	std::string fFileName;
	std::string fDetectorName ;//= "TestDetector";
	Detector *fDetector;
	std::vector<double> fXValueVector;
	std::vector<double> fYValueVector;
	TH1F* fFullDetectorHistX;
	TH1F* fFullDetectorHistY;

public:
	HitDistribution();
	HitDistribution(Detector *detector);
	HitDistribution(std::string detectorName);
	virtual ~HitDistribution();
	void Fill(int stripNum, double val, bool x);
	void Fill(double xval, double yval);
	void Write(std::string fileName);
	void Write();
	void Initialize();
	void Initialize2();
	void Initialize3();
	void Initialize4();

	//Function to delete the objects at the end,
	//Must be called at the end, but be careful.
	void Delete();
};

} /* namespace Tomography */

#endif /* HELPERS_INC_HITDISTRIBUTION_H_ */
