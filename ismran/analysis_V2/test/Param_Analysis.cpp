/*
---------
DOCBEGIN
---------
** Macro to plot the 2D histogram between delT and Z Predict and Z actual
** Need a text file as an comnad line argument, having 3 columns,
** 1)delT 2) Z_predicted 3) Z_actual
**
** usage : root -l
**         .x hist2.C("filename")
**
** output : 2D histograms, as described above
--------
DOCEND
--------
*/

#include <fstream>
#include <iostream>
#include "includes.h"
int main(int argc, char *argv[])
{
  TApplication *fApp                 = new TApplication("Test", NULL, NULL);
  std::string filename               = argv[1];
  TH2F *h1                           = new TH2F("h1", "delt Vs Zpred", 1000, -25., 50., 1000, -60., 60.);
  TH2F *h2                           = new TH2F("h2", "delt Vs Z Rev", 1000, -25., 25., 1000, -60., 60.);
  lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
  TF1 *formRev                       = calib->GetCalibrationDataOf(40)->fParameterization_F_Rev;
  std::ifstream inp;
  double x, y, yact;
  inp.open(filename);
  //   for (int i=1; i<=1000; i++)
  while (!inp.eof()) {
    inp >> x >> y; // >> yact;
    h1->Fill(x / 1000., y);
    h2->Fill(formRev->Eval(y), y);
  }

  TF1 *formula = new TF1(Form("fzparam_Rev_41%s", vecOfBarsNamess[40].c_str()), Pol3, -60, 60, 4);

  h2->Fit(formula, "r");

  TFile *fp = new TFile("Bar41.root", "RECREATE");
  fp->cd();
  new TCanvas("DelTVsZPred", "DelTVsZPred ");
  h1->Draw();
  new TCanvas("DelTVsZ Rev", "DelTVsZ Rev");
  h2->Draw("colz");
  h2->Write();
  formula->Write();
  fp->Close();
  fApp->Run();
}
