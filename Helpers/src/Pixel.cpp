/*
 * Pixel.cpp
 *
 *  Created on: Aug 7, 2018
 *      Author: raman
 */

#include "Pixel.h"
#include "Coordinates.h"
#include "DetectorMapping.h"

namespace Tomography {

using Vec_t = Tracking::Vector3D<double>;

Pixel::Pixel() {
	// TODO Auto-generated constructor stub;
	//fPixelWidth = 31.25;
	fPixelWidth = Tomography::DetectorMapping::instance()->GetDetectorVector()[3]->sDetectorLength
						/ Tomography::DetectorMapping::instance()->GetNumberOfStripsInEachPlane();
	fHalfPixelWidth = fPixelWidth/2.;

}

Pixel::Pixel(Vec_t pixelCenter) {
	//fPixelWidth = 31.25;
	fPixelWidth = Tomography::DetectorMapping::instance()->GetDetectorVector()[3]->sDetectorLength
					/ Tomography::DetectorMapping::instance()->GetNumberOfStripsInEachPlane();
	fHalfPixelWidth = fPixelWidth/2.;
	fPixelCenter = pixelCenter;
	GenerateRandom();
}

Pixel::~Pixel() {
	// TODO Auto-generated destructor stub
}

void Pixel::GenerateRandom(){
	//Logic to get the random point within the pixel
	fRandomPoint = fPixelCenter;
	Coordinates c;
#ifdef UNIFORM
	fRandomPoint.SetX(c.GenRandom(fPixelCenter.x()-fHalfPixelWidth,fPixelCenter.x()+fHalfPixelWidth));
	fRandomPoint.SetY(c.GenRandom(fPixelCenter.y()-fHalfPixelWidth,fPixelCenter.y()+fHalfPixelWidth));
#else
	fRandomPoint.SetX(c.GenRandomGauss(fPixelCenter.x(),fHalfPixelWidth));
	fRandomPoint.SetY(c.GenRandomGauss(fPixelCenter.y(),fHalfPixelWidth));
#endif
}

} /* namespace Tomography */
