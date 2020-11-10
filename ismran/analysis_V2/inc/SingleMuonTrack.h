/*
 * SingleMuonTrack.h
 *
 *  Created on: 10-Nov-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_V2_INC_SINGLEMUONTRACK_H_
#define ISMRAN_ANALYSIS_V2_INC_SINGLEMUONTRACK_H_

#include <vector>
#pragma once
#include <TROOT.h>
namespace lite_interface {

class ScintillatorBar_V2;

class SingleMuonTrack {
	std::vector<ScintillatorBar_V2*> fSingleMuonTrack;
public:
	SingleMuonTrack();
	virtual ~SingleMuonTrack();
	/*
	 * Sorting related functions
	 */
	static bool CompareBarIndexInScintillator(ScintillatorBar_V2 *i1, ScintillatorBar_V2 *i2);
	void Sort();

	/*
	 * Function similar to std::vector
	 */
	void push_back(ScintillatorBar_V2* hittedBar);
	void clear();
	unsigned long int size();

	void Print();
	std::vector<ScintillatorBar_V2*> GetMuonTrack()const;
	ClassDef(SingleMuonTrack,1)
};

} /* namespace lite_interface */

#endif /* ISMRAN_ANALYSIS_V2_INC_SINGLEMUONTRACK_H_ */
