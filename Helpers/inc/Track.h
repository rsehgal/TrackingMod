/*
 * Track.h
 *
 *  Created on: Jul 5, 2016
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_TRACK_H_
#define TEMPLATIZEDINTERFACE_INC_TRACK_H_

#include "base/Vector3D.h"
#include "base/Global.h"

namespace Tomography {

class Track {
	Tracking::Vector3D<double> fP1;
	Tracking::Vector3D<double> fP2;
	Tracking::Vector3D<double> fDirCosine;
	Tracking::Vector3D<double> fDirection;
public:
  Track();
  Track(Tracking::Vector3D<double> p1,Tracking::Vector3D<double> p2);
  void CalculateDirCosine();
  Tracking::Vector3D<double> GetP1(){return fP1;}
  Tracking::Vector3D<double> GetP2(){return fP2;}
  void SetP1(Tracking::Vector3D<double> val){fP1 = val; CalculateDirCosine();}
  void SetP2(Tracking::Vector3D<double> val){fP2 = val; CalculateDirCosine();}
  Tracking::Vector3D<double> GetDirCosine(){return fDirCosine;}
  Tracking::Vector3D<double> GetDirection(){return fDirection;}
  void Print(){fP1.Print(); std::cout<<" : " ; fP2.Print();}

  ~Track();
};

} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_TRACK_H_ */
