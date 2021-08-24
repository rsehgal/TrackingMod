/*
**	Filename : MuonTrackAnalysis_EnergySum.cpp
**	2021-07-29
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
#include "langaus.h"
#include "PsBar.h"
int main(int argc, char *argv[])
{
  gStyle->SetOptStat(0);
#ifdef USE_FOR_SIMULATION
  IsSimulation = true;
#endif
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename                                  = argv[1];
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename, 0);
  std::vector<TH1F *> vecOfEnergySumHist                = PlotEnergyDistributionWithMultiplicity(smtVec);
  TH2D *energySumWithMultiplicity                       = new TH2D("EnergySum2", "EnergySum2", 11, 0, 11, 11, 0., 250);
  TCanvas *can                                          = new TCanvas("Energy sum");
  TLegend *legendEnergySum                              = new TLegend(0.2, 0.2, .8, .8);
  legendEnergySum->SetHeader("#splitline{Layer}{Multiplicity}", "C");
  for (unsigned int i = 0; i < vecOfEnergySumHist.size(); i++) {

    vecOfEnergySumHist[i]->SetLineWidth(3);
    new TCanvas;
    vecOfEnergySumHist[i]->Draw();

    can->cd();
    if (vecOfEnergySumHist[i]->GetEntries() > 10) {
      std::string barString = std::to_string(i + 1);
      if (i == 9) vecOfEnergySumHist[i]->SetLineColor(1);
      legendEnergySum->AddEntry(vecOfEnergySumHist[i], barString.c_str(), "l");

      vecOfEnergySumHist[i]->Scale(1 / vecOfEnergySumHist[i]->Integral());
      vecOfEnergySumHist[i]->Draw("same");
      if (i == 3) langaus(vecOfEnergySumHist[i]);
    }
  }
  legendEnergySum->Draw("same");

  // legendEnergySum->AddEntry(histQNear, "Left PMT", "l");
  // legendEnergySum->AddEntry(histQFar, "Right PMT", "l");

  for (unsigned int i = 0; i < smtVec.size(); i++) {
    if (smtVec[i]->SingleHitInEachLayer()) {
      energySumWithMultiplicity->Fill(smtVec[i]->size(), smtVec[i]->GetEnergySum());
    }
  }
  new TCanvas;
  energySumWithMultiplicity->Draw("colz");
  fApp->Run();
}
