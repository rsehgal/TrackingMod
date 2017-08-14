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
	std::vector<Tracking::Vector3D<double>> GetPocaPointsVector(){return fVectPointsInVoxel;}
	int GetVoxelNum(){return fVoxelNum;}
	Tracking::Vector3D<double> GetVoxelDimensions(){return fDim;}
	Tracking::Vector3D<double> GetVoxelCenter(){return fVoxelCenter;}

	void Insert(Tracking::Vector3D<double>, int voxelNum);
/*
	static void InsertVoxel(Voxel *vox, int voxelNum){
		fVoxelVector.push_back(vox);
		fVisitedVoxelNumVector.push_back(voxelNum);
	}
*/

	std::vector<double> GetScatteringVector();
	void CalcSD();
	double GetStandardDeviation();

	static int IfVoxelExist(int voxelNum);
	static std::vector<Voxel*> GetVoxelVector() {return fVoxelVector;}

};

//std::vector<Voxel*> Voxel::fVoxelVector = {0};


} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_VOXEL_H_ */
