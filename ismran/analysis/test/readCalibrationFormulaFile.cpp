/*
 * readCalibrationFormulaFile.cpp
 *
 *  Created on: 01-Oct-2020
 *      Author: rsehgal
 */

#include "Calibration.h"
#include <vector>
#include <TApplication.h>

int main(){
	Calibration *cb = new Calibration("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib.root");
	std::vector<CalibrationData*> vecOfCalibData = cb->GetCalibrationDataVector();
	std::cout << "Size of Calibration data vector : " << vecOfCalibData.size() << std::endl;
	for(unsigned int i = 0 ; i< vecOfCalibData.size() ; i++){
		std::cout << "For Scintillator : " << i << " : Calculated Velocity : " << vecOfCalibData[i]->fVelocityInsideScintillator << std::endl;
	}
	return 0;
}



