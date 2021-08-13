/*
**	Filename : MuonTrackAnalysis_MuonLifeTime.cpp
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

int main(int argc, char *argv[])
{
  // gStyle->SetOptStat(0);
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename                                  = argv[1];
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename, 0);
  unsigned int layerIndex                               = 4;
  bool startFlag                                        = false;
  ULong64_t startTime;
  ULong64_t endTime;
  TH1F *histMuonRate = new TH1F("Muon Rate Estimation", "", 100, 0, 50);
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    if (smtVec[i]->SingleHitInLayer(layerIndex)) {

      /*std::cout << "======================================" << std::endl;
      smtVec[i]->Print();*/

          std::vector<lite_interface::ScintillatorBar_V2 *>
              scintVec = smtVec[i]->GetMuonTrack();

      for (unsigned int j = 0; j < scintVec.size(); j++) {
        if (scintVec[j]->GetLayerIndex() == layerIndex) {
          if (!startFlag) {
            startFlag = true;
            // startTime = scintVec[j]->GetTSmallTimestamp();
            startTime = scintVec[j]->GetTAverage();
          } else {
            // endTime = scintVec[j]->GetTSmallTimestamp();
            endTime       = scintVec[j]->GetTAverage();
            Long64_t diff = endTime - startTime;
            std::cout << "Diff : " << diff << std::endl;
            startTime = endTime;
            // histMuonRate->Fill((1.*diff)/(1.*1000000));
            histMuonRate->Fill(diff / 1e+9);
            // startFlag=false;
          }
        }
      }
    }
  }
  TF1 *formula = new TF1("Formula", "expo", 0.2, 50.);
  histMuonRate->Fit(formula, "qn");

  histMuonRate->GetYaxis()->SetRangeUser(0, 10000);
  histMuonRate->Draw("p");
  histMuonRate->SetMarkerStyle(8);
  histMuonRate->SetMarkerSize(1.);
  formula->SetLineWidth(4);
  formula->Draw("same");

  TLegend *legendRate = new TLegend(0.2, 0.2, .8, .8);
  legendRate->AddEntry(histMuonRate, "Data", "p");
  legendRate->AddEntry(formula, "Fit", "l");
  legendRate->Draw("same");

  fApp->Run();
}
