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
//std::vector<ScintillatorBar*> eventsVec2={};

/*void PrintPsBarVector(){
	std::cout <<"6666666666 Printing from PrintPsBarVector 66666666" << std::endl;
	for(unsigned int i = 0 ; i < eventsVec2.size(); i++){
		eventsVec2[i]->Print();
	}
}*/

namespace Tomography {

PsBar::PsBar() {
	// TODO Auto-generated constructor stub

}

PsBar::~PsBar() {
	// TODO Auto-generated destructor stub
}

} /* namespace Tomography */