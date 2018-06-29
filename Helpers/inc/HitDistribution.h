/*
 * HitDistribution.h
 *
 *  Created on: Apr 13, 2018
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_HITDISTRIBUTION_H_
#define HELPERS_INC_HITDISTRIBUTION_H_

#include <vector>
#include <iostream>
#include <TH1F.h>

namespace Tomography {



class HitDistribution {
	std::vector<TH1F*>fXHistVector;
	std::vector<TH1F*>fYHistVector;

public:
	HitDistribution();
	virtual ~HitDistribution();
	void Fill(int stripNum, double val, bool x);
	void Fill(double xval, double yval);
	void Write(std::string fileName);
};

} /* namespace Tomography */

#endif /* HELPERS_INC_HITDISTRIBUTION_H_ */
