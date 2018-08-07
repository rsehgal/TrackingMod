/*
 * Pixel.h
 *
 *  Created on: Aug 7, 2018
 *      Author: raman
 */

#ifndef PIXEL_H_
#define PIXEL_H_

#include "base/Vector3D.h"

namespace Tomography {

using Vec_t = Tracking::Vector3D<double>;

class Pixel {
	Vec_t fPixelCenter;
	Vec_t fRandomPoint;
	double fPixelWidth;
	double fHalfPixelWidth;
public:
	Pixel();
	Pixel(Vec_t pixelCenter);
	virtual ~Pixel();
	Vec_t GetPixelCenter() const {return fPixelCenter;}
	void GenerateRandom();
	Vec_t GetRandomPoint() const {return fRandomPoint;}
};

} /* namespace Tomography */

#endif /* PIXEL_H_ */
