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

TrackPredictor::TrackPredictor(std::vector<int>xStripNumVector, std::vector<int>yStripNumVector){
	//TODO: 1) Convert the pixel combintation to unique pixelCombinationId and fPixelCombinationId
}

bool TrackPredictor::PixelCombinationExist(){
	//TODO:   Check if the fPixelCombinationId exist in the pixelCombinationVector;
	//        if yes then return true else return false
	//std::string pixCombinSha = sha256(pixelCombString);
	for(auto &pixelComb : fPixelCombinationVector){
		if(pixelComb.sPixelCombinationSha == fPixelCombinationSha){
			pixelComb.sTrackVector.push_back(new Track());
			break;
		}else{
			fPixelCombinationVector.push_back(PixelCombination(fPixelCombinationSha));
		}
	}

}

void TrackPredictor::InsertCombination(){
	//TODO:  if pixelCombinationId does not exist then insert the full pixelCombination
	//       along with track
	//       else just append the track in the pixelCombination
}
} /* namespace Tomography */
