/*
 * TrackPredictor.cpp
 *
 *  Created on: 24-Jun-2019
 *      Author: rsehgal
 */

#include "TrackPredictor.h"
#include "sha256.h"

namespace Tomography {

TrackPredictor::TrackPredictor() {
	// TODO Auto-generated constructor stub

}

TrackPredictor::~TrackPredictor() {
	// TODO Auto-generated destructor stub
}

/*TrackPredictor::TrackPredictor(std::vector<int>xStripNumVector, std::vector<int>yStripNumVector){
	//TODO: 1) Convert the pixel combintation to unique pixelCombinationId and fPixelCombinationId
}*/

void TrackPredictor::Process(){
    //LOOP OVER ALL THE EVENTS
	{
		fPixelCombinationSha="ABCD"; // Logic to Calculate Combination String
		HitPointVector hitPtVector;  // Logic to Calculate HitPtVector
		CheckCombinationExistance();
		CreateInsertionData(hitPtVector);
		InsertCombination();
	}
}

void TrackPredictor::CheckCombinationExistance(){
	//TODO:   Check if the fPixelCombinationId exist in the pixelCombinationVector;
	//        if yes then return true else return false
	//std::string pixCombinSha = sha256(pixelCombString);

	/*for(auto &pixelComb : fPixelCombinationVector){
		if(pixelComb.sPixelCombinationSha == fPixelCombinationSha){
			pixelComb.sTrackVector.push_back(new Track());
			break;
		}else{
			fPixelCombinationVector.push_back(PixelCombination(fPixelCombinationSha));
		}
	}*/

	fCombinationExist = false;
	fCombinationId = 1000000000;
	/*for(auto &pixelComb : fPixelCombinationVector){
		combinationExist = (pixelComb.sPixelCombinationSha == fPixelCombinationSha);
		if(fCombinationExist){
			break;
		}
	}*/

	for(unsigned int i = 0 ; i < fPixelCombinationVector.size() ;i++){
			fCombinationExist = (fPixelCombinationVector[i].sPixelCombinationSha == fPixelCombinationSha);
			if(fCombinationExist){
				fCombinationId = i;
				break;
			}
	}
}

void TrackPredictor::CreateInsertionData(HitPointVector hitPtVector){
	//TODO : Create Track to be inserted
	Track tr(hitPtVector[0],hitPtVector[hitPtVector.size()-1]);
	fCombData=CombData(tr,hitPtVector);
}


void TrackPredictor::InsertCombination(){
	//TODO:  if pixelCombinationId does not exist then insert the full pixelCombination
	//       along with track
	//       else just append the track in the pixelCombination
	if(fCombinationExist){
		//Just insert the track and hitPointVector
		fPixelCombinationVector[fCombinationId].sCombDataVector.push_back(fCombData);

	}else{
		//Create a new PixelCombination and insert it in the pixelCombinationVector
		fPixelCombinationVector.push_back(PixelCombination(fPixelCombinationSha,fCombData));
	}
}

} /* namespace Tomography */
