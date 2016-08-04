/*
 * SimulateScatteredTracks.h
 *
 *  Created on: Jul 5, 2016
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_SIMULATESCATTEREDTRACKS_H_
#define TEMPLATIZEDINTERFACE_INC_SIMULATESCATTEREDTRACKS_H_

#include <TGeoShape.h>
#include "base/Vector3D.h"
#include "base/Global.h"
#include <vector>
#include "Properties.h"
#include <cstring>
#include <TGeoShape.h>
#include <TGeoBBox.h>
#include "Track.h"
#include "Coordinates.h"

//#define Vec_t Tracking::Vector3D<double>

typedef Tomography::Properties Detector;


namespace Tomography {

class SimulateScatteredTracks {
	std::vector<Detector*> fDetectorVect;
	std::vector<Tracking::Vector3D<double>> fHitVector;
	TGeoShape *fScatterer;
	Tracking::Vector3D<double> fPOI; //Assuming single point scattering inside scatterer
	Track *fIncomingTrack;
	Track *fOutgoingTrack;
	Coordinates c;
	double fPenetrationDepth;


public:
  SimulateScatteredTracks();
  SimulateScatteredTracks(std::string detType);
  SimulateScatteredTracks(TGeoShape *scatterer, std::string detType);
  ~SimulateScatteredTracks();
  void GenerateIncomingTrack();
  void GenerateOutgoingTrack();
  std::vector<Tracking::Vector3D<double>> GetHitVector(){return fHitVector;}
  Track* GetIncomingTrack(){return fIncomingTrack;}
  Track* GetOutgoingTrack(){return fOutgoingTrack;}
  Tracking::Vector3D<double> GetScatteringPoint(){return fPOI;}
  void SetPenetrationDepth(double val){fPenetrationDepth=val;}
};

} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_SIMULATESCATTEREDTRACKS_H_ */
