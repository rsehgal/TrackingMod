/*
 * Hist2D.h
 *
 *  Created on: Jan 9, 2018
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_HIST2D_H_
#define HELPERS_INC_HIST2D_H_

#include <string>
#include "Histogram.h"

namespace Tomography {

class Hist2D : public Histogram {
public:
	Hist2D();
	virtual ~Hist2D();
	Hist2D(std::string histname, int binsx, double startx, double endx,int binsy, double starty, double endy);
	void Fill(double x, double y);
	int GetBinNum(double x, double y);

public:
	double fStartX;
	double fEndX;
	double fStartY;
	double fEndY;
	int fBinsX;
	int fBinsY;
	double binsizeX;
	double binsizeY;

};

} /* namespace Tomography */

#endif /* HELPERS_INC_HIST2D_H_ */
