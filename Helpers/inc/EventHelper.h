/*
 * EventHelper.h
 *
 *  Created on: Aug 10, 2017
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_EVENTHELPER_H_
#define HELPERS_INC_EVENTHELPER_H_

#include "Track.h"
#include "base/Vector3D.h"
#include "Imaging.h"
#include "Voxel.h"
using Tracking::ImageReconstruction;

using Tomography::Track;
using Tracking::Vector3D;
namespace Tomography {

class EventHelper {
	Track fIncoming;
	Track fOutgoing;
	Vector3D<double> fPocaPt;
	double fScatteringAngle;
	ImageReconstruction fIm;

//#ifdef VOXELIZE
	Voxel *fVoxel;
//#endif


public:
	EventHelper();
	EventHelper(Track incoming, Track outgoing);

	//Getters
	Track GetIncomingVector() const {return fIncoming;}
	Track GetOutgoingVector() const {return fOutgoing;}
	Vector3D<double> GetPocaPt() const {return fPocaPt;}
	double GetScatteringAngle() const {return fScatteringAngle;}
//#ifdef VOXELIZE
	Voxel* GetVoxel(){return fVoxel;}
//#endif

	//Calculators
	void CalculatePOCA();
	void CalculateScatterAngle();
	void CalculateVoxel();
	int GetVoxelNum();
	virtual ~EventHelper();
};

} /* namespace Tomography */

#endif /* HELPERS_INC_EVENTHELPER_H_ */
