/*
 * Histograms.h
 *
 *  Created on: 04-Sep-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_INC_HISTOGRAMS_H_
#define ISMRAN_ANALYSIS_INC_HISTOGRAMS_H_


#include <TH1D.h>

class TH1D;
class ScintillatorBar_V2;
class TPad;

class Histograms {
//Declaring Public for the time being
	unsigned short int padWidth ;
	unsigned short int padHeight ;
public:
	TH1D *fhistQNear;
	TH1D *fhistQFar;
	TH1D *fhistQMean;
	TH1D *fhistDelT;
	TH1D *fhistDelTCorrected;

	/*TPad *fhistQNearFarPad;
	//TPad *fhistQFarPad;
	TPad *fhistQMeanPad;
	TPad *fhistDelTPad;
	TPad *fhistDelTCorrectedPad;*/

public:
	Histograms();
	Histograms(std::string barName, unsigned int barIndex);
	void FillHistogram(ScintillatorBar_V2 *scintBarVec);
	virtual ~Histograms();
};

#endif /* ISMRAN_ANALYSIS_INC_HISTOGRAMS_H_ */
