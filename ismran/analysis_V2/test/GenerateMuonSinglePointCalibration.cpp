/*
 * GenerateMuonSinglePointCalibration.cpp
 *
 *  Created on: 17-Sep-2020
 *      Author: rsehgal
 */

#include "SinglePointEnergyCalibrationForMuon.h"
#include "Calibration.h"

int main(){
	//SinglePointEnergyCalibrationForMuon sp("/home/rsehgal/BackBoneSoftwares/ismranData/ISMRAN_81bars_Th10All_CosmicRun_15hrs42mins_26Aug2020_2.root");
	lite_interface::Calibration *calib = lite_interface::Calibration::instance("CAL.root");
	SinglePointEnergyCalibrationForMuon sp("/home/rsehgal/BackBoneSoftwares/ismranData/ISMRAN_90bars_GW48ns_CFD2_StaggeredGeom_CosmicRun_09hrs37mins25secs_22Dec2020_Run224.root");

	return 0;
}



