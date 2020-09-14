/*
 * SingleMuonTrack.h
 *
 *  Created on: 07-Sep-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_INC_SINGLEMUONTRACK_H_
#define ISMRAN_ANALYSIS_INC_SINGLEMUONTRACK_H_

#include<vector>
#include "includes.hh"


class ScintillatorBar_V2;
class TGraph;
class Point3D;

extern double LinearFit();

class SingleMuonTrack {
//Keeping data members public for the time being
public:
	double fTotalEnergyDeposited;
	std::vector<ScintillatorBar_V2*> fSingleMuonTrack;
	bool fIsValid;
	static int fMuonTrackNum;

	/*
	 * Storing just the fitted muon track, NOT the vector of Scintillator bars
	 */
	std::vector<Point3D*> fSingleMuonTrackFitted;


public:
	SingleMuonTrack();
	virtual ~SingleMuonTrack();
	void CalculateTotalEnergyDeposited();

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


	/*
	 * Print related functions
	 */
	void Print();

	/*
	 * Function to plot the track
	 */
	std::vector<Point3D*> PlotTrack();
	void DrawGrid(std::string t, Int_t ngx, Int_t ngy);
	std::vector<double> GetFittedXorZ(TGraph *grxy);
	//std::vector<double> GetFittedZ(TGraph *grzy);


	/*
	 * Function to give Step information with the bar
	 */
	//void CalculateStep();
};

#endif /* ISMRAN_ANALYSIS_INC_SINGLEMUONTRACK_H_ */
