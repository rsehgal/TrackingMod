/*
 * TestCoordinates2.cpp
 *
 *  Created on: 27-Jun-2019
 *      Author: rsehgal
 */

#include <iostream>
#include "Coordinates.h"
#include "DetectorMapping.h"
#include "base/Vector3D.h"
using Tomography::DetectorMapping;
using Tomography::Coordinates;
using Vec_t = Tracking::Vector3D<double>;

int main(){
	DetectorMapping *detMapping = Tomography::DetectorMapping::create("testMapping.txt");
	Coordinates c;
	std::cout << "========== Checking Strip number generated from Cooardinates ===========" << std::endl;
	std::cout << "Starting STrip num : " << c.GetStripNum(-498.5) << std::endl;
	std::cout << "End STrip num : " << c.GetStripNum(498.5) << std::endl;
	std::cout << "XYZString : " << c.GetStripNumXYZString(Vec_t(-498.5,-498.5,450.)) << std::endl;

	return 0;
}


