/*
 * TestStripToPixelMapping.cpp
 *
 *  Created on: 16-Oct-2018
 *      Author: rsehgal
 */

#include "Coordinates.h"
#include "DetectorMapping.h"
#include "base/Vector3D.h"
#include <iostream>

#undef NDEBUG
#include <assert.h>
using Tracking::Vector3D;
int main(){
	Tomography::Coordinates c;
	Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::create("testMapping.txt");
	int numOfStripsInEachPlane = detectorMap->GetNumberOfStripsInEachPlane();
	double detectorLength = detectorMap->GetDetectorVector()[3]->sDetectorLength;
	double stripWidth = detectorLength / numOfStripsInEachPlane ;
	double halfStripWidth = stripWidth / 2.;
	assert(numOfStripsInEachPlane == 100 && "Number Of Strips in each plane NOT matches with what is read from testMapping.txt.......");
	int counter = 0;
	for(int i = 0 ; i < numOfStripsInEachPlane ; i++){
		for(int j = 0 ; j < numOfStripsInEachPlane ; j++){
			counter++;
			Vector3D<double> center = c.GetStripCoordinate(i,j);
			//center.Print();
			assert(center.x() == (-detectorLength/2. + (i+0.5)*stripWidth)  && center.y() == (-detectorLength/2. + (j+0.5)*stripWidth)
									&& "Calculated Pixel Center does not matches with expected value .....");

		}
	}

	assert(counter == (numOfStripsInEachPlane*numOfStripsInEachPlane) && "Total Number of Pixels DOES NOT match ...." );
	std::cout << "Total Number of Pixels : " << counter << std::endl;
	return 0;
}


