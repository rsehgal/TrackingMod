/*
 * Histogram.h
 *
 *  Created on: Jan 9, 2018
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_HISTOGRAM_H_
#define HELPERS_INC_HISTOGRAM_H_

#include <vector>

namespace Tomography {

class Histogram {
public:
	Histogram();
	virtual ~Histogram();
	int GetNumberOfBins()const { return fTotalNumOfBins; }
	void SetNumOfBins(int nbins);
	int GetBinCount(int binNum){ return fBins[binNum]; }
	void Fill2(int binNum){ fBins[binNum]++; }

	//virtual void CalculateBinNum() = 0;


protected:
	std::vector<int> fBins;

private:
	int fTotalNumOfBins;
};

} /* namespace Tomography */

#endif /* HELPERS_INC_HISTOGRAM_H_ */
