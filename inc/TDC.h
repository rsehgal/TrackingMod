/*
 * TDC.h
 *
 *  Created on: Mar 15, 2016
 *      Author: rsehgal
 */

#ifndef INC_TDC_H_
#define INC_TDC_H_
#include "base/Global.h"
#include<string>
#include<vector>
#include<TTree.h>

typedef std::vector<unsigned int> HitsPerChannel_v;
typedef std::vector<HitsPerChannel_v> ModuleVector_v;

namespace Tracking{

class TDC{

private:
  int fNumOfChannels;
  std::string fTDCName;
  ModuleVector_v fModule;
  TTree *fModuleTree;

  //Important TDC parameters
  int fBaseAddress;
  int fTriggerWindowWidth;
  int fTriggerWindowOffset;
  int fTriggerExtraSearchMargin;
  int fTriggerRejectMargin;
  int fEnableTriggerTimeSubstraction;
  int fIndividualLSB;


public:
  TDC():fNumOfChannels(0), fTDCName("TestTDC") {}
  TDC(int numOfChannels, std::string tdcName);

  void Set(int numOfChannels, std::string tdcName){
    fNumOfChannels = numOfChannels;
    fTDCName = tdcName;
  }

  TRACKING_INLINE
  void SetName(std::string tdcName){fTDCName = tdcName;}
  TRACKING_INLINE
  void SetNumOfChannels(int numOfChannels){fNumOfChannels = numOfChannels;}
  TRACKING_INLINE
  void SetBaseAddress(int baseAddress){fBaseAddress = baseAddress;}
  TRACKING_INLINE
  void SetTriggerWindowWidth(int triggerWindowWidth){fTriggerWindowWidth = triggerWindowWidth;}
  TRACKING_INLINE
  void SetTriggerWindowOffset(int triggerwindowOffset){fTriggerWindowOffset = triggerwindowOffset;}
  TRACKING_INLINE
  void SetTriggerExtraSearchMargin(int triggerExtraSearchMargin){fTriggerExtraSearchMargin=triggerExtraSearchMargin;}
  TRACKING_INLINE
  void SetTriggerRejectMargin(int triggerRejectMargin){fTriggerRejectMargin=triggerRejectMargin;}
  TRACKING_INLINE
  void SetEnableTriggerTimeSubstraction(int enableTriggerTimeSubstraction){fEnableTriggerTimeSubstraction = enableTriggerTimeSubstraction;}
  TRACKING_INLINE
  void SetIndividualLSB(int individualLSB){fIndividualLSB=individualLSB;}
  TRACKING_INLINE
  int GetBaseAddress(){return fBaseAddress;}
  TRACKING_INLINE
  int GetTriggerWindowWidth(){return fTriggerWindowWidth;}
  TRACKING_INLINE
  int GetTriggerWindowOffset(){return fTriggerWindowOffset;}
  TRACKING_INLINE
  int GetTriggerExtraSearchMargin(){return fTriggerExtraSearchMargin;}
  TRACKING_INLINE
  int GetTriggerRejectMargin(){return fTriggerRejectMargin;}
  TRACKING_INLINE
  int GetEnableTriggerTimeSubstraction(){return fEnableTriggerTimeSubstraction;}
  TRACKING_INLINE
  int IndividualLSB(){return fIndividualLSB;}
  TRACKING_INLINE
  std::string GetName(){return fTDCName;}
  TRACKING_INLINE
  int GetNumOfChannels(){return fNumOfChannels;}
  TRACKING_INLINE
  ModuleVector_v GetModuleVector(){return fModule;}
  TRACKING_INLINE
  TTree* GetModuleTree(){return fModuleTree;}

  void FillModuleTree();

  void Print();
  ~TDC(){}

}; //end of TDC class

}//end of Tracking namespace


#endif /* INC_TDC_H_ */
