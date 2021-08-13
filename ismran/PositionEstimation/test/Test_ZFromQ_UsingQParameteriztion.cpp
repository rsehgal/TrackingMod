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
#include "CharacterizationFileReader.h"
int main(int argc, char *argv[])
{
  gStyle->SetOptStat(0);
  TApplication *fApp = new TApplication("Test", NULL, NULL);

  new TCanvas();
  // TLegend *legend = new TLegend();
  std::string filename = argv[1];
  CharacterizationFileReader cfReader(filename);
  FillAttenCoeffVec();
  std::vector<Event *> eventsVec = cfReader.GetAllEvents(0);//std::atoi(argv[2]));
  std::string barName            = cfReader.GetBarName();

  //using Source
  //TFile *fpCalib    = new TFile("completeCalib2.root", "r");
  //TF1 *paramFormula = (TF1 *)fpCalib->Get(("fQparam_" + barName).c_str());
  
  TFile *fpCalib    = new TFile("muonParam.root", "r");
  TF1 *paramFormula = (TF1 *)fpCalib->Get(("fzparam_Muon_Q_" + barName).c_str());

  TH1F *qHist = new TH1F("QHist", "QHist", 100, -5., 5.);
  qHist->SetTitle("Histogram of Q");
  TH1F *zHist = new TH1F("zEstimation", "zEstimation", 50, -50., 50.);
  zHist->SetTitle("Histogram of estimated Z position");

  for (unsigned int i = 0; i < eventsVec.size(); i++) {
    double logQNearbyQFar = eventsVec[i]->GetLogQNearByQFar();
    qHist->Fill(logQNearbyQFar);
    zHist->Fill(paramFormula->Eval(logQNearbyQFar));
  }

  TLegend *legendTZ = new TLegend(0.2, 0.2, .8, .8);
  TF1 *formulaQ     = new TF1("FormulaQ", "gaus", -50., 50.);
  formulaQ->SetParameters(zHist->GetMaximum(), zHist->GetMean(), zHist->GetStdDev());
  zHist->Scale(1 / zHist->Integral());
  zHist->Fit(formulaQ, "qn");

  double mean           = formulaQ->GetParameter(1);
  double sigma          = formulaQ->GetParameter(2);

  std::string sigmaString = "#sigma = " + std::to_string(sigma);//+"\\ #mu = "+std::to_string(mean);
  std::string meanString = "#mu = " + std::to_string(mean);//+"\\ #mu = "+std::to_string(mean);

  new TCanvas("Histogram of Z Estimation", "Histogram of Z Estimation");
  zHist->Draw();

  legendTZ->AddEntry(zHist, "Data Point", "p");
  legendTZ->AddEntry(formulaQ, "Fit", "l");
  legendTZ->AddEntry((TObject*)0, sigmaString.c_str(), "");
  legendTZ->AddEntry((TObject*)0, meanString.c_str(), "");

  formulaQ->Draw("same");
  legendTZ->Draw("same");


  qHist->SetLineColor(6);
  //qHist->Draw("same");

  fApp->Run();
}
