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

}

Histogram::~Histogram() {
	// TODO Auto-generated destructor stub
}

void Histogram::SetNumOfBins(int nbins){
	fBins.clear();
	fTotalNumOfBins = nbins;
	fBins.reserve(fTotalNumOfBins);
}

} /* namespace Tomography */
