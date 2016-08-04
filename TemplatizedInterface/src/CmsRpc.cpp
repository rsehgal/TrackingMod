/*
 * CmsRpc.cpp
 *
 *  Created on: May 12, 2016
 *      Author: rsehgal
 */

#include "CmsRpc.h"
#include "Scintillator.h"
#include "ScintillatorPlane.h"

namespace Tomography {

//int CmsRpc::fClusterSize = 2;

CmsRpc::CmsRpc() {
  // TODO Auto-generated constructor stub
}

CmsRpc::~CmsRpc() {
  // TODO Auto-generated destructor stub
}

CmsRpc::CmsRpc(std::string name, std::vector<int> channelDimVector):
    Properties(name, channelDimVector){
  int numOfPlanes=2;
  SetNumOfPlanes(numOfPlanes);
  /*for(int i = 0  ; i < numOfPlanes ; i++){
    InsertPlane(new ScintillatorPlane());
  }*/
}

CmsRpc::CmsRpc(int moduleId, std::string rpcName, double zPos, int startId) {
  SetDetectorType("CMS");
  Scintillator::SetStartingId(startId);
  int numOfPlanes = 3;
  SetNumOfPlanes(numOfPlanes);
  SetName(rpcName);
  SetZPos(zPos);
  SetLBH(159,80,1);
  Initialize();
  /*for(int i = 0 ; i < numOfPlanes ; i++){
    InsertPlane(new ScintillatorPlane(moduleId,32,zPos,GetLength(),GetBreadth()));
  }*/
  InsertPlane(new ScintillatorPlane(moduleId,32,zPos,GetLength()/3,GetBreadth(),GetHeight(),-1*GetLength()/3.,false));
  InsertPlane(new ScintillatorPlane(moduleId,32,zPos,GetLength()/3,GetBreadth(),GetHeight(),0.,false));
  InsertPlane(new ScintillatorPlane(moduleId,32,zPos,GetLength()/3,GetBreadth(),GetHeight(),GetLength()/3.,false));
  SetClusterSize(2);

}

} /* namespace Tomography */
