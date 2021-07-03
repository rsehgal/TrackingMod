/*
**	Filename : MuonTrackAnalysis_PlotMuonTracks.cpp
**	2021-07-03
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
#include "includes.h"
#include "HardwareNomenclature.h"
#include <TSpectrum.h>
#include "Histograms.h"
#include <TH2F.h>
#include "colors.h"

int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename               = argv[1];
  // std::string otherfile = argv[2];

  // unsigned int numOfEventsToRead = NumOfEventsToRead(filename,otherfile,std::string("TracksTree"));
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename);
  TH1F *smearanceHistZ                                  = new TH1F("SmearanceHistZ", "SmearanceHistZ", 250, -60., 60.);
  TH1F *smearanceHistX                                  = new TH1F("SmearanceHistX", "SmearanceHistX", 250, -60., 60.);
  unsigned int counter                                  = 0;
  for (unsigned int i = 0; i < smtVec.size(); i++) {

    if (smtVec[i]->size() >= 6) {
      if (counter < 5) {
        counter++;
        /* new TCanvas("Exact_XY");
           lite_interface::PlotMuonTrack(smtVec[i]->GetExact3DHitPointVector(), 1)->Draw("ap");
           new TCanvas("Exact_ZY");
           lite_interface::PlotMuonTrack(smtVec[i]->GetExact3DHitPointVector(), 2)->Draw("ap");
           new TCanvas("Smeared_XY");
           lite_interface::PlotMuonTrack(smtVec[i]->GetSmeared3DHitPointVector(), 1)->Draw("ap");
           new TCanvas("Smeared_ZY");
           lite_interface::PlotMuonTrack(smtVec[i]->GetSmeared3DHitPointVector(), 2)->Draw("ap");
       */

        std::string title = "Event Num : " + std::to_string(i);
        TCanvas *can      = new TCanvas(title.c_str(), title.c_str());
        can->Divide(3, 2);
        can->cd(1);
        DrawGrid("Exact Muon Track in XY plane; X axis ; Y axis", 10, 20);
        lite_interface::PlotMuonTrackXY(InCm(smtVec[i]->GetExact3DHitPointVector()))->Draw("p");

        can->cd(2);
        DrawGrid("Smeared Muon Track in XY plane; Z axis ; Y axis", 10, 20);
        lite_interface::PlotMuonTrackXY(InCm(smtVec[i]->GetSmeared3DHitPointVector()))->Draw("p");

        can->cd(3);
        DrawGrid("Fitted Muon Track in XY plane; Z axis ; Y axis", 10, 20);
        lite_interface::PlotMuonTrackXY(InCm(CreateFittedTrack(smtVec[i]->GetSmeared3DHitPointVector())))->Draw("p");

        can->cd(4);
        DrawGrid("Exact Muon Track in ZY plane; Z axis ; Y axis", 10, 20);
        lite_interface::PlotMuonTrackZY(InCm(smtVec[i]->GetExact3DHitPointVector()))->Draw("p");

        can->cd(5);
        DrawGrid("Smeared Muon Track in ZY plane; Z axis ; Y axis", 10, 20);
       lite_interface::PlotMuonTrackZY(InCm(smtVec[i]->GetSmeared3DHitPointVector()))->Draw("p");
 
        can->cd(6);
        DrawGrid("Fitted Muon Track in ZY plane; Z axis ; Y axis", 10, 20);
        lite_interface::PlotMuonTrackZY(InCm(CreateFittedTrack(smtVec[i]->GetSmeared3DHitPointVector())))->Draw("p");

      }
    }
  }
  fApp->Run();
}
