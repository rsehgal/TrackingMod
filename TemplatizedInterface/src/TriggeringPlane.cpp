/*
 * TriggeringPlane.cpp
 *
 *  Created on: May 12, 2016
 *      Author: rsehgal
 */

#include "TriggeringPlane.h"
#include "Scintillator.h"
#include "ScintillatorPlane.h"

namespace Tomography {

TriggeringPlane::TriggeringPlane() {
  // TODO Auto-generated constructor stub
}

TriggeringPlane::~TriggeringPlane() {
  // TODO Auto-generated destructor stub
}

TriggeringPlane::TriggeringPlane(std::string name, std::vector<int> channelDimVector):
    Properties(name, channelDimVector){
  int numOfPlanes=2;
  SetNumOfPlanes(numOfPlanes);
  /*for(int i = 0  ; i < numOfPlanes ; i++){
    InsertPlane(new ScintillatorPlane());
  }*/
}

TriggeringPlane::TriggeringPlane(int moduleId, std::string rpcName, double zPos, int startId) {
  SetDetectorType("TRG");
  Scintillator::SetStartingId(startId);
  int numOfPlanes = 1;
  SetNumOfPlanes(numOfPlanes);
  SetName(rpcName);
  SetZPos(zPos);
  SetLBH(144,180,1);
  Initialize();
  /*for(int i = 0 ; i < numOfPlanes ; i++){
    InsertPlane(new ScintillatorPlane(moduleId,32,zPos,GetLength(),GetBreadth()));
  }*/
  //InsertPlane(new ScintillatorPlane(moduleId,32,zPos,GetLength()/3,GetBreadth(),GetHeight(),-1*GetLength()/3.,false));
  InsertPlane(new ScintillatorPlane(moduleId,8,zPos,GetLength(),GetBreadth(),GetHeight(),0.,false));
  //InsertPlane(new ScintillatorPlane(moduleId,32,zPos,GetLength()/3,GetBreadth(),GetHeight(),GetLength()/3.,false));
}

} /* namespace Tomography */
