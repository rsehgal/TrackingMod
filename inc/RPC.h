/*
 * RPC.h
 *
 *  Created on: Mar 9, 2016
 *      Author: rsehgal
 */

#ifndef INC_RPC_H_
#define INC_RPC_H_

#include "Gap.h"
#include "FEB.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include "Statistics.h"

#include "Scintillator.h"
#include "base/Global.h"
#include "Tree.h"

//Considering each RPC strip equivalent to Scintillator
#define Rpc ScintillatorPlane
#define RpcStrip Scintillator

namespace Tracking {
class RPC{
private:
	std::vector<Gap*> gap;
	FEB feb[3]; //0->(0-31) ; 1->(32-63) ; 2->(64-95)
	std::string fName;
	std::string fEtaPartition;
	std::string fTriggerLayer;

	int fNumOfChannels;
	int fModuleId;
	//std::vector<RpcStrip> fReadOut;
	Rpc *rpc;//(2,96,"First-RPC");

    Precision fEfficiency;
    int fEventCount;

	Statistics stat;

public:
  int map_station;
  int map_module;
  int map_start;
  int map_finish;
  int map_module1;
  int map_start1;
  int map_finish1;

public:
	RPC(){/*gap = new Gap[3];*/
		gap.push_back(new Gap());
		gap.push_back(new Gap());
		gap.push_back(new Gap());
	}
	RPC(std::string, Precision, Precision, Precision, int);
	RPC(int moduleId, int numOfChannels, std::string rpcName, int startId = 31);
	RPC(int moduleId, int numOfChannels, std::string rpcName,double zPos, int startId = 31);
	~RPC(){}

	void SetStatictics(){stat.SetStatistics(fName,map_station,fEtaPartition,fTriggerLayer);}

	void SetGap(int i,std::string gapName, Precision gapCurr, Precision gapVset, Precision gapVmon, int gapStation);
	void SetFeb(int i,Precision th1, Precision th2, Precision th3, Precision th4);
	TRACKING_INLINE
	Gap GetGap(int i){
		return *gap[i];
	}

	TRACKING_INLINE
	Rpc* GetRpc(){
	  return rpc;
	}

	TRACKING_INLINE
	void SetName(std::string name){fName = name;}
	TRACKING_INLINE
	void SetEtaPartition(std::string etaPartition){fEtaPartition = etaPartition;}

	TRACKING_INLINE
	void SetTriggerLayer(std::string triggerLayer){fTriggerLayer = triggerLayer;}

	void Set(std::string name, std::string etaPartition, std::string triggerLayer){
	  fName=name;
	  fEtaPartition = etaPartition;
	  fTriggerLayer = triggerLayer;
	}

	TRACKING_INLINE
	std::string GetEtaPartition(){return fEtaPartition;}
	TRACKING_INLINE
	std::string GetTriggerLayer(){return fTriggerLayer;}

	TRACKING_INLINE
	std::string GetName(){return fName;}

	int GetGapVectorSize(){return gap.size();}

	TRACKING_INLINE
	FEB GetFeb(int i){return feb[i];}
	friend inline std::ostream& operator<<(std::ostream& os, RPC const &rpc) {
			os << "{"<<std::endl<<
					//"TopNarrow : " << rpc.gap[0] << std::endl<<
					//"TopWide : " << rpc.gap[1] << std::endl<<
					//"Bottom : " << rpc.gap[2] << std::endl<<
			        "}"<<std::endl;
			/*os << "{"<<std::endl<<
					"FEB[0] : " << rpc.feb[0] << std::endl<<
					"FEB[1] : " << rpc.feb[1] << std::endl<<
					"FEB[2] : " << rpc.feb[2] << std::endl<<
					"}"<<std::endl;
			 */

			  return os;
			}
	void Print(){
		for(int i=0;i<3;i++){
			std::cout<<*gap[i]<<std::endl;
		}
		for(int i=0;i<3;i++){
			std::cout<<feb[i]<<std::endl;
		}

	}

	void PrintStrips(){
	  rpc->PrintStrips();
	}

	bool EventDetected(Tree &t, int evNo);
	Precision CalculateEfficiency(Tree &t);
	Precision GetEfficiency(int numOfEvents){
		//return CalculateEfficiency();
	    return (Precision(fEventCount)/numOfEvents)*100.;

	}

	int GetEventCount(){return fEventCount;}
	void GetHitMap(){rpc->GetHitMap();}
	int GetHitCount(){return rpc->GetShowerCount();}
	TGeoVolume* GetPlaneTGeoVolume(){return rpc->GetPlaneTGeoVolume();}
#ifdef USE_EVE
	void Show(){rpc->Show();}
#endif

};//end of RPC class

}//end of Tracking namespace




#endif /* INC_RPC_H_ */
