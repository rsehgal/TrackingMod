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
int main(int argc, char *argv[])
{
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
  // GenerateParameterizationUsingMuons(filename,13);
GenerateTrainingDataUsingCosmicMuon(filename,13);
/*  std::vector<TH1F *> vecOfHist = EvaluateMuonParameterization(filename, 5, 13);

  new TCanvas("ZUsing_Q", "ZUsing_Q");
  vecOfHist[0]->Draw();


  new TCanvas("ZUsing_DelT", "ZUsing_DelT");
  vecOfHist[1]->Draw();

  fApp->Run();*/
  return 0;
}