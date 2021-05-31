#include "ml.h"
#include <iostream>
#include "HardwareNomenclature.h"
#include "Calibration.h"
#include <vector>
#include "SingleMuonTrack.h"
#include <fstream>
void GenerateTrainingData(std::string filename, unsigned int inspectedLayerIndex, std::string outfileName)
{
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename);
  std::cout << "Size of SMTVec : " << smtVec.size() << std::endl;
  std::string matWithExt = filename.substr(13);
  matWithExt             = matWithExt.substr(0, matWithExt.find("."));
  outfileName            = (matWithExt + "_" + outfileName);
  std::ofstream outfile(outfileName);
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    unsigned int hittBarIndex = 10000;
    ushort startIndex         = GetStartIndex(inspectedLayerIndex);
    ushort endIndex           = GetEndIndex(inspectedLayerIndex);
    bool check                = smtVec[i]->CheckTrackForLayerNum(startIndex, hittBarIndex);
    lite_interface::ScintillatorBar_V2 *scintStart;
    lite_interface::ScintillatorBar_V2 *scintEnd;
    if (check) {
      scintStart = smtVec[i]->GetScintillator(hittBarIndex);
    }
    check &= smtVec[i]->CheckTrackForLayerNum(endIndex, hittBarIndex);
    if (check) {
      scintEnd = smtVec[i]->GetScintillator(hittBarIndex);
    }
    check &= smtVec[i]->CheckTrackForLayerNum(inspectedLayerIndex, hittBarIndex);
    if (check) {
      lite_interface::ScintillatorBar_V2 *scint = smtVec[i]->GetScintillator(hittBarIndex);
      lite_interface::Point3D *hitPoint =
          scint->GetExactHitPosition(); // Get3DHitPointOnLayer(smtVec[i], inspectedLayerIndex);
      /*if (hitPoint->GetX() < 9000. && hitPoint->GetZ() < 9000 &&
          scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer()) {
        hist2D_Layer->Fill(hitPoint->GetX(), hitPoint->GetZ());
      }*/

      if (!vecOfLayersOrientation[inspectedLayerIndex]) {
        outfile << scintStart->GetExactHitPosition()->GetZ() << "," << scint->GetExactHitPosition()->GetX() << ","
                << scintEnd->GetExactHitPosition()->GetZ() << "," << scint->GetExactHitPosition()->GetZ() << std::endl;
      }
    }
  }
  outfile.close();
}
