#include "ml.h"
#include <iostream>
#include "HardwareNomenclature.h"
#include "Calibration.h"
#include <vector>
#include "SingleMuonTrack.h"
#include <fstream>
/* Funtion to generate the Training and testing data for layer 8*/
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
  std::ofstream outfile2("interpolated.txt");
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
      if (!vecOfLayersOrientation[inspectedLayerIndex] &&
          (scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer()))
#ifdef USE_FOR_SIMULATION
        outfile << scintStart->GetExactHitPosition()->GetZ() << "," << scint->GetExactHitPosition()->GetX() << ","
                << scintEnd->GetExactHitPosition()->GetZ() << "," << scint->GetExactHitPosition()->GetZ() << std::endl;
#else

        outfile << scintStart->EstimateHitPosition_Param()->GetZ() << "," << scint->EstimateHitPosition_Param()->GetZ()
                << "," << scintEnd->EstimateHitPosition_Param()->GetZ() << std::endl;
      lite_interface::Point3D *hitPt = Get3DHitPointOnLayer(smtVec[i], inspectedLayerIndex);
      outfile2 << hitPt->GetX() << "," << hitPt->GetZ() << std::endl;
#endif
    }
  }

outfile.close();
outfile2.close();
}

/* Funtion to generate the Training and testing data for layer 9*/
#if (1)
void GenerateTrainingData_9(std::string filename, unsigned int inspectedLayerIndex, std::string outfileName)
{
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename);
  std::cout << "Size of SMTVec : " << smtVec.size() << std::endl;
  std::string matWithExt = filename.substr(13);
  matWithExt             = matWithExt.substr(0, matWithExt.find("."));
  outfileName            = ("Layer" + std::to_string(inspectedLayerIndex) + matWithExt + "_" + outfileName);
  std::ofstream outfile(outfileName);
  std::ofstream outfile2("interpolated_Layer9.txt");
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
      if ((scintStart->GetBarIndexInLayer() == scint->GetBarIndexInLayer())) {
#ifdef USE_FOR_SIMULATION
        outfile << scintStart->GetExactHitPosition()->GetZ() << "," << scint->GetExactHitPosition()->GetZ() << ","
                << scintEnd->GetExactHitPosition()->GetX() << "," << scint->GetExactHitPosition()->GetX() << std::endl;
#else

        outfile << scintStart->EstimateHitPosition_Param()->GetZ() << "," << scint->EstimateHitPosition_Param()->GetZ()
                << "," << scintEnd->EstimateHitPosition_Param()->GetZ() << std::endl;
        lite_interface::Point3D *hitPt = Get3DHitPointOnLayer(smtVec[i], inspectedLayerIndex);
        outfile2 << hitPt->GetX() << "," << hitPt->GetZ() << std::endl;

#endif
      }
    }
  }
  outfile.close();
  outfile2.close();
}
#endif

/* Funtion to generate the Training and testing data for all layers */
#if (1)
void GenerateTrainingData_All(std::string filename, unsigned int inspectedLayerIndex, std::string outfileName)
{
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename);
  std::cout << "Size of SMTVec : " << smtVec.size() << std::endl;
  std::string matWithExt = filename.substr(13);
  matWithExt             = matWithExt.substr(0, matWithExt.find("."));
  outfileName            = ("Layer" + std::to_string(inspectedLayerIndex) + matWithExt + "_" + outfileName);
  std::ofstream outfile(outfileName);
  std::ofstream outfile2("interpolated_Layer9.txt");
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
      // if (!vecOfLayersOrientation[inspectedLayerIndex] && (scintStart->GetBarIndexInLayer() ==
      // scintEnd->GetBarIndexInLayer())) {
      // if ((scintStart->GetBarIndexInLayer() == scint->GetBarIndexInLayer()))
      // if ((scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer()))

      /*Critical condition just to see the scintillators fingers*/
      bool fingerCondition = false;
      fingerCondition =
          (scintStart->GetLayerIndex() < inspectedLayerIndex && scintEnd->GetLayerIndex() < inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scint->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() > inspectedLayerIndex && scintEnd->GetLayerIndex() > inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scint->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() < inspectedLayerIndex && scintEnd->GetLayerIndex() > inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() > inspectedLayerIndex && scintEnd->GetLayerIndex() < inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer());
//#define USE_CRITICAL_COND
#ifdef USE_CRITICAL_COND
      if (fingerCondition)
#endif
      {
#ifdef USE_FOR_SIMULATION
        if (!vecOfLayersOrientation[inspectedLayerIndex]) {
          outfile << scintStart->GetExactHitPosition()->GetZ() << "," << scint->GetExactHitPosition()->GetX() << ","
                  << scintEnd->GetExactHitPosition()->GetZ() << "," << scint->GetExactHitPosition()->GetZ()
                  << std::endl;
        } else {
          if (inspectedLayerIndex == 7 || inspectedLayerIndex == 9) {
            outfile << scintStart->GetExactHitPosition()->GetZ() << "," << scint->GetExactHitPosition()->GetZ() << ","
                    << scintEnd->GetExactHitPosition()->GetX() << "," << scint->GetExactHitPosition()->GetX()
                    << std::endl;
          } else {
            outfile << scintStart->GetExactHitPosition()->GetX() << "," << scint->GetExactHitPosition()->GetZ() << ","
                    << scintEnd->GetExactHitPosition()->GetX() << "," << scint->GetExactHitPosition()->GetX()
                    << std::endl;
          }
        }
#else

        outfile << scintStart->EstimateHitPosition_Param()->GetZ() << "," << scint->EstimateHitPosition_Param()->GetZ()
                << "," << scintEnd->EstimateHitPosition_Param()->GetZ() << std::endl;
        // << "," << scint->EstimateHitPosition_Param()->GetZ() << std::endl;
        lite_interface::Point3D *hitPt = Get3DHitPointOnLayer(smtVec[i], inspectedLayerIndex);
        outfile2 << hitPt->GetX() << "," << hitPt->GetZ() << std::endl;

#endif
      }
    }
 } 
  outfile.close();
  outfile2.close();
}
#endif
