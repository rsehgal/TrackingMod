/*
 * TrackPredictorV2.cpp
 *
 *  Created on: 19-Jul-2019
 *      Author: rsehgal
 */

#include "TrackPredictorV2.h"
#include <stdlib.h>
#include <time.h>
#include <chrono>



namespace Tomography {

TrackPredictorV2::TrackPredictorV2() {
	// TODO Auto-generated constructor stub
	fRoot = NULL;

}

TrackPredictorV2::~TrackPredictorV2() {
	// TODO Auto-generated destructor stub
}

//Function to be used
void TrackPredictorV2::Process(std::string combString,HitPointVector hitPtVector){

	fPixelCombinationSha = sha256(combString);
	CreateInsertionData(hitPtVector);



}

void TrackPredictorV2::CreateInsertionData(HitPointVector hitPtVector){
	//TODO : Create Track to be inserted
	Track tr(hitPtVector[0],hitPtVector[hitPtVector.size()-1]);
	fCombData=CombData(tr,hitPtVector);
	PixelCombination pixelComb(fPixelCombinationSha,fCombData);
	fPixelCombChecker.Insert(fRoot,pixelComb);
}


} /* namespace Tomography */
