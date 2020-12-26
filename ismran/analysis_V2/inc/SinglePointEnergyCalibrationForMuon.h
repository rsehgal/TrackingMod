/*
 * SinglePointEnergyCalibrationForMuon.h
 *
 *  Created on: 17-Sep-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_INC_SINGLEPOINTENERGYCALIBRATIONFORMUON_H_
#define ISMRAN_ANALYSIS_INC_SINGLEPOINTENERGYCALIBRATIONFORMUON_H_

#include "TreeEntry.h"
#include <vector>
#include <string>
#include <iterator>
#include <map>
#include <TMap.h>
#include "ScintillatorBar_V2.h"

//class ScintillatorBar_V2;
class Histograms;
//class Calibration;
class SingleMuonTrack;
class TFile;


class SinglePointEnergyCalibrationForMuon {
	std::string fDatafileName;
	TFile *fout;
	std::vector<TreeEntry*> fVecOfTreeEntry;
	std::vector<TreeEntry*> fVecOfPairedTreeEntry;
	std::vector<lite_interface::ScintillatorBar_V2*> fVecOfScintillatorBar;
	std::vector<UInt_t> fVecOfEnergyCalibFactorForMuon;
	std::vector<Histograms*> fhistogramsVec;
	std::map<std::string,unsigned int> fMapOfEnergyCalibFactorForMuon;
	//TMap *fMapOfEnergyCalibFactorForMuon;
public:
	SinglePointEnergyCalibrationForMuon();
	SinglePointEnergyCalibrationForMuon(std::string datafileName);
	SinglePointEnergyCalibrationForMuon(std::vector<lite_interface::ScintillatorBar_V2*> vecOfScintillatorBar);
	virtual ~SinglePointEnergyCalibrationForMuon();
	void LoadDataAndSort();
	void CheckPairs();
	void ValidatePairs();
	void CreateScintillatorVector();
	void DoSinglePointEnergyCalibrationForMuon();
	void InitializeHistograms();
	void FillHistograms();
	std::vector<unsigned int> GetEnergyCalibFactorVec()const{return fVecOfEnergyCalibFactorForMuon;}
	void FillEnergyCalibrationMap();
	void Reset();
	//void FillCorrectedQMeanHistogram();


};

#endif /* ISMRAN_ANALYSIS_INC_SINGLEPOINTENERGYCALIBRATIONFORMUON_H_ */
