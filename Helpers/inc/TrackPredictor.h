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
#include "sha256.h"

namespace Tomography {

using HitPointVector = std::vector<Tracking::Vector3D<double>>;
void PrintHitPointVector(HitPointVector hitPtVector){
	for(auto &pt : hitPtVector){
		pt.Print();
	}
}

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

	std::string GetCombinationName()const {return sPixelCombinationSha;}

	unsigned int GetNumOfTracks()const {return sCombDataVector.size();}

};

class TrackPredictor {
	std::vector<PixelCombination> fPixelCombinationVector;
	std::string fPixelCombinationSha;
	CombData fCombData;
	bool fCombinationExist;
	unsigned int fCombinationId;
	Track fTrack;

    SHA256 sha256;
public:
	TrackPredictor();
	//TrackPredictor(std::vector<int>xStripNumVector, std::vector<int>yStripNumVector);
	virtual ~TrackPredictor();

	void CheckCombinationExistance();
	void CreateInsertionData(HitPointVector hitPtVector);
	void InsertCombination();
	void Process();
	void Process(HitPointVector hitPtVector);
	unsigned int Process(std::string combString,HitPointVector hitPtVector);

	HitPointVector GetSample(std::string combString);
	unsigned int GetTotalNumOfCombinations()const{return fPixelCombinationVector.size();}
	std::string GetCombinationName(unsigned int combinationIndex)const{return fPixelCombinationVector[combinationIndex].GetCombinationName();}
	std::vector<PixelCombination> GetPixelCombinationVector()const {return fPixelCombinationVector;}
	unsigned int GetNumOfTracks(unsigned int combinationIndex) const {return  fPixelCombinationVector[combinationIndex].GetNumOfTracks();}
	unsigned int GetTotalNumOfRegisteredTracks();
	unsigned int GetRandomTrackNum();
};

} /* namespace Tomography */

#endif /* HELPERS_INC_TRACKPREDICTOR_H_ */
