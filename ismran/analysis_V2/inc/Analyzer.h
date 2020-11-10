/*
 * Analyzer.h
 *
 *  Created on: 09-Nov-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_V2_INC_ANALYZER_H_
#define ISMRAN_ANALYSIS_V2_INC_ANALYZER_H_

#include <string>
#include <vector>


namespace lite_interface {

class PairFinder;
class ScintillatorBar_V2;

class Analyzer {
	std::string fDatafileName;
	PairFinder *fPairFinder;
	std::vector<ScintillatorBar_V2*> fVecOfScintillatorBar;

public:
	Analyzer();
	Analyzer(std::string datafileName);
	void CreateScintillatorVector();
	void ReconstructMuonTrack();
	static bool CompareTimestampScintillator(ScintillatorBar_V2 *i1, ScintillatorBar_V2 *i2);
	virtual ~Analyzer();

	/*
	 * Required Getters
	 */
	PairFinder* GetPairFinder()const;
	std::vector<ScintillatorBar_V2*> GetVectorOfScintillators()const;
};

} /* namespace lite_interface */

#endif /* ISMRAN_ANALYSIS_V2_INC_ANALYZER_H_ */
