/*
**	Filename : MuonTrackAnalysis_FittedTracks_FromFile.cpp
**	2021-07-06
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

  bool verbose       = false;
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  GenerateScintMatrixXYCenters();
  // lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filenameString              = argv[1];
  std::vector<std::string> vecOfFileNames = SplitString(filenameString);
  std::vector<std::ifstream> vecOfFilePointer;
  for (unsigned int i = 0; i < vecOfFileNames.size(); i++) {
    vecOfFilePointer.push_back(std::ifstream(vecOfFileNames[i]));
  }

  std::vector<lite_interface::Point3D *> vecOfExactPoint3D;
  std::vector<lite_interface::Point3D *> vecOfSmearedPoint3D;

  TH1F *histDiffX        = new TH1F("DiffXHist", "DiffXHist", 100, -50., 50.);
  TH1F *histDiffZ        = new TH1F("DiffZHist", "DiffZHist", 100, -50., 50.);
  TH1F *histDiffX_fitted = new TH1F("DiffXHist_fitted", "DiffXHist_fitted", 100, -50., 50.);
  TH1F *histDiffZ_fitted = new TH1F("DiffZHist_fitted", "DiffZHist_fitted", 100, -50., 50.);

  unsigned int counter = 0;
  while (!(vecOfFilePointer[0].eof())) {
    vecOfExactPoint3D.clear();
    vecOfSmearedPoint3D.clear();

    if (counter < 5) std::cout << "======================================" << std::endl;
    for (unsigned int i = 0; i < vecOfFilePointer.size(); i++) {

      double delT;
      double layerIndex = 0, barIndex = 0;
      double q, smearedx, smearedz, actx, actz;
      vecOfFilePointer[i] >> layerIndex >> barIndex >> q >> delT >> smearedx >> smearedz >> actx >> actz;
      if (counter < 5)
        std::cout << "File Index : " << i << " : " << smearedx << " " << smearedz << " " << actx << " " << actz
                  << std::endl;
      vecOfExactPoint3D.push_back(new lite_interface::Point3D(actx, GetYOfLayer(layerIndex) * 10., actz));
      vecOfSmearedPoint3D.push_back(new lite_interface::Point3D(smearedx, GetYOfLayer(layerIndex) * 10., smearedz));
      if (i == 3) {
        histDiffX->Fill((actx - smearedx)/10.);
        histDiffZ->Fill((actz - smearedz)/10.);
      }
    }

    std::string title = "Event Num : " + std::to_string(counter);

    std::vector<lite_interface::Point3D *> vecOfFittedPoint3D = CreateFittedTrack(vecOfSmearedPoint3D);
    histDiffX_fitted->Fill((vecOfExactPoint3D[0]->GetX() - vecOfFittedPoint3D[0]->GetX())/10.);
    histDiffZ_fitted->Fill((vecOfExactPoint3D[0]->GetZ() - vecOfFittedPoint3D[0]->GetZ())/10.);

    if (counter < 5) {

      std::cout << "--------------------------------------" << std::endl;
      for (unsigned int j = 0; j < vecOfExactPoint3D.size(); j++) {
        vecOfExactPoint3D[j]->Print();
      }

      TCanvas *can = new TCanvas(title.c_str(), title.c_str());
      can->Divide(3, 2);
      can->cd(1);
      DrawGrid("Exact Muon Track in XY plane; X axis ; Y axis", 10, 20);
      lite_interface::PlotMuonTrackXY(InCm(vecOfExactPoint3D))->Draw("p");

      can->cd(2);
      DrawGrid("Smeared Muon Track in XY plane; Z axis ; Y axis", 10, 20);
      lite_interface::PlotMuonTrackXY(InCm(vecOfSmearedPoint3D))->Draw("p");

      can->cd(3);
      DrawGrid("Fitted Muon Track in XY plane; Z axis ; Y axis", 10, 20);
      lite_interface::PlotMuonTrackXY(InCm(CreateFittedTrack(vecOfSmearedPoint3D)))->Draw("p");

      can->cd(4);
      DrawGrid("Exact Muon Track in ZY plane; Z axis ; Y axis", 10, 20);
      lite_interface::PlotMuonTrackZY(InCm(vecOfExactPoint3D))->Draw("p");

      can->cd(5);
      DrawGrid("Smeared Muon Track in ZY plane; Z axis ; Y axis", 10, 20);
      lite_interface::PlotMuonTrackZY(InCm(vecOfSmearedPoint3D))->Draw("p");

      can->cd(6);
      DrawGrid("Fitted Muon Track in ZY plane; Z axis ; Y axis", 10, 20);
      lite_interface::PlotMuonTrackZY(InCm(CreateFittedTrack(vecOfSmearedPoint3D)))->Draw("p");

      counter++;
    }
  }
  new TCanvas("HistDiffX_Can", "HistDiffX_Can");
  histDiffX->Draw();
  new TCanvas("HistDiffZ_Can", "HistDiffZ_Can");
  histDiffZ->Draw();
  new TCanvas("HistDiffX_Fitted_Can", "HistDiffX_Fitted_Can");
  histDiffX_fitted->Draw();
  new TCanvas("HistDiffZ_Fitted_Can", "HistDiffZ_Fitted_Can");
  histDiffZ_fitted->Draw();
  fApp->Run();
}
