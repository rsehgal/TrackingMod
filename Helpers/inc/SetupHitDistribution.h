/*
 * SetupHitDistribution.h
 *
 *  Created on: Aug 15, 2018
 *      Author: raman
 */

#ifndef SETUPHITDISTRIBUTION_H_
#define SETUPHITDISTRIBUTION_H_

#include "HitDistribution.h"
#include <vector>
#include <cstring>
#include <Properties.h>
#include "base/Vector3D.h"

namespace Tomography {

using Detector = Properties;
using Vec_t = Tracking::Vector3D<double>;

class SetupHitDistribution {
	std::vector<std::string> fDetectorNameVector;
	std::vector<HitDistribution*> fHitDistributionVector;
	//std::vector<Vec_t> fHitPtVector;


public:
	SetupHitDistribution();
	SetupHitDistribution(std::vector<std::string> detectorNameVector);
	SetupHitDistribution(std::string detectorName);
	SetupHitDistribution(std::vector<Detector*> detectorVector){}
	SetupHitDistribution(Detector* detector){}
	void FillHitPointVector(std::vector<Vec_t> hitPtVector);
	void Write();

	//Function to delete the objects at the end,
	//Must be called at the end, but be careful.
	void Delete();

	virtual ~SetupHitDistribution();
};

} /* namespace Tomography */

#endif /* SETUPHITDISTRIBUTION_H_ */
