/*
 * TestDelta.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: rsehgal
 */

#undef NDEBUG
#include "Delta.h"
#include <iostream>
#include <assert.h>
#include "Track.h"

using Tomography::Track;
using Vec_t = Tracking::Vector3D<double>;

int main(){

//Simplest test case, Vertical Track.
	double FirstBottomDetectorZ = 45.;
	Track incoming(Vec_t(10.,20.,-45.),Vec_t(10.,20.,-15.));
	Track outgoing(Vec_t(20.,30.,FirstBottomDetectorZ), Vec_t(30.,40.,75.));

	double deltaX = DeltaLinear<true>(incoming,outgoing,FirstBottomDetectorZ);
	double deltaY = DeltaLinear<false>(incoming,outgoing,FirstBottomDetectorZ);
	std::cout<<"DeltaX : " << deltaX <<" : DeltaY : " << deltaY << std::endl;
//	assert(deltaX==10. && deltaY==10.);

	incoming.SetP2(Vec_t(10.,25.,-15.));
	deltaX = DeltaLinear<true>(incoming,outgoing,FirstBottomDetectorZ);
    deltaY = DeltaLinear<false>(incoming,outgoing,FirstBottomDetectorZ);
	std::cout<<"DeltaX : " << deltaX <<" : DeltaY : " << deltaY << std::endl;


	//Trying to calculate DeltaAngular
	double deltaAngX = DeltaAngular<true>(incoming,outgoing);
	double deltaAngY = DeltaAngular<false>(incoming,outgoing);
	std::cout<< "DeltaAngX : "<< deltaAngX <<" : DeltaAngY : " << deltaAngY << std::endl;

	return 0;

}


