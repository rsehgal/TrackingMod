/*
 * TrackPredictorV2.h
 *
 *  Created on: 19-Jul-2019
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_TRACKPREDICTORV2_H_
#define HELPERS_INC_TRACKPREDICTORV2_H_
#include "HelperStructs.h"
#include "sha256.h"
#include "PixelCombinationChecker.h"

namespace Tomography {

class TrackPredictorV2 {
	static bool fRootSet;
	std::string fPixelCombinationSha;
    //CombData *fCombData;
    SHA256 sha256;
    PixelCombinationChecker fPixelCombChecker;
    TreeNode *fRoot;

public:
	TrackPredictorV2();
	virtual ~TrackPredictorV2();
	void CreateInsertionData(HitPointVector hitPtVector);
	unsigned int Process(std::string combString,HitPointVector hitPtVector);
	void Print(){fPixelCombChecker.InOrder(fRoot);}
	unsigned int GetTotalNumOfCombinations();
	unsigned int GetTotalNumOfRegisteredTracks();
	TreeNode* Locate(std::string combString);
};

} /* namespace Tomography */

#endif /* HELPERS_INC_TRACKPREDICTORV2_H_ */
