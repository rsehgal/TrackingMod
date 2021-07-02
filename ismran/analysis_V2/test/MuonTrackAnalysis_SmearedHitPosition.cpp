/*
**	Filename : MuonTrackAnalysis_SmearedHitPosition.cpp
**	2021-07-01
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

int main(int argc, char *argv[])
{
  // TApplication *fApp = new TApplication("Test", NULL, NULL);
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename               = argv[1];
  // std::string otherfile = argv[2];

  // unsigned int numOfEventsToRead = NumOfEventsToRead(filename,otherfile,std::string("TracksTree"));
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename);

  for (unsigned int i = 0; i < smtVec.size(); i++) {
    std::vector<lite_interface::ScintillatorBar_V2 *> vecOfScint = smtVec[i]->GetMuonTrack();
    if (i < 10) {
      std::cout << "**************************************" << std::endl;
      for (unsigned int j = 0; j < vecOfScint.size(); j++) {
        std::cout << "Exact Hit Point : BarIndex : " << vecOfScint[j]->GetBarIndex() << " : LayerIndex : " << vecOfScint[j]->GetLayerIndex() << " : ";
        vecOfScint[j]->GetExactHitPosition()->Print();
        std::cout << "Smeared Hit Point : BarIndex : " << vecOfScint[j]->GetBarIndex() << " : LayerIndex : " << vecOfScint[j]->GetLayerIndex() << " : ";
        vecOfScint[j]->GetSmearedHitPosition()->Print();
      }
    }
  }
  // fApp->Run();
}
