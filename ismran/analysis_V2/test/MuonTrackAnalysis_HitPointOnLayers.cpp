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

  // lite_interface::Analyzer analyzerObj(filename);
  // analyzerObj->ReconstructMuonTrack();

  TFile *trackFile                                      = new TFile(filename.c_str(), "READ");
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename); //,2000000);

  std::cout << "Size of SMTVec : " << smtVec.size() << std::endl;
  /*
  ** Template of Filled histograms
  ** TH2F *hist2D_Layer0 = new TH2F("HitPointOnLayer_0","HitPointOnLayer_0",200,-50,50,200,-50,50);
  */
  TH2F *hist2D_Layer8 = PlotHitPointOnLayer(smtVec, 8);

  std::string matWithExt = filename.substr(13);
  TFile *fp              = new TFile(("HitPattern" + matWithExt).c_str(), "RECREATE");
  fp->cd();

  hist2D_Layer8->Write();
  fp->Close();
  // fApp->Run();

  return 0;
}
