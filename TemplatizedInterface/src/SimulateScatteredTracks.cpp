/*
 * SimulateScatteredTracks.cpp
 *
 *  Created on: Jul 5, 2016
 *      Author: rsehgal
 */

#include "SimulateScatteredTracks.h"
#include "SetupManager.h"
//#include "Paddle.h"
//typedef Tomography::Paddle Scatterer;

namespace Tomography {

SimulateScatteredTracks::SimulateScatteredTracks() {

}

SimulateScatteredTracks::SimulateScatteredTracks(std::string detType) {
  // TODO Auto-generated constructor stub
  SetupManager *setup = SetupManager::instance();
  std::vector<Detector *> detVect = setup->GetDetectorVector(detType);
  for (int i = 0; i < detVect.size(); i++) {
    detVect[i]->Random().Print();
    fHitVector.push_back(detVect[i]->Random());
  }
}

SimulateScatteredTracks::SimulateScatteredTracks(TGeoShape *scatterer, std::string detType) {
  fScatterer = scatterer;
  SetupManager *setup = SetupManager::instance();
  fDetectorVect = setup->GetDetectorVector(detType);
  fIncomingTrack = new Track();
  fOutgoingTrack = new Track();
  fPenetrationDepth = 0.02;
  //GenerateIncomingTrack();
  //GenerateOutgoingTrack();
/*
  for (int i = 0; i < detVect.size(); i++) {
    //detVect[i]->Random().Print();
    fHitVector.push_back(detVect[i]->Random());
  }
*/

}


void SimulateScatteredTracks::GenerateIncomingTrack(){
	double dist = 0.;
	do{
		fHitVector.clear();
		fIncomingTrack->SetP1(fDetectorVect[0]->Random());
		fIncomingTrack->SetP2(fDetectorVect[fDetectorVect.size()/2 -1]->Random());

		dist = fScatterer->DistFromOutside(fIncomingTrack->GetP1().GetArr(), fIncomingTrack->GetDirCosine().GetArr());
		fPOI = (fIncomingTrack->GetP1() + fIncomingTrack->GetDirCosine()*(dist+fPenetrationDepth));
		//std::cout<<"Dist : " << dist << std::endl;
	} while(!fScatterer->Contains(fPOI.GetArr()));

	//std::cout<<"Valid Direction found" << std::endl;
	//delete fIncomingTrack;
	//delete fOutgoingTrack;
}

void SimulateScatteredTracks::GenerateOutgoingTrack(){
	fOutgoingTrack->SetP1(fPOI);
	Tracking::Vector3D<double> p2 = fDetectorVect[fDetectorVect.size() -1]->Random();
	fOutgoingTrack->SetP2(p2);
	c.SetP1(fPOI);
	c.SetP2(p2);
	Tracking::Vector3D<double> p1 = c.GetPOI(fDetectorVect[fDetectorVect.size()-2],true);
	fOutgoingTrack->SetP1(p1);


}

SimulateScatteredTracks::~SimulateScatteredTracks() {
  // TODO Auto-generated destructor stub
}


} /* namespace Tomography */
