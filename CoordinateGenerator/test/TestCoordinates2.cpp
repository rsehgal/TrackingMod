/*
 * TestCoordinates2.cpp
 *
 *  Created on: 27-Jun-2019
 *      Author: rsehgal
 */

#include <iostream>
#include "Coordinates.h"
#include "DetectorMapping.h"
using Tomography::DetectorMapping;
using Tomography::Coordinates;

int main(){
	DetectorMapping *detMapping = Tomography::DetectorMapping::create("testMapping.txt");
	Coordinates c;
	std::cout << "========== Checking Strip number generated from Cooardinates ===========" << std::endl;
	std::cout << "Starting STrip num : " << c.GetStripNum(-498.5) << std::endl;
	std::cout << "End STrip num : " << c.GetStripNum(498.5) << std::endl;

	return 0;
}


