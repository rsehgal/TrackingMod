/*
 * Plane.h
 *
 *  Created on: May 16, 2016
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_SCINTILLATOR_H_
#define TEMPLATIZEDINTERFACE_INC_SCINTILLATOR_H_

#include "Tree.h"
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
  
  //Strip num always start from 0, ScintId can start from other number also
  static int fSno;
  int fStripNo;

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
  static void SetStartingStripNum(){fSno = -1;}

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
  int GetStripNum(){return fStripNo;}
  template <bool ForRpc>
  void DetectAndSetHit( int evNo);

/*
#ifndef USE_EVE
  void CreateTGeoVolume();
#else

  //This block is for EVE
#endif
*/

};

template <bool ForRpc>
//void Scintillator::DetectAndSetHit(Tracking::Tree &t, int evNo) {
void Scintillator::DetectAndSetHit(int evNo) {
  /*
  *  For the time being hard coding the information related to
  *  trigger module and channel.
  *  triggerModule : 0 , triggerChannel : 31
  *
  *  ModuleVector variable "modVector" should be filled after reading the ROOT file
  *
  */
  Tracking::Tree *t = Tracking::Tree::instance()->GetTree();
  fScintHit = false;
  Tracking::Channel *trigMultiHit = t->GetEntry("Module2_LE_CH31", evNo);
  long trig = 0;
  if(trigMultiHit->size())
  trig = trigMultiHit->at(0);
  ch = 0;
  ch = t->GetEntry(fName, evNo);
  if (ch->size()) {
    long scintillator = 0;
    scintillator = ch->at(0);
    /* The below logic is duplicate for the time being.
     * Based on the actual logic, it needs to be changed
     * for Scintiallator and Rpc
     */

    if(ForRpc) {
    	long rpcData = scintillator;
    	if (rpcData >= 19450 && rpcData <= 22550)
    	          fScintHit = true;

    }else {
    	long scintillatorData = scintillator;
    	if (scintillatorData >= 19450 && scintillatorData <= 22550)
    	          fScintHit = true;
    }

/*
    if (scintillator > 0) {
      if (ForRpc) {
        long rpcData = scintillator;
        if (rpcData >= 19450 && rpcData <= 20550)
          fScintHit = true;

      } else {
        if (abs(trig - scintillator) < scintMax)
          fScintHit = true;
      }
    }
  }
*/

  // std::cout<<"fScintHit : "<< fScintHit <<std::endl;
}
}
} /* namespace Tomography */

#endif /* PLANE_H_ */
