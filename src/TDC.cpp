/*
 * TDC.cpp
 *
 *  Created on: Mar 15, 2016
 *      Author: rsehgal
 */

#include "TDC.h"
#include <iostream>
namespace Tracking{

TDC::TDC(int numOfChannels,std::string tdcName): fNumOfChannels(numOfChannels), fTDCName(tdcName) {

}

void TDC::Print(){

  std::cout<<"{ "<<std::endl<<"Name : "<<fTDCName<<std::endl
           <<"TriggerWindowWidth : "<<fTriggerWindowWidth<<std::endl
           <<"TriggerWindowOffset : "<<fTriggerWindowOffset<<std::endl
           <<"TriggerExtraSearchMargin : "<<fTriggerExtraSearchMargin<<std::endl
           <<"TriggerRejectMargin : "<<fTriggerRejectMargin<<std::endl
           <<"EnableTriggerTimeSubstraction : "<<fEnableTriggerTimeSubstraction<<std::endl
           <<"IndividualLSB : "<<fIndividualLSB<<std::endl<<"}"<<std::endl;


}

void TDC::FillModuleTree(){}

}//end of Tracking namespace


