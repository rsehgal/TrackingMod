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
	fMinPointsInVoxel = 6;
	fVectPointsInVoxel.push_back(pocaPt);
	fPointCount = fVectPointsInVoxel.size();
	fOutlier = fPointCount < fMinPointsInVoxel;

}

//Voxel::Voxel(Tracking::Vector3D<double> pocaPt, int voxelNum, Tracking::Vector3D<double> voxelDim,bool useEnclosingVoxels):fVoxelNum(-100){
Voxel::Voxel(Tracking::Vector3D<double> pocaPt, int voxelNum, bool useEnclosingVoxels):fVoxelNum(-100){
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
	//fDim = voxelDim;
	fDim.Set(5*cm,5*cm,4.5*cm);
	fScatteringDensity = 0.;
	fSD = 0.;
	fRL = 0.;
	fMinPointsInVoxel = 6;
	fTotalVoxelsCreated++;
	//std::cout << "New Voxel Created ........ " << std::endl;
	fVoxelNum = voxelNum;
/*
	fVoxelNum = voxelNum;
	fVectPointsInVoxel.push_back(pocaPt);
	fPointCount = fVectPointsInVoxel.size();
	fOutlier = fPointCount < fMinPointsInVoxel;
*/
	Insert(pocaPt, voxelNum, useEnclosingVoxels);

	fVisitedVoxelNumVector.push_back(fVoxelNum);
	fVoxelCenter = Tomography::evolution::Voxelator::instance()->GetVoxelCenter(fVoxelNum);
	fVoxelVector.push_back(this);
	//std::cout<<" @@@@@  VoxelVectorSize :  " << fVoxelVector.size() << "   @@@@@@@" << std::endl;

}

/*
std::vector<Tracking::Vector3D<double>> Voxel::GetEightCorners_Of_ImaginaryVoxel_CentredAtPocaPoint(Tracking::Vector3D<double> pocaPt,){
	int count = 0;
	std::vector<Tracking::Vector3D<double>> corners;
	for(int i=-1 ; i <= 1 ; i=i+2  ) // X axis
		  for(int j=-1 ; j <= 1 ; j=j+2  ) // Y axis
			  for(int k=-1 ; k <= 1 ; k=k+2  ){ // Z axis
				  count++;
				  Tracking::Vector3D<double> corner(pocaPt.x()+(i*fDim.x()/2.) , pocaPt.y()+(j*fDim.y()/2.) , pocaPt.z()+(k*fDim.z()/2.) );
				  //corners.push_back(Tracking::Vector3D<double>(pocaPt.x()+(i*fDim.x()/2.) , pocaPt.y()+(j*fDim.y()/2.) , pocaPt.z()+(k*fDim.z()/2.) ));
				  corners.push_back(corner);
				  //std::cout<<"Corner "<< count << " :: " << corner.x() << " , " << corner.y() << " , " << corner.z() << std::endl;
			  }

	return corners;
}
*/


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
	fRL = CommonFunc::Functions::instance()->RadiationLength(fSD);
	//std::cout << "Setting RL : " << fRL << std::endl;
}

void Voxel::CalcSD(){
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
