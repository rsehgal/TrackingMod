/*
 * AllRunLayout.cpp
 *
 *  Created on: Mar 24, 2017
 *      Author: rsehgal
 */

#include "AllRunLayout.h"
#include "Run.h"
AllRunLayout::AllRunLayout() {
	// TODO Auto-generated constructor stub

}

AllRunLayout::~AllRunLayout() {
	// TODO Auto-generated destructor stub
}

void AllRunLayout::CreateLayout(std::vector<QString> runNumVect, std::vector<Detector*> detectorVect){

	for(int i=0 ; i<runNumVect.size() ; i++){

		fVBoxLayout->addLayout((new Run(runNumVect[i],detectorVect))->GetLayout());
	}
	setLayout(fVBoxLayout);
}

