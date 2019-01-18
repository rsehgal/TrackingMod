/*
 * VoxelV2.cpp
 *
 *  Created on: 23-Oct-2018
 *      Author: rsehgal
 */

#include "VoxelV2.h"
#include "CommonFunc.h"
#include "Voxelator_Evolution.h"
#include "G4SystemOfUnits.hh"
#include "base/Global.h"


namespace Tomography {

Voxel_V2::Voxel_V2() {
	// TODO Auto-generated constructor stub

}

Voxel_V2::Voxel_V2(Tracking::Vector3D<double> pocaPt, int voxelNum) {
	fVectPointsInVoxel.push_back(pocaPt);
	//fVectPointsInVoxel.push_back(new Tracking::Vector3D<double>(pocaPt.x(),pocaPt.y(),pocaPt.z()));
	fVoxelNum = voxelNum;
	fDim = Tomography::evolution::Voxelator::instance()->GetEachVoxelDim();
	fScatteringDensity = 0.;
	fSD = 0.;
	fRL = 0.;
	fMinPointsInVoxel = Tomography::minPointsInAVoxel;
	fPointCount = fVectPointsInVoxel.size();
	fOutlier = fPointCount < fMinPointsInVoxel;
	fVoxelCenter = Tomography::evolution::Voxelator::instance()->GetVoxelCenter(fVoxelNum);
	fCleanCount = 0;
}

Voxel_V2::Voxel_V2(int voxelNum){
	fVectPointsInVoxel.push_back(Tracking::Vector3D<double>(0.,0.,0.));
	fVoxelNum = voxelNum;
	fDim = Tomography::evolution::Voxelator::instance()->GetEachVoxelDim();
	fScatteringDensity = 0.;
	fSD = 0.;
	fRL = 0.;
	fMinPointsInVoxel = Tomography::minPointsInAVoxel;
	fPointCount = 0;
	fOutlier = false;// Setting specifically to false // (fPointCount < fMinPointsInVoxel);
	fVoxelCenter = Tomography::evolution::Voxelator::instance()->GetVoxelCenter(fVoxelNum);
	fCleanCount = 1;

}


void Voxel_V2::Print(){
	std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
	if(fVoxelNum < 0 || fVoxelNum > Tomography::evolution::Voxelator::instance()->GetTotalNumberOfVoxels())
		return;
	std::cout << "Total Num Of PocaPoint in Voxel : " << fVoxelNum << " :: " << fVectPointsInVoxel.size() << std::endl;
	for(int i = 0 ; i < fVectPointsInVoxel.size() ; i++){
		fVectPointsInVoxel[i].Print();
	}
	std::cout << std::endl;
	std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
}

void Voxel_V2::Reset_V2(){
	fVectPointsInVoxel.clear();
	fCleanCount = 0;
}

void Voxel_V2::Insert(Tracking::Vector3D<double> point){
	fVectPointsInVoxel.push_back(point);
	fPointCount = fVectPointsInVoxel.size();
	fOutlier = fPointCount < fMinPointsInVoxel;
}

void Voxel_V2::CalcRadiationLength(){
	fRL = CommonFunc::Functions::instance()->RadiationLength(fSD);
	//std::cout << "Setting RL : " << fRL << std::endl;
}

void Voxel_V2::CalcSD(){
	std::vector<double> scatteringAngleVector = GetScatteringVector();
	std::cout << "ScatteringAngleVectorSize for Voxel : " << scatteringAngleVector.size() << " : " << __FILE__ << " : " << __LINE__ << std::endl;
	fSD = CommonFunc::Functions::instance()->StandardDeviation(GetScatteringVector(),true);
}

std::vector<double> Voxel_V2::GetScatteringVector(){
	std::vector<double> scatteringVect;
	std::cout << "fPointCount : " << fPointCount <<" : " << __FILE__ << " : " << __LINE__ << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	for(int i = 0  ; i < fPointCount ; i++){
		std::cout << "ScatteringValue : " << fVectPointsInVoxel[i].GetColor() << std::endl;
		//std::cout << "ScatteringValue : " << fVectPointsInVoxel[i]->GetColor() << std::endl;
		scatteringVect.push_back(fVectPointsInVoxel[i].GetColor());///1000.);
		//scatteringVect.push_back(fVectPointsInVoxel[i]->GetColor());///1000.);
	}
	std::cout << "----------------------------------------" << std::endl;
	return scatteringVect;
}


Voxel_V2::~Voxel_V2() {
	// TODO Auto-generated destructor stub
}

} /* namespace Tomography */
