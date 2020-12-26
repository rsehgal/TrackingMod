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

extern bool IsSimulation;

class Analyzer {
	std::string fDatafileName;
	std::string fOutputfileName;
	PairFinder *fPairFinder;
	std::vector<ScintillatorBar_V2*> fVecOfScintillatorBar;

public:
	Analyzer();
	//Analyzer(std::string datafileName,std::string outputfileName, bool simulation=false);
	Analyzer(std::string datafileName,std::string outputfileName);
	Analyzer(std::string datafileName );
	void CreateScintillatorVector();
#ifdef USE_FOR_SIMULATION
	void CreateScintillatorVector_FromSimulation();
#endif
	void ReconstructMuonTrack();
	//std::vector< lite_interface::SingleMuonTrack* > ReconstructMuonTrack();
	static bool CompareTimestampScintillator(ScintillatorBar_V2 *i1, ScintillatorBar_V2 *i2);
	virtual ~Analyzer();

	/*
	 * Required Getters
	 */
	PairFinder* GetPairFinder()const;
	std::vector<ScintillatorBar_V2*> GetVectorOfScintillators()const;


	/*
	 * variable for Shots mechanism
	 */
	static int numOfShots;
	static int shotNo;
};

} /* namespace lite_interface */

#endif /* ISMRAN_ANALYSIS_V2_INC_ANALYZER_H_ */
