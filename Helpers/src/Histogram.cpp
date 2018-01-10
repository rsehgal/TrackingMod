/*
 * Histogram.cpp
 *
 *  Created on: Jan 9, 2018
 *      Author: rsehgal
 */

#include "Histogram.h"

namespace Tomography {

Histogram::Histogram() {
	// TODO Auto-generated constructor stub
	fCutVal = 1e+10;

}

Histogram::~Histogram() {
	// TODO Auto-generated destructor stub
}

void Histogram::SetNumOfBins(int nbins){
	fBins.clear();
	fTotalNumOfBins = nbins;
	fBins.resize(fTotalNumOfBins,0);

}

} /* namespace Tomography */
