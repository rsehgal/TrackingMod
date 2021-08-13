/*
**	Filename : MuonTrackAnalysis_HitMultiplicity.cpp
**	2021-07-28
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
#include <colors.h>

int main(int argc, char *argv[])
{
  gStyle->SetOptStat(111);
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename                                  = argv[1];
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename, 0);
  // std::vector<lite_interface::SingleMuonTrack*> smtVec = GetMuonTracksVector(filename,5000000);
  // std::vector<unsigned short> layerIndexVector={0,1,2,3,4,5,6,7,8,9};
  std::vector<unsigned int> layerIndexVector = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<unsigned int> countVec         = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  TH1F *multiplicity                         = new TH1F("HitMultiplicty", "HitMultiplicity Top Layer", 10, 1, 10);
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    // if (smtVec[i]->MuonPassingThroughLayer(9)) {
    // if (smtVec[i]->HitInRequiredLayers(layerIndexVector))
    {
      unsigned numOfHits = smtVec[i]->NumOfHitsInLayer(9);
      if (numOfHits > 0) {
        countVec[numOfHits]++;
        multiplicity->Fill(numOfHits);
      }
    }
  }

  unsigned int total = 0;

  for (unsigned int i = 0; i < countVec.size(); i++) {
    total += countVec[i];
  }

  std::cout << "Total num of events : " << total << std::endl;

  std::cout << RED;

  for (unsigned int i = 0; i < countVec.size(); i++) {
    std::cout << "Percent of events where   " << i << " strip fires : " << (1.0 * countVec[i]) / (1. * total)
              << std::endl;
  }

  std::cout << RESET;
  multiplicity->Draw();
  fApp->Run();

#if (0)
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    unsigned int numOfHits = 0;
    // if (smtVec[i]->HitInRequiredLayers())
    {

      numOfHits = smtVec[i]->NumOfHitsInLayer(9);

      std::cout << CYAN
                << "===================================================================================" << RESET
                << std::endl;
      if (numOfHits > 2) {

        std::cout << RED << "RED : NumOfHits : " << numOfHits << RESET << std::endl;
        std::cout << BLUE;
        smtVec[i]->Print();
        std::cout << RESET;
      } else {
        smtVec[i]->Print();
      }
    }
  }
#endif
#if (0)
  TH1F *muonTrackSize = new TH1F("MuonTrackSize", "MuonTrackSize", 15, 0., 15.);
  /*
  //Old
  for(unsigned int i = 0 ; i < layerIndexVector.size() ; i++){
    std::string canName = "Can_Layer_"+std::to_string(layerIndexVector[i]);
    new TCanvas(canName.c_str(),canName.c_str());
    lite_interface::PlotStripProfileOfLayer(smtVec,layerIndexVector[i])->Draw();
  }*/

  std::string canName = "Can_";
  for (unsigned int i = 0; i < layerIndexVector.size(); i++) {
    canName += std::to_string(layerIndexVector[i]);
  }
  canName += "Layers";
  new TCanvas(canName.c_str(), canName.c_str());

  std::string outfileName = "HitMultiplicitye_";
  outfileName += filename.substr(13);
  // TFile *fp=new TFile(outfileName.c_str(),"RECREATE");
  // fp->cd();
  std::vector<TH1F *> vecOfHitMultiplicity;
  for (unsigned int i = 0; i < layerIndexVector.size(); i++) {
    vecOfHitMultiplicity.push_back(lite_interface::PlotHitMultiplicityOfLayer(smtVec, layerIndexVector[i]));
  }

  for (unsigned int i = 0; i < layerIndexVector.size(); i++) {
    new TCanvas;
    vecOfHitMultiplicity[i]->SetLineColor(1);
    vecOfHitMultiplicity[i]->SetLineWidth(3);
    vecOfHitMultiplicity[i]->Draw();
    // vecOfHitMultiplicity[i]->SetLineColor(i+1);
    // vecOfHitMultiplicity[i]->Draw("same");
    // vecOfHitMultiplicity[i]->Write();
  }
  // fp->Close();
  std::cout << "File Written and closed........." << std::endl;

#endif
  // fApp->Run();
}
