/*
 * PsBar.cpp
 *
 *  Created on: 14-Aug-2020
 *      Author: rsehgal
 */

#include "PsBar.h"

const int offset=0;
const double kDelTBar = 32.0; //! ns
TStopwatch timer;
unsigned int numOfLayers=3;
unsigned int numOfBarsInEachLayer=9;
double fwhm = 2.355 * 5.66353 ;

namespace Tomography {

PsBar::PsBar() {
	// TODO Auto-generated constructor stub

}

PsBar::~PsBar() {
	// TODO Auto-generated destructor stub
}

} /* namespace Tomography */
