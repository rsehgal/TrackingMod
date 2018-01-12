/*
 * Hist2D.cpp
 *
 *  Created on: Jan 9, 2018
 *      Author: rsehgal
 */

#include "Hist2D.h"

namespace Tomography {

Hist2D::Hist2D() {
	// TODO Auto-generated constructor stub

}

Hist2D::~Hist2D() {
	// TODO Auto-generated destructor stub
}

Hist2D::Hist2D(std::string histname, int binsx, double startx, double endx,int binsy, double starty, double endy){
	fBinsX = binsx;
	fStartX = startx;
	fEndX = endx;
	fBinsY = binsy;
	fStartY = starty;
	fEndY = endy;
	binsizeX = (fEndX-fStartX)/fBinsX;
	binsizeY = (fEndY-fStartY)/fBinsY;
	SetNumOfBins(fBinsX*fBinsY);

}

int Hist2D::GetBinNum(double x, double  y){
	int binNum;
	//Logic to calculate bin num
	int xbinNum = (x - fStartX) / binsizeX;
	int ybinNum = (y - fStartY) / binsizeY;
	if(ybinNum)
		binNum = (ybinNum)*fBinsX + xbinNum;
	else
		binNum = xbinNum;
	return binNum;
}

void Hist2D::Fill(double x, double y){
	Fill2(GetBinNum(x,y));
}

} /* namespace Tomography */
