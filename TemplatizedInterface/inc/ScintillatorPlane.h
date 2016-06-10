/*
 * ScintillatorPlane.h
 *
 *  Created on: May 17, 2016
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_SCINTILLATORPLANE_H_
#define TEMPLATIZEDINTERFACE_INC_SCINTILLATORPLANE_H_

#include <vector>
#include "Scintillator.h"
#include "base/Vector3D.h"
#include "Tree.h"

namespace Tomography {
class ScintillatorPlane {
  std::vector<Scintillator*> fScintVector;
  int fScintTotal;
  int fNumOfScintillators;
  std::string fName;

  //Plane dimensions
  int fLength;
  int fBreadth;
  int fHeight;
  std::vector<int> fFiredStrips;

public:
  ScintillatorPlane();
  //ScintillatorPlane(int moduleId, int numOfScintillators, double zPos, double scintPlaneLength,
  //double scintPlaneBreadth, std::string planeName = "Test-ScintillatorPlane"){
/*
  ScintillatorPlane(int moduleId, int numOfScintillators, double zPos, double scintLength,
                    double scintBreadth, std::string planeName = "Test-ScintillatorPlane"){
    fNumOfScintillators=numOfScintillators;
    for(int i = 0 ; i < numOfScintillators ; i++)
      fScintVector.push_back(new Scintillator(moduleId,fLength,fBreadth,fHeight));
  }
*/
  ScintillatorPlane(int moduleId, int numOfScintillators, double zPos, double scintPlaneLength,
                      double scintPlaneBreadth, double scintPlaneHeight,double planeShift, bool xdir=true, std::string planeName = "Test-ScintillatorPlane"):
                      fLength(scintPlaneLength),fBreadth(scintPlaneBreadth), fHeight(scintPlaneHeight){
      fNumOfScintillators=numOfScintillators;
      Tracking::Vector3D<double> scintPlacedLocation;
      for (int i = 0; i < numOfScintillators; i++) {
        scintPlacedLocation.Set(0.,0.,0.);
        scintPlacedLocation.SetZ(zPos);

        double len=0., brd=0.;
        if(xdir){
          len =  fLength / numOfScintillators;
          brd = fBreadth;
          scintPlacedLocation.SetX(-fLength/2.+i*len);
          scintPlacedLocation.SetY(planeShift);
        }else{
          len =  fLength ;
          brd = fBreadth / numOfScintillators;
          scintPlacedLocation.SetX(planeShift);
          scintPlacedLocation.SetY(-fBreadth/2.+i*brd);
        }

        fScintVector.push_back(new Scintillator(moduleId, len, brd, fHeight,scintPlacedLocation));

      }

    }
  ~ScintillatorPlane();
  std::string GetName(){return fName;}
  int GetTotalScintillatorFired(){return fScintTotal;}
  int GetNumOfScintillators(){return fNumOfScintillators;}
  int DetectTotalScinitillatorFired();
  std::vector<Scintillator*> GetScintVector(){return fScintVector;}
  double GetLength(){return fLength;}
  double GetBreadth(){return fBreadth;}
  double GetHeight(){return fHeight;}
  //template<bool ForRpc>
 // bool IsShowerEvent(Tracking::Tree &t, int evNo);
  void SetFiredStrips(Tracking::Tree &t, int evNo);
  std::vector<int> GetFiredStripsVector(){return fFiredStrips;}

};
/*
//template<bool ForRpc>
bool ScintillatorPlane::IsShowerEvent(Tree &t, int evNo){
  fScintTotal = 0;
  int scintPlaneSize = fScintVector.size();
  for(int i = 0 ; i < scintPlaneSize ; i++){
    fScintVector[i]->DetectAndSetHit<false>(t,evNo);
  }
  DetectTotalScintFired();
  return (fScintTotal >= scint_multiplicity_max);
}
*/
} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_SCINTILLATORPLANE_H_ */
