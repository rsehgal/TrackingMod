/*
 * DetectorFactory.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: rsehgal
 */

#include "DetectorFactory.h"

namespace Tomography {

DetectorFactory::DetectorFactory() {
	// TODO Auto-generated constructor stub
#if(0)
	SetDetectorType("GLASS");
	Scintillator::SetStartingId(startId);
	Scintillator::SetStartingStripNum();
	int numOfPlanes = 2;
	SetNumOfPlanes(numOfPlanes);
	SetName(rpcName);
	SetZPos(zPos);
	SetLBH(100,100,1);
	Initialize();
	if(numOfPlanes==1){
		//This will be used to create top and bottom scintillator plane
		InsertPlane(new ScintillatorPlane(moduleId,32,zPos,GetLength(),GetBreadth(),GetHeight(),0.,true));
	}
	if(numOfPlanes==2){
		//This will be used to create Detectors
	InsertPlane(new ScintillatorPlane(moduleId,32,zPos,GetLength(),GetBreadth(),GetHeight(),0.,true));
	InsertPlane(new ScintillatorPlane(moduleId,32,zPos,GetLength(),GetBreadth(),GetHeight(),0.,false));
	}
	SetClusterSize(2);
#endif
}

DetectorFactory::~DetectorFactory() {
	// TODO Auto-generated destructor stub
}

} /* namespace Tomography */
