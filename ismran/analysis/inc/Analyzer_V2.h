/*
 * Analyzer_V2.h
 *
 *  Created on: 03-Sep-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_INC_ANALYZER_V2_H_
#define ISMRAN_ANALYSIS_INC_ANALYZER_V2_H_

#include "TreeEntry.h"
#include <vector>
#include <string>

class ScintillatorBar_V2;
class Histograms;
class Calibration;

class Analyzer_V2 {

	std::string fDatafileName;
	Calibration *fCalib;

public:
	std::vector<TreeEntry*> fVecOfTreeEntry;
	std::vector<TreeEntry*> fVecOfPairedTreeEntry;
	std::vector<ScintillatorBar_V2*> fVecOfScintillatorBar;


	//Various Required Histograms
	std::vector<Histograms*> fhistogramsVec;


public:
	Analyzer_V2();
	Analyzer_V2(std::string datafileName,Calibration *calib=NULL);
	virtual ~Analyzer_V2();
	void LoadDataAndSort();
	void CheckPairs();
	void ValidatePairs();
	void CreateScintillatorVector();
	std::vector< std::vector<ScintillatorBar_V2*> > ReconstrutTrack();

	/*
	 * Helper function for sorting
	 */
	static bool CompareTimestampScintillator(ScintillatorBar_V2 *i1, ScintillatorBar_V2 *i2);
	static bool CompareBarIndexInScintillator(ScintillatorBar_V2 *i1, ScintillatorBar_V2 *i2);

	ULong64_t GetMeanTValueOfATrack(std::vector<ScintillatorBar_V2*> singleMuonTrack);

	/*
	 * Function to Print Vector of Muon Track
	 */
	void PrintMuonTrackVector(std::vector< std::vector<ScintillatorBar_V2*> > muonTrackVec);

	void InitializeHistograms();
	/*
	 * Functions to plot the require Histograms
	 */
	void PlotHistOfNumOfMuonHitsInMuonTracks(std::vector< std::vector<ScintillatorBar_V2*> > muonTrackVec);
	void PlotHistOfDelTBetweenMuonTracks(std::vector< std::vector<ScintillatorBar_V2*> > muonTrackVec);
	void FillHistograms();
	void DisplayHistograms();
	void PlotTracks(std::vector<std::vector<ScintillatorBar_V2*>> muonTrackVec,unsigned int numOfTracks=20);
	void PlotOneTrack(std::vector<ScintillatorBar_V2*> singleMuonTrack);

	/*
	 * Function to estimate hit position
	 */
	void EstimateHitPosition(ScintillatorBar_V2 *scint);
};

#endif /* ISMRAN_ANALYSIS_INC_ANALYZER_V2_H_ */
