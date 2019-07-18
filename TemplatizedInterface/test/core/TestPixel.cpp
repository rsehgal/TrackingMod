/*
 * TestPixel.cpp
 *
 *  Created on: 17-Jul-2019
 *      Author: rsehgal
 */

#include "base/Vector3D.h"
#include "Pixel.h"
#include <iostream>

using Vec_t = Tracking::Vector3D<double>;
using Tomography::Pixel;

int main(){

	Vec_t pt(5.,6.,7.);
	Pixel p(pt);
	p.GetRandomPoint().Print();
	return 0;

}


