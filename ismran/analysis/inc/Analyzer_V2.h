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
class SingleMuonTrack;

class Analyzer_V2 {

	std::string fDatafileName;
	Calibration *fCalib;
	static unsigned long int fMuonTrackNum;

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
	void PlotEnergyLossDistributionOfMuons();

	/*
	 * Helper function to draw the Grid
	 */
	//void DrawGrid(TString t, Int_t ngx, Int_t ngy);
	void DrawGrid(std::string t, Int_t ngx, Int_t ngy);

	/*
	 * Function to estimate hit position
	 */
	void EstimateHitPosition(ScintillatorBar_V2 *scint);



	/*
	 * V2 of few functions
	 */
	void PlotHistOfNumOfMuonHitsInMuonTracks_V2(std::vector< SingleMuonTrack > muonTrackVec);
	void PlotHistOfDelTBetweenMuonTracks_V2(std::vector< SingleMuonTrack > muonTrackVec);
	std::vector < SingleMuonTrack >ReconstrutTrack_V2();
	void PrintMuonTrackVector_V2(std::vector< SingleMuonTrack > muonTrackVec);
	void PlotTracks_V2(std::vector< SingleMuonTrack > muonTrackVec,unsigned int numOfTracks=20);
};

#endif /* ISMRAN_ANALYSIS_INC_ANALYZER_V2_H_ */
