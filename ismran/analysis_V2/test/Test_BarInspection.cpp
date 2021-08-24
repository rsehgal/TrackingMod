/*
**	Filename : Test_BarInspection.cpp
**	2021-08-17
**	username : rsehgal
*/
#include <iostream>
#include <vector>
#include "PairFinder.h"
#include "Analyzer.h"
#include "Calibration.h"
#include <TApplication.h>
#include <TCanvas.h>
#include "Plotter.h"
#include "includes.hh"
#include "HardwareNomenclature.h"
#include <TSpectrum.h>
#include "Histograms.h"
#include <TLatex.h>
#include "langaus.h"
#include "colors.h"
int main(int argc, char *argv[])
{
/*  TLatex latex;
  latex.SetTextSize(0.025);
  latex.SetTextAlign(12);
  latex.DrawLatex(10,10000,"Natural Background");
  latex.DrawLatex(10,5000,"Cosmic Muons");
  latex.DrawLatex(10,12000,"#^{40}K");
  latex.DrawLatex(10,14000,"#^{208}Tl");
*/
#ifdef USE_FOR_SIMULATION
  IsSimulation     = true;
  std::string name = "SimData";
#else
  std::string name = "ExpData";
#endif
  TFile *fp = new TFile((name + ".root").c_str(), "RECREATE");
  //  gStyle->SetOptStat(0);
  TApplication *fApp = new TApplication("Test", NULL, NULL);

  lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");

  lite_interface::Analyzer analyzerObj(argv[1], false);
  std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = analyzerObj.GetVectorOfScintillators();
  TH1F *histDelT                                                = new TH1F(name.c_str(), name.c_str(), 100, -10., 10);
  TH1F *qmeanCorr = new TH1F((name + "_Ener").c_str(), (name + "_Ener").c_str(), 600, 0., 60);
  for (unsigned int i = 0; i < scintBarVec.size(); i++) {
    if (scintBarVec[i]->GetBarIndex() == 0) {
      double delT = scintBarVec[i]->GetDelTCorrected();
      // std::cout << CYAN << "DelT : " << delT << std::endl;
      double energyVal = scintBarVec[i]->GetQMeanCorrected();
      if (energyVal > qmeanCorrThreshold)
      { // && energyVal < 30. ){ //qmeanCorrThresholdUpper) {
        //        if (energyVal < qmeanCorrThreshold)
        {
         // std::cout << RED << "Somehow detected less : " << energyVal << std::endl;
        }
        histDelT->Fill(delT / 1000.);
        qmeanCorr->Fill(energyVal);
      }
    }
  }
  histDelT->Draw();
  new TCanvas("Energy", "Energy");
  qmeanCorr->Draw();
  fp->cd();
  //NormalizeHistUsingMaxCount(qmeanCorr);
  histDelT->Write();
  qmeanCorr->Write();
  fp->Close();
  std::cout << BLUE << "File written and closed........" << RESET << std::endl;
  fApp->Run();
}
