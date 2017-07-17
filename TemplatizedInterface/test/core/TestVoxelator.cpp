/*
 * TestVoxelator.cpp
 *
 *  Created on: Jul 17, 2017
 *      Author: rsehgal
 */

#include <iostream>
#include "Voxelator.h"
using Tomography::Voxelator;

int main(){

	Voxelator v;
	//v.SetVoxelator(4.5,4.5,4.5,3.,3.,3.);
	v.SetVoxelator(50.,50.,50.,10.,10.,10.);
	v.CalculateVoxelCenters();
	v.PrintVoxelCenters();


	return 0;
}




