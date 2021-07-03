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
#include "colors.h"
int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename               = argv[1];
  // std::string otherfile = argv[2];

  // unsigned int numOfEventsToRead = NumOfEventsToRead(filename,otherfile,std::string("TracksTree"));
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename);
  TH1F *smearanceHistZ                                  = new TH1F("SmearanceHistZ", "SmearanceHistZ", 250, -60., 60.);
  TH1F *smearanceHistX                                  = new TH1F("SmearanceHistX", "SmearanceHistX", 250, -60., 60.);
  for (unsigned int i = 0; i < smtVec.size(); i++) {
/*if(i<10){
    std::cout << "************* Printing Track : " << i << " *************************" << std::endl;
    std::vector<lite_interface::ScintillatorBar_V2 *> vecOfScint = smtVec[i]->GetMuonTrack();
    for (unsigned int j = 0; j < vecOfScint.size(); j++) {
  std::cout << "OBLONG  : " <<  vecOfLayersOrientation[vecOfScint[j]->GetLayerIndex()] <<" : " ;
  vecOfScint[j]->Print();
    }
}*/
#if (1)
    std::vector<lite_interface::ScintillatorBar_V2 *> vecOfScint = smtVec[i]->GetMuonTrack();
    for (unsigned int j = 0; j < vecOfScint.size(); j++) {
      if (i < 10) {
        if (j == 0) std::cout << "**************************************" << std::endl;
        if (vecOfLayersOrientation[vecOfScint[j]->GetLayerIndex()])
          std::cout << BLUE;
        else
          std::cout << RED;
        // vecOfScint[j]->Print();
        std::cout << "Mean Hit Point : BarIndex : " << vecOfScint[j]->GetBarIndex()
                  << " : LayerIndex : " << vecOfScint[j]->GetLayerIndex() << " : ";
        vecOfScint[j]->GetMeanHitPosition()->InCm()->Print();
        // vecOfScint[j]->GetExactHitPosition()->InCm()->Print();
        std::cout << "Exact Hit Point : BarIndex : " << vecOfScint[j]->GetBarIndex()
                  << " : LayerIndex : " << vecOfScint[j]->GetLayerIndex() << " : ";
        vecOfScint[j]->GetExactHitPosition()->InCm()->Print();

        std::cout << "Smeared Hit Point : BarIndex : " << vecOfScint[j]->GetBarIndex()
                  << " : LayerIndex : " << vecOfScint[j]->GetLayerIndex() << " : ";
        vecOfScint[j]->GetSmearedHitPosition()->InCm()->Print();
        std::cout << RESET;
      }

      if (vecOfScint[j]->GetBarIndex() == 85) {
        // std::cout << "Bar Center : " << vecOfScintXYCenter[vecOfScint[j]->GetBarIndex()].x << std::endl;
        smearanceHistZ->Fill(vecOfScint[j]->GetSmearedHitPosition()->InCm()->GetZ() -
                             vecOfScint[j]->GetExactHitPosition()->InCm()->GetZ());
        smearanceHistX->Fill(vecOfScint[j]->GetSmearedHitPosition()->InCm()->GetX() -
                             vecOfScint[j]->GetExactHitPosition()->InCm()->GetX());
      }
    }
#endif
  }
  smearanceHistZ->Draw();
  smearanceHistX->SetLineColor(6);
  smearanceHistX->Draw("same");
  fApp->Run();
}
