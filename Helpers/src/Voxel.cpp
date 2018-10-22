/*
 * Voxel.cpp
 *
 *  Created on: Aug 4, 2017
 *      Author: rsehgal
 */

#include "Voxel.h"
#include "CommonFunc.h"
#include "Voxelator_Evolution.h"
#include "G4SystemOfUnits.hh"
#include "base/Global.h"

namespace Tomography {
#if(1)
int Voxel::fTotalVoxelsCreated = 0;
std::vector<int> Voxel::fVisitedVoxelNumVector={};
std::vector<Voxel*> Voxel::fVoxelVector={};

void Voxel::Reset(){
	//Logic : Delete the objects and clear the vector.
	for(int i = 0 ; i < fVoxelVector.size() ; i++){
		delete fVoxelVector[i];
	}
	fVoxelVector.clear();

	//fVectPointsInVoxel.clear();

	fVisitedVoxelNumVector.clear();

}

Voxel::Voxel() {
	// TODO Auto-generated constructor stub
	fMinPointsInVoxel = Tomography::minPointsInAVoxel;

}

Voxel::Voxel(Tracking::Vector3D<double> pocaPt){
	fTotalVoxelsCreated++;
	std::cout<<"New Voxel Created ........ " << std::endl;
	fMinPointsInVoxel = Tomography::minPointsInAVoxel;
	fVectPointsInVoxel.push_back(pocaPt);
	fPointCount = fVectPointsInVoxel.size();
	fOutlier = fPointCount < fMinPointsInVoxel;

}

Voxel::Voxel(Tracking::Vector3D<double> pocaPt, int voxelNum, bool useEnclosingVoxels):fVoxelNum(-100){
	fDim = Tomography::evolution::Voxelator::instance()->GetEachVoxelDim();
	fScatteringDensity = 0.;
	fSD = 0.;
	fRL = 0.;
	fMinPointsInVoxel = Tomography::minPointsInAVoxel;
	fTotalVoxelsCreated++;
	//std::cout << "New Voxel Created ........ " << std::endl;
	fVoxelNum = voxelNum;
	Insert(pocaPt, voxelNum, useEnclosingVoxels);
	fVisitedVoxelNumVector.push_back(fVoxelNum);
	fVoxelCenter = Tomography::evolution::Voxelator::instance()->GetVoxelCenter(fVoxelNum);
	fVoxelVector.push_back(this);
	//std::cout<<" @@@@@  VoxelVectorSize :  " << fVoxelVector.size() << "   @@@@@@@" << std::endl;

}

void Voxel::Insert(Tracking::Vector3D<double> pocaPt, int voxelNum, bool useEnclosingVoxels , double scatteringDensity){
	//std::cout<<"Inserted Point in the found voxel........." << std::endl;
	if(useEnclosingVoxels)
		fScatteringDensity += scatteringDensity;
	else{
	this->fVoxelNum = voxelNum;
	this->fVectPointsInVoxel.push_back(pocaPt);
	this->fPointCount = fVectPointsInVoxel.size();
	this->fOutlier = fPointCount < fMinPointsInVoxel;

	/* For the time being, ScatteringDensity is incremented by 1.
	** But ideally this should be replaced with the function call.
	** that will allocate a fraction between 0. and 1., which should
	** be proportional to the proportion of volume of an imaginary
	** voxel (centred around PocaPt), that it shares with nearby
	** voxels.
	**
	** This is a nice idea to share the intensity with nearby voxels.
	**
	** TODO: Implement a function **CalcScatteringDensity()** to implement the
	** above mentioned idea.
	**/

	fScatteringDensity += scatteringDensity;
	}
}


void Voxel::Insert(Tracking::Vector3D<double> point){
	this->fVectPointsInVoxel.push_back(point);
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
	std::cout << "fPointCount : " << fPointCount <<" : " << __FILE__ << " : " << __LINE__ << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	for(int i = 0  ; i < fPointCount ; i++){
		std::cout << "ScatteringValue : " << fVectPointsInVoxel[i].GetColor() << std::endl;
		scatteringVect.push_back(fVectPointsInVoxel[i].GetColor());///1000.);
	}
	std::cout << "----------------------------------------" << std::endl;
	return scatteringVect;
}

double Voxel::GetStandardDeviation(){

	return fSD;
}

double Voxel::GetRadiationLength(){
	return fRL;
}

void Voxel::CalcRadiationLength(){
	fRL = CommonFunc::Functions::instance()->RadiationLength(fSD);
	//std::cout << "Setting RL : " << fRL << std::endl;
}

void Voxel::CalcSD(){
	std::vector<double> scatteringAngleVector = GetScatteringVector();
	std::cout << "ScatteringAngleVectorSize for Voxel : " << scatteringAngleVector.size() << " : " << __FILE__ << " : " << __LINE__ << std::endl;
	fSD = CommonFunc::Functions::instance()->StandardDeviation(GetScatteringVector(),true);
}

std::vector<Voxel*> Voxel::GetFilteredVoxelVector(){
	std::vector<Voxel*> filteredVoxelVector;
	for(int i= 0 ; i < fVoxelVector.size() ; i++){
		if(!fVoxelVector[i]->IsOutlier()){
			filteredVoxelVector.push_back(fVoxelVector[i]);

		}
	}

	std::cout <<  "@@@@@@@@@@@ Filtered Voxel Vector Size : "<< filteredVoxelVector.size() << " @@@@@@@@@@@@ " << std::endl;
	return filteredVoxelVector;
}

std::vector<Tracking::Vector3D<double>> Voxel::GetFilteredPocaPtVector(){
	std::vector<Voxel*> filteredVoxelVector = GetFilteredVoxelVector();
	std::vector<Tracking::Vector3D<double>> filteredPocaVector;
	for(int i = 0 ; i < filteredVoxelVector.size() ; i++){
		for(int j = 0 ; j < filteredVoxelVector[i]->GetPocaPointsVector().size() ; j++){
			filteredPocaVector.push_back(filteredVoxelVector[i]->GetPocaPointsVector()[j]);
		}
	}

	std::cout <<  "@@@@@@@@@@@ Filtered Poca Vector Size : "<< filteredPocaVector.size() << " @@@@@@@@@@@@ " << std::endl;
	return filteredPocaVector;

}

Voxel::~Voxel() {
	// TODO Auto-generated destructor stub
}
#endif
} /* namespace Tomography */
