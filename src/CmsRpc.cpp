/*
 * CmsRpc.cpp
 *
 *  Created on: May 2, 2016
 *      Author: rsehgal
 */

#include "CmsRpc.h"
#include "Scintillator.h"

namespace Tracking{

CmsRpc::CmsRpc(int moduleId, int numOfChannelsA, int numOfChannelsB, int numOfChannelsC,
    std::string rpcName,double zPos, int startId):
    fModuleId(moduleId),
    fName(rpcName),
    fNumOfChannelsA(numOfChannelsA),
    fNumOfChannelsB(numOfChannelsB),
    fNumOfChannelsC(numOfChannelsC),
    fNumOfChannels(fNumOfChannelsA + fNumOfChannels + fNumOfChannelsC ){

  Initialize(startId, zPos);
}

CmsRpc::CmsRpc(int moduleId, std::string rpcName,double zPos, int startId ):
    fModuleId(moduleId),
    fName(rpcName),
    fNumOfChannelsA(32),
    fNumOfChannelsB(32),
    fNumOfChannelsC(32),
    fNumOfChannels(fNumOfChannelsA + fNumOfChannelsB + fNumOfChannelsC ){

  Initialize(startId,zPos);
}

void CmsRpc::Initialize(int startId,double zPos){
  Scintillator::SetStartingId(startId);
  // Scintillator::SetStartingId(fModuleId,fNumOfchannels,zPos,true,len, brd, fName);
  fEtaA = new ScintillatorPlane(fModuleId, fNumOfChannelsA, zPos, true, 60, 53, fName);
  fEtaB = new ScintillatorPlane(fModuleId, fNumOfChannelsB, zPos, true, 70, 53, fName);
  fEtaC = new ScintillatorPlane(fModuleId, fNumOfChannelsC, zPos, true, 80, 53, fName);
}

void CmsRpc::PrintStrips(){
  fEtaA->PrintStrips();
  fEtaB->PrintStrips();
  fEtaC->PrintStrips();
}

}//end of Tracking namespace


