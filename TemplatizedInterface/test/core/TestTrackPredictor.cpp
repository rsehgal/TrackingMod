/*
 * TestTrackPredictor.cpp
 *
 *  Created on: 26-Jun-2019
 *      Author: rsehgal
 */

#include <iostream>
#include "TrackPredictor.h"

using Tomography::TrackPredictor;
using Vec_t = Tracking::Vector3D<double>;

int main(){
	TrackPredictor t;


	//Tomography::HitPointVector pixelCenterVector;
	//t.GetSample(pixelCenterVector);

	//First hitpointVector Track
	Tomography::HitPointVector v1;
	v1.push_back(Vec_t(249.71514449228925514, 312.07044576555085769, 1060));
	v1.push_back(Vec_t(268.74812821354197467, 246.92518778461368356, 760));
	v1.push_back(Vec_t(287.78005428979423641, 181.77926867546150902, 460));
	v1.push_back(Vec_t(344.89173365237667213, -13.659500672018651812, -439.99999999999988631));
	v1.push_back(Vec_t(363.92977063134026139, -78.804298120518140536, -740));
	v1.push_back(Vec_t(382.96624272932518807, -143.94733838096792056, -1040));
	t.Process(v1);

	t.Process("ABCD",v1);
	t.Process("ABCDE",v1);
	t.Process("BCDE",v1);
	t.Process("BCDE",v1);
	std::cout <<"Total Num of Combination : " << t.GetTotalNumOfCombinations() << std::endl;
    std::cout <<"Total Num of Registered Tracks : " << t.GetTotalNumOfRegisteredTracks() << std::endl;
	for(unsigned int i = 0 ; i < t.GetTotalNumOfCombinations() ; i++){
		std::cout << "Combination ID : " << i <<" : Combination Name : " << t.GetCombinationName(i)
				  << "  : Num of Tracks in Combination : " << t.GetNumOfTracks(i) <<  std::endl;
	}

	return 0;
}


