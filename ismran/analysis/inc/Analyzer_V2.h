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
class TFile;
class Analyzer_V2 {

	std::string fDatafileName;
	Calibration *fCalib;
	static unsigned long int fMuonTrackNum;
	unsigned long int fBunchSize;

public:
	TFile *fout;
	std::vector<TreeEntry*> fVecOfTreeEntry;
	std::vector<TreeEntry*> fVecOfPairedTreeEntry;
	std::vector<ScintillatorBar_V2*> fVecOfScintillatorBar;

	std::vector< std::vector<Point3D*> > fittedMuonTracks;


	/*
	 * Various Required Histograms,
	 *
	 * This can be used to get the Energy calibration data for MUON
	 */
	std::vector<Histograms*> fhistogramsVec;
	std::vector<unsigned int> fVecOfSinglePointEnergyCalibOffsetForMuon;


public:
	Analyzer_V2();
	Analyzer_V2(std::string datafileName,Calibration *calib=NULL, unsigned long int bunchSize=0);
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
	void DisplayHistograms(bool reverse=false);
	void DisplayHistogramsOf(unsigned int barIndex);


	std::vector< SingleMuonTrack* > PlotEnergyLossDistributionOfMuonTracks(std::vector< SingleMuonTrack* > muonTrackVec);

	/*
	 * Function to estimate hit position
	 */
	void EstimateHitPosition(ScintillatorBar_V2 *scint);

	/*
	 * V2 of few functions
	 */
	void PlotHistOfNumOfMuonHitsInMuonTracks_V2(std::vector< SingleMuonTrack* > muonTrackVec);
	void PlotHistOfDelTBetweenMuonTracks_V2(std::vector< SingleMuonTrack* > muonTrackVec);
	void FillCoincidenceHist_V2(std::vector< SingleMuonTrack* > muonTrackVec);
	std::vector < SingleMuonTrack* >ReconstrutTrack_V2();
	void PrintMuonTrackVector_V2(std::vector< SingleMuonTrack* > muonTrackVec);
	//void PlotTracks_V2(std::vector< SingleMuonTrack* > muonTrackVec,unsigned int numOfTracks=20);
	std::vector< std::vector<Point3D*> >  PlotTracks_V2(std::vector< SingleMuonTrack* > muonTrackVec,unsigned int numOfTracks=20);
	void CalculateTotalEnergyDepositionForMuonTracks(std::vector< SingleMuonTrack* > muonTrackVec);
	void PlotCoincidenceCountGraph();
	void PlotEnergyDistributionWithMultiplicity(std::vector<SingleMuonTrack*> muonTrackVec, unsigned int multiplicity = numOfLayers);
	std::vector<SingleMuonTrack*> FiltrationBasedOnCosmicMuonRate(std::vector<SingleMuonTrack*> muonTrackVec);


	/*
	 * Function to process data in bunch
	 */
	void ProcessBunch();
	void ResetBunchMemory();

};

#endif /* ISMRAN_ANALYSIS_INC_ANALYZER_V2_H_ */
