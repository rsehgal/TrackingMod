/*
**	Filename : MuonTrackAnalysis_Generate_ML_Data_ForEachLayer.cpp
**	2021-07-05
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
#include <TF1.h>
#include <fstream>
int main(int argc, char *argv[])
{
  bool verbose = false;
  // TApplication *fApp = new TApplication("Test", NULL, NULL);
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename                                  = argv[1];
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename);

  // std::vector<std::ofstream> vecOfFilePointer;
  std::vector<OfStream *> vecOfFilePointer;
  for (unsigned int i = 0; i < vecOfRequiredLayers.size(); i++) {
    std::string filename = "ML_Data_Layer_" + std::to_string(vecOfRequiredLayers[i]) + ".txt";
    vecOfFilePointer.push_back(new OfStream(filename));
  }

  for (unsigned int i = 0; i < smtVec.size(); i++) {
    if (smtVec[i]->HitInRequiredLayers() && smtVec[i]->SingleHitInEachLayer()) {
      std::vector<lite_interface::ScintillatorBar_V2 *> vecOfScint = smtVec[i]->GetMuonTrack();

      for (unsigned int j = 0; j < vecOfScint.size(); j++) {
        unsigned int layerIndex = vecOfScint[j]->GetLayerIndex();
        int fileIndex           = GetIndex<unsigned int>(vecOfRequiredLayers, layerIndex);
        // std::cout << "LayerIndex : " << layerIndex << " : FileIndex : " << fileIndex << std::endl;
        if (fileIndex >= 0)
          vecOfFilePointer[fileIndex]->fOutfile
              << vecOfScint[j]->GetLayerIndex() << "," << vecOfScint[j]->GetBarIndex() << ","
              << vecOfScint[j]->GetLogQNearByQFar_ForSimulation() << "," << vecOfScint[j]->GetDelTCorrected() << ","
              << vecOfScint[j]->GetExactHitPosition()->GetX() << "," << vecOfScint[j]->GetExactHitPosition()->GetZ()
              << std::endl;
      }
    }
  }

  for (unsigned int i = 0; i < vecOfFilePointer.size(); i++) {
    (vecOfFilePointer[i]->fOutfile).close(); // GetFilePointer().close();
    delete vecOfFilePointer[i];
  }

  // fApp->Run();
}
