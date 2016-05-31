/*
 * Plane.h
 *
 *  Created on: May 16, 2016
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_SCINTILLATOR_H_
#define TEMPLATIZEDINTERFACE_INC_SCINTILLATOR_H_

#include "base/Global.h"
#include "base/Vector3D.h"
#ifdef SHOW_VISUALIZATION
#include "TGeoVolume.h"
#endif
/*
#ifndef USE_EVE
#include "TGeo/Visualizer.h"
#endif
*/

namespace Tomography {

class Scintillator {
  double fLength;
  double fBreadth;
  double fHeight;
  std::string fName;
  int fModuleId; // represents the id of Module at which it is connected
  int fScintId; // represents it id of channel on a Module
  static int fId; // Static variable to increase whenever a new object is created.
  bool fScintHit;

  Tracking::Channel *ch;// Data structure to hold data for the scintillator

/*
#ifndef USE_EVE
  Tracking::Visualizer v;
#endif
*/

  //Placement coordinates
  Tracking::Vector3D<double> fPlacedLocation;

  //Variable to store TGeoVolume
#ifdef SHOW_VISUALIZATION
  TGeoVolume *fTGeoVolume;
#endif

public:
  Scintillator();
  Scintillator(int moduleId, double length, double breadth,double height);
  Scintillator(int moduleId, double length, double breadth,double height,Tracking::Vector3D<double> placedLocation);
  ~Scintillator();
  static void SetStartingId(int sId){fId = sId;}
  void SetLength(double length){fLength = length;}
  void SetBreadth(double breadth){fBreadth = breadth;}
  void SetHeight(double height){fHeight = height;}
  void SetLBH(double length,double breadth, double height){
  fLength = length;
  fBreadth = breadth;
  fHeight = height;
  }
  int GetModuleId(){return fModuleId;}
  int GetScintId(){return fScintId;}
  bool GetScintHit(){return fScintHit;}
  double GetLength(){return fLength;}
  double GetBreadth(){return fBreadth;}
  double GetHeight(){return fHeight;}
  double Area(){return fLength*fBreadth;}
  double Volume(){return fLength*fBreadth*fHeight;}
  Tracking::Vector3D<double> GetPlacedLocation(){return fPlacedLocation;}
#ifdef SHOW_VISUALIZATION
  TGeoVolume* GetTGeoVolume(){return fTGeoVolume;}
  TGeoBBox* GetScintShape();
#endif
  std::string GetName(){return fName;}

/*
#ifndef USE_EVE
  void CreateTGeoVolume();
#else

  //This block is for EVE
#endif
*/

};



} /* namespace Tomography */

#endif /* PLANE_H_ */
