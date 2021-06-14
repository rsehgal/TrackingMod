/*
-------------
DOCBEGIN
-------------

** This test program may be used to generate the reverse parameterization from
** calibration data.
** ie. It will calculate the parameterization function that actually maps the
** Z position to delT on scintillator.
**
** Will be useful to inject the scintillator behaviour in the simulations.

-------------
DOCEND
-------------
*/

#include <iostream>
#include <string>
#include "HardwareNomenclature.h"
#include "Calibration.h"
#include <TGraph.h>
#include <TCanvas.h>
#include <TApplication.h>
#include "HelperFunctions.h"
int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
  TFile *fp                          = new TFile("completeCalib2.root", "r");
  TFile *paramFile                   = new TFile("reverParam.root", "RECREATE");
  std::vector<float> zPosVec;
  std::vector<float> corrDelTMeanVec;

  for (unsigned int barIndex = 0; barIndex < vecOfBarsNamess.size(); barIndex++) {
    zPosVec.clear();
    corrDelTMeanVec.clear();
    std::string barName = vecOfBarsNamess[barIndex];

    std::cout << "%%%%%%%%%%%%%%%  Processing BAR : " << barName << "  %%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
    lite_interface::CalibrationData *calibData = calib->GetCalibrationDataOf(barIndex);
    double barCorrOffset                       = calibData->fDeltaTCorr;
    for (unsigned int sourcePosIndex = 0; sourcePosIndex < vecOfSourcePositions.size(); sourcePosIndex++) {
      std::string brchName =
          "fdelt_shift_Cs137_" + barName + "_" + std::to_string(vecOfSourcePositions[sourcePosIndex]) + "cm";
      if (barIndex == 89 && (sourcePosIndex == 4 || sourcePosIndex == 10)) {
      } else {
        zPosVec.push_back(vecOfSourcePositions[sourcePosIndex]);
        TF1 *delTFormula = (TF1 *)fp->Get(brchName.c_str());
        corrDelTMeanVec.push_back(delTFormula->GetParameter(1) - barCorrOffset);
      }
    }
    for (unsigned int index = 0; index < zPosVec.size(); index++) {
      std::cout << zPosVec[index] << " , " << corrDelTMeanVec[index] << std::endl;
    }

    // if (barIndex == 88)
    {
      TGraph *gr = new TGraph(zPosVec.size(), &zPosVec[0], &corrDelTMeanVec[0]);
      new TCanvas();
      gr->Draw("ap");

      TF1 *formula = new TF1(Form("fzparam_Rev_%s", barName.c_str()), Pol3, -60, 60, 4);

      gr->Fit(formula, "rq");
      paramFile->cd();
      formula->Write();
    }
  }
  paramFile->Close();
  fApp->Run();
  return 0;
}
