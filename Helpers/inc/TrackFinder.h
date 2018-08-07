/*
 * TrackFinder.h
 *
 *  Created on: Aug 7, 2018
 *      Author: raman
 */

#ifndef TRACKFINDER_H_
#define TRACKFINDER_H_

#include <vector>
#include "base/Vector3D.h"

namespace Tomography {

class Pixel;

//using  Tracking::Vector3D<double> = Tracking::Vector3D<double>;

class TrackFinder {
	std::vector<Pixel*> fPixelVector;
	std::vector< Tracking::Vector3D<double>> fFittedTrackPointVector;
public:
	TrackFinder();
	TrackFinder(std::vector< Tracking::Vector3D<double>> pixelCenterVector);
	void SetPixelCenterVector(std::vector< Tracking::Vector3D<double>> pixelCenterVector);
	virtual ~TrackFinder();
	std::vector<Pixel*> GetPixelVector() const {return fPixelVector;}
	void FindTrack_Algo1();
	void FindTrack_Algo2();
	std::vector< Tracking::Vector3D<double>> GetFittedTrackPointVector() const {return fFittedTrackPointVector;}
};

} /* namespace Tomography */

#endif /* TRACKFINDER_H_ */
