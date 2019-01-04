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
#include <G4ThreeVector.hh>


namespace Tomography {

class Track {
	Tracking::Vector3D<double> fP1;
	Tracking::Vector3D<double> fP2;
	Tracking::Vector3D<double> fDirCosine;
	Tracking::Vector3D<double> fDirection;
	Tracking::Vector3D<double> fDirectionRatio;
public:
  Track();
  Track(Tracking::Vector3D<double> p1,Tracking::Vector3D<double> p2);
  Track(G4ThreeVector p1,G4ThreeVector p2);
  void CalculateDirCosine();
  Tracking::Vector3D<double> GetP1(){return fP1;}
  Tracking::Vector3D<double> GetP2(){return fP2;}
  void SetP1(Tracking::Vector3D<double> val){fP1 = val; CalculateDirCosine();}
  void SetP2(Tracking::Vector3D<double> val){fP2 = val; CalculateDirCosine();}
  void Set(Tracking::Vector3D<double> p1,Tracking::Vector3D<double> p2){
    fP1 = p1; 
    fP2 = p2; 
    CalculateDirCosine();
  }
  void Set(G4ThreeVector p1,G4ThreeVector p2){
    fP1 = Tracking::Vector3D<double>(p1.x(),p1.y(),p1.z());
    fP2 = Tracking::Vector3D<double>(p2.x(),p2.y(),p2.z());
    CalculateDirCosine();
  }

  Tracking::Vector3D<double> GetDirCosine(){return fDirCosine;}
  Tracking::Vector3D<double> GetDirection(){return fDirection;}
  Tracking::Vector3D<double> GetDirectionRatio(){return fDirectionRatio;}
  void Print(){fP1.Print(); std::cout<<" : " ; fP2.Print();}

  //Overloading operator
  void operator = (const Track &newVal);

  ~Track();
};

} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_TRACK_H_ */
