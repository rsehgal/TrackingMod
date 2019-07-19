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

namespace Tomography {

class TrackPredictorV2 {
	std::string fPixelCombinationSha;
    CombData fCombData;
    SHA256 sha256;
public:
	TrackPredictorV2();
	virtual ~TrackPredictorV2();
	void CreateInsertionData(HitPointVector hitPtVector);
	void Process(std::string combString,HitPointVector hitPtVector);
};

} /* namespace Tomography */

#endif /* HELPERS_INC_TRACKPREDICTORV2_H_ */
