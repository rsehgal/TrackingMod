/*
**	Filename : MuonTrackAnalysis_TestMuonParam.cpp
**	2021-08-13
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
#include <TH2F.h>
#include "ml.h"
int main(int argc, char *argv[])
{
  gStyle->SetOptStat(0);
  std::cout << "ARGC : " << argc << std::endl;
  // return 0;
  GenerateScintMatrixXYCenters();
  for (unsigned int i = 0; i < vecOfScintXYCenter.size(); i++) {
    if (!(i % 9)) std::cout << "=================================" << std::endl;
    vecOfScintXYCenter[i].Print();
  }
  // return 0;

  TApplication *fApp                   = new TApplication("Test", NULL, NULL);
  lite_interface::SingleMuonTrack *smt = new lite_interface::SingleMuonTrack;

  lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename               = argv[1];
  unsigned int numOfEv               = std::atoi(argv[2]);
  std::cout << "$$$$$$$$$$$$$$  Going to Generated Muon Classification data $$$$$$$$$$$$$$$$$$" << std::endl;

  // GenerateMuonTestData(filename,std::atoi(argv[2]),std::atoi(argv[3]),std::atoi(argv[4]),true,std::atoi(argv[5]));
  // GenerateParameterizationUsingMuons(filename,13);
  unsigned int pixelNo = std::atoi(argv[4]);
  std::vector<TH1F *> vecOfHist = EvaluateMuonParameterization(filename, pixelNo, std::atoi(argv[3]), numOfEv);

  TLegend *legT = new TLegend(0.2, 0.2, .8, .8);
  TLegend *legQ = new TLegend(0.2, 0.2, .8, .8);

  new TCanvas("ZUsing_Q", "");
  TF1 *formulaQ = new TF1("formulaQ","gaus",-20.,20.);
  legQ->AddEntry(vecOfHist[0],"Data","p");
  //vecOfHist[0]->Scale(1/vecOfHist[0]->Integral());
  vecOfHist[0]->Fit(formulaQ,"qn");
  legQ->AddEntry(formulaQ,"Fit","l");
  legQ->AddEntry((TObject*)0,Form("#mu = %0.2f #pm %0.2f",formulaQ->GetParameter(1),formulaQ->GetParError(1)),"");
  legQ->AddEntry((TObject*)0,Form("#sigma = %0.2f #pm %0.2f",formulaQ->GetParameter(2),formulaQ->GetParError(2)),"");
  vecOfHist[0]->Draw();
  legQ->Draw("same");
  formulaQ->Draw("same");

  new TCanvas("ReWeightedHist","ReWeightedHist");
  vecOfHist[5]->Draw();
  /*TH1F *hc = (TH1F *)vecOfHist[0]->Clone();
  hc->Add(-5);
  hc->SetLineColor(6);
  hc->Draw("same");*/

  new TCanvas("ZUsing_DelT", "");
  TF1 *formulaT = new TF1("formulaT","gaus",-20.,20.);
  //vecOfHist[1]->Scale(1/vecOfHist[1]->Integral());
  vecOfHist[1]->Fit(formulaT,"qn");
  legT->AddEntry(vecOfHist[1],"Data","p");
  legT->AddEntry(formulaT,"Fit","l");
  legT->AddEntry((TObject*)0,Form("#mu = %0.2f #pm %0.2f",formulaT->GetParameter(1),formulaT->GetParError(1)),"");
  legT->AddEntry((TObject*)0,Form("#sigma = %0.2f #pm %0.2f",formulaT->GetParameter(2),formulaT->GetParError(2)),"");
  vecOfHist[1]->Draw();
  legT->Draw("same");
  formulaT->Draw("same");

  new TCanvas("Q_Can", "Q_Can");
  //vecOfHist[2]->Scale(1/vecOfHist[2]->Integral());
  vecOfHist[2]->Draw();

#ifdef USE_FOR_SIMULATION
  new TCanvas("ZUsing_Mean", "ZUsing_Mean");
  vecOfHist[3]->Draw();
  new TCanvas("Diff_Hist_C", "Diff_Hist_C");
  vecOfHist[4]->Draw();

#endif

/*  new TCanvas("WeightedCount","Weighted Histogram");
  GetReweightedHist(vecOfHist[0])->Draw();*/

  fApp->Run();
  return 0;
}
