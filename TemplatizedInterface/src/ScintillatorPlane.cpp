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

void ScintillatorPlane::SetFiredStrips(Tracking::Tree &t, int evNo) {
  fScintTotal = 0;
  int scintVectorSize = fScintVector.size();
  fFiredStrips.clear();
  for (int i = 0; i < scintVectorSize; i++) {
    fScintVector[i]->DetectAndSetHit<true>(t, evNo);
    if(fScintVector[i]->GetScintHit())
    	fFiredStrips.push_back(i);

  }
}

} /* namespace Tomography */
