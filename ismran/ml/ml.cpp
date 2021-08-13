#include "ml.h"
#include <iostream>
#include "HardwareNomenclature.h"
#include "Calibration.h"
#include <vector>
#include "SingleMuonTrack.h"
#include <fstream>
#include "colors.h"
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

/*
** Function to Generate data for classfication.
** The output should contain
** [1] log(QNear/QFar) [2] CorrectedDelT [3] zQ
*/

#if (1)
void GenerateDataForClassification(std::string filename, unsigned int inspectedLayerIndex, unsigned int startIndex,
                                   unsigned int endIndex)
{
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename);
  std::cout << "Size of SMTVec : " << smtVec.size() << std::endl;
  std::string matWithExt = filename.substr(13);
  matWithExt             = matWithExt.substr(0, matWithExt.find("."));
  matWithExt += ".txt";
  std::string outfileName = ("ClassificationData_Layer" + std::to_string(inspectedLayerIndex) + matWithExt);
  std::cout << "OutfileName : " << outfileName << std::endl;
  // return;
  std::ofstream outfile(outfileName);
  int simpleCounter = 0;
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    unsigned int hittBarIndex = 10000;
    /*ushort startIndex         = GetStartIndex(inspectedLayerIndex);
    ushort endIndex           = GetEndIndex(inspectedLayerIndex);*/
    bool check = smtVec[i]->CheckTrackForLayerNum(startIndex, hittBarIndex);
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
      // std::cout << "FINGER CONDITION IS SET TO : " << fingerCondition << std::endl;
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
        // Set for the time being
        bool qparam = true;
        if (qparam) {
          if (simpleCounter < 10) {
            std::cout << BLUE << scintStart->GetBarIndex() << "," << scintStart->GetLogQNearByQFar() << ","
                      << scintStart->GetDelTCorrected() << "," << scintStart->EstimateHitPosition_QParam()->GetZ()
                      << "," << RED << scint->GetBarIndex() << "," << scint->GetLogQNearByQFar() << ","
                      << scint->GetDelTCorrected() << "," << scint->EstimateHitPosition_QParam()->GetZ() << ","
                      << MAGENTA << scintEnd->GetBarIndex() << "," << scintEnd->GetLogQNearByQFar() << ","
                      << scintEnd->GetDelTCorrected() << "," << scintEnd->EstimateHitPosition_QParam()->GetZ() << RESET
                      << std::endl;
            simpleCounter++;
          }

          outfile << scintStart->GetBarIndex() << "," << scintStart->GetLogQNearByQFar() << ","
                  << scintStart->GetDelTCorrected() << "," << scintStart->EstimateHitPosition_QParam()->GetZ() << ","
                  << scint->GetBarIndex() << "," << scint->GetLogQNearByQFar() << "," << scint->GetDelTCorrected()
                  << "," << scint->EstimateHitPosition_QParam()->GetZ() << "," << scintEnd->GetBarIndex() << ","
                  << scintEnd->GetLogQNearByQFar() << "," << scintEnd->GetDelTCorrected() << ","
                  << scintEnd->EstimateHitPosition_QParam()->GetZ() << std::endl;
        }
        /* << scint->EstimateHitPosition_QParam()->GetZ() << ","
         << scintEnd->EstimateHitPosition_QParam()->GetZ() << std::endl;*/
        else
          outfile << scintStart->EstimateHitPosition_Param()->GetZ() << ","
                  << scint->EstimateHitPosition_Param()->GetZ() << "," << scintEnd->EstimateHitPosition_Param()->GetZ()
                  << std::endl;
        lite_interface::Point3D *hitPt = Get3DHitPointOnLayer(smtVec[i], inspectedLayerIndex);

#endif
      }
    }
  }
  outfile.close();
}
#endif

/*
** Function to generate test data for muon, so that the classification accurracy
** can be tested even with muons.
**
** Should give the output in same format as it is there for training and testing
** files of source data.
**
** Since we are working with complete layer at a time, so for scintillator
** identification we will also store (layerIndex, barIndex)
** layerIndex, barIndex, Q, delT, zQ, class
*/
#if (1)
std::vector<TH1F *> QAnalysis(std::string filename, unsigned int inspectedLayerIndex, unsigned int startIndex,
                              unsigned int endIndex, unsigned int numOfEvents)
{
  GenerateScintMatrixXYCenters();
  TH1F *histCenter          = new TH1F("Q_Center", "Q_Center", 100, -2., 2.);
  TH1F *histCenterLeft      = new TH1F("Q_CenterLeft", "Q_CenterLef", 100, -2., 2.);
  TH1F *histCenterRight     = new TH1F("Q_CenterRight", "Q_CenterRight", 100, -2., 2.);
  TH1F *histCenterDelT      = new TH1F("DelT_Center", "DelT_Center", 15000, -5000., 10000.);
  TH1F *histCenterLeftDelT  = new TH1F("DelT_CenterLeft", "DelT_CenterLef", 15000, -5000., 10000.);
  TH1F *histCenterRightDelT = new TH1F("DelT_CenterRight", "DelT_CenterRight", 15000, -5000., 10000.);

  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename, numOfEvents);
  std::cout << "Size of SMTVec : " << smtVec.size() << std::endl;
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    unsigned int hittBarIndex = 10000;
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
      if (scint->GetBarIndex() == 13) {
        if (scintStart->GetBarIndexInLayer() == 4 && scintEnd->GetBarIndexInLayer() == 4) {
          histCenter->Fill(scint->GetLogQNearByQFar());
          histCenterDelT->Fill(scintStart->GetTSmallTimestamp() - scintEnd->GetTSmallTimestamp());
        }
        if (scintStart->GetBarIndexInLayer() == 4 && scintEnd->GetBarIndexInLayer() == 3) {
          histCenterLeft->Fill(scint->GetLogQNearByQFar());
          histCenterLeftDelT->Fill(scintStart->GetTSmallTimestamp() - scintEnd->GetTSmallTimestamp());
        }
        if (scintStart->GetBarIndexInLayer() == 4 && scintEnd->GetBarIndexInLayer() == 5) {
          histCenterRight->Fill(scint->GetLogQNearByQFar());
          histCenterRightDelT->Fill(scintStart->GetTSmallTimestamp() - scintEnd->GetTSmallTimestamp());
        }
      }
    }
  }
  std::vector<TH1F *> vecOfHist;
  vecOfHist.push_back(histCenter);
  vecOfHist.push_back(histCenterLeft);
  vecOfHist.push_back(histCenterRight);
  vecOfHist.push_back(histCenterDelT);
  vecOfHist.push_back(histCenterLeftDelT);
  vecOfHist.push_back(histCenterRightDelT);

  return vecOfHist;
}
#endif

#if (1)
void GenerateMuonTestData(std::string filename, unsigned int inspectedLayerIndex, unsigned int startIndex,
                          unsigned int endIndex, bool qparam, unsigned int numOfEvents)
{
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename, numOfEvents);
  std::cout << "Size of SMTVec : " << smtVec.size() << std::endl;
  std::string matWithExt = filename.substr(13);
  matWithExt             = matWithExt.substr(0, matWithExt.find("."));
  matWithExt += ".txt";
  std::string outfileName = ("Layer_" + std::to_string(inspectedLayerIndex) + "_MuonTestData" + matWithExt);
  std::cout << "OutfileName : " << outfileName << std::endl;
  // return;
  std::ofstream outfile(outfileName);
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    unsigned int hittBarIndex = 10000;
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

        if (scint->GetBarIndex() == 13) {
          if (!(scint->GetLayerIndex() % 2))
            outfile << scint->GetLayerIndex() << "," << scint->GetBarIndex() << "," << scint->GetLogQNearByQFar() << ","
                    << scint->GetDelTCorrected() << "," << scint->EstimateHitPosition_QParam()->GetZ() << ","
                    << GetClass(scintStart->GetBarIndexInLayer()) << std::endl;
          else
          // if (GetClass(scintStart->GetBarIndexInLayer()) == 0)
          {
            std::cout << RED << __FILE__ << " : " << __LINE__ << std::endl;
            outfile << scint->GetLayerIndex() << "," << scint->GetBarIndex() << ","
                    << scint->GetLogQNearByQFar_ForSimulation() << "," << scint->GetDelTCorrected() << ","
                    << scint->EstimateHitPosition_QParam()->GetX() << "," << GetClass(scintStart->GetBarIndexInLayer())
                    << std::endl;
          }
        }

#else
        // if (scint->GetBarIndex() == 13 && GetClass(scintStart->GetBarIndexInLayer()) == -20 ) {
        if (scint->GetBarIndex() == 13) {
          if (!(scint->GetLayerIndex() % 2))
            outfile << scint->GetLayerIndex() << "," << scint->GetBarIndex() << "," << scint->GetLogQNearByQFar() << ","
                    << scint->GetDelTCorrected() << "," << scint->EstimateHitPosition_QParam()->GetZ() << ","
                    << GetClass(scintStart->GetBarIndexInLayer()) << std::endl;
          else
          // if (GetClass(scintStart->GetBarIndexInLayer()) == 0)
          {
            outfile << scint->GetLayerIndex() << "," << scint->GetBarIndex() << "," << scint->GetLogQNearByQFar() << ","
                    << scint->GetDelTCorrected() << "," << scint->EstimateHitPosition_QParam()->GetX() << ","
                    << GetClass(scintStart->GetBarIndexInLayer()) << std::endl;
          }
        }

#endif
      }
    }
  }
  outfile.close();
}
#endif

/*
** Funtion to generate the Training and testing data for all layers
** Provide the provision to specify the startIndex and endIndex, in addition
** to inspectedLayerIndex
*/
#if (1)
void GenerateTrainingData_All(std::string filename, unsigned int inspectedLayerIndex, unsigned int startIndex,
                              unsigned int endIndex, bool qparam)
{
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename);
  std::cout << "Size of SMTVec : " << smtVec.size() << std::endl;
  std::string matWithExt = filename.substr(13);
  matWithExt             = matWithExt.substr(0, matWithExt.find("."));
  matWithExt += ".txt";
  std::string outfileName = ("Layer" + std::to_string(inspectedLayerIndex) + matWithExt);
  std::cout << "OutfileName : " << outfileName << std::endl;
  // return;
  std::ofstream outfile(outfileName);
  std::ofstream outfile2("interpolated_Layer_" + std::to_string(inspectedLayerIndex) + ".txt");
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    unsigned int hittBarIndex = 10000;
    /*ushort startIndex         = GetStartIndex(inspectedLayerIndex);
    ushort endIndex           = GetEndIndex(inspectedLayerIndex);*/
    bool check = smtVec[i]->CheckTrackForLayerNum(startIndex, hittBarIndex);
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
        /*outfile << scintStart->GetExactHitPosition()->GetZ() << "," << scintEnd->GetExactHitPosition()->GetZ() << ","
                << scint->GetExactHitPosition()->GetZ() << std::endl;*/

        // Commenting for the time being
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
        /*//Commenting for the time being
                outfile << scintStart->EstimateHitPosition_Param()->GetZ() << "," <<
           scint->EstimateHitPosition_Param()->GetZ()
                        << "," << scintEnd->EstimateHitPosition_Param()->GetZ() << std::endl;*/
        if (qparam) {
          if (!vecOfLayersOrientation[inspectedLayerIndex]) {
            outfile << scintStart->EstimateHitPosition_QParam()->GetZ() << ","
                    << scint->EstimateHitPosition_QParam()->GetX() << ","
                    << scintEnd->EstimateHitPosition_QParam()->GetZ() << std::endl;
          } else {
            outfile << scintStart->EstimateHitPosition_QParam()->GetX() << ","
                    << scint->EstimateHitPosition_QParam()->GetZ() << ","
                    << scintEnd->EstimateHitPosition_QParam()->GetX() << std::endl;
          }

        } else {
          outfile << scintStart->EstimateHitPosition_Param()->GetZ() << ","
                  << scint->EstimateHitPosition_Param()->GetZ() << "," << scintEnd->EstimateHitPosition_Param()->GetZ()
                  << std::endl;
        }
        /*if(qparam)
                outfile << scintStart->EstimateHitPosition_Param()->GetZ() << ","
                        << scintEnd->EstimateHitPosition_Param()->GetZ() << "," <<
        scint->EstimateHitPosition_Param()->GetZ()
                        << "," << scint->GetDelTCorrected() << "," << scint->GetBarIndex() << std::endl;
        else
             outfile << scintStart->EstimateHitPosition_QParam()->GetZ() << ","
                        << scintEnd->EstimateHitPosition_QParam()->GetZ() << "," <<
        scint->EstimateHitPosition_QParam()->GetZ()
                        << "," << scint->GetDelTCorrected() << "," << scint->GetBarIndex() << std::endl;
        */
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

/* Funtion to generate the Model to predict angle */
#if (1)
void GenerateModelForAngle(std::string filename, unsigned int inspectedLayerIndex, std::string outfileName)
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
                  << scintEnd->GetExactHitPosition()->GetZ() << "," << scint->GetExactHitPosition()->GetZ() << ","
                  << smtVec[i]->GetZenithAngle_ExactHitPoint() << std::endl;
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

#if (1)
void GenerateData_GeometricalBoundation(std::string filename, unsigned int inspectedLayerIndex, unsigned int startIndex,
                                        unsigned int endIndex)
{
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename);
  std::cout << "Size of SMTVec : " << smtVec.size() << std::endl;
  std::string matWithExt = filename.substr(13);
  matWithExt             = matWithExt.substr(0, matWithExt.find("."));
  matWithExt += ".txt";
  std::string outfileName = ("Layer_" + std::to_string(inspectedLayerIndex) + "_Geometrical_" + matWithExt);
  std::cout << "OutfileName : " << outfileName << std::endl;
  // return;
  std::ofstream outfile(outfileName);
  std::ofstream outfile2("interpolated_Layer_Geometrical_" + std::to_string(inspectedLayerIndex) + ".txt");
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    if (smtVec[i]->HitInRequiredLayers() && smtVec[i]->SingleHitInEachLayer()) {

      unsigned int hittBarIndex = 10000;
      /*ushort startIndex         = GetStartIndex(inspectedLayerIndex);
      ushort endIndex           = GetEndIndex(inspectedLayerIndex);*/
      bool check = smtVec[i]->CheckTrackForLayerNum(startIndex, hittBarIndex);
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
        // if (fingerCondition)
#endif
        {
#ifdef USE_FOR_SIMULATION
          /*outfile << scintStart->GetExactHitPosition()->GetZ() << "," << scintEnd->GetExactHitPosition()->GetZ() <<
             ","
                  << scint->GetExactHitPosition()->GetZ() << std::endl;*/

          // Commenting for the time being

          if (vecOfLayersOrientation[inspectedLayerIndex]) {
            outfile << inspectedLayerIndex << "," << scint->GetBarIndex() << ","
                    << scint->GetLogQNearByQFar_ForSimulation() << "," << scint->GetDelTCorrected() << ","
                    << (scint->GetSmearedHitPosition())->GetX() << ","
                    << GetGaussianRandomSample(vecOfScintXYCenter[scintStart->GetBarIndex()].x * 10, 50.)
                    << "," //(scintStart->GetSmearedHitPosition())->GetZ() << ","
                    //<< scint->GetSmearedHitPosition()->GetX() << "," << scint->GetSmearedHitPosition()->GetZ() << ","
                    << (scint->GetExactHitPosition())->GetX() << "," << (scint->GetExactHitPosition())->GetZ()
                    << std::endl;
          } else {
          }

          /*if (!vecOfLayersOrientation[inspectedLayerIndex]) {
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
          }*/
#else
          // Commenting for the time being, otherwiser working logic
          /*if (qparam)
            outfile << scintStart->EstimateHitPosition_QParam()->GetZ() << ","
                    << scint->EstimateHitPosition_QParam()->GetZ() << ","
                    << scintEnd->EstimateHitPosition_QParam()->GetZ() << std::endl;
          else
            outfile << scintStart->EstimateHitPosition_Param()->GetZ() << ","
                    << scint->EstimateHitPosition_Param()->GetZ() << ","
                    << scintEnd->EstimateHitPosition_Param()->GetZ() << std::endl;
                    // << "," << scint->EstimateHitPosition_Param()->GetZ() << std::endl;
          lite_interface::Point3D *hitPt = Get3DHitPointOnLayer(smtVec[i], inspectedLayerIndex);
          outfile2 << hitPt->GetX() << "," << hitPt->GetZ() << std::endl;*/

#endif
        }
      }
    }
  }
  outfile.close();
  outfile2.close();
}
#endif

/* Funtion to generate the Training and testing data for all layers
   Working nicely for layer 7 & 9, needs to be thoroghly scrutinize
 */
#if (1)
void GenerateTrainingData_All_V2(std::string filename, unsigned int inspectedLayerIndex, std::string outfileName)
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
            /*outfile << scintStart->GetExactHitPosition()->GetZ() << "," << scint->GetExactHitPosition()->GetZ() << ","
                    << scintEnd->GetExactHitPosition()->GetX() << "," << scint->GetExactHitPosition()->GetX()
                    << std::endl;*/
            /*outfile << scintStart->GetExactHitPosition()->GetX() << "," << scintStart->GetExactHitPosition()->GetY()
                                << "," << scintStart->GetExactHitPosition()->GetZ() << ","
                                << scint->GetExactHitPosition()->GetY() << "," << scint->GetExactHitPosition()->GetZ()
               << ","
                                << scintEnd->GetExactHitPosition()->GetX() << "," <<
               scintEnd->GetExactHitPosition()->GetY() << ","
                                << scintEnd->GetExactHitPosition()->GetZ() << "," <<
               scint->GetExactHitPosition()->GetX()
                                << std::endl;
            */ /*outfile << scintStart->GetExactHitPosition()->GetX() << "," << scintStart->GetExactHitPosition()->GetY()
                    << "," << scintStart->GetExactHitPosition()->GetZ() << ","
                    << scint->GetExactHitPosition()->GetY() << "," << scint->GetExactHitPosition()->GetZ() << ","
                    << scintEnd->GetExactHitPosition()->GetX() << "," << scintEnd->GetExactHitPosition()->GetY() << ","
                    << scintEnd->GetExactHitPosition()->GetZ() << "," << scint->GetExactHitPosition()->GetX()
                    << std::endl;*/
            outfile << scintStart->GetExactHitPosition()->GetZ() << "," << GetYOfLayer(scintStart->GetLayerIndex())
                    << "," << scint->GetExactHitPosition()->GetZ() << "," << GetYOfLayer(scint->GetLayerIndex()) << ","
                    << scintEnd->GetExactHitPosition()->GetX() << "," << GetYOfLayer(scintEnd->GetLayerIndex()) << ","
                    << scint->GetExactHitPosition()->GetX() << std::endl;

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
