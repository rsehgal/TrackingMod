/*
**	Filename : MuonTrackAnalysis_LanGaus.cpp
**	2021-08-16
**	username : rsehgal
*/
#include <iostream>
#include "langaus.h"
#include "TApplication.h"
#include <TFile.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLegend.h>
int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  TFile *fp          = new TFile(argv[1], "r");
#ifdef USE_FOR_SIMULATION
  TH1F *hist = (TH1F *)fp->Get("SimData_Ener");
#else
  TH1F *hist = (TH1F *)fp->Get("ExpData_Ener");
#endif

  unsigned int nbinsx = hist->GetNbinsX();

/*  for(unsigned int i = 1 ; i <= nbinsx ; i++){
	if(hist->GetBinContent(i)>0)
	hist->SetBinContent(i,TMath::Log10(hist->GetBinContent(i)));
}*/

//  hist->Scale(1./hist->Integral());
  TF1 *fitf = langaus(hist);

  new TCanvas;
  hist->Draw();
  fitf->Draw("same");
/*TLegend *legend = new TLegend(0.2, 0.2, .8, .8);
  legend->AddEntry(hist, "Data", "l");
  legend->AddEntry(fitf, "Fit", "l");
legend->Draw("same");
legend->Write();*/
  fp->Close();
  fApp->Run();
  return 0;
}
