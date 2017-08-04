/*
 * Voxel.cpp
 *
 *  Created on: Aug 4, 2017
 *      Author: rsehgal
 */

#include "Voxel.h"

namespace Tomography {

int Voxel::fTotalVoxelsCreated = 0;

Voxel::Voxel() {
	// TODO Auto-generated constructor stub
	fMinPointsInVoxel = 10;

}

Voxel::Voxel(Tracking::Vector3D<double> pocaPt){
	fTotalVoxelsCreated++;
	std::cout<<"New Voxel Created ........ " << std::endl;
	fMinPointsInVoxel = 10;
	fVectPointsInVoxel.push_back(pocaPt);
	fPointCount = fVectPointsInVoxel.size();
	fOutlier = fPointCount < fMinPointsInVoxel;

}

Voxel::Voxel(Tracking::Vector3D<double> pocaPt, int voxelNum){
	fTotalVoxelsCreated++;
	std::cout<<"New Voxel Created ........ " << std::endl;
	fVoxelNum = voxelNum;
	fMinPointsInVoxel = 10;
	fVectPointsInVoxel.push_back(pocaPt);
	fPointCount = fVectPointsInVoxel.size();
	fOutlier = fPointCount < fMinPointsInVoxel;

}

void Voxel::Insert(Tracking::Vector3D<double> pocaPt, int voxelNum){
	std::cout<<"Inserted Point in the found voxel........." << std::endl;
	fVoxelNum = voxelNum;
	fVectPointsInVoxel.push_back(pocaPt);
	fPointCount = fVectPointsInVoxel.size();
	fOutlier = fPointCount < fMinPointsInVoxel;
}

Voxel::~Voxel() {
	// TODO Auto-generated destructor stub
}

} /* namespace Tomography */
