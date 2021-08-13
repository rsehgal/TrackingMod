/*
 * Track.cpp
 *
 *  Created on: Jul 5, 2016
 *      Author: rsehgal
 */

#include "Track.h"
#include "CommonFunc.h"

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
Track::Track(Tracking::Vector3D<double> p1, Tracking::Vector3D<double> p2,double col){
color=col;
fP1=p1;
fP2=p2;
}

/*Track::Track(std::vector<lite_interface::Point3D*> vecOfPoint3D,double col){
color=col;
unsigned int len = vecOfPoint3D.size();
fP1=Tracking::Vector3D<double>(vecOfPoint3D[0]->GetX(),vecOfPoint3D[0]->GetY(),vecOfPoint3D[0]->GetZ());
fP2=Tracking::Vector3D<double>(vecOfPoint3D[len-1]->GetX(),vecOfPoint3D[len-1]->GetY(),vecOfPoint3D[len-1]->GetZ());
}*/
Track::Track(G4ThreeVector p1,G4ThreeVector p2){
	fP1 = CommonFunc::Functions::instance()->ConvertToVector3D(p1);
	fP2 = CommonFunc::Functions::instance()->ConvertToVector3D(p2);
	CalculateDirCosine();
}

void Track::CalculateDirCosine(){
	fDirCosine = (fP2-fP1).Unit();
	fDirection = (fP2-fP1).Unit();
	fDirectionRatio = (fP2-fP1);
	fTrackLength = fDirectionRatio.Mag();
}

void Track::operator = (const Track &newVal){
	this->fP1 = newVal.fP1;
	this->fP2 = newVal.fP2;
	this->fDirCosine = newVal.fDirCosine;
	this->fDirection = newVal.fDirection;
	this->fDirectionRatio = newVal.fDirectionRatio;
}

} /* namespace Tomography */
