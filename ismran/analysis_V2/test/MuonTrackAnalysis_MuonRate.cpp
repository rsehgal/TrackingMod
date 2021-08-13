/*
**	Filename : MuonTrackAnalysis_MuonRate.cpp
**	2021-07-30
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

int main(int argc, char *argv[])
{
  gStyle->SetOptStat(0);
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename                                  = argv[1];
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename, 0);
  unsigned int layerIndex                               = 9;
  bool startFlag                                  = false;
  ULong64_t startTime;
  ULong64_t endTime;
  TH1F *histMuonRate = new TH1F("MuonRate","",10,0, 10);
  std::vector<int> muonRateVec = GetMuonRateAtLayers(smtVec);

  double total = 0 ;  
  for(unsigned int i = 0 ; i < muonRateVec.size() ; i++){
	std::cout << "index : " << i <<" : Rate : " << muonRateVec[i] << std::endl;
	histMuonRate->SetBinContent(i+1,muonRateVec[i]/0.91);
  	total += muonRateVec[i];
  } 

    std::cout << "Averge Rate : " << total/10. << std::endl;
    histMuonRate->Draw();
  fApp->Run();
}
