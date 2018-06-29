/*
 * ScintillatorPlane.cpp
 *
 *  Created on: May 17, 2016
 *      Author: rsehgal
 */

#include "ScintillatorPlane.h"

namespace Tomography {

//int ScintillatorPlane::fClusterSize = 2;

ScintillatorPlane::ScintillatorPlane() {
  // TODO Auto-generated constructor stub
}


ScintillatorPlane::~ScintillatorPlane() {
  // TODO Auto-generated destructor stub
}

int ScintillatorPlane::DetectTotalScinitillatorFired(){
	for(int i = 0 ; i < fScintVector.size() ; i++){
	  if(fScintVector[i]->GetScintHit())
		  fScintTotal++;
	}

}

void ScintillatorPlane::SetEfficiency(){
	  std::cout << "Entered SetEfficiency of Plane ............ " << std::endl;
	  int count = 0;
	  int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
	  std::vector<int> nonFiredEvents ;
	  for(int i = 0 ; i <  numOfEvents ; i++){
		  SetFiredStripsVector(i);
		  if(GetFiredStripsVector().size() <= fClusterSize && GetFiredStripsVector().size() > 0){
			  count++;
		  }else{
			  nonFiredEvents.push_back(i);
		  }

	  }

	  std::cout << "======== Following are Non Fired Event number which is really strange.......... ==========" << std::endl;
	  std::cout << "-------- Total Number of NonFired Events : " << nonFiredEvents.size() << " ---------" << std::endl;
/*
	  for(int i = 0 ; i < nonFiredEvents.size() ; i++)
		  std::cout << "============ Event No : " << nonFiredEvents[i] <<" ========== ClusterSize : " << fClusterSize << std::endl;
*/
	  fEfficiency = count/(double)numOfEvents*100;
  }

//void ScintillatorPlane::SetFiredStripsVector(Tracking::Tree &t, int evNo) {
//Provided the Root file is registered with SetupManager
void ScintillatorPlane::SetFiredStripsVector(int evNo) {
  fScintTotal = 0;
  int scintVectorSize = fScintVector.size();
  fFiredStripsVector.clear();
  fFiredStripsIDVector.clear();
  fFiredStripsNameVector.clear();
  for (int i = 0; i < scintVectorSize; i++) {
  //  fScintVector[i]->DetectAndSetHit<true>(t, evNo);
	//Scintillator::ResetBadEventCounter();
	fScintVector[i]->DetectAndSetHit<true>(evNo);
/*
	if(detType=="TRG")
		fScintVector[i]->DetectAndSetHit<false>(evNo);
	else
		fScintVector[i]->DetectAndSetHit<true>(evNo);
*/

	  //	std::cout << "\t" << fScintVector[i]->GetScintHit();
    if(fScintVector[i]->GetScintHit()){
    	fFiredStripsVector.push_back(i);
    	fFiredStripsIDVector.push_back(fScintVector[i]->GetScintId());
    	fFiredStripsNameVector.push_back(fScintVector[i]->GetName());
    }

  }
  //std::cout << std::endl;
}

} /* namespace Tomography */
