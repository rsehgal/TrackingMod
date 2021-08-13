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
//  gStyle->SetOptStat(0);
  TApplication *fApp = new TApplication("Test", NULL, NULL);

  new TCanvas();
  // TLegend *legend = new TLegend();
  std::string filename = argv[1];
  CharacterizationFileReader cfReader(filename);
  FillAttenCoeffVec();
  std::vector<Event *> eventsVec = cfReader.GetAllEvents(0);//std::atoi(argv[2]));
  std::string barName            = cfReader.GetBarName();

  TFile *fpCalib    = new TFile("fQparam_temp.root", "r");
  TF1 *paramFormula = (TF1 *)fpCalib->Get("fQparam_temp");

  TH1F *qHist = new TH1F("QHist", "QHist", 100, -5., 5.);
  qHist->SetTitle("Histogram of Q");
  TH1F *zHist = new TH1F("zEstimation", "zEstimation", 100, -50., 50.);
  zHist->SetTitle("Histogram of estimated Z position");

  for (unsigned int i = 0; i < eventsVec.size(); i++) {
    double logQNearbyQFar = eventsVec[i]->GetLogQNearByQFar();
    qHist->Fill(logQNearbyQFar);
    zHist->Fill(paramFormula->Eval(logQNearbyQFar));
  }

  new TCanvas("Histogram of Z Estimation", "Histogram of Z Estimation");
  zHist->Draw();
  qHist->SetLineColor(6);
  qHist->Draw("same");

  fApp->Run();
}
