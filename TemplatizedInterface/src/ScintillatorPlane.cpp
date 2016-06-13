/*
 * ScintillatorPlane.cpp
 *
 *  Created on: May 17, 2016
 *      Author: rsehgal
 */

#include "ScintillatorPlane.h"

namespace Tomography {


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

//void ScintillatorPlane::SetFiredStripsVector(Tracking::Tree &t, int evNo) {
//Provided the Root file is registered with SetupManager
void ScintillatorPlane::SetFiredStripsVector(int evNo) {
  fScintTotal = 0;
  int scintVectorSize = fScintVector.size();
  fFiredStripsVector.clear();
  for (int i = 0; i < scintVectorSize; i++) {
  //  fScintVector[i]->DetectAndSetHit<true>(t, evNo);
	fScintVector[i]->DetectAndSetHit<true>(evNo);
    if(fScintVector[i]->GetScintHit())
    	fFiredStripsVector.push_back(i);

  }
}

} /* namespace Tomography */
