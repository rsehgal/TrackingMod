/*
 * TestTrackPredictorV2.cpp
 *
 *  Created on: 19-Jul-2019
 *      Author: rsehgal
 */

#include <iostream>
#include "TrackPredictorV2.h"
#include "PixelCombinationChecker.h"

using Tomography::TrackPredictorV2;
using Vec_t = Tracking::Vector3D<double>;

int main(){

	TrackPredictorV2 t;
	Tomography::HitPointVector v1;
	v1.push_back(Vec_t(249.71514449228925514, 312.07044576555085769, 1060));
	v1.push_back(Vec_t(268.74812821354197467, 246.92518778461368356, 760));
	v1.push_back(Vec_t(287.78005428979423641, 181.77926867546150902, 460));
	v1.push_back(Vec_t(344.89173365237667213, -13.659500672018651812,-440));
	v1.push_back(Vec_t(363.92977063134026139, -78.804298120518140536, -740));
	v1.push_back(Vec_t(382.96624272932518807, -143.94733838096792056, -1040));
	t.Process("ABCD",v1);
	//t.Process("ABCD",v1);

	t.Print();

}


