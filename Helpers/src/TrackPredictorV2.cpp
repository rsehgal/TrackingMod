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

using namespace std::chrono;

namespace Tomography {

bool TrackPredictorV2::fRootSet = false;

TrackPredictorV2::TrackPredictorV2() {
	// TODO Auto-generated constructor stub
	fRoot = NULL;

}

TrackPredictorV2::~TrackPredictorV2() {
	// TODO Auto-generated destructor stub
}

//Function to be used
unsigned int TrackPredictorV2::Process(std::string combString,HitPointVector hitPtVector){

	fPixelCombinationSha = sha256(combString);
	auto start = high_resolution_clock::now();
	CreateInsertionData(hitPtVector);
	auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    return duration.count();



}

unsigned int TrackPredictorV2::GetTotalNumOfCombinations(){
	return TreeNode::sNodeCounter;
}

unsigned int TrackPredictorV2::GetTotalNumOfRegisteredTracks(){
	fPixelCombChecker.CalcTotalNumOfRegisteredTracks(fRoot);
	return PixelCombinationChecker::fTotalNumOfRegTracks;
}

void TrackPredictorV2::CreateInsertionData(HitPointVector hitPtVector){
	//TODO : Create Track to be inserted
	Track tr(hitPtVector[0],hitPtVector[hitPtVector.size()-1]);
	//fCombData = new CombData(tr,hitPtVector);
	//PixelCombination *pixelComb = new PixelCombination(fPixelCombinationSha,fCombData);
	PixelCombination *pixelComb = new PixelCombination(fPixelCombinationSha,new CombData(tr,hitPtVector));
	//std::cout << "RooSet : " << fRootSet << std::endl;
	if(fRootSet)
		TreeNode *temp = fPixelCombChecker.Insert(fRoot,pixelComb);
	else{
		fRoot = fPixelCombChecker.Insert(fRoot,pixelComb);
		fRootSet = true;
	}
	//delete pixelComb;
}


} /* namespace Tomography */
