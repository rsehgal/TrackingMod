/*
 * TestPoca.cpp
 *
 *  Created on: 15-Jan-2019
 *      Author: rsehgal
 */

#include "Imaging.h"
#include <iostream>
#include "base/Vector3D.h"
#include "Track.h"
int main(){

	Tracking::ImageReconstruction im;
	Tomography::Track incoming(Tracking::Vector3D<double>(338.198, -26.4614, 1060),Tracking::Vector3D<double>(199.706, 38.0842, 460));
	Tomography::Track outgoing(Tracking::Vector3D<double>(-8.07893, 134.916, -440),Tracking::Vector3D<double>(-146.596, 199.468 ,-1040));

	Tracking::Vector3D<double> pocaPt = im.POCA(incoming,outgoing);
	pocaPt.Print();

	return 0;
}



