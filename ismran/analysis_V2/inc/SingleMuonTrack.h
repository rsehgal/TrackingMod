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
class Point3D;
class SingleMuonTrack {
public:

	std::vector<ScintillatorBar_V2*> fSingleMuonTrack;
public:
	SingleMuonTrack();
	SingleMuonTrack(std::vector<ScintillatorBar_V2*> vecOfScintBars);
	SingleMuonTrack(const SingleMuonTrack &smt);
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
	std::vector<unsigned short> GetLayersHitCountVector();

	/*
	 * Function to detect if the track has single hit in each layer
	 */
	bool IsClearTrack();

	/*
	 * Function to get the hit point vector .i.e vector<Point3D>
	 */
	std::vector<lite_interface::Point3D*> Get3DHitPointVector();

	/*
	 * Function to get the sum of energy deposited in different layers
	 */
	double GetEnergySum();

	ClassDef(SingleMuonTrack,1)
};

} /* namespace lite_interface */

#endif /* ISMRAN_ANALYSIS_V2_INC_SINGLEMUONTRACK_H_ */
