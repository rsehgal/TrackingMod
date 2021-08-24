/*
**	Filename : Test_EnergyDepComparison.cpp
**	2021-08-18
**	username : rsehgal
*/
#include <iostream>
#include "includes.h"
#include <TH1F.h>
#include <TFile.h>
#include <TApplication.h>

int main(int argc, char *argv[])
{

  gStyle->SetOptStat(0);
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  TFile *simFile     = new TFile(argv[1], "r");
  TFile *expFile     = new TFile(argv[2], "r");

  TH1F *simEner = (TH1F *)simFile->Get("EnerHistLandau");
  TH1F *expEner = (TH1F *)expFile->Get("ExpData_Ener");

  simEner->SetLineColor(2);
  expEner->SetLineColor(1);
  simEner->SetLineWidth(4);
  expEner->SetLineWidth(4);

  simEner->SetFillStyle(3005);

  TLegend *legend = new TLegend(0.2, 0.2, .8, .8);
  legend->AddEntry(simEner, "Simulation", "l");
  legend->AddEntry(expEner, "Data", "l");

  simEner->Scale(1/simEner->Integral());
  expEner->Scale(1/expEner->Integral());

  simEner->Draw("ehist");
  expEner->Draw("ehistsame");
  legend->Draw("same");

  fApp->Run();
}
