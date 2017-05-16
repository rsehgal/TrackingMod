/*
 * GenericDetector.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: rsehgal
 */

#include "GenericDetector.h"

namespace Tomography {

GenericDetector::GenericDetector() {
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

GenericDetector::GenericDetector(std::string detName, std::string detType,int startId,int numOfPlanes,int numOfStrips, int clusterSize,double zpos,int moduleId1="2", moduleId2="2"){
	SetDetectorType(detType);
	Scintillator::SetStartingId(startId);
	Scintillator::SetStartingStripNum();
	//int numOfPlanes = numOfPlanes;
	SetNumOfPlanes(numOfPlanes);
	SetName(detName);
	SetZPos(zpos);
	SetLBH(100,100,1);
	Initialize();
	if(numOfPlanes==1){
		//This will be used to create top and bottom scintillator plane
		InsertPlane(new ScintillatorPlane(moduleId1,numOfStrips,zpos,length,breadth,height,0.,true));
	}
	if(numOfPlanes==2){
		//This will be used to create Detectors
	InsertPlane(new ScintillatorPlane(moduleId1,numOfStrips,zpos,length,breadth,height,0.,true));
	InsertPlane(new ScintillatorPlane(moduleId2,numOfStrips,zpos,length,breadth,height,0.,false));
	}
	SetClusterSize(clusterSize);

}

GenericDetector::~GenericDetector() {
	// TODO Auto-generated destructor stub
}

} /* namespace Tomography */
