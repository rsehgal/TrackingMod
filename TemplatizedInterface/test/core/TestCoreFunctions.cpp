/*
 * TestCoreFunctions.cpp
 *
 *  Created on: 09-Jul-2019
 *      Author: rsehgal
 *
 *  This contains tests for basic functions that are used
 *  all over the code.
 *  Like : Function to check VoxelCenter.
 *         Function to check returned VoxelNumber, using Coordinates as input
 *         Function to check returned VoxelCenter, using Cooardinates as input
 *         Function to check returned VoxelCenter, using VoxelNumber as input
 *
 *         etc.. etc..
 */

// ensure asserts are compiled in
#undef NDEBUG

#include "base/Vector3D.h"
#include "base/TomoUnits.h"
#include "Voxelator_Evolution.h"
#include <cassert>

using Vec_t = Tracking::Vector3D<double>;
using Voxelator = Tomography::evolution::Voxelator;
//using namespace Tomography::Units;

bool TestVoxelCenter(Vec_t pt){

	Voxelator *voxelator = Voxelator::instance();
	bool valid = true;
	Vec_t voxCenter = voxelator->GetVoxelCenter(pt,valid);
	std::cout << "VoxelCenter : " ; voxCenter.Print();

	return true;
}

bool TestVoxelCenter(int voxelNum){
	Voxelator *voxelator = Voxelator::instance();
	bool valid = true;
	Vec_t voxCenter = voxelator->GetVoxelCenter(voxelNum,valid);
	std::cout << "Voxel Num : " << voxelNum << " : VoxelCenter : " ; voxCenter.Print();


	return true;
}


int main(){

	Vec_t zero(0.,0.,0.);
	Voxelator::Create(50.*Tomography::Units::cm,50.*Tomography::Units::cm,50.*Tomography::Units::cm,
					  5*Tomography::Units::cm,5*Tomography::Units::cm,5*Tomography::Units::cm);
	assert(TestVoxelCenter(zero) && TestVoxelCenter(555) && "Some of the test got failed");

	for(unsigned int i = 0 ; i < Voxelator::instance()->GetTotalNumberOfVoxels() ; i++){
		TestVoxelCenter(i);
	}


	std::cout << "================================" << std::endl;
	std::cout << "TestCoreFunctions passed......." << std::endl;

	return 0;
}



