/*
**	Filename : MuonTrackAnalysis_ParameterizationUsingMuon.cpp
**	2021-07-15
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
#include "ml.h"
#include <TApplication.h>
#include <TStyle.h>
int main(int argc, char *argv[])
{
gStyle->SetOptStat(0);

  std::cout << "ARGC : " << argc << std::endl;
  // return 0;
  GenerateScintMatrixXYCenters();
  for (unsigned int i = 0; i < vecOfScintXYCenter.size(); i++) {
    if (!(i % 9)) std::cout << "=================================" << std::endl;
    vecOfScintXYCenter[i].Print();
  }
  // return 0;

  TApplication *fApp                   = new TApplication("Test", NULL, NULL);
  lite_interface::SingleMuonTrack *smt = new lite_interface::SingleMuonTrack;

  lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename               = argv[1];
  std::cout << "$$$$$$$$$$$$$$  Going to Generated Muon Classification data $$$$$$$$$$$$$$$$$$" << std::endl;

  // GenerateMuonTestData(filename,std::atoi(argv[2]),std::atoi(argv[3]),std::atoi(argv[4]),true,std::atoi(argv[5]));

  // Working
  GenerateParameterizationUsingMuons(filename, 13);
  // Working
  //  GenerateParameterizationUsingMuons_2(filename,std::atoi(argv[2]));
fApp->Run();
  return 0;
}
