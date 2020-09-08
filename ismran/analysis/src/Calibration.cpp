/*
 * Calibration.cpp
 *
 *  Created on: 21-Aug-2020
 *      Author: rsehgal
 */

#include "Calibration.h"
#include "TFile.h"
#include "PsBar.h"
#include "includes.hh"

Calibration::Calibration() {
	// TODO Auto-generated constructor stub

}

Calibration::~Calibration() {
	// TODO Auto-generated destructor stub
}

Calibration::Calibration(std::string fileName) {
	fFileName = fileName;
	TFile *fp = new TFile(fileName.c_str(), "r");
	unsigned int numOfBars=vecOfBarsName.size();
	for(unsigned int barIndex = 0; barIndex < numOfBars; barIndex++){

		TF1 *delTShift_F = (TF1*)fp->Get(Form("fdelt_shift_Cs137_%s_0cm",vecOfBarsName[barIndex].c_str()));
		TF1 *temp_F = (TF1*)fp->Get(Form("fzparam_%s",vecOfBarsName[barIndex].c_str()));
		TF1 *paramertization_F = new TF1(Form("fzparam_%s",vecOfBarsName[barIndex].c_str()),temp_F->GetExpFormula(),-60.0,60.0);
		for(int ip=0; ip < temp_F->GetNpar(); ip++){
			paramertization_F->SetParameter(ip,temp_F->GetParameter(ip));
		}
	    fVecOfCalibrationData.push_back(new CalibrationData( delTShift_F, paramertization_F ));
	}
}

CalibrationData* Calibration::GetCalibrationDataOf(int barNo){
	return fVecOfCalibrationData[barNo];
}

void Calibration::SetEnergyCalibrationFactorForMuon(int barNo, double eCalibFactor){
	fVecOfCalibrationData[barNo]->fEnergyCalibFactorForMuon = eCalibFactor;
}
