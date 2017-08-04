/*
 * Voxel.h
 *
 *  Created on: Aug 4, 2017
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_VOXEL_H_
#define TEMPLATIZEDINTERFACE_INC_VOXEL_H_

#include "base/Vector3D.h"
#include <vector>

namespace Tomography {

class Voxel {
	Tracking::Vector3D<double> fDim;
	int fVoxelNum;
	std::vector<Tracking::Vector3D<double>> fVectPointsInVoxel;
	int fPointCount;
	bool fOutlier;


	//Minimum num of point in a Voxel to make it genuine voxel
	int fMinPointsInVoxel;

	static int fTotalVoxelsCreated;

public:
	Voxel();
	Voxel(Tracking::Vector3D<double> pocaPt);
	Voxel(Tracking::Vector3D<double> pocaPt, int voxelNum);
	virtual ~Voxel();
	bool IsOutlier(){return fOutlier;}
	int GetNumOfPoints(){return fPointCount;}
	std::vector<Tracking::Vector3D<double>> GetPointsVector(){return fVectPointsInVoxel;}
	int GetVoxelNum(){return fVoxelNum;}
	Tracking::Vector3D<double> GetVoxelDimensions(){return fDim;}
	void Insert(Tracking::Vector3D<double>, int voxelNum);
};

} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_VOXEL_H_ */
