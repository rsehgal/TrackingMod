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
#include <TGraphErrors.h>
#include "HelperFunctions.h"
#include "PsBar.h"

namespace lite_interface {

struct CalibrationData {
  float fEnergyCalibrationFactor;
  float fDeltaTCorr;
  TF1 *fDelTCorr_F;
  TF1 *fParameterization_F;
  TF1 *fQParameterization_F;
  TF1 *fParameterization_F_Rev;
  TF1 *fEnergyCalibration_F;
  TGraph *fParameterization_G;
  TGraph *fGraphWithMuonPoint;

  double fVelocityInsideScintillator;
  double c;
  // double fEnergyCalibFactorForMuon;

  /*
   * Vector of TF1 corresponding to delT histogram at different locations
   */
  std::vector<TF1 *> fVectorOfDelT_F;

  CalibrationData() {}
  CalibrationData(TF1 *delTCorr, TF1 *parameterization, float energyCalibrationFactor = 1)
  {
    fDelTCorr_F              = delTCorr;
    fParameterization_F      = parameterization;
    fDeltaTCorr              = fDelTCorr_F->GetParameter(1);
    fEnergyCalibrationFactor = energyCalibrationFactor;
  }

  CalibrationData(TF1 *delTCorr_F, TF1 *parameterization_F, TF1 *energyCalib_F)
  {
    fDelTCorr_F         = delTCorr_F;
    fParameterization_F = parameterization_F;
    fDeltaTCorr         = fDelTCorr_F->GetParameter(1);
    // fEnergyCalibrationFactor = energyCalibrationFactor;
    fEnergyCalibration_F = energyCalib_F;
  }

  CalibrationData(TF1 *delTCorr_F, TF1 *parameterization_F, TF1 *parameterization_F_Rev, TF1 *energyCalib_F)
  {
    fDelTCorr_F             = delTCorr_F;
    fParameterization_F     = parameterization_F;
    fParameterization_F_Rev = parameterization_F_Rev;
    fDeltaTCorr             = fDelTCorr_F->GetParameter(1);
    // fEnergyCalibrationFactor = energyCalibrationFactor;
    fEnergyCalibration_F = energyCalib_F;
  }

  CalibrationData(TF1 *delTCorr_F, TF1 *parameterization_F, TF1 *parameterization_F_Rev, TF1 *q_parameterization_F,
                  TF1 *energyCalib_F)
  {
    fDelTCorr_F             = delTCorr_F;
    fParameterization_F     = parameterization_F;
    fQParameterization_F    = q_parameterization_F;
    fParameterization_F_Rev = parameterization_F_Rev;
    fDeltaTCorr             = fDelTCorr_F->GetParameter(1);
    // fEnergyCalibrationFactor = energyCalibrationFactor;
    fEnergyCalibration_F = energyCalib_F;
  }

  CalibrationData(TF1 *delTCorr, TF1 *parameterization, TGraph *parameterization_g, float energyCalibrationFactor = 1)
  {
    fDelTCorr_F              = delTCorr;
    fParameterization_F      = parameterization;
    fDeltaTCorr              = fDelTCorr_F->GetParameter(1);
    fEnergyCalibrationFactor = energyCalibrationFactor;

    // CalibrationData(delTCorr,parameterization,energyCalibrationFactor);
    fParameterization_G = parameterization_g;
    EstimateVelocity();
  }

  CalibrationData(TF1 *delTCorr, TF1 *parameterization, TGraph *parameterization_g, std::vector<TF1 *> vecOfDelTFormula,
                  float energyCalibrationFactor = 1)
  {
    fDelTCorr_F              = delTCorr;
    fParameterization_F      = parameterization;
    fDeltaTCorr              = fDelTCorr_F->GetParameter(1);
    fEnergyCalibrationFactor = energyCalibrationFactor;
    fVectorOfDelT_F          = vecOfDelTFormula;

    // CalibrationData(delTCorr,parameterization,energyCalibrationFactor);
    fParameterization_G = parameterization_g;
    EstimateVelocity();
  }

  CalibrationData(TF1 *delTCorr, TF1 *parameterization, TGraph *parameterization_g, std::vector<TF1 *> vecOfDelTFormula,
                  TGraphErrors *calibGraph, float energyCalibrationFactor = 1)
  {
    fDelTCorr_F              = delTCorr;
    fParameterization_F      = parameterization;
    fDeltaTCorr              = fDelTCorr_F->GetParameter(1);
    fEnergyCalibrationFactor = energyCalibrationFactor;
    fVectorOfDelT_F          = vecOfDelTFormula;

    std::vector<Double_t> x; //[4]; = calibGraph->GetX();
    std::vector<Double_t> y; // = calibGraph->GetY();
    unsigned int i = 0;
    for (i = 0; i < 3; i++) {
      x.push_back(calibGraph->GetX()[i]);
      y.push_back(calibGraph->GetY()[i]);
    }
    x.push_back(energyCalibrationFactor);
    y.push_back(muonEnergyPeak / 1000.);
    // TF1 *formula = new TF1("Formula",LinearFit,0,14000,2);
    TF1 *formula        = new TF1("Formula", Pol2, 0, 14000, 2);
    TGraph *gr          = new TGraph(4, &x[0], &y[0]);
    fGraphWithMuonPoint = new TGraph(4, &x[0], &y[0]);
    gr->Fit(formula, "qn");

    fEnergyCalibration_F = formula; // energyCalibFormula;

    // CalibrationData(delTCorr,parameterization,energyCalibrationFactor);
    fParameterization_G = gr; // parameterization_g;
    EstimateVelocity();
  }

  CalibrationData(TF1 *delTCorr, TF1 *parameterization, TGraph *parameterization_g, std::vector<TF1 *> vecOfDelTFormula,
                  TF1 *energyCalibFormula, float energyCalibrationFactor = 1)
  {
    fDelTCorr_F              = delTCorr;
    fParameterization_F      = parameterization;
    fDeltaTCorr              = fDelTCorr_F->GetParameter(1);
    fEnergyCalibrationFactor = energyCalibrationFactor;
    fVectorOfDelT_F          = vecOfDelTFormula;
    fEnergyCalibration_F     = energyCalibFormula;

    // CalibrationData(delTCorr,parameterization,energyCalibrationFactor);
    fParameterization_G = parameterization_g;
    EstimateVelocity();
  }

  void Print() { std::cout << "DelT Correction : " << fDeltaTCorr << std::endl; }

  void EstimateVelocity()
  {
    TF1 *formula = new TF1("LinearFit", "[0]+[1]*x", -10., 10.);
    fParameterization_G->Fit(formula, "qn");
    fVelocityInsideScintillator = formula->GetParameter(1);
    c                           = formula->GetParameter(0);
    // long double correctedDelT = scint->deltaTstampCorrected / 1000.;
    // double correctedDelT = scint->deltaTstampCorrected / 1000.;
    // float estZ = param->Eval(correctedDelT);
    // float estZ = 0.5*correctedDelT*m;
  }
};

class Calibration {

  std::string fFileName;
  std::vector<CalibrationData *> fVecOfCalibrationData;
  static Calibration *s_instance;

public:
  Calibration();
  static Calibration *instance();
  static Calibration *instance(std::string filename);
  Calibration(std::string fileName);
  virtual ~Calibration();

  /*
   * Function to return the std::vector of Calibration data of all
   * the Scintillator Bars
   */
  std::vector<CalibrationData *> GetCalibrationDataVector() const { return fVecOfCalibrationData; }

  unsigned int GetNumberOfBars() const { return fVecOfCalibrationData.size(); }

  /*
   * Function to return Calibration data of specific Scintillator bar
   */
  CalibrationData *GetCalibrationDataOf(int barNo);

  /*
   * Function to set the energyCalibrationFactor for Muon
   */
  void SetEnergyCalibrationFactorForMuon(int barNo, double eCalibFactor);
};

} // namespace lite_interface
#endif /* ISMRAN_ANALYSIS_INC_CALIBRATION_H_ */
