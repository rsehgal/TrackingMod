/*
 * Track.cpp
 *
 *  Created on: Jul 5, 2016
 *      Author: rsehgal
 */

#include "Track.h"

namespace Tomography {

Track::Track() {
  // TODO Auto-generated constructor stub
	fP1 = Tracking::Vector3D<double>(0.,0.,0.);
	fP2 = Tracking::Vector3D<double>(0.,0.,0.);
	CalculateDirCosine();
}

Track::~Track() {
  // TODO Auto-generated destructor stub
}

Track::Track(Tracking::Vector3D<double> p1, Tracking::Vector3D<double> p2){
  fP1 = p1;
  fP2 = p2;
  CalculateDirCosine();

}

void Track::CalculateDirCosine(){
	fDirCosine = (fP2-fP1).Unit();
}

} /* namespace Tomography */
