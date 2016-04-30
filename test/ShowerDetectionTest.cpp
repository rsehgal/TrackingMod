/*
 * ShowerDetectionTest.cpp
 *
 *  Created on: Mar 27, 2016
 *      Author: rsehgal
 *
 *  This test is written with an idea to have a single instance of 
 *  Tree class, so that the ROOT file is opened only once.
 *  
 *  So whatever funtions it will use, should have an additional 
 *  argument of Type Tree &.
 *
 *  Now if the detected event is Shower event then it should continue
 *  the event loop, without considering that event for efficiency 
 *  calculation.
 */

#include "Tree.h"
#include "Scintillator.h"
int main(){
    //ScintillatorPlane topPlane(2,8,"TopPlane");
    ScintillatorPlane *topPlane = new ScintillatorPlane(2,8,-105.,false,"TopPlane");
  
   Tree t("6133.root","BSC_DATA_TREE");
    int count=0;
    constexpr bool ForRpc=false;
    for(int i = 0 ; i < t.GetNumOfEvents() ; i++){
    //for(int i = 0 ; i < 100 ; i++){
        bool shower = topPlane->IsShowerEvent<ForRpc>(t,i) ;
        if(shower){
            //continue;
            std::cout << "Event No : " << i << "  :: IsShowerEvent : " << shower 
                      << " : ShowerCount : " << topPlane->GetShowerCount() << std::endl;
            count++;
        }
        else{
            //std::cout<<"Processing Non Shower Events...." << std::endl;

            //Efficiency  Calculation should be done in this block
        }
    }

    std::cout<<"Num of Shower Events : "<< count << std::endl;
    
    
}