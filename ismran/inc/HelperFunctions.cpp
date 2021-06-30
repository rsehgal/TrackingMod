/*
 * HelperFunctions.cpp
 *
 *  Created on: 16-Sep-2020
 *      Author: rsehgal
 */

#include "HelperFunctions.h"
#include "PsBar.h"
#include "includes.hh"
#include <TMath.h>
#include "HardwareNomenclature.h"
#include <bits/stdc++.h>
#include "ScintillatorBar_V2.h"
#include <TSystemDirectory.h>
#include <TList.h>
#include <TSystemFile.h>

void AutoCallers()
{
#ifdef USE_FOR_SIMULATION
  IsSimulation = true;
#endif
}

std::vector<std::vector<unsigned long int>> myhist2D;
#if (0)
lite_interface::Point3D *Get3DHitPointOnLayer(lite_interface::SingleMuonTrack *smt, unsigned int layerIndex)
{
  lite_interface::Point3D *hitPointInInspectedLayer = new lite_interface::Point3D(10000., 10000., 10000.);

  // std::vector<unsigned int>
  // if(smt->IfPassThroughOneOrMoreOfScintillators())
  {
    if (smt->HitInRequiredLayers()) {
      if (layerIndex < numOfLayers - 1) {
        /*ushort belowIndex = 0;
        ushort upperIndex = 0;*/
        ushort startIndex = 0;
        ushort endIndex   = 0;
        if (layerIndex == 0) {
          /*belowIndex = layerIndex+1;
          upperIndex = layerIndex+3;*/
          startIndex = layerIndex + 3;
          endIndex   = layerIndex + 1;
        } else {
          if (layerIndex == 5 || layerIndex == 4) {
            /*belowIndex = layerIndex-3;
            upperIndex = layerIndex-1;*/

            /*upperIndex = layerIndex-3;
            belowIndex = layerIndex-1;*/
            startIndex = layerIndex - 3;
            endIndex   = layerIndex - 1;
          } else {
            /*belowIndex = layerIndex-1;
            upperIndex = layerIndex+1;*/
            startIndex = layerIndex + 1;
            endIndex   = layerIndex - 1;
          }
        }
        if (smt->SingleHitInLayer(startIndex) && smt->SingleHitInLayer(endIndex)) {
          unsigned int barIndexInInspectedLayer = 100000;
          /*unsigned int barIndexInBelowLayer = 100000;
          unsigned int barIndexInUpperLayer = 100000;*/
          unsigned int barIndexInStartLayer = 100000;
          unsigned int barIndexInEndLayer   = 100000;

          /*if(smt->CheckTrackForLayerNum(layerIndex,barIndexInInspectedLayer) &&
          smt->CheckTrackForLayerNum(belowIndex,barIndexInBelowLayer) &&
          smt->CheckTrackForLayerNum(upperIndex,barIndexInUpperLayer) )*/
          if (smt->CheckTrackForLayerNum(layerIndex, barIndexInInspectedLayer) &&
              smt->CheckTrackForLayerNum(startIndex, barIndexInStartLayer) &&
              smt->CheckTrackForLayerNum(endIndex, barIndexInEndLayer)) {
            lite_interface::ScintillatorBar_V2 *scintillatorInInspectedLayer =
                smt->GetScintillator(barIndexInInspectedLayer);
            /*lite_interface::ScintillatorBar_V2 *scintillatorInBelowLayer = smt->GetScintillator(barIndexInBelowLayer);
            lite_interface::ScintillatorBar_V2 *scintillatorInUpperLayer = smt->GetScintillator(barIndexInUpperLayer);*/
            lite_interface::ScintillatorBar_V2 *scintillatorInStartLayer = smt->GetScintillator(barIndexInStartLayer);
            lite_interface::ScintillatorBar_V2 *scintillatorInEndLayer   = smt->GetScintillator(barIndexInEndLayer);
            // std::cout << "BaR InDeX : " << scintillatorInUpperLayer->GetBarIndex() << " : " << __FILE__ << " : " <<
            // __LINE__ << std::endl;

            /*lite_interface::Point3D *hitPointInBelowLayer = scintillatorInBelowLayer->EstimateHitPosition_Param();
            lite_interface::Point3D *hitPointInUpperLayer = scintillatorInUpperLayer->EstimateHitPosition_Param();*/
            /*lite_interface::Point3D *startPt = scintillatorInBelowLayer->EstimateHitPosition_Param();
            lite_interface::Point3D *endPt = scintillatorInUpperLayer->EstimateHitPosition_Param();*/
            lite_interface::Point3D *startPt = scintillatorInStartLayer->EstimateHitPosition_Param();
            lite_interface::Point3D *endPt   = scintillatorInEndLayer->EstimateHitPosition_Param();
            hitPointInInspectedLayer         = scintillatorInInspectedLayer->EstimateHitPosition_Param();

            /*double xOrZ = Interpolate(Point2D(hitPointInBelowLayer->GetZ(),hitPointInBelowLayer->GetY()) ,
                          Point2D(hitPointInUpperLayer->GetZ(),hitPointInUpperLayer->GetY()) ,
                          hitPointInInspectedLayer->GetY());*/
            double xOrZ = Interpolate(startPt, endPt, hitPointInInspectedLayer);

            if (layerIndex == 1 || layerIndex == 3 || layerIndex == 5 || layerIndex == 8) {
              // Cross Layers

              hitPointInInspectedLayer->SetXYZ(hitPointInInspectedLayer->GetZ(), hitPointInInspectedLayer->GetY(),
                                               xOrZ);
              // hitPointInInspectedLayer->SetXYZ(xOrZ,hitPointInInspectedLayer->GetY(),hitPointInInspectedLayer->GetZ());

              // hitPointInInspectedLayer.SetZ(xOrZ);
            } else {
              // Oblong layers
              // hitPointInInspectedLayer.SetX(xOrZ);

              hitPointInInspectedLayer->SetXYZ(xOrZ, hitPointInInspectedLayer->GetY(),
                                               hitPointInInspectedLayer->GetZ());
              // hitPointInInspectedLayer->SetXYZ(hitPointInInspectedLayer->GetZ(),hitPointInInspectedLayer->GetY(),xOrZ);
            }
          }
        }
      }
    }
  }
  return hitPointInInspectedLayer;
}
#endif

std::vector<lite_interface::SingleMuonTrack *> GetMuonTracksVector(std::string filename, unsigned int numOfTracks)
{
  lite_interface::SingleMuonTrack *smt = new lite_interface::SingleMuonTrack;

  // lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
  // std::string filename = argv[1];

  // std::string subFileName = filename.substr(13);

  TFile *trackFile = new TFile(filename.c_str(), "READ");
  TTree *trackTree = (TTree *)trackFile->Get("TracksTree");
  trackTree->SetBranchAddress("MuonTracks", &smt);

  Long64_t nentries = trackTree->GetEntries();

  Long64_t nbytes = 0;

  std::vector<lite_interface::SingleMuonTrack *> smtVec;

  int counter              = 0;
  unsigned int testCounter = 0;

  if (numOfTracks > 0) nentries = numOfTracks;
  Long64_t i = 0;
  for (i = 0; i < nentries; i++) {

    nbytes += trackTree->GetEntry(i);
    if (!(i % 1000000) && i != 0) {
      std::cout << "Processed : " << i << " Tracks ........" << std::endl;
      testCounter++;
      /*if(testCounter==2)
              break;*/
    }
    testCounter++;
    // if(testCounter==19500001)
    //      break;

    smtVec.push_back(new lite_interface::SingleMuonTrack(*smt));
  }
  std::cout << "Number of Muon Tracks Read from GetMuonTracksVector function : " << i << std::endl;

  return smtVec;
}

lite_interface::Point3D *ExtrapolatePointOnLayer(lite_interface::Point3D *startPt, lite_interface::Point3D *endPt,
                                                 unsigned int layerIndex)
{

  Tracking::Vector3D<double> start(startPt->GetX(), startPt->GetY(), startPt->GetZ());
  Tracking::Vector3D<double> end(endPt->GetX(), endPt->GetY(), endPt->GetZ());
  /*Tracking::Vector3D<double> dir = (end-start).Unit();
  double dist = (GetYOfLayer(1)-hitPoint->GetY())/dir.y();
          double xExtraPolated = hitPoint->GetX()+dir.x()*dist;
          double zExtraPolated = hitPoint->GetZ()+dir.z()*dist;*/
  Tracking::Vector3D<double> extraPolatedPt = ExtrapolatePointOnLayer(start, end, layerIndex);
  return (new lite_interface::Point3D(extraPolatedPt.x(), extraPolatedPt.y(), extraPolatedPt.z()));
}

lite_interface::Point3D *ExtrapolatePointOnLayer(lite_interface::Point3D *startPt, lite_interface::Point3D *endPt,
                                                 lite_interface::Point3D *intermediatePt, unsigned int layerIndex)
{

  Tracking::Vector3D<double> start(startPt->GetX(), startPt->GetY(), startPt->GetZ());
  Tracking::Vector3D<double> end(endPt->GetX(), endPt->GetY(), endPt->GetZ());
  Tracking::Vector3D<double> intermediate(intermediatePt->GetX(), intermediatePt->GetY(), intermediatePt->GetZ());
  /*Tracking::Vector3D<double> dir = (end-start).Unit();
  double dist = (GetYOfLayer(1)-hitPoint->GetY())/dir.y();
          double xExtraPolated = hitPoint->GetX()+dir.x()*dist;
          double zExtraPolated = hitPoint->GetZ()+dir.z()*dist;*/
  Tracking::Vector3D<double> extraPolatedPt = ExtrapolatePointOnLayer(start, end, intermediate, layerIndex);
  return (new lite_interface::Point3D(extraPolatedPt.x(), extraPolatedPt.y(), extraPolatedPt.z()));
}

Tracking::Vector3D<double> ExtrapolatePointOnLayer(Tracking::Vector3D<double> start, Tracking::Vector3D<double> end,
                                                   unsigned int layerIndex)
{
  /*Tracking::Vector3D<double> dir = (end-start).Unit();
  double yOnLayer = GetYOfLayer(layerIndex);
  double dist = (yOnLayer-start.y())/dir.y();
  double xExtraPolated = start.x()+dir.x()*dist;
  double zExtraPolated = start.z()+dir.z()*dist;
  return Tracking::Vector3D<double>(xExtraPolated,yOnLayer,zExtraPolated);
  */
  double yOnLayer = GetYOfLayer(layerIndex);
  return ExtrapolatePointOnLayer(start, end, yOnLayer);
}

Tracking::Vector3D<double> ExtrapolatePointOnLayer(Tracking::Vector3D<double> start, Tracking::Vector3D<double> end,
                                                   double destinationYval)
{
  Tracking::Vector3D<double> dir = (end - start).Unit();
  std::cout << "Direction =============== : ";
  dir.Print();
  double yOnLayer = destinationYval;
  double dist     = (yOnLayer - start.y()) / dir.y();
  std::cout << "Dist : " << dist << std::endl;
  double xExtraPolated = start.x() + dir.x() * dist;
  double zExtraPolated = start.z() + dir.z() * dist;
  return Tracking::Vector3D<double>(xExtraPolated, yOnLayer, zExtraPolated);
}

lite_interface::Point3D *ExtrapolatePointOnLayer(lite_interface::Point3D *startPt, lite_interface::Point3D *endPt,
                                                 double destinationYval)
{
  Tracking::Vector3D<double> start(startPt->GetX(), startPt->GetY(), startPt->GetZ());
  Tracking::Vector3D<double> end(endPt->GetX(), endPt->GetY(), endPt->GetZ());
  Tracking::Vector3D<double> extraPolatedPt = ExtrapolatePointOnLayer(start, end, destinationYval);
  return (new lite_interface::Point3D(extraPolatedPt.x(), extraPolatedPt.y(), extraPolatedPt.z()));
}

Tracking::Vector3D<double> ExtrapolatePointOnLayer(Tracking::Vector3D<double> start, Tracking::Vector3D<double> end,
                                                   Tracking::Vector3D<double> intermediate, unsigned int layerIndex)
{
  Tracking::Vector3D<double> dir = (end - start).Unit();
  double yOnLayer                = GetYOfLayer(layerIndex);
  double dist                    = (yOnLayer - intermediate.y()) / dir.y();
  double xExtraPolated           = intermediate.x() + dir.x() * dist;
  double zExtraPolated           = intermediate.z() + dir.z() * dist;
  return Tracking::Vector3D<double>(xExtraPolated, yOnLayer, zExtraPolated);
}

unsigned int NumOfEventsToRead(std::string file1, std::string file2, std::string treeName)
{
  TFile *fp1                    = new TFile(file1.c_str(), "READ");
  TFile *fp2                    = new TFile(file2.c_str(), "READ");
  unsigned int numOfEventsFile1 = ((TTree *)fp1->Get(treeName.c_str()))->GetEntries();
  unsigned int numOfEventsFile2 = ((TTree *)fp2->Get(treeName.c_str()))->GetEntries();
  if (numOfEventsFile1 < numOfEventsFile2)
    return numOfEventsFile1;
  else
    return numOfEventsFile2;
}

ushort GetStartIndex(unsigned int layerIndex)
{
  return GetStartEndIndex(layerIndex, true);
}
ushort GetEndIndex(unsigned int layerIndex)
{
  return GetStartEndIndex(layerIndex, false);
}

ushort GetStartEndIndex(unsigned int layerIndex, bool start)
{
  ushort startIndex = 0;
  ushort endIndex   = 0;

  if (layerIndex == 0) {
    startIndex = layerIndex + 3;
    endIndex   = layerIndex + 1;
  } else {
    if (layerIndex == 5 || layerIndex == 4) {
      startIndex = layerIndex - 3;
      endIndex   = layerIndex - 1;
    } else {
      if (layerIndex == 7) {
        startIndex = 9;
        endIndex   = 8;
      } else {
        if (layerIndex == 9) {
          startIndex = 7;
          endIndex   = 8;
        } else {
          startIndex = layerIndex + 1;
          endIndex   = layerIndex - 1;
        }
      }
    }
  }
  if (start)
    return startIndex;
  else
    return endIndex;
}

lite_interface::Point3D *Get3DHitPointOnLayer(lite_interface::SingleMuonTrack *smt, unsigned int layerIndex)
{
  lite_interface::Point3D *hitPointInInspectedLayer = new lite_interface::Point3D(10000., 10000., 10000.);

  // std::vector<unsigned int>
  // if(smt->IfPassThroughOneOrMoreOfScintillators())
  {
    if (smt->HitInRequiredLayers()) {
      if (layerIndex < numOfLayers - 1) {
        ushort startIndex = 0;
        ushort endIndex   = 0;
        if (layerIndex == 0) {
          startIndex = layerIndex + 3;
          endIndex   = layerIndex + 1;
        } else {
          if (layerIndex == 5 || layerIndex == 4) {
            startIndex = layerIndex - 3;
            endIndex   = layerIndex - 1;
          } else {
            startIndex = layerIndex + 1;
            endIndex   = layerIndex - 1;
          }
        }
        if (smt->SingleHitInLayer(startIndex) && smt->SingleHitInLayer(endIndex) && smt->SingleHitInLayer(layerIndex)) {
          unsigned int barIndexInInspectedLayer = 100000;
          unsigned int barIndexInStartLayer     = 100000;
          unsigned int barIndexInEndLayer       = 100000;

          if (smt->CheckTrackForLayerNum(layerIndex, barIndexInInspectedLayer) &&
              smt->CheckTrackForLayerNum(startIndex, barIndexInStartLayer) &&
              smt->CheckTrackForLayerNum(endIndex, barIndexInEndLayer)) {
            lite_interface::ScintillatorBar_V2 *scintillatorInInspectedLayer =
                smt->GetScintillator(barIndexInInspectedLayer);
            lite_interface::ScintillatorBar_V2 *scintillatorInStartLayer = smt->GetScintillator(barIndexInStartLayer);
            lite_interface::ScintillatorBar_V2 *scintillatorInEndLayer   = smt->GetScintillator(barIndexInEndLayer);
            // std::cout << "BaR InDeX : " << scintillatorInUpperLayer->GetBarIndex() << " : " << __FILE__ << " : " <<
            // __LINE__ << std::endl;

            lite_interface::Point3D *startPt = scintillatorInStartLayer->EstimateHitPosition_Param();
            lite_interface::Point3D *endPt   = scintillatorInEndLayer->EstimateHitPosition_Param();
            hitPointInInspectedLayer         = scintillatorInInspectedLayer->EstimateHitPosition_Param();

            double xOrZ = Interpolate(startPt, endPt, hitPointInInspectedLayer);

            if (layerIndex == 1 || layerIndex == 3 || layerIndex == 5 || layerIndex == 8) {
              // Cross Layers

              hitPointInInspectedLayer->SetXYZ(hitPointInInspectedLayer->GetZ(), hitPointInInspectedLayer->GetY(),
                                               xOrZ);
              // hitPointInInspectedLayer->SetXYZ(xOrZ,hitPointInInspectedLayer->GetY(),hitPointInInspectedLayer->GetZ());
            } else {
              // Oblong layers
              hitPointInInspectedLayer->SetXYZ(xOrZ, hitPointInInspectedLayer->GetY(),
                                               hitPointInInspectedLayer->GetZ());
              // hitPointInInspectedLayer->SetXYZ(hitPointInInspectedLayer->GetZ(),hitPointInInspectedLayer->GetY(),xOrZ);
            }
          }
        }
      }
    }
  }
  return hitPointInInspectedLayer;
}

TVector3 GetTVector3(lite_interface::Point3D *start, lite_interface::Point3D *end)
{
  return TVector3(end->GetX() - start->GetX(), end->GetY() - start->GetY(), end->GetZ() - start->GetZ());
}

TVector3 GetTVector3(lite_interface::Point3D *pt)
{
  return TVector3(pt->GetX(), pt->GetY(), pt->GetZ());
}

std::vector<lite_interface::Point3D *> GetTrackFromLayers(lite_interface::SingleMuonTrack *smt,
                                                          std::vector<unsigned int> vecOfLayerIndex)
{
  std::vector<lite_interface::Point3D *> refinedTrack;
  if (smt->HitInRequiredLayers()) {
    for (unsigned int i = 0; i < vecOfLayerIndex.size(); i++) {
      refinedTrack.push_back(Get3DHitPointOnLayer_Refined(smt, vecOfLayerIndex[i]));
    }
  }
  return refinedTrack;
}

lite_interface::Point3D *GetHitPointOnLayer_FromParam(lite_interface::SingleMuonTrack *smt, unsigned int layerIndex)
{
  ushort startIndex = GetStartIndex(layerIndex);
  ushort endIndex   = GetEndIndex(layerIndex);
  if (smt->SingleHitInLayer(layerIndex)) {
    unsigned int barIndexInInspectedLayer = 100000;
    if (smt->CheckTrackForLayerNum(layerIndex, barIndexInInspectedLayer)) {
      lite_interface::ScintillatorBar_V2 *scintillatorInInspectedLayer = smt->GetScintillator(barIndexInInspectedLayer);
      lite_interface::Point3D *hiPt_Param = scintillatorInInspectedLayer->EstimateHitPosition_Param();
      return hiPt_Param;
    }
  }
  return NULL;
}

lite_interface::Point3D *Get3DHitPointOnLayer_Refined(lite_interface::SingleMuonTrack *smt, unsigned int layerIndex)
{
  ushort startIndex                                 = GetStartIndex(layerIndex);
  ushort endIndex                                   = GetEndIndex(layerIndex);
  lite_interface::Point3D *startPt                  = new lite_interface::Point3D(10000., 10000., 10000.);
  lite_interface::Point3D *endPt                    = new lite_interface::Point3D(10000., 10000., 10000.);
  lite_interface::Point3D *hitPointInInspectedLayer = new lite_interface::Point3D(10000., 10000., 10000.);
  if (layerIndex == 9 || layerIndex == 7) {
    if (layerIndex == 9 || layerIndex == 7) {
      lite_interface::Point3D *temp = Get3DHitPointOnLayer(smt, 8);
      if (temp != NULL) endPt->SetXYZ(temp->GetX(), temp->GetY(), temp->GetZ());
      // endPt->SetXYZ(temp->GetZ(),temp->GetY(),temp->GetX());
    }
    if (layerIndex == 9) {
      lite_interface::Point3D *temp = GetHitPointOnLayer_FromParam(smt, 7);
      if (temp != NULL) startPt->SetXYZ(temp->GetX(), temp->GetY(), temp->GetZ());
    }
    if (layerIndex == 7) {
      lite_interface::Point3D *temp = GetHitPointOnLayer_FromParam(smt, 9);
      if (temp != NULL) startPt->SetXYZ(temp->GetX(), temp->GetY(), temp->GetZ());
    }

    /*lite_interface::Point3D* temp = ExtrapolatePointOnLayer(startPt,endPt,layerIndex);
    hitPointInInspectedLayer->SetXYZ(temp->GetX(),temp->GetY(),temp->GetZ());*/

    lite_interface::Point3D *temp = GetHitPointOnLayer_FromParam(smt, layerIndex);
    if (temp != NULL) hitPointInInspectedLayer->SetXYZ(temp->GetX(), temp->GetY(), temp->GetZ());

    double xOrZ = Interpolate(startPt, endPt, hitPointInInspectedLayer);
    if (layerIndex == 1 || layerIndex == 3 || layerIndex == 5 || layerIndex == 8) {
      // Cross Layers
      hitPointInInspectedLayer->SetXYZ(hitPointInInspectedLayer->GetZ(), hitPointInInspectedLayer->GetY(), xOrZ);
    } else {
      // Oblong layers
      hitPointInInspectedLayer->SetXYZ(xOrZ, hitPointInInspectedLayer->GetY(), hitPointInInspectedLayer->GetZ());
    }
  } else {
    lite_interface::Point3D *temp = Get3DHitPointOnLayer(smt, layerIndex);
    if (temp != NULL) hitPointInInspectedLayer->SetXYZ(temp->GetX(), temp->GetY(), temp->GetZ());
  }
  return hitPointInInspectedLayer;
}

lite_interface::Point3D *Get3DHitPointOnLayer(lite_interface::SingleMuonTrack *smt, unsigned int layerIndex,
                                              lite_interface::Point3D *extrapolatedPt, unsigned int onLayer)
{
  lite_interface::Point3D *hitPointInInspectedLayer = new lite_interface::Point3D(10000., 10000., 10000.);

  // std::vector<unsigned int>
  // if(smt->IfPassThroughOneOrMoreOfScintillators())
  {
    if (smt->HitInRequiredLayers()) {
      if (layerIndex < numOfLayers - 1) {
        ushort startIndex = 0;
        ushort endIndex   = 0;
        if (layerIndex == 0) {
          startIndex = layerIndex + 3;
          endIndex   = layerIndex + 1;
        } else {
          if (layerIndex == 5 || layerIndex == 4) {
            startIndex = layerIndex - 3;
            endIndex   = layerIndex - 1;
          } else {
            startIndex = layerIndex + 1;
            endIndex   = layerIndex - 1;
          }
        }

        lite_interface::Point3D *startPt = new lite_interface::Point3D(10000., 10000., 10000.);
        lite_interface::Point3D *endPt   = new lite_interface::Point3D(10000., 10000., 10000.);
        if (smt->SingleHitInLayer(startIndex) && smt->SingleHitInLayer(endIndex) && smt->SingleHitInLayer(layerIndex)) {
          unsigned int barIndexInInspectedLayer = 100000;
          unsigned int barIndexInStartLayer     = 100000;
          unsigned int barIndexInEndLayer       = 100000;

          if (smt->CheckTrackForLayerNum(layerIndex, barIndexInInspectedLayer) &&
              smt->CheckTrackForLayerNum(startIndex, barIndexInStartLayer) &&
              smt->CheckTrackForLayerNum(endIndex, barIndexInEndLayer)) {
            lite_interface::ScintillatorBar_V2 *scintillatorInInspectedLayer =
                smt->GetScintillator(barIndexInInspectedLayer);
            lite_interface::ScintillatorBar_V2 *scintillatorInStartLayer = smt->GetScintillator(barIndexInStartLayer);
            lite_interface::ScintillatorBar_V2 *scintillatorInEndLayer   = smt->GetScintillator(barIndexInEndLayer);
            // std::cout << "BaR InDeX : " << scintillatorInUpperLayer->GetBarIndex() << " : " << __FILE__ << " : " <<
            // __LINE__ << std::endl;

            startPt                  = scintillatorInStartLayer->EstimateHitPosition_Param();
            endPt                    = scintillatorInEndLayer->EstimateHitPosition_Param();
            hitPointInInspectedLayer = scintillatorInInspectedLayer->EstimateHitPosition_Param();

            double xOrZ = Interpolate(startPt, endPt, hitPointInInspectedLayer);

            if (layerIndex == 1 || layerIndex == 3 || layerIndex == 5 || layerIndex == 8) {
              // Cross Layers

              hitPointInInspectedLayer->SetXYZ(hitPointInInspectedLayer->GetZ(), hitPointInInspectedLayer->GetY(),
                                               xOrZ);
            } else {
              // Oblong layers
              hitPointInInspectedLayer->SetXYZ(xOrZ, hitPointInInspectedLayer->GetY(),
                                               hitPointInInspectedLayer->GetZ());
            }
          }
        }
        lite_interface::Point3D *extPolPt = ExtrapolatePointOnLayer(startPt, endPt, hitPointInInspectedLayer, onLayer);
        extrapolatedPt->SetXYZ(extPolPt->GetX(), extPolPt->GetY(), extPolPt->GetZ());
        // std::cout << "EXTRAPOLATED POINT : " ; extrapolatedPt->Print();
        return hitPointInInspectedLayer;
      }
    }
  }
}

lite_interface::Point3D *Get3DHitPointOnLayer(lite_interface::SingleMuonTrack *smt, unsigned int layerIndex,
                                              lite_interface::Point3D *extrapolatedPt, unsigned int onLayer,
                                              double incomingAngle)
{
  lite_interface::Point3D *hitPointInInspectedLayer = new lite_interface::Point3D(10000., 10000., 10000.);

  // std::vector<unsigned int>
  // if(smt->IfPassThroughOneOrMoreOfScintillators())
  {
    if (smt->HitInRequiredLayers()) {
      if (layerIndex < numOfLayers - 1) {
        ushort startIndex = 0;
        ushort endIndex   = 0;
        if (layerIndex == 0) {
          startIndex = layerIndex + 3;
          endIndex   = layerIndex + 1;
        } else {
          if (layerIndex == 5 || layerIndex == 4) {
            startIndex = layerIndex - 3;
            endIndex   = layerIndex - 1;
          } else {
            startIndex = layerIndex + 1;
            endIndex   = layerIndex - 1;
          }
        }

        lite_interface::Point3D *startPt = new lite_interface::Point3D(10000., 10000., 10000.);
        lite_interface::Point3D *endPt   = new lite_interface::Point3D(10000., 10000., 10000.);
        if (smt->SingleHitInLayer(startIndex) && smt->SingleHitInLayer(endIndex) && smt->SingleHitInLayer(layerIndex)) {
          unsigned int barIndexInInspectedLayer = 100000;
          unsigned int barIndexInStartLayer     = 100000;
          unsigned int barIndexInEndLayer       = 100000;

          if (smt->CheckTrackForLayerNum(layerIndex, barIndexInInspectedLayer) &&
              smt->CheckTrackForLayerNum(startIndex, barIndexInStartLayer) &&
              smt->CheckTrackForLayerNum(endIndex, barIndexInEndLayer)) {
            lite_interface::ScintillatorBar_V2 *scintillatorInInspectedLayer =
                smt->GetScintillator(barIndexInInspectedLayer);
            lite_interface::ScintillatorBar_V2 *scintillatorInStartLayer = smt->GetScintillator(barIndexInStartLayer);
            lite_interface::ScintillatorBar_V2 *scintillatorInEndLayer   = smt->GetScintillator(barIndexInEndLayer);
            // std::cout << "BaR InDeX : " << scintillatorInUpperLayer->GetBarIndex() << " : " << __FILE__ << " : " <<
            // __LINE__ << std::endl;

            startPt                  = scintillatorInStartLayer->EstimateHitPosition_Param();
            endPt                    = scintillatorInEndLayer->EstimateHitPosition_Param();
            hitPointInInspectedLayer = scintillatorInInspectedLayer->EstimateHitPosition_Param();

            double xOrZ = Interpolate(startPt, endPt, hitPointInInspectedLayer);

            if (layerIndex == 1 || layerIndex == 3 || layerIndex == 5 || layerIndex == 8) {
              // Cross Layers

              hitPointInInspectedLayer->SetXYZ(hitPointInInspectedLayer->GetZ(), hitPointInInspectedLayer->GetY(),
                                               xOrZ);
            } else {
              // Oblong layers
              hitPointInInspectedLayer->SetXYZ(xOrZ, hitPointInInspectedLayer->GetY(),
                                               hitPointInInspectedLayer->GetZ());
            }
          }
        }
        lite_interface::Point3D *extPolPt = ExtrapolatePointOnLayer(startPt, endPt, hitPointInInspectedLayer, onLayer);
        extrapolatedPt->SetXYZ(extPolPt->GetX(), extPolPt->GetY(), extPolPt->GetZ());
        // std::cout << "EXTRAPOLATED POINT : " ; extrapolatedPt->Print();
        return hitPointInInspectedLayer;
      }
    }
  }
}

bool CheckRange(std::vector<Point3D *> singleMuonTrack)
{
  for (unsigned int i = 0; i < singleMuonTrack.size(); i++) {
    if (std::isinf(singleMuonTrack[i]->x) || std::isinf(singleMuonTrack[i]->y) || std::isinf(singleMuonTrack[i]->z)
        // || (abs((singleMuonTrack[i]->x) <= 45.) ) || (abs((singleMuonTrack[i]->y) <= 45.) ) ||
        // (abs((singleMuonTrack[i]->z) <= 50.) )
    ) {
      return true;
    }
  }
  return false;
}

void HistInitializer()
{
  for (unsigned int i = 0; i < numOfLayers; i++) {
    std::vector<unsigned long int> yvec;
    for (unsigned int j = 0; j < numOfBarsInEachLayer; j++) {
      yvec.push_back(0);
    }
    myhist2D.push_back(yvec);
  }
}

void Fill2DHist(unsigned int x, unsigned int y)
{
  /*unsigned long int count = myhist2D[x][y];
  count++;
  myhist2D[x][y] = count;*/
  myhist2D[x][y]++;
}

Tracking::Vector3D<double> ConvertToTomoVector3D(Point3D pt)
{
  return Tracking::Vector3D<double>(pt.x, pt.y, pt.z);
}

/*
template<typename T>
void ResetVector(std::vector<T*> vecToReset){
  if(vecToReset.size()!=0){
    for(unsigned int i = 0 ; i < vecToReset.size() ; i++){
      delete vecToReset[i];
    }
  }
  vecToReset.clear();
}
*/

double LinearFit(Double_t *x, Double_t *par)
{

  double fitval = par[0] + par[1] * x[0];
  return fitval;
}

double Pol2(Double_t *x, Double_t *par)
{

  double fitval = par[0] + par[1] * x[0] + par[2] * x[0] * x[0];
  return fitval;
}

double Pol3(Double_t *x, Double_t *par)
{

  double fitval = par[0] + par[1] * x[0] + par[2] * pow(x[0], 2) + par[3] * pow(x[0], 3);
  return fitval;
}

double Cos2ThetaFit(Double_t *x, Double_t *par)
{
  double fitval = par[0] * pow(cos(x[0]), par[1]);
  return fitval;
}

double NewCos2ThetaFit(Double_t *x, Double_t *par)
{
  double fitval = par[0] + par[1] * pow(cos(par[2] * x[0] + par[3]), 2);
  return fitval;
}

// std::vector<double> GetFittedXorZ(TGraph *gr){
void PlotFittedLine(TGraph *gr)
{
  TF1 *formula = new TF1("Formula", LinearFit, -45, 45, 2);
  gr->Fit(formula, "rq");
  double c = formula->GetParameter(0);
  double m = formula->GetParameter(1);
  delete formula;

  /*std::vector<double> vecOfEstimatedXorZ;
  for(unsigned int i = 0 ; i < fSingleMuonTrack.size() ; i++){
    vecOfEstimatedXorZ.push_back( ((fSingleMuonTrack[i]->hitPosition).y - c)/m );
  }
  return vecOfEstimatedXorZ;*/
}

void DrawGrid(std::string t, Int_t ngx, Int_t ngy)
{
  //	std::cout << "DrawGrid Called........: " << t << " : ....." << std::endl;
  // new TCanvas();
  Double_t x1 = -45;
  Double_t x2 = 45;
  Double_t y1 = -45;
  Double_t y2 = 45;
  Double_t xs = (x2 - x1) / ngx;
  Double_t ys = (y2 - y1) / ngy;
  Int_t i;

  TH1F *h = gPad->DrawFrame(x1, y1, x2, y2);
  h->GetXaxis()->SetNdivisions(20);
  h->GetYaxis()->SetNdivisions(20);
  h->GetYaxis()->SetTickLength(0.);
  h->GetXaxis()->SetTickLength(0.);
  h->GetXaxis()->SetLabelSize(0.025);
  h->GetYaxis()->SetLabelSize(0.025);
  h->SetTitle(t.c_str());
  gPad->Update();

  TLine l;
  l.SetLineColor(kGray);
  Double_t x = x1 + xs;
  for (i = 0; i < ngx - 1; i++) {
    l.DrawLine(x, y1, x, y2);
    x = x + xs;
  }

  Double_t y = y1 + xs;
  for (i = 0; i < ngy - 1; i++) {
    l.DrawLine(x1, y, x2, y);
    y = y + ys;
  }
}

TH1D *PlotZenithAngle(std::vector<std::vector<Point3D *>> muonTrackVec, std::string histTitle)
{
  TVector3 ref(0., -1., 0.);
  int numOfBins = 100;
  TH1D *zenithAngleHist;
  // zenithAngleHist = new TH1D("ZenithAngle", "ZenithAngle Distribution",numOfBins,0.,1.5);
  zenithAngleHist    = new TH1D(histTitle.c_str(), histTitle.c_str(), numOfBins, 0., 1.39626);
  int invalidCounter = 0;
  for (unsigned int trackIndex = 0; trackIndex < muonTrackVec.size(); trackIndex++) {
    std::vector<Point3D *> singleMuonTrack = muonTrackVec[trackIndex];
    // std::cout << "============================= Muon Track from : " << histTitle << "==============================="
    // << std::endl;
    /*for (unsigned int i = 0 ; i < singleMuonTrack.size() ; i++){
      singleMuonTrack[i]->Print();
    }*/
    Point3D *startPoint = singleMuonTrack[0];
    Point3D *endPoint   = singleMuonTrack[singleMuonTrack.size() - 1];
    TVector3 muonDir(TVector3(endPoint->x, endPoint->y, endPoint->z) -
                     TVector3(startPoint->x, startPoint->y, startPoint->z));
    double zenitAngle = muonDir.Angle(ref);
    // std::cout <<"Zenith Angle before condition Check for : " << histTitle <<" : " << zenitAngle << std::endl;
    if (zenitAngle > -0.0872665 && zenitAngle < 0.0872665) {

    } else {
      /*if(zenitAngle > 0.96)
      {
        invalidCounter++;
        PrintPoint3DVector(singleMuonTrack);
      }else*/
      {
        if (startPoint->y > 35. && endPoint->y < -35) {
          // std::cout <<"Zenith Angle for : " << histTitle <<" : " << zenitAngle << std::endl;
          zenithAngleHist->Fill(zenitAngle);
        }
      }
    }
  }
  std::cout << std::endl << "Total number of invalid angle tracks : " << invalidCounter << std::endl;
  // TF1 *zenForm = new TF1("zenForm", "[0]*sin(x)*pow(cos(x),[1])", 0.05,M_PI/2.);
  TF1 *zenForm = new TF1("zenForm", "[0]*sin(x)*cos(x)*pow(cos(x),[1])", 0.05, 1.39626);
  // zenithAngleHist->Fit(zenForm,"r");
  // zenithAngleHist->Draw();

  return zenithAngleHist;
}

TH1D *PlotZenithAngle_XY(std::vector<std::vector<Point3D *>> muonTrackVec, std::string histTitle)
{
  TVector3 ref(0., -1., 0.);
  int numOfBins = 100;
  TH1D *zenithAngleHist;
  // zenithAngleHist = new TH1D("ZenithAngle", "ZenithAngle Distribution",numOfBins,0.,1.5);
  zenithAngleHist    = new TH1D(histTitle.c_str(), histTitle.c_str(), numOfBins, 0., 1.39626);
  int invalidCounter = 0;
  for (unsigned int trackIndex = 0; trackIndex < muonTrackVec.size(); trackIndex++) {
    std::vector<Point3D *> singleMuonTrack = muonTrackVec[trackIndex];
    // std::cout << "============================= Muon Track from : " << histTitle << "==============================="
    // << std::endl;
    /*for (unsigned int i = 0 ; i < singleMuonTrack.size() ; i++){
      singleMuonTrack[i]->Print();
    }*/
    Point3D *startPoint = singleMuonTrack[0];
    Point3D *endPoint   = singleMuonTrack[singleMuonTrack.size() - 1];
    TVector3 muonDir(TVector3(endPoint->x, endPoint->y, endPoint->z) -
                     TVector3(startPoint->x, startPoint->y, startPoint->z));
    double zenitAngle = muonDir.Angle(ref);
    // std::cout <<"Zenith Angle before condition Check for : " << histTitle <<" : " << zenitAngle << std::endl;
    if (zenitAngle > -0.0872665 && zenitAngle < 0.0872665) {

    } else {
      if (zenitAngle > 0.96) {
        invalidCounter++;
        PrintPoint3DVector(singleMuonTrack);
      } else {
        if (startPoint->y > 35. && endPoint->y < -35) {
          // std::cout <<"Zenith Angle for : " << histTitle <<" : " << zenitAngle << std::endl;
          // zenithAngleHist->Fill(zenitAngle);
          double zenithAngle = atan(fabs(endPoint->x - startPoint->x) / fabs(endPoint->y - startPoint->y));
          zenithAngleHist->Fill(zenithAngle);
        }
      }
    }
  }
  std::cout << std::endl << "Total number of invalid angle tracks : " << invalidCounter << std::endl;
  // TF1 *zenForm = new TF1("zenForm", "[0]*sin(x)*pow(cos(x),[1])", 0.05,M_PI/2.);
  TF1 *zenForm = new TF1("zenForm", "[0]*sin(x)*cos(x)*pow(cos(x),[1])", 0.05, 1.39626);
  // zenithAngleHist->Fit(zenForm,"r");
  // zenithAngleHist->Draw();

  return zenithAngleHist;
}

void PrintPoint3DVector(std::vector<Point3D *> vect)
{
  std::cout << "=================================================" << std::endl;
  for (unsigned short int i = 0; i < vect.size(); i++) {
    vect[i]->Print();
  }
}

double Interpolate(lite_interface::Point3D *startPoint, lite_interface::Point3D *endPoint,
                   lite_interface::Point3D *pointOnInspectedLayer)
{
  Tracking::Vector3D<double> startPt(startPoint->GetX(), startPoint->GetY(), startPoint->GetZ());
  Tracking::Vector3D<double> endPt(endPoint->GetX(), endPoint->GetY(), endPoint->GetZ());
  Tracking::Vector3D<double> inspectedLayerPt(pointOnInspectedLayer->GetX(), pointOnInspectedLayer->GetY(),
                                              pointOnInspectedLayer->GetZ());
  Tracking::Vector3D<double> dir = (endPt - startPt).Unit();
  double dist                    = (inspectedLayerPt.y() - startPt.y()) / dir.y();
  double retval                  = startPt.z() + dir.z() * dist;
  return retval;
}

double Interpolate(Point2D p1, Point2D p2, double y)
{
  double slope  = (p2.y - p1.y) / (p2.x - p1.x);
  double retval = (y - p1.y) / slope + p1.x;
  return retval;
}

TF1 *GetMuonPeakFitFormula(std::string barName)
{
  unsigned int peakPos;
  unsigned int startOffset = 1500;
  unsigned int endOffset   = 200;
  /*std::vector<unsigned int> vecOfPeakPos = {6000,8800,9600,8900,8000,8340,9800,8900,8000,
                                            8800,8000,8200,9700,9000,9200,8800,7000,8000,
                        9500,9000,9200,8000,8500,8200,9000,8800,9200,
                        8600,8000,8800,9000,8400,9200,9200,8400,9600,
                        8400,8400,8350,5500,8200,7700,7600,7800,6200,
                        8050,5200,7200,7000,5950,6800,8050,8400,8150,
                        8200,7800,8800,9000,8700,8150,8400,9200,9100,
                        8500,8600,7100,8400,8150,8500,9000,13500,3800,
                        6500,9700,10250,11800,10500,10200,10000,9600,1500,
                        10000,11500,9700,9550,10300,10200,5000,9900,11700};*/

  std::vector<std::string>::iterator it;
  it = std::find(sequentialBarNames.begin(), sequentialBarNames.end(), barName);
  if (it != sequentialBarNames.end()) {
    unsigned int index = it - sequentialBarNames.begin();
    peakPos            = vecOfPeakPos[index];
  }
  return (new TF1("g1", "gaus", peakPos - startOffset, peakPos + endOffset));
}

std::string GenerateSubFileName(char *exeName, char *processedFileName)
{
  std::string subExeName = std::string(exeName).substr(std::string(exeName).find("_"));
  std::string matWithExt = std::string(processedFileName).substr(13);
  return subExeName + matWithExt;
}
std::string GenerateSubFileName(std::string exeName, std::string processedFileName)
{
  std::string subExeName = exeName.substr(exeName.find("_"));
  std::string matWithExt = processedFileName.substr(13);
  return subExeName + matWithExt;
}

void FillAttenCoeffVec()
{

  for (unsigned int i = 0; i < vecOfBarsNamess.size(); i++) {
    if (i == 54)
      vecOfAttenCoeff.push_back(0.0068);
    else {
      if (i == 74)
        vecOfAttenCoeff.push_back(0.0128);
      else {
        if (i == 64)
          vecOfAttenCoeff.push_back(0.00584123);
        else {
          if (i == 79)
            vecOfAttenCoeff.push_back(0.0116217);
          else
            vecOfAttenCoeff.push_back(0.);
        }
      }
    }
  }
}

std::vector<std::string> GetVectorOfFiles(const char *dirname, const char *ext)
{
  std::vector<std::string> vecOfFileNames;
  TSystemDirectory dir(dirname, dirname);
  TList *files = dir.GetListOfFiles();
  if (files) {
    TSystemFile *file;
    TString fname;
    TIter next(files);
    while ((file = (TSystemFile *)next())) {
      fname = file->GetName();
      if (!file->IsDirectory() && fname.EndsWith(ext)) {
        // std::cout << (fname.Data()) << std::endl;
        // std::cout << std::string(fname.Data()) << std::endl;
        vecOfFileNames.push_back(std::string(fname.Data()));
      }
    }
  }
  return vecOfFileNames;
}

double GetGaussianRandomSample(double mean, double sigma)
{
  TRandom3 *grandom = new TRandom3();
  return grandom->Gaus(mean, sigma);
}
HelperFunctions::HelperFunctions()
{
  // TODO Auto-generated constructor stub
}

HelperFunctions::~HelperFunctions()
{
  // TODO Auto-generated destructor stub
}
