/*
**	Filename : TestQMean_CalibAndUnCalib.cpp
**	2021-08-01
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
  gStyle->SetOptStat(0);
  TApplication *fApp = new TApplication("Test", NULL, NULL);

  lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");

  lite_interface::Analyzer analyzerObj(argv[1], false);
  std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = analyzerObj.GetVectorOfScintillators();

  unsigned int index = std::atoi(argv[2]);
  unsigned int runNo = std::atoi(argv[3]);
  TFile *fp=new TFile(("Qmean"+std::to_string(runNo)+".root").c_str(),"RECREATE");
  std::vector<TH1F *> vecOfHists;
  vecOfHists.push_back(new TH1F((vecOfBarsNamess[index] + "_QNear").c_str(), "", 1000, 0, 32000));
  vecOfHists.push_back(new TH1F((vecOfBarsNamess[index] + "_QFar").c_str(), "", 1000, 0, 32000));
  vecOfHists.push_back(new TH1F((vecOfBarsNamess[index] + "_QMean").c_str(), "", 1000, 0, 32000));
  vecOfHists.push_back(new TH1F((vecOfBarsNamess[index] + "_Calib").c_str(), "", 1000, 0, 50));

  std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
  for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
    if ((*itr)->GetBarIndex() == index) {
      vecOfHists[0]->Fill((*itr)->GetQLongNear());
      vecOfHists[1]->Fill((*itr)->GetQLongFar());
      vecOfHists[2]->Fill((*itr)->GetQLongMean());
      vecOfHists[3]->Fill((*itr)->GetQMeanCorrected());
    }
  }

  std::cout << "Required histograms filledd...." << std::endl;

TCanvas *canNonCalib= (new TCanvas("QMean_NonCalibrated", "QMean_NonCalibrated"));
/*latex.SetTextAlign(12);
  latex.DrawLatex(10,10000,"Natural Background");
  latex.DrawLatex(10,5000,"Cosmic Muons");
  latex.DrawLatex(10,12000,"#^{40}K");
  latex.DrawLatex(10,14000,"#^{208}Tl");*/
 vecOfHists[0]->SetLineColor(2);
 vecOfHists[1]->SetLineColor(4);
 vecOfHists[2]->SetLineColor(1);
 vecOfHists[2]->SetLineWidth(3);
 vecOfHists[3]->SetLineColor(3);
 vecOfHists[3]->SetLineWidth(2);
// vecOfHists[0]->Draw("");
// vecOfHists[1]->Draw("same");
 vecOfHists[2]->Draw("same");
TCanvas *canCalib =  (new TCanvas("QMean_Calibrated", "QMean_Calibrated"));
/*latex.SetTextAlign(12);
  latex.DrawLatex(10,10000,"Natural Background");
  latex.DrawLatex(10,5000,"Cosmic Muons");
  latex.DrawLatex(10,12000,"#^{40}K");
  latex.DrawLatex(10,14000,"#^{208}Tl");*/
  vecOfHists[3]->Draw();

  fp->cd();
/*  vecOfHists[0]->Scale(1/vecOfHists[0]->Integral());
  vecOfHists[1]->Scale(1/vecOfHists[1]->Integral());
  vecOfHists[2]->Scale(1/vecOfHists[2]->Integral());
  vecOfHists[3]->Scale(1/vecOfHists[3]->Integral());
*/
  vecOfHists[0]->Write();
  vecOfHists[1]->Write();
  vecOfHists[2]->Write();
  vecOfHists[3]->Write();
  canNonCalib->Write();
  canCalib->Write();

  fp->Close();

  fApp->Run();
}
