#include "Calibration.h"
#include <iostream>
#include "Analyzer.h"
int main(){

	std::cout << "Intializing Calibration data........" << std::endl;
	Calibration cb("/media/rsehgal/CMSSW/IsmranDataCalibrationFiles/FinalCorrectionFactors_70bars.root");
	std::cout << "Calibration data loaded....." << std::endl;

	//Testing
	cb.GetCalibrationDataOf(0)->Print();
	cb.GetCalibrationDataOf(1)->Print();
	cb.GetCalibrationDataOf(5)->Print();


	Analyzer az("/media/rsehgal/CMSSW/IsmranDataCalibrationFiles/FinalCorrectionFactors_70bars.root",
				"/home/rsehgal/BackBoneSoftwares/ismranData/Board13_54Det_noHVDet27_30July2020.root");



}
