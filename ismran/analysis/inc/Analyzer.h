/*
 * Analyzer.h
 *
 *  Created on: 21-Aug-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_INC_ANALYZER_H_
#define ISMRAN_ANALYSIS_INC_ANALYZER_H_

#include "Calibration.h"
#include <iostream>
#include "TreeEntry.h"

#include "PsBar.h"
using TreeEntryVector = std::vector<TreeEntry>;

class Analyzer {

	Calibration *fCalib;
	std::string fDataFileName;
	TreeEntryVector fVecOfTreeEntry;
	std::vector<ScintillatorBar*> scintBarVec;
public:
	Analyzer();
	Analyzer(std::string calibFileName, std::string dataFileName);
	void PerformDelTCorrection();
	void PlotHistOf(unsigned int barIndex);
	void EstimateZPositionOn(unsigned int barIndex);
	void EstimateZPositionForAnEvenOnBar(std::vector<ScintillatorBar*> singleMuonTrack);
	virtual ~Analyzer();
};

#endif /* ISMRAN_ANALYSIS_INC_ANALYZER_H_ */
