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

   GenerateMuonTestData(filename,std::atoi(argv[2]),std::atoi(argv[3]),std::atoi(argv[4]),true,std::atoi(argv[5]));
/*
  std::vector<TH1F *> vecOfHist =
      QAnalysis(filename, std::atoi(argv[2]), std::atoi(argv[3]), std::atoi(argv[4]), std::atoi(argv[5]));

TCanvas *canQ= new TCanvas("QCan","QCan");
TCanvas *canDelT= new TCanvas("DelTCan","DelTCan");
  for (unsigned int i = 0; i < vecOfHist.size(); i++) {
    if (i == 0 || i==3) {
vecOfHist[i]->SetLineColor(1);
}
    if (i == 1 || i==4) {
vecOfHist[i]->SetLineColor(2);
}
    if (i == 2 || i==5) {
vecOfHist[i]->SetLineColor(4);
}

  }

canQ->cd();
    vecOfHist[0]->Draw();
    vecOfHist[1]->Draw("same");
    vecOfHist[2]->Draw("same");
canDelT->cd();
    vecOfHist[3]->Draw();
    vecOfHist[4]->Draw("same");
    vecOfHist[5]->Draw("same");
*/
  /*if(argc==6)
     GenerateMuonTestData(filename,std::atoi(argv[2]),std::atoi(argv[3]),std::atoi(argv[4]),true);
   else
     GenerateMuonTestData(filename,std::atoi(argv[2]),std::atoi(argv[3]),std::atoi(argv[4]));*/
  //fApp->Run();

  return 0;
}
