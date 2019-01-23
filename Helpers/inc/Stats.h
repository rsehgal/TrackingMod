/*
 * Stats.h
 *
 *  Created on: 23-Jan-2019
 *      Author: rsehgal
 *
 *  Ideas is to define all the statistics related stuff like all histogram here only
 *  and finally those all should be written to single ROOT File
 */

#ifndef HELPERS_INC_STATS_H_
#define HELPERS_INC_STATS_H_

#include <string>
#include "RunHelper.h"

class TFile;
class TH1F;
class TH2F;


namespace Tomography {

class Stats {
private:
	//DataMember to get the processed data from RunHelper
	Tomography::RunHelper *fRunHelper;

	//Define Root file handle
	TFile *fStats;

	//Define all the required Histograms
	TH1F *fHistScattering;
public:
	Stats();
	Stats(std::string filename);
	void CreateScatteringHistogram();
	virtual ~Stats();
};

} /* namespace Tomography */

#endif /* HELPERS_INC_STATS_H_ */
