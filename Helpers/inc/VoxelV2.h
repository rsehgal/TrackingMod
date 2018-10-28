/*
 * VoxelV2.h
 *
 *  Created on: 23-Oct-2018
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_VOXELV2_H_
#define HELPERS_INC_VOXELV2_H_

#include "base/Vector3D.h"
#include "base/Global.h"
#include <vector>

namespace Tomography {

class Voxel_V2 {
	Tracking::Vector3D<double> fDim;
	int fVoxelNum;
	std::vector<Tracking::Vector3D<double>> fVectPointsInVoxel;
	int fPointCount;
	bool fOutlier;
	Tracking::Vector3D<double> fVoxelCenter;
	double fSD;
	double fRL;
	int fMinPointsInVoxel; //Minimum num of point in a Voxel to make it genuine voxel

	// This is basically a weight value that represent scattering Density for the voxel
	// Required for implementing TUSHAR ROY's idea.
	double fScatteringDensity;
public:
	Voxel_V2();
	Voxel_V2(Tracking::Vector3D<double> pocaPt, int voxelNum);
	void Reset_V2();
	void Insert(Tracking::Vector3D<double> point);
	void CalcSD();
	void CalcRadiationLength();
	double GetRadiationLength(){return fRL;}
	double GetStandardDeviation(){return fSD;}
	std::vector<Tracking::Vector3D<double>> GetPocaPointsVector(){return fVectPointsInVoxel;}
	Tracking::Vector3D<double> GetVoxelCenter(){return fVoxelCenter;}
	std::vector<double> GetScatteringVector();
	int GetVoxelNum(){return fVoxelNum;}
	void SetMinPointsInAVoxel(int minPts) {fMinPointsInVoxel = minPts;}
	int GetMinPointsInAVoxel() const { return fMinPointsInVoxel;}
	bool IsOutlier(){return fOutlier;}
	virtual ~Voxel_V2();

};

} /* namespace Tomography */

#endif /* HELPERS_INC_VOXELV2_H_ */
