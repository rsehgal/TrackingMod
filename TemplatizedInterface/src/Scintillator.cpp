/*
 * Scintillator.cpp
 *
 *  Created on: May 16, 2016
 *      Author: rsehgal
 */

#include "Scintillator.h"
#include <sstream>
#ifdef SHOW_VISUALIZATION 
#include "TGeoBBox.h"
#endif
namespace Tomography {



//Scintillator::Scintillator(){}
int Scintillator::fId = -1;
int Scintillator::fSno = -1;
long Scintillator::fStart = 19450;
long Scintillator::fEnd = 21000;
//int Scintillator::badEventCounter = 0;

Scintillator::Scintillator(){}

Scintillator::Scintillator(int moduleId, double length, double breadth,double height):
  fModuleId(moduleId),fLength(length), fBreadth(breadth),fHeight(height){
  fValue=100000;
  fId++;
  fScintId = fId;
  fSno++;
  fStripNo = fSno;
  std::stringstream ss;
  ss << "Module" << fModuleId <<"_LE_CH" << fScintId;
  fName = ss.str();
}

Scintillator::Scintillator(int moduleId, double length, double breadth,double height,Tracking::Vector3D<double> placedLocation):
  fModuleId(moduleId),fLength(length), fBreadth(breadth),fHeight(height){
  fValue=100000;
  fPlacedLocation = placedLocation;
  fId++;
  fScintId = fId;
  fSno++;
  fStripNo = fSno;
  std::stringstream ss;
  ss << "Module" << fModuleId <<"_LE_CH" << fScintId;
  fName = ss.str();
  FillVerticesVector();
}

Scintillator::~Scintillator() {
  // TODO Auto-generated destructor stub
}

#ifdef SHOW_VISUALIZATION
TGeoBBox* Scintillator::GetScintShape(){
    return new TGeoBBox(fName.c_str(),fLength/2., fBreadth/2., fHeight/2.);
  }

#ifndef USE_EVE
#if 0
void Scintillator::CreateTGeoVolume(){
  fTGeoVolume = v.CreateTGeoVolume(fName.c_str(),new TGeoBBox(fName.c_str(), fLength / 2., fBreadth / 2., fHeight / 2.));
  fTGeoVolume->SetVisibility(kTRUE);
  fTGeoVolume->SetLineColor(kGreen);
}
#endif
#endif
#endif
} /* namespace Tomography */
