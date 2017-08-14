/*
 * Voxel.cpp
 *
 *  Created on: Aug 4, 2017
 *      Author: rsehgal
 */

#include "Voxel.h"
#include "CommonFunc.h"
#include "Voxelator_Evolution.h"

namespace Tomography {
#if(1)
int Voxel::fTotalVoxelsCreated = 0;
std::vector<int> Voxel::fVisitedVoxelNumVector={};
std::vector<Voxel*> Voxel::fVoxelVector={};

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

Voxel::Voxel(Tracking::Vector3D<double> pocaPt, int voxelNum):fVoxelNum(-100){
	/*fMinPointsInVoxel = 10;
	int existingVoxelNumber = IfVoxelExist(voxelNum);
	if(existingVoxelNumber < 0.){
		fTotalVoxelsCreated++;
		std::cout<<"New Voxel Created ........ " << std::endl;
		fVoxelNum = voxelNum;
	}



	fVectPointsInVoxel.push_back(pocaPt);
	fPointCount = fVectPointsInVoxel.size();
	fOutlier = fPointCount < fMinPointsInVoxel;*/
	fSD = 0.;
	fRL = 0.;
	fMinPointsInVoxel = 100;
	fTotalVoxelsCreated++;
	//std::cout << "New Voxel Created ........ " << std::endl;
	fVoxelNum = voxelNum;
	fVectPointsInVoxel.push_back(pocaPt);
	fPointCount = fVectPointsInVoxel.size();
	fOutlier = fPointCount < fMinPointsInVoxel;
	fVisitedVoxelNumVector.push_back(fVoxelNum);
	fVoxelVector.push_back(this);
	fVoxelCenter = Tomography::Voxelator::instance()->GetVoxelCenter(fVoxelNum);

}

void Voxel::Insert(Tracking::Vector3D<double> pocaPt, int voxelNum){
	//std::cout<<"Inserted Point in the found voxel........." << std::endl;
	this->fVoxelNum = voxelNum;
	this->fVectPointsInVoxel.push_back(pocaPt);
	this->fPointCount = fVectPointsInVoxel.size();
	this->fOutlier = fPointCount < fMinPointsInVoxel;
}


int Voxel::IfVoxelExist(int voxelNum){
//std::cout<< "VoxelVectorSize : " << run->fVoxelVector.size() << std::endl;
	if(fVisitedVoxelNumVector.size()){
		for(int i = 0 ; i < fVisitedVoxelNumVector.size() ; i++){
			if(fVisitedVoxelNumVector[i] == voxelNum){
				//std::cout<<"Voxel Hit.........Found previously created Voxel... :  "  << fVisitedVoxelNumVector[i] << std::endl;
				//return fVisitedVoxelNumVector[i];
				return i;
			}
		}
		return -1;
	}else{
		return -1;
	}
}


std::vector<double> Voxel::GetScatteringVector(){
	std::vector<double> scatteringVect;
	for(int i = 0  ; i < fPointCount ; i++){
		scatteringVect.push_back(fVectPointsInVoxel[i].GetColor());///1000.);
	}
	return scatteringVect;
}

double Voxel::GetStandardDeviation(){

	return fSD;
}

double Voxel::GetRadiationLength(){
	return fRL;
}

void Voxel::CalcRadiationLength(){
	fRL = CommonFunc::Functions::RadiationLength(fSD);
	//std::cout << "Setting RL : " << fRL << std::endl;
}

void Voxel::CalcSD(){
	fSD = CommonFunc::Functions::StandardDeviation(GetScatteringVector());
}

Voxel::~Voxel() {
	// TODO Auto-generated destructor stub
}
#endif
} /* namespace Tomography */
