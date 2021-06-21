/*
 * ReadCalibData.cpp
 *
 *  Created on: 29-Dec-2020
 *      Author: rsehgal
 */

#include <iostream>
//#include "includes.hh"
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
  double offsetVal     = delTFormula->GetParameter(1);
  
  TF1 *paramFormula    = (TF1 *)fpCalib->Get(("fzparam_Pos_" + barName).c_str());

  TTree *tr = (TTree *)fp->Get("ftree");

  int delt;
  // ULong64_t delt;

  tr->SetBranchAddress("fDelt", &delt);
  Long64_t nEntries = tr->GetEntries();
  Long64_t nb       = 0;
  const int offset  = 0;
  TH1F *delTHist    = new TH1F("delTHist", "delTHist", 100, -20, 20);
  delTHist->SetTitle("Histogram of #Delta T");
  TH1F *zhist = new TH1F("zEstimation", "zEstimation", 100, -50, 50);
  // TH1F *zhistNew = new TH1F("zEstimationNew","zEstimationNew",100,-50,50);
  zhist->SetTitle("Histogram of estimated Z position");
  std::ofstream outfile("deltval.txt");
  for (Long64_t iev = 0; iev < nEntries; iev++) {
    nb += tr->GetEntry(iev);
    float corrDelT = delt / 1000. - offsetVal;
    double zval    = paramFormula->Eval(corrDelT);
    outfile << (corrDelT * 1000.) << "," << zval << std::endl;
    delTHist->Fill(corrDelT);
    zhist->Fill(paramFormula->Eval(corrDelT));
    // zhistNew->Fill(paramNewFormula->Eval(corrDelT));
  }
  /*legend->AddEntry(delTHist);
  delTHist->Scale(1/delTHist->Integral());
  if(i==4)
    delTHist->SetLineColor(i+2);
  else
    delTHist->SetLineColor(i+1);*/
  delTHist->Draw("same");

  new TCanvas("Histogram of Z Estimation", "Histogram of Z Estimation");
  zhist->Draw();

  // new TCanvas("Histogram of Z Estimation New","Histogram of Z Estimation New");
  // zhistNew->SetLineColor(6);
  // zhistNew->Draw("same");

  new TCanvas("parameterization", "parameterization");
  paramFormula->Draw();
  // paramNewFormula->SetLineColor(6);
  // paramNewFormula->Draw("same");
  // legend->Draw("same");
  outfile.close();
  fApp->Run();
}
