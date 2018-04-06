/*
 * TestVoxelator.cpp
 *
 *  Created on: Jul 17, 2017
 *      Author: rsehgal
 */

#include <iostream>
#include "Voxelator.h"
#include <cassert>
#include <G4SystemOfUnits.hh>
#include "Voxelator_Evolution.h"
#include "base/Vector3D.h"

#undef NDEBUG

using Tomography::Voxelator;

int main(){

/*
	Voxelator v;
	//v.SetVoxelator(4.5,4.5,4.5,3.,3.,3.);
	v.SetVoxelator(50.,50.,50.,10.,10.,10.);
	v.CalculateVoxelCenters();
	//v.PrintVoxelCenters();


	assert(v.GetTotalNumberOfVoxels() == 1000);


    std::cout<<" ---------------------------- " << std::endl;
    Voxelator t;
    t.SetVoxelator(50.,50.,45.,10.,10.,9.);
    std::cout << "Total num of voxels : " << t.GetTotalNumberOfVoxels() << std::endl;
    int voxNum = t.GetVoxelNumber(24.,15.,13.5);
    std::cout<<"Voxel Number of target : " << voxNum << std::endl;
*/

	Tomography::evolution::Voxelator::Create(50*cm,50*cm,45*cm,5*cm,5*cm,4.5*cm);
	Tomography::evolution::Voxelator *v = Tomography::evolution::Voxelator::instance();
	std::cout << "Total num of voxels : " << v->GetTotalNumberOfVoxels() << std::endl;

	Tracking::Vector3D<double> pt(-192.20365137082084, -30.000000000335845, 11.779620394170763, 0);
	v->Insert(pt);


	return 0;
}




