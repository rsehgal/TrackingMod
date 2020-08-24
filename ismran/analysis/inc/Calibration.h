/*
 * Calibration.h
 *
 *  Created on: 21-Aug-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_INC_CALIBRATION_H_
#define ISMRAN_ANALYSIS_INC_CALIBRATION_H_

#include <string>
#include <vector>
#include <iostream>

#include <TF1.h>


struct CalibrationData{
	float fDeltaTCorr;
	TF1* fDelTCorr_F;
	TF1* fParameterization_F;
	CalibrationData(){}
	CalibrationData(TF1* delTCorr, TF1* parameterization){
		fDelTCorr_F = delTCorr;
		fParameterization_F = parameterization;
		fDeltaTCorr = fDelTCorr_F->GetParameter(1);
	}

	void Print(){
		std::cout << "DelT Correction : " << fDeltaTCorr << std::endl;
	}

};

class Calibration {

	std::string fFileName;
	std::vector<CalibrationData*> fVecOfCalibrationData;

public:
	Calibration();
	Calibration(std::string fileName);
	virtual ~Calibration();

	/*
	 * Function to return the std::vector of Calibration data of all
	 * the Scintillator Bars
	 */
	std::vector<CalibrationData*> GetCalibrationDataVector()const{
		return fVecOfCalibrationData;
	}

	unsigned int GetNumberOfBars()const{
		return fVecOfCalibrationData.size();
	}

	/*
	 * Function to return Calibration data of specific Scintillator bar
	 */
	CalibrationData* GetCalibrationDataOf(int barNo);
};

#endif /* ISMRAN_ANALYSIS_INC_CALIBRATION_H_ */
