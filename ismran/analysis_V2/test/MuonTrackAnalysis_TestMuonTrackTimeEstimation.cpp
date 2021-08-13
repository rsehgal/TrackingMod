/*
 * TestMuonTrackTimeEstimation.cpp
 *
 *  Created on: 01-Mar-2021
 *      Author: rsehgal
 */

#include "PairFinder.h"
#include "Analyzer.h"
#include "Calibration.h"
#include <TApplication.h>
#include <TCanvas.h>
#include "Plotter.h"
#include "includes.hh"
#include "HardwareNomenclature.h"

int main(int argc, char *argv[])
{
  lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
  gStyle->SetOptStat(0);
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  //  lite_interface::Analyzer analyzerObj(argv[1],true,20000);
  /*lite_interface::Analyzer analyzerObj(argv[1],true,0);
  std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = analyzerObj.GetVectorOfScintillators();*/
  std::string filename                                  = argv[1];
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename, 0);
  ULong64_t timeWindow                                          = GetMuonTrackTimeWindow(smtVec);
  fApp->Run();

  return 0;
}
