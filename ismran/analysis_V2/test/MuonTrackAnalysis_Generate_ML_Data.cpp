/*
**	Filename : MuonTrackAnalysis_Generate_ML_Data.cpp
**	2021-07-04
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
#include <TF1.h>
#include <fstream>
int main(int argc, char *argv[])
{
  bool verbose = false;
  // TApplication *fApp = new TApplication("Test", NULL, NULL);
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
  TH1F *hist_Diff_C                                     = new TH1F("HistDiff_C", "HistDiff_C", 200, -25., 25.);
  std::ofstream outfile("ml_data_for_track_param.txt");
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    /*if (smtVec[i]->size() >= 6) {
    }*/
    if (smtVec[i]->HitInRequiredLayers()) {
      if (counter < 5) {
        counter++;
        std::cout << "---------------------------------------------------------------" << std::endl;
        std::vector<lite_interface::ScintillatorBar_V2 *> vecOfScint = smtVec[i]->GetMuonTrack();
        for (unsigned int j = 0; j < vecOfScint.size(); j++) {
          auto color = BLACK;
          if (vecOfLayersOrientation[vecOfScint[j]->GetLayerIndex()])
            color = BLUE;
          else
            color = RED;
          std::cout << color << "LayeIndex : " << vecOfScint[j]->GetLayerIndex() << " : "
                    << "BarIndex : " << vecOfScint[j]->GetBarIndex() << " : "
                    << "Q : " << vecOfScint[j]->GetLogQNearByQFar_ForSimulation() << " : "
                    << "DelT : " << vecOfScint[j]->GetDelTCorrected() << RESET << std::endl;
        }
        std::cout << "---------------------------------------------------------------" << std::endl;
        TF1 *formula_Exact   = smtVec[i]->GetFitFormula(1, false);
        TF1 *formula_Smeared = smtVec[i]->GetFitFormula(2, false);
        std::cout << MAGENTA << "Fit Parameter For Exact Track : C : " << formula_Exact->GetParameter(0)
                  << " : M : " << formula_Exact->GetParameter(1) << RESET << std::endl;
        std::cout << MAGENTA << "Fit Parameter For Smeared Track : C : " << formula_Smeared->GetParameter(0)
                  << " : M : " << formula_Smeared->GetParameter(1) << RESET << std::endl;
      }

      TF1 *formula_Exact                                           = smtVec[i]->GetFitFormula(1, false);
      std::vector<lite_interface::ScintillatorBar_V2 *> vecOfScint = smtVec[i]->GetMuonTrack();
      for (unsigned int j = 0; j < vecOfScint.size(); j++) {
        outfile << vecOfScint[j]->GetLayerIndex() << "," << vecOfScint[j]->GetBarIndex() << ","
                << vecOfScint[j]->GetLogQNearByQFar_ForSimulation() << "," << vecOfScint[j]->GetDelTCorrected() << ",";
      }

      outfile << formula_Exact->GetParameter(0) << "," << formula_Exact->GetParameter(1) << std::endl;
    }
  }
  outfile.close();
  // fApp->Run();
}
