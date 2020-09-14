#include "Calibration.h"
#include <iostream>
#include "Analyzer.h"
#include "TApplication.h"
int main(){

	TApplication *fApp = new TApplication("Test", NULL, NULL);

	std::cout << "Intializing Calibration data........" << std::endl;
	Calibration cb("/media/rsehgal/CMSSW/IsmranDataCalibrationFiles/FinalCorrectionFactors_70bars.root");
	std::cout << "Calibration data loaded....." << std::endl;

	//Testing
	cb.GetCalibrationDataOf(0)->Print();
	cb.GetCalibrationDataOf(1)->Print();
	cb.GetCalibrationDataOf(5)->Print();


	//Analyzer az("../ismran/FinalCorrectionFactors_70bars.root",
		//		"../ismran/Board13_72Det_noHVDet27_30July2020.root");
	Analyzer az("/media/rsehgal/CMSSW/IsmranDataCalibrationFiles/FinalCorrectionFactors_70bars.root",
					"/home/rsehgal/BackBoneSoftwares/ismranData/Board13_54Det_noHVDet27_30July2020.root");
	//az.PerformDelTCorrection();
	//az.PlotHistOf(13);
	//az.EstimateZPositionOn(20);

	fApp->Run();



}
