/*
 * Global.h
 *
 *  Created on: Mar 8, 2016
 *      Author: rsehgal
 */

#ifndef BASE_GLOBAL_H_
#define BASE_GLOBAL_H_

#include <vector>
#include <algorithm>
#include <map>
#include <iostream>
namespace Tracking {

typedef std::vector<unsigned int> HitsPerChannel_v; //Multihit information for channel
typedef HitsPerChannel_v Channel;
typedef std::vector<Channel*> Channel_v; //Basically hitsPerChannel for all the events
typedef Channel_v Module;
typedef std::vector<Module> ModuleVector;

#define scintMax 500

#define scint_multiplicity_max 2

#define Precision double
#define TRACKING_INLINE inline

#define PI 3.14159265359

double begin=-500;
double end=500;

class Global{
public:
	Global(){}
static double GenRandomDet(double min, double max) {
  return (rand() / (static_cast<double>(RAND_MAX) + 1.0)) * (max - min) + min;
}

static double GetSmeared(double around, double deviation) {
	//Resetting begin and end for the current event
	begin=-500;
	end=500;
	if((around-deviation) > begin)
		begin=(around-deviation);
	if((around+deviation) < end)
		end=(around+deviation);
	std::cout << "Around : " << around << " : Begin : " << begin <<" : End : " << end << std::endl;
	return GenRandomDet(begin,end);

}

};

}//end of Tracking namespace

namespace Tomography{

 constexpr int start = 19000;
 constexpr int end = 24500;

 constexpr int minPointsInAVoxel = 10;

 static unsigned long long int effEvNo = 0;

 static unsigned long long int numOfTrigger = 0;

 constexpr double confidenceInterval = 3.;

 constexpr double unscatteringThreshold = 0.0005;

 constexpr double filteringThetaCut = 0.07;

 constexpr int cleanVoxelCount = 50;

 constexpr double refMomentum = 3000.; //MeV

 /* Singleton class to break the Simulation when desired number 
 ** events are done.
 */
 class EventBreak{
    EventBreak(){}
    static EventBreak *fInstance;
 public:
    static EventBreak* instance(){
        if(!fInstance){
            fInstance = new EventBreak();
            return fInstance;
       }else{
            return fInstance;
        }
    }
    static unsigned long long int fNumOfEvents ;
    static unsigned long long int fEffEvNo ;
    static unsigned long long int fNumOfTrigger;


    static bool BreakSimulation(){
#ifdef USE_TRIGGER
	std::cout << "Ayush : Num of Triggers  : " << fNumOfTrigger << std::endl;
	return fNumOfTrigger > fNumOfEvents;
#else
        return fEffEvNo > fNumOfEvents ;
#endif
    }

    static void SetMaxNumOfEvents(int numOfEvents){
        fNumOfEvents = numOfEvents;
    }
 };
 
 enum operation {read, write, append, close};

 //enum material {BG, G4_Al, G4_Fe, G4_Pb};

#ifndef MATERIAL
#define MATERIAL
// std::map<std::string,unsigned int> material;
#endif

 /*void FillMaterial() {
	material.insert(std::pair<std::string, unsigned int>("BG", 0));
	material.insert(std::pair<std::string, unsigned int>("Al", 1));
	material.insert(std::pair<std::string, unsigned int>("Fe", 2));
	material.insert(std::pair<std::string, unsigned int>("Pb", 3));
	material.insert(std::pair<std::string, unsigned int>("U", 4));
}
*/

}//end of Tomography namespace


#endif /* BASE_GLOBAL_H_ */
