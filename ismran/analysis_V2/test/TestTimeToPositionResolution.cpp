/*
 * ReadCalibData.cpp
 *
 *  Created on: 29-Dec-2020
 *      Author: rsehgal
 */
#include <iomanip>
#include <sstream>
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
  std::string barName  = vecOfBarsNamess[0];
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
  TH1F *zhist = new TH1F("zEstimation", "zEstimation", 50, -50, 50);
  zhist->SetTitle("Histogram of estimated Z position");
  for (Long64_t iev = 0; iev < nEntries; iev++) {
    nb += tr->GetEntry(iev);
    float corrDelT = delt / 1000. - offsetVal;
    delTHist->Fill(corrDelT);
    zhist->Fill(paramFormula->Eval(corrDelT));
  }

  TLegend *legendTZ = new TLegend(0.2, 0.2, .8, .8);
  TF1 *formulaQ     = new TF1("FormulaQ", "gaus", -50., 50.);
  formulaQ->SetParameters(zhist->GetMaximum(), zhist->GetMean(), zhist->GetStdDev());
  zhist->Scale(1 / zhist->Integral());
  zhist->Fit(formulaQ, "qn");

  std::setprecision(2);

//  double mean           = roundoff(formulaQ->GetParameter(1),2);
  //double sigma          = roundoff(formulaQ->GetParameter(2),2);
double mean           = formulaQ->GetParameter(1);
  double sigma          = formulaQ->GetParameter(2);


/*std::ostringstream streamObj3;
streamObj3 << std::fixed;
streamObj3 << std::setprecision(2);
streamObj3 << mean;
std::string meanString= "#mu = " + streamObj3.str();
streamObj3 << sigma;
std::string sigmaString= "#sigma = " + streamObj3.str();
*/


  std::string sigmaString = "#sigma = " + std::to_string(sigma);//+"\\ #mu = "+std::to_string(mean);
  std::string meanString = "#mu = " + std::to_string(mean);//+"\\ #mu = "+std::to_string(mean);

  new TCanvas("Histogram of Z Estimation", "Histogram of Z Estimation");
  zhist->Draw();
  legendTZ->AddEntry(zhist, "Data Point", "p");
  legendTZ->AddEntry(formulaQ, "Fit", "l");
  legendTZ->AddEntry((TObject*)0, sigmaString.c_str(), "");
  legendTZ->AddEntry((TObject*)0, meanString.c_str(), "");
  
  delTHist->SetLineColor(6);
  // delTHist->Draw("same");

  formulaQ->Draw("same");
  legendTZ->Draw("same");

  fApp->Run();
}
