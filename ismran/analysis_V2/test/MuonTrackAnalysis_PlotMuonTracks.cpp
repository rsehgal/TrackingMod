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
  bool verbose       = false;
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
  TH1F *hist_Diff_M                                     = new TH1F("HistDiff_M", "HistDiff_M", 200, -25., 25.);
  TH1F *hist_Diff_C = new TH1F("HistDiff_C", "HistDiff_C", 200, -25., 25.);
  TH1F *diffXHist                                       = new TH1F("diffXHist", "diffXHist", 100, -50., 50.);
  TH1F *diffZHist                                       = new TH1F("diffZHist", "diffZHist", 100, -50., 50.);
  TH1F *diffXHist_1                                       = new TH1F("diffXHistBeforeFitting", "diffXHistBeforeFitting", 100, -50., 50.);
  TH1F *diffZHist_1                                       = new TH1F("diffZHistBeforeFitting", "diffZHistBeforeFitting", 100, -50., 50.);

  for (unsigned int i = 0; i < smtVec.size(); i++) {

    if (smtVec[i]->HitInRequiredLayers() && smtVec[i]->SingleHitInEachLayer()) {
      // if (smtVec[i]->size() >= 6)
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

      std::vector<lite_interface::Point3D *> vecOfExactHit = InCm(smtVec[i]->GetExact3DHitPointVector());
      std::vector<lite_interface::Point3D *> vecOfSmearedHit = InCm(smtVec[i]->GetSmeared3DHitPointVector());
      std::vector<lite_interface::Point3D *> vecOfFittedHit =
          InCm(CreateFittedTrack(smtVec[i]->GetSmeared3DHitPointVector()));

      unsigned int layerToInspect = 8;// 9-std::atoi(argv[2]);
      //std::cout << "Y of inspected layer : " << vecOfExactHit[layerToInspect]->GetY() << std::endl;
      diffXHist->Fill(vecOfExactHit[layerToInspect]->GetX() - vecOfFittedHit[layerToInspect]->GetX());
      diffZHist->Fill(vecOfExactHit[layerToInspect]->GetZ() - vecOfFittedHit[layerToInspect]->GetZ());
      diffXHist_1->Fill(vecOfExactHit[layerToInspect]->GetX()-vecOfSmearedHit[layerToInspect]->GetX());
      diffZHist_1->Fill(vecOfExactHit[layerToInspect]->GetZ()-vecOfSmearedHit[layerToInspect]->GetZ());

      /*
      //Working Block
      std::vector<lite_interface::Point3D *> vecOfPoint3D_Exact   = InCm(smtVec[i]->GetExact3DHitPointVector());
      std::vector<lite_interface::Point3D *> vecOfPoint3D_Smeared = InCm(smtVec[i]->GetSmeared3DHitPointVector());
      TF1 *formulaXY_Exact   = lite_interface::GetFittedMuonTrackFormulaZY(vecOfPoint3D_Exact,true);
      TF1 *formulaXY_Smeared = lite_interface::GetFittedMuonTrackFormulaZY(vecOfPoint3D_Smeared,false);*/

      /*
      ** if Second argument set to true then give formula for track generated using XY planes
      ** otherwise return formula for track generated using ZY planes
      */
      TF1 *formula_Exact   = smtVec[i]->GetFitFormula(1, false);
      TF1 *formula_Smeared = smtVec[i]->GetFitFormula(2, false);
      if (verbose)
        std::cout << "M : " << formula_Exact->GetParameter(1) << " : C : " << formula_Exact->GetParameter(0)
                  << std::endl;
      double diff_m = formula_Exact->GetParameter(1) - formula_Smeared->GetParameter(1);
      double diff_c = formula_Exact->GetParameter(0) - formula_Smeared->GetParameter(0);
      if (verbose) std::cout << "Diff_M : " << diff_m << " : Diff_C : " << diff_c << std::endl;
      hist_Diff_M->Fill(diff_m);
      hist_Diff_C->Fill(diff_c);
    }
  }

  new TCanvas("DiffXHist", "DiffXHist");
  diffXHist->Draw();

  new TCanvas("DiffZHist", "DiffZHist");
  diffZHist->Draw();

  new TCanvas("DiffXHistBeforeFitting", "DiffXHistBeforeFitting");
  diffXHist_1->Draw();

  new TCanvas("DiffZHistBeforeFitting", "DiffZHistBeforeFitting");
  diffZHist_1->Draw();


 new TCanvas("Diff_M", "Diff_M");
  hist_Diff_M->Draw();
  new TCanvas("Diff_C", "Diff_C");
  hist_Diff_C->Draw();
  fApp->Run();
}
