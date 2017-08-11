/*
 * RunHelper.h
 *
 *  Created on: Aug 10, 2017
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_RUNHELPER_H_
#define HELPERS_INC_RUNHELPER_H_

#include <vector>
#include "base/Vector3D.h"
//#include "Voxel.h"

using Tracking::Vector3D;

namespace Tomography {

class RunHelper {

 	std::vector<double> fScatteringAngleVector;
	std::vector<Vector3D<double>> fPocaPtVector;

#ifdef VOXELIZE
	//std::vector<Voxel*> fVoxelVector;
#endif


 // std::vector<EventHelper*> fEventHelperVector;
public:
	RunHelper();

	std::vector<double> GetScatteringAngleVector() const {return fScatteringAngleVector;}
#ifdef VOXELIZE
	std::vector<Voxel*> GetVoxelVector() const {return fVoxelVector;}
#endif
	std::vector<Vector3D<double>> GetPocaPtVector() const {return fPocaPtVector;}

	void Insert(double scatteringAngle){ fScatteringAngleVector.push_back(scatteringAngle);}
	void Insert(Vector3D<double> pocaPt) { fPocaPtVector.push_back(pocaPt);}

#ifdef VOXELIZE
	//void Insert(Voxel *obj){ fVoxelVector.push_back(obj); }
#endif


//	std::vector<EventHelper*> GetEventHelperVector(){return fEventHelperVector;}
//	void Insert(EventHelper *obj){fEventHelperVector.push_back(obj);}
	virtual ~RunHelper();
};

} /* namespace Tomography */

#endif /* HELPERS_INC_RUNHELPER_H_ */
