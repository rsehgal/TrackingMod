/*
 * Voxel.h
 *
 *  Created on: Aug 4, 2017
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_VOXEL_H_
#define HELPERS_INC_VOXEL_H_

#include "base/Vector3D.h"
#include <vector>
//#include "CommonFunc.h"

namespace Tomography {

class Voxel {
	Tracking::Vector3D<double> fDim;
	int fVoxelNum;
	std::vector<Tracking::Vector3D<double>> fVectPointsInVoxel;
	int fPointCount;
	bool fOutlier;
	static std::vector<int> fVisitedVoxelNumVector ;
	static std::vector<Voxel*> fVoxelVector ;
	Tracking::Vector3D<double> fVoxelCenter;
	double fSD;
	double fRL;

	int fMinPointsInVoxel; //Minimum num of point in a Voxel to make it genuine voxel
	static int fTotalVoxelsCreated;

	// This is basically a weight value that represent scattering Density for the voxel
	// Required for implementing TUSHAR ROY's idea.
	double fScatteringDensity;

public:
	Voxel();
	Voxel(Tracking::Vector3D<double> pocaPt);
	Voxel(Tracking::Vector3D<double> pocaPt, int voxelNum, bool useEnclosingVoxels = false);
	//Voxel(Tracking::Vector3D<double> pocaPt, int voxelNum, Tracking::Vector3D<double> voxelDim, bool useEnclosingVoxels = false);
	virtual ~Voxel();
	bool IsOutlier(){return fOutlier;}
	int GetNumOfPoints(){return fPointCount;}
	std::vector<Tracking::Vector3D<double>> GetPocaPointsVector(){return fVectPointsInVoxel;}
	int GetVoxelNum(){return fVoxelNum;}
	Tracking::Vector3D<double> GetVoxelDimensions(){return fDim;}
	Tracking::Vector3D<double> GetVoxelCenter(){return fVoxelCenter;}

	void Insert(Tracking::Vector3D<double>, int voxelNum, bool useEnclosingVoxels = false , double scatteringDensity = 0.);
	void Insert(Tracking::Vector3D<double> point);

	static Voxel* GetVoxel(int voxelNum){return fVoxelVector[voxelNum];}

	void SetMinPointsInAVoxel(int minPts) {fMinPointsInVoxel = minPts;}
	int GetMinPointsInAVoxel() const { return fMinPointsInVoxel;}
/*
	static void InsertVoxel(Voxel *vox, int voxelNum){
		fVoxelVector.push_back(vox);
		fVisitedVoxelNumVector.push_back(voxelNum);
	}
*/

	std::vector<double> GetScatteringVector();
	void CalcSD();
	void CalcRadiationLength();
	double GetRadiationLength();
	double GetStandardDeviation();

	static int IfVoxelExist(int voxelNum);
	static std::vector<Voxel*> GetVoxelVector() {return fVoxelVector;}
	static std::vector<Voxel*> GetFilteredVoxelVector();
	static std::vector<Tracking::Vector3D<double>> GetFilteredPocaPtVector();


	double GetScatteringDensity()const { return fScatteringDensity; }
	void SetScatteringDensity(double scatteringDensity) { fScatteringDensity = scatteringDensity; }
	//static std::vector<Tracking::Vector3D<double>> GetEightCorners_Of_ImaginaryVoxel_CentredAtPocaPoint(Tracking::Vector3D<double> pocaPt);

	static void Reset();

};

} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_VOXEL_H_ */
