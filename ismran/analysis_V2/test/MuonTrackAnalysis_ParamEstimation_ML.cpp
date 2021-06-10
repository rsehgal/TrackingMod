/*
 * MuonTrackAnalysis.cpp
 *
 *  Created on: 01-Mar-2021
 *       Author: rsehgal
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
  GenerateScintMatrixXYCenters();
  for (unsigned int i = 0; i < vecOfScintXYCenter.size(); i++) {
    if (!(i % 9)) std::cout << "=================================" << std::endl;
    vecOfScintXYCenter[i].Print();
  }
  // return 0;

  //  TApplication *fApp                   = new TApplication("Test", NULL, NULL);
  lite_interface::SingleMuonTrack *smt = new lite_interface::SingleMuonTrack;

  lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename               = argv[1];

  std::cout << "$$$$$$$$$$$$$$  Going to Try ML Stuff $$$$$$$$$$$$$$$$$$" << std::endl;
  GenerateParamData(filename, std::atoi(argv[2]));
  // fApp->Run();

  return 0;
}
