/*
 * TrackPredictor.h
 *
 *  Created on: 24-Jun-2019
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_TRACKPREDICTOR_H_
#define HELPERS_INC_TRACKPREDICTOR_H_

#include "Track.h"

namespace Tomography {

struct PixelCombination{
	std::string sPixelCombinationSha;
	std::vector<Track*> sTrackVector;
	PixelCombination(std::string pixCombSha){
		sPixelCombinationSha = pixCombSha;
	}

};

class TrackPredictor {
	std::vector<PixelCombination> fPixelCombinationVector;
	std::string fPixelCombinationSha;
	Track fTrack;
public:
	TrackPredictor();
	TrackPredictor(std::vector<int>xStripNumVector, std::vector<int>yStripNumVector);
	virtual ~TrackPredictor();

	bool PixelCombinationExist();
	void InsertCombination();
};

} /* namespace Tomography */

#endif /* HELPERS_INC_TRACKPREDICTOR_H_ */
