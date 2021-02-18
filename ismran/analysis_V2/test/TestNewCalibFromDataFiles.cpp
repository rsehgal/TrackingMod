/*
 * TestNewCalibFromDataFiles.cpp
 *
 *  Created on: 18-Feb-2021
 *      Author: rsehgal
 */

#include "ReadCalibrationDataFile.h"
#include <TApplication.h>
#include "HardwareNomenclature.h"

int main(){
	//TApplication *fApp = new TApplication("Test", NULL, NULL);
	ReadCalibrationDataFile rc("Cs137_PS1_S2AB1017_Couples_N1164V_Th10_F1160V_Th12_+40cm.root");
	//rc.GetFileNameStruct("Source_BarName_Mode_NVoltage_NTh_FVoltage_FTh_SourcePos");
	//rc.GetFileNameStruct("Cs137_PS12_2341_Couples_N800V_Th10_F987V_Th11_-50cm.root");

	//rc.ProcessFile("Na22_PS1_S2AB1017_Couples_N1164V_Th10_F1160V_Th12_+40cm.root");
	//rc.DoParameterization_V2("/home/rsehgal/TrackingMod/ismran/analysis_V2/build");

	//rc.DoParameterization_V2("/home/rsehgal/PS1_S2AB1017/");

	std::string dir="/home/rsehgal/ismranData/Hall9/Characterization/";
	//for(unsigned int i = 0 ; i < sequentialBarNames.size() ; i++)
	{
		rc.DoParameterization_V2((dir+"PS1_S2AB1017/"));
		//rc.DoParameterization_V2((dir+sequentialBarNames[i]+"/"));
	}



	//fApp->Run();

	return 0;
}
