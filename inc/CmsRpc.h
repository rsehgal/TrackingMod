/*
 * CmsRpc.h
 *
 *  Created on: May 2, 2016
 *      Author: rsehgal
 */

#ifndef INC_CMSRPC_H_
#define INC_CMSRPC_H_

#include "base/Global.h"
#include <vector>
#include <algorithm>
#include "Scintillator.h"
//class Scintillator;
//class ScintillatorPlane;


namespace Tracking{

class CmsRpc{
  int fNumOfChannelsA;
  int fNumOfChannelsB;
  int fNumOfChannelsC;
  int fModuleId;
  ScintillatorPlane *fEtaA;
  ScintillatorPlane *fEtaB;
  ScintillatorPlane *fEtaC;
  std::string fName;
  int fNumOfChannels;

private:

public:
  CmsRpc(int moduleId, int numOfChannelsA, int numOfChannelsB, int numOfChannelsC,
      std::string rpcName,double zPos, int startId=31);
  CmsRpc(int moduleId, std::string rpcName,double zPos, int startId=31 );
  ScintillatorPlane* GetEtaA(){return fEtaA;}
  ScintillatorPlane* GetEtaB(){return fEtaB;}
  ScintillatorPlane* GetEtaC(){return fEtaC;}
  std::string GetName(){return fName;}
  int GetNumOfChannelsA(){return fNumOfChannelsA;}
  int GetNumOfChannelsB(){return fNumOfChannelsB;}
  int GetNumOfChannelsC(){return fNumOfChannelsC;}
  void Initialize(int startId,double zPos);



  //Helper and debugging functions
  void PrintStrips();

};

}//end of Tracking namespace



#endif /* INC_CMSRPC_H_ */
