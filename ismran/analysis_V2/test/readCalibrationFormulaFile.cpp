/*
 * readCalibrationFormulaFile.cpp
 *
 *  Created on: 01-Oct-2020
 *      Author: rsehgal
 */

#include "Calibration.h"
#include <vector>
#include <TApplication.h>
#include <TCanvas.h>
#include <TH1D.h>

int main(){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	lite_interface::Calibration *cb = new lite_interface::Calibration("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib.root");
	std::vector<lite_interface::CalibrationData*> vecOfCalibData = cb->GetCalibrationDataVector();
	std::cout << "Size of Calibration data vector : " << vecOfCalibData.size() << std::endl;
	for(unsigned int i = 0 ; i< vecOfCalibData.size() ; i++){
		std::cout << "For Scintillator : " << i << " : Calculated Velocity : " << vecOfCalibData[i]->fVelocityInsideScintillator << std::endl;
	}

	/*
	 * Lets try to see the delT function of first Bar
	 */
	TCanvas *can = new TCanvas();
	can->Divide(3,3);

	short int barIndex = 72;
	for(unsigned short int i = 0 ; i < (vecOfCalibData[barIndex]->fVectorOfDelT_F).size() ; i++){
		can->cd(i+1);
		//(vecOfCalibData[barIndex]->fVectorOfDelT_F)[i]->Scale(1/(vecOfCalibData[barIndex]->fVectorOfDelT_F)[i]->Integral)->Draw();
		(vecOfCalibData[barIndex]->fVectorOfDelT_F)[i]->Draw();
		/*
		 * Lets try to sample some random number from the formula and
		 * display its histogram
		 */
		unsigned int numOfSamples = 1400000;
		char *name = Form("HistSampleFromData_%d",i);
		TH1D *histo = new TH1D(name,name,100,-50.,50.);
		for(unsigned int sampleNo = 0 ; sampleNo < numOfSamples ; sampleNo++){
			histo->Fill((vecOfCalibData[barIndex]->fVectorOfDelT_F)[i]->GetRandom());
		}
		//histo->Scale(1/histo->Integral())->Draw("same");
		histo->Draw("same");

	}


	fApp->Run();
	return 0;
}



