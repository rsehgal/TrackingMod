/*
 * GenericXYDetector.cpp
 *
 *  Created on: Apr 7, 2017
 *      Author: rsehgal
 */

#include "GenericXYDetector.h"
#include "SetupManager.h"

namespace Tomography {

GenericXYDetector::GenericXYDetector() {
	// TODO Auto-generated constructor stub

}

GenericXYDetector::~GenericXYDetector() {
	// TODO Auto-generated destructor stub
}

GenericXYDetector::GenericXYDetector(int moduleId,std::string detName,double zPos,int startId, int numOfStrips, double length, double breadth, double height){
	SetDetectorType("TRG");
	  Scintillator::SetStartingId(startId);
	  Scintillator::SetStartingStripNum();
	  int numOfPlanes = 2;
	  SetNumOfPlanes(numOfPlanes);
	  SetName(detName);
	  SetZPos(zPos);
	  SetLBH(length,breadth,height);
	  Initialize();
	  /*for(int i = 0 ; i < numOfPlanes ; i++){
	    InsertPlane(new ScintillatorPlane(moduleId,32,zPos,GetLength(),GetBreadth()));
	  }*/
	  InsertPlane(new ScintillatorPlane(moduleId,numOfStrips,zPos,GetLength(),GetBreadth(),GetHeight(),0.,true));
	  InsertPlane(new ScintillatorPlane(moduleId,numOfStrips,zPos,GetLength(),GetBreadth(),GetHeight(),0.,false));

	  Tomography::SetupManager::instance()->Register(this);


}
} /* namespace Tomography */
