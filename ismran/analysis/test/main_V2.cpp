/*
 * main_V2.cpp
 *
 *  Created on: 03-Sep-2020
 *      Author: rsehgal
 */


#include "Calibration.h"
#include <iostream>
#include "Analyzer_V2.h"
#include "TApplication.h"
#include "TreeEntry.h"
int main(){

	TApplication *fApp = new TApplication("Test", NULL, NULL);

	/*std::cout << "Intializing Calibration data........" << std::endl;
	Calibration cb("/media/rsehgal/CMSSW/IsmranDataCalibrationFiles/FinalCorrectionFactors_70bars.root");
	std::cout << "Calibration data loaded....." << std::endl;

	//Testing
	cb.GetCalibrationDataOf(0)->Print();
	cb.GetCalibrationDataOf(1)->Print();
	cb.GetCalibrationDataOf(5)->Print();
*/

	//Analyzer az("../ismran/FinalCorrectionFactors_70bars.root",
		//		"../ismran/Board13_72Det_noHVDet27_30July2020.root");

	/*Analyzer az("/media/rsehgal/CMSSW/IsmranDataCalibrationFiles/FinalCorrectionFactors_70bars.root",
					"/home/rsehgal/BackBoneSoftwares/ismranData/ISMRAN_81bars_Th10All_CosmicRun_09hrs42mins_26Aug2020_0.root");
	*/



	//az.PerformDelTCorrection();
	//az.PlotHistOf(13);
	//az.EstimateZPositionOn(20);

	//if don't need calibration
	//Analyzer_V2 av2("/home/rsehgal/BackBoneSoftwares/ismranData/ISMRAN_81bars_Th10All_CosmicRun_15hrs42mins_26Aug2020_2.root");

	//If calibration required
	//Calibration *cb = new Calibration("/media/rsehgal/CMSSW/IsmranDataCalibrationFiles/FinalCorrectionFactors_70bars.root");
	Calibration *cb = new Calibration("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib.root");
	Analyzer_V2 av2("/home/rsehgal/BackBoneSoftwares/ismranData/ISMRAN_81bars_Th10All_CosmicRun_15hrs42mins_26Aug2020_2.root",cb);
	//Analyzer_V2 av2("/home/rsehgal/BackBoneSoftwares/ismranData/completeData_02Sep2020.root",cb);


	//Analyzer_V2 av2("/home/rsehgal/BackBoneSoftwares/ismranData/Board13_54Det_noHVDet27_30July2020.root");
	//PrintEntryVector_V2(av2.fVecOfTreeEntry,100);

	fApp->Run();



}


