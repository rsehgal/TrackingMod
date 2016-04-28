/*
 * RPC.cpp
 *
 *  Created on: Mar 9, 2016
 *      Author: rsehgal
 */


#include "RPC.h"

namespace Tracking{

RPC::RPC(std::string gapName, Precision curr, Precision vset, Precision vmon, int stat):
    fEventCount(0){

}

RPC::RPC(int moduleId, int numOfChannels, std::string rpcName,int startId):
  fModuleId(moduleId),
  fNumOfChannels(numOfChannels),
  fName(rpcName),
  fEventCount(0){

  Scintillator::SetStartingId(startId);
  //rpc->resize(fNumOfChannels);
  //rpc = new ScintillatorPlane(fModuleId,fNumOfChannels,fName);
  rpc = new Rpc(fModuleId,fNumOfChannels,fName);
}

RPC::RPC(int moduleId, int numOfChannels, std::string rpcName,double zPos,int startId):
  fModuleId(moduleId),
  fNumOfChannels(numOfChannels),
  fName(rpcName),
  fEventCount(0){

  Scintillator::SetStartingId(startId);
  //rpc->resize(fNumOfChannels);
  //rpc = new ScintillatorPlane(fModuleId,fNumOfChannels,fName);
  rpc = new Rpc(fModuleId,fNumOfChannels,zPos,true,fName);
}

void RPC::SetGap(int i, std::string gapName, Precision gapCurr, Precision gapVset, Precision gapVmon, int gapStation){
//	/std::cout<<"Entered SetGap"<<std::endl;

	gap[i]->Set(gapName,gapCurr,gapVset,gapVmon,gapStation);
/*	if(i==0)
		gap_tn.Set(gapName,gapCurr,gapVset,gapVmon,gapStation);
	if(i==1)
		gap_tw.Set(gapName,gapCurr,gapVset,gapVmon,gapStation);
	if(i==2)
		gap_b.Set(gapName,gapCurr,gapVset,gapVmon,gapStation);*/

}


/*Gap RPC::GetGap2(int i){
	std::cout<<"Entered GETGap"<<std::endl;
if(i==0)
	return gap_tn;
if(i==1)
	return gap_tw;
if(i==2)
	return gap_b;
}*/

void RPC::SetFeb(int i, Precision th1,Precision th2, Precision th3, Precision th4){

	feb[i].Set(th1,th2,th3,th4);
}

Precision RPC::CalculateEfficiency(Tree &t){
    //return (fEventCount/Tree::GetNumOfEvents())*100.;
    return (fEventCount/t.GetNumOfEvents())*100.;
}

bool RPC::EventDetected(Tree &t, int evNo){
    bool detected = rpc->IsShowerEvent<true>(t,evNo);
    if(detected) 
        fEventCount++;
    return detected; 
}

}// end of Tracking namespace

