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
#include "HardwareNomenclature.h"
#include <TH1D.h>

namespace lite_interface{

Calibration *Calibration::s_instance = 0;

Calibration::Calibration() {
	// TODO Auto-generated constructor stub

}

Calibration::~Calibration() {
	// TODO Auto-generated destructor stub
}

Calibration* Calibration::instance(std::string filename) {
        if (!s_instance)
          s_instance = new Calibration(filename);
        return s_instance;
}

Calibration* Calibration::instance() {
        if (s_instance)
          return s_instance;
        else{
        	std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
        	std::cout<<"@@ Instance of Calibration class does not exist in Memory, please create it first....@@" << std::endl;
        	std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
        }
}
#if(0)
Calibration::Calibration(std::string fileName) {
	fFileName = fileName;
	TFile *fp = new TFile(fileName.c_str(), "r");
	unsigned int numOfBars=vecOfBarsNamess.size();//vecOfBarsName.size();
	for(unsigned int barIndex = 0; barIndex < numOfBars; barIndex++){

		TF1 *delTShift_F = (TF1*)fp->Get(Form("fdelt_shift_Cs137_%s_0cm",vecOfBarsNamess[barIndex].c_str()));
		TF1 *temp_F = (TF1*)fp->Get(Form("fzparam_%s",vecOfBarsNamess[barIndex].c_str()));
		TF1 *paramertization_F = new TF1(Form("fzparam_%s",vecOfBarsNamess[barIndex].c_str()),temp_F->GetExpFormula(),-60.0,60.0);
		int barPhyNum = std::stoi(vecOfBarsNamess[barIndex].substr(2,2));
		TGraph *gr;
		if(barPhyNum <= 70)
			gr = (TGraphErrors*)fp->Get(Form("grDeltMZpos_Cs137_%s",vecOfBarsNamess[barIndex].c_str()));
		else
			gr = (TGraph*)fp->Get(Form("grDelTCorrected-%s",vecOfBarsNamess[barIndex].c_str()));

		for(int ip=0; ip < temp_F->GetNpar(); ip++){
			paramertization_F->SetParameter(ip,temp_F->GetParameter(ip));
		}

		std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
		std::cout << "Processing Bar : " << vecOfBarsNamess[barIndex] << std::endl;
		std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;

		TH1D *histEner = ((TH1D*)fp->Get((vecOfBarsNamess[barIndex]+"-QMean").c_str()));
		int binmax = histEner->GetMaximumBin();
		double bin = histEner->GetXaxis()->GetBinCenter(binmax);
		int revBin = histEner->GetXaxis()->FindBin(bin);
		double energyCalibrationFactor = bin;// (1.0*muonEnergyPeak) - bin;

		//double energyCalibrationFactor = 1.;
		//TF1 *enerCalibFormula = (TF1*)fp->Get(Form("fecalib_%s",vecOfBarsNamess[barIndex].c_str()));

		//Taking energy Calibration formula from cosmic data
		TF1 *enerCalibFormula = (TF1*)fp->Get(Form("%s_Energy_F",vecOfBarsNamess[barIndex].c_str()));

		TGraphErrors *enerCalibGraph = (TGraphErrors*)fp->Get(Form("grEneCalib_%s",vecOfBarsNamess[barIndex].c_str()));

		//int barPhyNum = std::stoi(vecOfBarsName[barIndex].substr(2,2));
		/*if(barPhyNum <= 70)
			//fVecOfCalibrationData.push_back(new CalibrationData( delTShift_F, paramertization_F , energyCalibrationFactor));
			fVecOfCalibrationData.push_back(new CalibrationData( delTShift_F, temp_F , energyCalibrationFactor));
		else
			fVecOfCalibrationData.push_back(new CalibrationData( delTShift_F, temp_F ,energyCalibrationFactor));*/
		//fVecOfCalibrationData.push_back(new CalibrationData( delTShift_F, temp_F ,energyCalibrationFactor));


		//Better idea would be to calculate the offset here itself and send as an parameter.
		//fVecOfCalibrationData.push_back(new CalibrationData( delTShift_F, temp_F ,gr,energyCalibrationFactor));

		//Filling delT formula at different source position
		std::vector<TF1*> vecOfDelTFormula;
		for(unsigned short int sourceIndex = 0 ;  sourceIndex < vecOfSourcePositions.size() ; sourceIndex++){
			vecOfDelTFormula.push_back((TF1*)fp->Get(Form("fdelt_shift_Cs137_%s_%dcm",vecOfBarsNamess[barIndex].c_str(),vecOfSourcePositions[sourceIndex])) );
		}
		//fVecOfCalibrationData.push_back(new CalibrationData( delTShift_F, temp_F ,gr,vecOfDelTFormula,energyCalibrationFactor));
		/*
		 * Also including energy calibration formula
		 */
		fVecOfCalibrationData.push_back(new CalibrationData( delTShift_F, temp_F ,gr,vecOfDelTFormula,enerCalibFormula,energyCalibrationFactor));
#if(0)
		if(barPhyNum <= 70)
			fVecOfCalibrationData.push_back(new CalibrationData( delTShift_F, temp_F ,gr,vecOfDelTFormula,enerCalibGraph,energyCalibrationFactor));
		else{
			//fVecOfCalibrationData.push_back(new CalibrationData( delTShift_F, temp_F ,gr,vecOfDelTFormula,enerCalibFormula,energyCalibrationFactor));
			/*
			 * Currently we don't have energy calibration formula for the bar > 70,
			 * and hence using the calibration formula of first bar.
			 *
			 * This is done to avoid any break while running the code with full matrix.
			 * but later this need to be fixed by generating the formula from the
			 * calibration data.
			 *
			 */
			enerCalibGraph = (TGraphErrors*)fp->Get(Form("grEneCalib_%s",vecOfBarsNamess[0].c_str()));
			fVecOfCalibrationData.push_back(new CalibrationData( delTShift_F, temp_F ,gr,vecOfDelTFormula,enerCalibGraph,energyCalibrationFactor));
		}
#endif

		/*double offset = delTShift_F->GetParameter(1);
		fVecOfCalibrationData.push_back(new CalibrationData( delTShift_F,offset, temp_F ,gr,energyCalibrationFactor));*/

	}
}
#endif

Calibration::Calibration(std::string fileName) {
	fFileName = fileName;
	TFile *fp = new TFile(fileName.c_str(), "r");
	unsigned int numOfBars=vecOfBarsNamess.size();//vecOfBarsName.size();
	for(unsigned int barIndex = 0; barIndex < numOfBars; barIndex++){

		TF1 *delTShift_F = (TF1*)fp->Get(Form("fdelt_shift_Cs137_%s_0cm",vecOfBarsNamess[barIndex].c_str()));
		TF1 *paramertization_F = (TF1*)fp->Get(Form("fzparam_%s",vecOfBarsNamess[barIndex].c_str()));
		TF1 *q_paramertization_F = (TF1*)fp->Get(Form("fQparam_%s",vecOfBarsNamess[barIndex].c_str()));
		TF1 *paramertization_F_Rev = (TF1*)fp->Get(Form("fzparam_Rev_%s",vecOfBarsNamess[barIndex].c_str()));
		//TF1 *enerCalibFormula = (TF1*)fp->Get(Form("%s_Energy_F",vecOfBarsNamess[barIndex].c_str()));
		TF1 *enerCalibFormula;
		/*if(barIndex == 34)
			enerCalibFormula = (TF1*)fp->Get("PS35_S1AA6520_Energy_F");
		else*/
			enerCalibFormula = (TF1*)fp->Get(Form("%s_Energy_F",vecOfBarsNamess[barIndex].c_str()));

		std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
		std::cout << "Processing Bar : " << vecOfBarsNamess[barIndex] << std::endl;
		std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;

		//if(barIndex!=34)

			//std::cout << "Parameters of " << vecOfBarsNamess[barIndex] << " : " <<  enerCalibFormula->GetParameter(0) << " , " << enerCalibFormula->GetParameter(1) << std::endl;
			//std::cout << "DelTparam : " << delTShift_F->GetParameter(1) << std::endl;

		//fVecOfCalibrationData.push_back(new CalibrationData( delTShift_F, paramertization_F ,enerCalibFormula));
		//fVecOfCalibrationData.push_back(new CalibrationData( delTShift_F, paramertization_F , paramertization_F_Rev, enerCalibFormula));
		fVecOfCalibrationData.push_back(new CalibrationData( delTShift_F, paramertization_F , paramertization_F_Rev, q_paramertization_F,enerCalibFormula));

	}
}


CalibrationData* Calibration::GetCalibrationDataOf(int barNo){
	//std::cout <<" @@@@@@2 BAR NO : " << barNo << std::endl;
	return fVecOfCalibrationData[barNo];
}

void Calibration::SetEnergyCalibrationFactorForMuon(int barNo, double eCalibFactor){
	//fVecOfCalibrationData[barNo]->fEnergyCalibFactorForMuon = eCalibFactor;
	fVecOfCalibrationData[barNo]->fEnergyCalibrationFactor = eCalibFactor;

}

} /* end of lite_interface */
