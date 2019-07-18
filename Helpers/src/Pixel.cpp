/*
 * Pixel.cpp
 *
 *  Created on: Aug 7, 2018
 *      Author: raman
 */

#include "Pixel.h"
#include "Coordinates.h"
#include "DetectorMapping.h"
#include <cmath>

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
	fRandomPoint.SetZ(fPixelCenter.z());
	GenerateRandom();
}

Pixel::~Pixel() {
	// TODO Auto-generated destructor stub
}

unsigned int Pixel::CalcPixelNumber(){
	double detHalfLength = Tomography::DetectorMapping::instance()->GetDetectorVector()[3]->sDetectorLength/2.;
	unsigned int xnum = (unsigned int) (fPixelCenter.x()+detHalfLength)/fPixelWidth;
	unsigned int ynum = (unsigned int) (fPixelCenter.y()+detHalfLength)/fPixelWidth;
	unsigned int pixNum = ynum*Tomography::DetectorMapping::instance()->GetNumberOfStripsInEachPlane()+xnum;
	return pixNum;
}

void Pixel::GenerateRandom(){
	//Logic to get the random point within the pixel
	fRandomPoint = fPixelCenter;
	Coordinates c;
	//double sigma = fHalfPixelWidth;
	double sigma = fPixelWidth/std::sqrt(12);

#define UNIFORM

	#ifdef UNIFORM
	fRandomPoint.SetX(c.GenRandom(fPixelCenter.x()-sigma,fPixelCenter.x()+sigma));
	fRandomPoint.SetY(c.GenRandom(fPixelCenter.y()-sigma,fPixelCenter.y()+sigma));
	#else
	fRandomPoint.SetX(c.GenRandomGauss(fPixelCenter.x(),sigma));
	fRandomPoint.SetY(c.GenRandomGauss(fPixelCenter.y(),sigma));
	#endif
}

} /* namespace Tomography */
