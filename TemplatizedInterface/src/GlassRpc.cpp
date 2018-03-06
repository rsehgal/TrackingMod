/*
 * GlassRpc.cpp
 *
 *  Created on: May 12, 2016
 *      Author: rsehgal
 */

#include "GlassRpc.h"
#include "Scintillator.h"
#include "ScintillatorPlane.h"
#include "SetupManager.h"

namespace Tomography {

//int GlassRpc::fClusterSize = 2;
GlassRpc::GlassRpc() {
  // TODO Auto-generated constructor stub
}

GlassRpc::~GlassRpc() {
  // TODO Auto-generated destructor stub
}

GlassRpc::GlassRpc(std::string name, std::vector<int> channelDimVector):
    Properties(name, channelDimVector){
  int numOfPlanes=2;
  SetNumOfPlanes(numOfPlanes);
  /*for(int i = 0  ; i < numOfPlanes ; i++){
    InsertPlane(new ScintillatorPlane());
  }*/
}

//Currently using this Constructor
GlassRpc::GlassRpc(int moduleId, std::string rpcName, double zPos, int startId) {
  SetDetectorType("GLASS");
  Scintillator::SetStartingId(startId);
  Scintillator::SetStartingStripNum();
  int numOfPlanes = 2;
  SetNumOfPlanes(numOfPlanes);
  SetName(rpcName);
  SetZPos(zPos);
  SetLBH(1000,1000,1);
  Initialize();
  /*for(int i = 0 ; i < numOfPlanes ; i++){
    InsertPlane(new ScintillatorPlane(moduleId,32,zPos,GetLength(),GetBreadth()));
  }*/
  InsertPlane(new ScintillatorPlane(moduleId,32,zPos,GetLength(),GetBreadth(),GetHeight(),0.,true));
  InsertPlane(new ScintillatorPlane(moduleId,32,zPos,GetLength(),GetBreadth(),GetHeight(),0.,false));
  //SetClusterSize(2);
  Tomography::SetupManager::instance()->Register(this);

}

GlassRpc::GlassRpc(int moduleId, std::string rpcName, double xPos, double yPos, double zPos, double theta,  int startId) {
	SetDxDyDTheta(xPos,yPos,theta);
  SetDetectorType("GLASS");
  Scintillator::SetStartingId(startId);
  Scintillator::SetStartingStripNum();
  int numOfPlanes = 2;
  SetNumOfPlanes(numOfPlanes);
  SetName(rpcName);
  SetZPos(zPos);
  SetLBH(1000,1000,1);
  Initialize();
  /*for(int i = 0 ; i < numOfPlanes ; i++){
    InsertPlane(new ScintillatorPlane(moduleId,32,zPos,GetLength(),GetBreadth()));
  }*/
  InsertPlane(new ScintillatorPlane(moduleId,32,xPos,yPos,zPos,theta,GetLength(),GetBreadth(),GetHeight(),0.,true));
  InsertPlane(new ScintillatorPlane(moduleId,32,xPos,yPos,zPos,theta,GetLength(),GetBreadth(),GetHeight(),0.,false));
}

} /* namespace Tomography */
