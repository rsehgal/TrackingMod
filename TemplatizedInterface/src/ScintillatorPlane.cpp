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
	  int count = 0;
	  int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
	  for(int i = 0 ; i <  numOfEvents ; i++){
		  SetFiredStripsVector(i);
		  if(GetFiredStripsVector().size() <= fClusterSize && GetFiredStripsVector().size() > 0){
			  count++;
		  }

	  }
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
