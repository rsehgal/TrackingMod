/*
 * VoxelNavigator.h
 *
 *  Created on: 01-Jan-2019
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_VOXELNAVIGATOR_H_
#define HELPERS_INC_VOXELNAVIGATOR_H_

#include "base/Vector3D.h"
#include <G4Box.hh>


namespace Tomography {

class VoxelNavigator {
	G4Box *fVoxelizedVolume;
	G4Box *fBox;
	Tracking::Vector3D<double> fPlacement;
	double fStep;


public:
	VoxelNavigator();
	virtual ~VoxelNavigator();
	double ComputeStep(Tracking::Vector3D<double> point, Tracking::Vector3D<double> dir,Tracking::Vector3D<double> placement);
	EInside IsPointOnSurfaceOfVoxelizedVolume(Tracking::Vector3D<double> point);

	void Debugger();
};

} /* namespace Tomography */

#endif /* HELPERS_INC_VOXELNAVIGATOR_H_ */
