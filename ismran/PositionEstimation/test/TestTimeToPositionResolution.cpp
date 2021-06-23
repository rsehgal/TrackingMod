/*
 * ReadCalibData.cpp
 *
 *  Created on: 29-Dec-2020
 *      Author: rsehgal
 */

#include <iostream>
#include "includes.hh"
#include <TFitResult.h>
#include <TH1F.h>
#include <TTree.h>
#include <TFile.h>
#include <vector>
#include <TApplication.h>
#include <TStyle.h>
#include "includes.h"
int main(int argc, char *argv[])
{
  gStyle->SetOptStat(0);
  TApplication *fApp = new TApplication("Test", NULL, NULL);

  new TCanvas();
  // TLegend *legend = new TLegend();
  std::string filename = argv[1];
  TFile *fp            = new TFile(filename.c_str(), "r");
  TFile *fpCalib       = new TFile("completeCalib2.root", "r");
  std::string barName  = vecOfBarsNamess[std::atoi(argv[2])];
  TF1 *delTFormula     = (TF1 *)fpCalib->Get(("fdelt_shift_Cs137_" + barName + "_0cm").c_str());
  TF1 *paramFormula    = (TF1 *)fpCalib->Get(("fzparam_" + barName).c_str());
  double offsetVal     = delTFormula->GetParameter(1);

  TTree *tr = (TTree *)fp->Get("ftree");
  int delt;

  tr->SetBranchAddress("fDelt", &delt);
  Long64_t nEntries = tr->GetEntries();
  Long64_t nb       = 0;
  const int offset  = 0;
  TH1F *delTHist    = new TH1F("delTHist", "delTHist", 100, -20, 20);
  delTHist->SetTitle("Histogram of #Delta T");
  TH1F *zhist = new TH1F("zEstimation", "zEstimation", 100, -50, 50);
  zhist->SetTitle("Histogram of estimated Z position");
  for (Long64_t iev = 0; iev < nEntries; iev++) {
    nb += tr->GetEntry(iev);
    float corrDelT = delt / 1000. - offsetVal;
    delTHist->Fill(corrDelT);
    zhist->Fill(paramFormula->Eval(corrDelT));
  }

  new TCanvas("Histogram of Z Estimation", "Histogram of Z Estimation");
  zhist->Draw();
  delTHist->SetLineColor(6);
  delTHist->Draw("same");

  fApp->Run();
}
