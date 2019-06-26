/*
 * TrackPredictor.h
 *
 *  Created on: 24-Jun-2019
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_TRACKPREDICTOR_H_
#define HELPERS_INC_TRACKPREDICTOR_H_

#include "Track.h"
#include "base/Vector3D.h"

namespace Tomography {

using HitPointVector = std::vector<Tracking::Vector3D<double>>;

struct CombData{
	Track sTrack;
	HitPointVector sHitPointVector;
	CombData(){}
	CombData(Track tr, HitPointVector hitPtVector){
		sTrack = tr;
		sHitPointVector = hitPtVector;
	}

	void Set(Track tr, HitPointVector hitPtVector){
		sTrack = tr;
		sHitPointVector = hitPtVector;
	}
};

struct PixelCombination{
	std::string sPixelCombinationSha;
	//std::vector<Track> sTrackVector;
	//std::vector<HitPointVector> sVectorOfHitPointVector;
	std::vector<CombData> sCombDataVector;
	PixelCombination(std::string pixCombSha,CombData cd){
		sPixelCombinationSha = pixCombSha;
		sCombDataVector.push_back(cd);
	}

};

class TrackPredictor {
	std::vector<PixelCombination> fPixelCombinationVector;
	std::string fPixelCombinationSha;
	CombData fCombData;
	bool fCombinationExist;
	unsigned int fCombinationId;
	Track fTrack;
public:
	TrackPredictor();
	//TrackPredictor(std::vector<int>xStripNumVector, std::vector<int>yStripNumVector);
	virtual ~TrackPredictor();

	void CheckCombinationExistance();
	void InsertCombination();
	void CreateInsertionData(HitPointVector hitPtVector);
	void Process();
};

} /* namespace Tomography */

#endif /* HELPERS_INC_TRACKPREDICTOR_H_ */
