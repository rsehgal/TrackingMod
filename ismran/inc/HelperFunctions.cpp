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
#include "colors.h"
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
    // if (smt->HitInRequiredLayers())
    {
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
            std::cout << "Interpolated XorZ : " << xOrZ << std::endl;

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

Tracking::Vector3D<double> *ConvertToTomoVector3D(lite_interface::Point3D *pt)
{
  return new Tracking::Vector3D<double>(pt->GetX(), pt->GetY(), pt->GetZ());
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
  Double_t y1 = -150;
  Double_t y2 = 100;
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
  /*Tracking::Vector3D<double> dir = (endPt - startPt).Unit();
  double dist                    = (inspectedLayerPt.y() - startPt.y()) / dir.y();
  double retval                  = startPt.z() + dir.z() * dist;*/
  double retval =
      (((inspectedLayerPt.y() - startPt.y()) * (endPt.z() - startPt.z())) / (endPt.y() - startPt.y())) + startPt.z();
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
  TRandom3 *grandom = new TRandom3(0);
  return grandom->Gaus(mean, sigma);
}

double GetUniformRandomSample(double lower, double upper) {}

std::vector<lite_interface::Point3D *> InCm(std::vector<lite_interface::Point3D *> vecOfPt3D)
{
  std::vector<lite_interface::Point3D *> cmPointVec;
  for (unsigned int i = 0; i < vecOfPt3D.size(); i++) {
    cmPointVec.push_back(vecOfPt3D[i]->InCm());
  }
  return cmPointVec;
}

std::vector<std::string> SplitString(std::string s)
{
  std::string temp = "";
  std::vector<std::string> v;
  for (int i = 0; i < s.length(); ++i) {
    if (s[i] == ' ') {
      v.push_back(temp);
      temp = "";
    } else {
      temp.push_back(s[i]);
    }
  }
  v.push_back(temp);
  return v;
}

/*template <typename T>
int GetIndex(std::vector<T> v, T K)
{
  auto it = find(v.begin(), v.end(), K);
  // If element was found
  if (it != v.end()) {
    // calculating the index of K
    int index = it - v.begin();
    // cout << index << endl;
    return index;
  } else {
    // If the element is not present in the vector
    // cout << "-1" << endl;
    return -1;
  }
}*/

double GenRandom(double min, double max)
{
  return (rand() / (static_cast<double>(RAND_MAX) + 1.0)) * (max - min) + min;
}

int GetClass(unsigned int barIndexInLayer)
{
  if (barIndexInLayer == 0) return -40;
  if (barIndexInLayer == 1) return -30;
  if (barIndexInLayer == 2) return -20;
  if (barIndexInLayer == 3) return -10;
  if (barIndexInLayer == 4) return 0;
  if (barIndexInLayer == 5) return 10;
  if (barIndexInLayer == 6) return 20;
  if (barIndexInLayer == 7) return 30;
  if (barIndexInLayer == 8) return 40;
}
/*
std::vector<TF1 *> GenerateCorrectionParameterizationUsingMuons(std::string filename, unsigned int barIndex)
{
  std::ofstream outfile("testOut.txt");
  unsigned int startIndex          = 2;
  unsigned int endIndex            = 0;
  unsigned int inspectedLayerIndex = 1;
  unsigned int numOfEvents         = 500000;

  std::vector<TH1F *> vecOfHist_DelT;
  std::vector<TH1F *> vecOfHist_Z;
  std::vector<TH1F *> vecOfHist_Q;

  for (unsigned int i = 0; i < numOfBarsInEachLayer; i++) {
    std::string histNameDelT = "Hist_delT_Pixel_" + std::to_string(i);
    std::string histNameZ    = "Hist_Z_Pixel_" + std::to_string(i);
    std::string histNameQ    = "Hist_Q_Pixel_" + std::to_string(i);
    vecOfHist_DelT.push_back(new TH1F(histNameDelT.c_str(), histNameDelT.c_str(), 200, -50., 50.));
    vecOfHist_Z.push_back(new TH1F(histNameZ.c_str(), histNameZ.c_str(), 160, -80., 80.));
    vecOfHist_Q.push_back(new TH1F(histNameQ.c_str(), histNameQ.c_str(), 100, -5., 5.));
  }

  GenerateScintMatrixXYCenters();

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
      bool fingerCondition                      = false;
      fingerCondition =
          (scintStart->GetLayerIndex() < inspectedLayerIndex && scintEnd->GetLayerIndex() < inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scint->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() > inspectedLayerIndex && scintEnd->GetLayerIndex() > inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scint->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() < inspectedLayerIndex && scintEnd->GetLayerIndex() > inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() > inspectedLayerIndex && scintEnd->GetLayerIndex() < inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer());

      if (scint->GetBarIndex() == 13 && fingerCondition) {
        outfile << scint->GetLayerIndex() << "," << scint->GetBarIndex() << "," << scint->GetLogQNearByQFar() << ","
                << scint->GetDelTCorrected() / 1000. << "," << scint->EstimateHitPosition_QParam()->GetX() << ","
                << scintStart->GetBarIndexInLayer() << std::endl;
        unsigned int pixelIndex = scintStart->GetBarIndexInLayer();
        vecOfHist_DelT[pixelIndex]->Fill(scint->GetDelTCorrected() / 1000.);
        vecOfHist_Z[pixelIndex]->Fill(scint->EstimateHitPosition_QParam()->GetX());
        vecOfHist_Q[pixelIndex]->Fill(scint->GetLogQNearByQFar());
      }
    }
  }

  outfile.close();
  std::vector<double> vecOfMean_DelT;
  std::vector<double> vecOfMean_Q;
  std::vector<double> vecOfMean_Z;

  TFile *fp = new TFile("muonParam.root", "RECREATE");

  std::cout << "--------------------------------------" << std::endl;
  std::vector<double> vecOfPixelPos = {-40., -30., -20., -10., 0., 10., 20., 30., 40.};
  for (unsigned int i = 0; i < numOfBarsInEachLayer; i++) {
    vecOfHist_DelT[i]->Write();
    vecOfHist_Z[i]->Write();
    vecOfHist_Q[i]->Write();

    TF1 *formulaDelT = new TF1("FormulaDelT", "gaus", -50., 50.);
    formulaDelT->SetParameters(vecOfHist_DelT[i]->GetMaximum(), vecOfHist_DelT[i]->GetMean(),
                               vecOfHist_DelT[i]->GetRMS());
    vecOfHist_DelT[i]->Fit(formulaDelT, "qn");
    vecOfMean_DelT.push_back(formulaDelT->GetParameter(1));
    std::cout << "DelT Gauss Parameter at pixel : " << i << " : " << formulaDelT->GetParameter(0) << " : "
              << formulaDelT->GetParameter(1) << " : " << formulaDelT->GetParameter(2) << std::endl;

    TF1 *formulaQ = new TF1("FormulaQ", "gaus", -5., 5.);
    formulaQ->SetParameters(vecOfHist_Q[i]->GetMaximum(), vecOfHist_Q[i]->GetMean(), vecOfHist_Q[i]->GetRMS());
    vecOfHist_Q[i]->Fit(formulaQ, "qn");
    vecOfMean_Q.push_back(formulaQ->GetParameter(1));
    std::cout << "Q Gauss Parameter at pixel : " << i << " : " << formulaQ->GetParameter(0) << " : "
              << formulaQ->GetParameter(1) << " : " << formulaQ->GetParameter(2) << std::endl;

    TF1 *formulaZ = new TF1("FormulaZ", "gaus", -50., 50.);
    formulaZ->SetParameters(vecOfHist_Z[i]->GetMaximum(), vecOfHist_Z[i]->GetMean(), vecOfHist_Z[i]->GetRMS());
    vecOfHist_Z[i]->Fit(formulaZ, "qn");
    vecOfMean_Z.push_back(formulaZ->GetParameter(1));
    std::cout << "Z Gauss Parameter at pixel : " << i << " : " << formulaZ->GetParameter(0) << " : "
              << formulaZ->GetParameter(1) << " : " << formulaZ->GetParameter(2) << std::endl;
  }

  std::cout << "--------------------------------------" << std::endl;
  TGraph *grQ_pos = new TGraph(vecOfPixelPos.size(), &vecOfPixelPos[0], &vecOfMean_Q[0]);
  grQ_pos->SetName("Q_as_function_of_Pixel_Position");
  grQ_pos->SetMarkerStyle(4);
  TGraph *grDelT_Z = new TGraph(vecOfMean_DelT.size(), &vecOfMean_DelT[0], &vecOfMean_Z[0]);
  grDelT_Z->SetName("Z_as_function_of_Mean_DelT");
  grDelT_Z->SetMarkerStyle(4);
  TGraph *grQ_Z = new TGraph(vecOfMean_Q.size(), &vecOfMean_Q[0], &vecOfMean_Z[0]);
  grQ_Z->SetName("Z_as_function_of_Mean_Q");
  grQ_Z->SetMarkerStyle(4);
  TF1 *formula_DelT_Z = new TF1(("fzparam_Muon_DelT" + vecOfBarsNamess[barIndex]).c_str(), Pol3, -10, 10, 4);
  TF1 *formula_Q_Z    = new TF1(("fzparam_Muon_Q" + vecOfBarsNamess[barIndex]).c_str(), Pol3, -3., 3., 4);
  grDelT_Z->Fit(formula_DelT_Z, "qn");
  grQ_Z->Fit(formula_Q_Z, "qn");

  std::vector<TF1 *> vecOfParam;
  vecOfParam.push_back(formula_DelT_Z);
  vecOfParam.push_back(formula_Q_Z);
  // TFile *fp = new TFile("muonParam.root", "RECREATE");
  fp->cd();
  grQ_pos->Write();
  grDelT_Z->Write();
  grQ_Z->Write();
  formula_DelT_Z->Write();
  formula_Q_Z->Write();
  fp->Close();
  return vecOfParam;
}
*/

/*
Function to generate parameterization for any bar
*/
std::vector<TF1 *> GenerateParameterizationUsingMuons_2(std::string filename, unsigned int barIndex)
{
  std::ofstream outfile("testOut.txt");
  unsigned int startIndex          = 3;
  unsigned int endIndex            = 1;
  unsigned int inspectedLayerIndex = 0;
  unsigned int numOfEvents         = 1000000;

  std::vector<TH1F *> vecOfHist_DelT;
  std::vector<TH1F *> vecOfHist_Z;
  std::vector<TH1F *> vecOfHist_Q;

  for (unsigned int i = 0; i < numOfBarsInEachLayer; i++) {
    std::string histNameDelT = "Hist_delT_Pixel_" + std::to_string(i);
    std::string histNameZ    = "Hist_Z_Pixel_" + std::to_string(i);
    std::string histNameQ    = "Hist_Q_Pixel_" + std::to_string(i);
    vecOfHist_DelT.push_back(new TH1F(histNameDelT.c_str(), histNameDelT.c_str(), 200, -50., 50.));
    vecOfHist_Z.push_back(new TH1F(histNameZ.c_str(), histNameZ.c_str(), 160, -80., 80.));
    vecOfHist_Q.push_back(new TH1F(histNameQ.c_str(), histNameQ.c_str(), 100, -5., 5.));
  }

  GenerateScintMatrixXYCenters();

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
      bool fingerCondition                      = false;
      fingerCondition =
          (scintStart->GetLayerIndex() < inspectedLayerIndex && scintEnd->GetLayerIndex() < inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scint->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() > inspectedLayerIndex && scintEnd->GetLayerIndex() > inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scint->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() > inspectedLayerIndex && scintEnd->GetLayerIndex() > inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() < inspectedLayerIndex && scintEnd->GetLayerIndex() > inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() > inspectedLayerIndex && scintEnd->GetLayerIndex() < inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer());

      if (scint->GetBarIndex() == barIndex && fingerCondition) {
        std::cout << BLUE << "PixelIndex start Layer : " << scintStart->GetBarIndexInLayer()
                  << " : PixelIndex end Layer : " << scintEnd->GetBarIndexInLayer() << RESET << std::endl;
        outfile << scint->GetLayerIndex() << "," << scint->GetBarIndex() << "," << scint->GetLogQNearByQFar() << ","
                << scint->GetDelTCorrected() / 1000. << "," << scint->EstimateHitPosition_QParam()->GetX() << ","
                << scintStart->GetBarIndexInLayer() << std::endl;
        std::cout << RED << "FILLING HIST" << RESET << std::endl;
        unsigned int pixelIndex = scintStart->GetBarIndexInLayer();
        std::cout << MAGENTA << "DELT : " << (scint->GetDelTCorrected() / 1000.)
                  << " : Z : " << (scint->EstimateHitPosition_QParam()->GetX())
                  << " : Q : " << (scint->GetLogQNearByQFar()) << RESET << std::endl;
        vecOfHist_DelT[pixelIndex]->Fill(scint->GetDelTCorrected() / 1000.);
        vecOfHist_Z[pixelIndex]->Fill(scint->EstimateHitPosition_QParam()->GetX());
        vecOfHist_Q[pixelIndex]->Fill(scint->GetLogQNearByQFar());
      }
    }
  }

  outfile.close();
  std::vector<double> vecOfMean_DelT;
  std::vector<double> vecOfMean_Q;
  std::vector<double> vecOfMean_Z;

  TFile *fp = new TFile(("muonParam_" + std::to_string(barIndex) + ".root").c_str(), "RECREATE");

  std::cout << "--------------------------------------" << std::endl;
  std::vector<double> vecOfPixelPos = {-40., -30., -20., -10., 0., 10., 20., 30., 40.};
  for (unsigned int i = 0; i < numOfBarsInEachLayer; i++) {
    vecOfHist_DelT[i]->Write();
    vecOfHist_Z[i]->Write();
    vecOfHist_Q[i]->Write();

    TF1 *formulaDelT = new TF1("FormulaDelT", "gaus", -50., 50.);
    formulaDelT->SetParameters(vecOfHist_DelT[i]->GetMaximum(), vecOfHist_DelT[i]->GetMean(),
                               vecOfHist_DelT[i]->GetRMS());
    vecOfHist_DelT[i]->Fit(formulaDelT, "qn");
    vecOfMean_DelT.push_back(formulaDelT->GetParameter(1));
    std::cout << "DelT Gauss Parameter at pixel : " << i << " : " << formulaDelT->GetParameter(0) << " : "
              << formulaDelT->GetParameter(1) << " : " << formulaDelT->GetParameter(2) << std::endl;

    TF1 *formulaQ = new TF1("FormulaQ", "gaus", -5., 5.);
    formulaQ->SetParameters(vecOfHist_Q[i]->GetMaximum(), vecOfHist_Q[i]->GetMean(), vecOfHist_Q[i]->GetStdDev());
    // formulaQ->SetParameters(vecOfHist_Q[i]->GetMaximum(), vecOfHist_Q[i]->GetMean(), vecOfHist_Q[i]->GetRMS());
    vecOfHist_Q[i]->Fit(formulaQ, "qn");
    vecOfMean_Q.push_back(formulaQ->GetParameter(1));
    std::cout << "Q Gauss Parameter at pixel : " << i << " : " << formulaQ->GetParameter(0) << " : "
              << formulaQ->GetParameter(1) << " : " << formulaQ->GetParameter(2) << std::endl;

    TF1 *formulaZ = new TF1("FormulaZ", "gaus", -50., 50.);
    formulaZ->SetParameters(vecOfHist_Z[i]->GetMaximum(), vecOfHist_Z[i]->GetMean(), vecOfHist_Z[i]->GetRMS());
    vecOfHist_Z[i]->Fit(formulaZ, "qn");
    vecOfMean_Z.push_back(formulaZ->GetParameter(1));
    std::cout << "Z Gauss Parameter at pixel : " << i << " : " << formulaZ->GetParameter(0) << " : "
              << formulaZ->GetParameter(1) << " : " << formulaZ->GetParameter(2) << std::endl;
  }

  std::cout << "--------------------------------------" << std::endl;
  TGraph *grQ_pos = new TGraph(vecOfPixelPos.size(), &vecOfPixelPos[0], &vecOfMean_Q[0]);
  grQ_pos->SetName("Q_as_function_of_Pixel_Position");
  grQ_pos->SetMarkerStyle(4);
  TGraph *grDelT_Z = new TGraph(vecOfMean_DelT.size(), &vecOfMean_DelT[0], &vecOfPixelPos[0]);
  // TGraph *grDelT_Z = new TGraph(vecOfMean_DelT.size(), &vecOfMean_DelT[0], &vecOfMean_Z[0]);
  grDelT_Z->SetName("Z_as_function_of_Mean_DelT");
  grDelT_Z->SetMarkerStyle(4);
  TGraph *grQ_Z = new TGraph(vecOfMean_Q.size(), &vecOfMean_Q[0], &vecOfPixelPos[0]);
  // TGraph *grQ_Z = new TGraph(vecOfMean_Q.size(), &vecOfMean_Q[0], &vecOfMean_Z[0]);
  grQ_Z->SetName("Z_as_function_of_Mean_Q");
  grQ_Z->SetMarkerStyle(4);
  TF1 *formula_DelT_Z = new TF1(("fzparam_Muon_DelT_" + vecOfBarsNamess[barIndex]).c_str(), Pol3, -10, 10, 4);
  TF1 *formula_Q_Z    = new TF1(("fzparam_Muon_Q_" + vecOfBarsNamess[barIndex]).c_str(), Pol3, -3., 3., 4);
  grDelT_Z->Fit(formula_DelT_Z, "qn");
  grQ_Z->Fit(formula_Q_Z, "qn");

  std::vector<TF1 *> vecOfParam;
  vecOfParam.push_back(formula_DelT_Z);
  vecOfParam.push_back(formula_Q_Z);
  // TFile *fp = new TFile("muonParam.root", "RECREATE");
  fp->cd();
  grQ_pos->Write();
  grDelT_Z->Write();
  grQ_Z->Write();
  formula_DelT_Z->Write();
  formula_Q_Z->Write();
  fp->Close();
  return vecOfParam;
}

std::vector<TF1 *> GenerateParameterizationUsingMuons(std::string filename, unsigned int barIndex)
{
  std::ofstream outfile("testOut.txt");
  unsigned int startIndex          = 2;
  unsigned int endIndex            = 0;
  unsigned int inspectedLayerIndex = 1;
  unsigned int numOfEvents         = 500000;

  std::vector<TH1F *> vecOfHist_DelT;
  std::vector<TH1F *> vecOfHist_Z;
  std::vector<TH1F *> vecOfHist_Q;

  for (unsigned int i = 0; i < numOfBarsInEachLayer; i++) {
    std::string histNameDelT = "Hist_delT_Pixel_" + std::to_string(i);
    std::string histNameZ    = "Hist_Z_Pixel_" + std::to_string(i);
    std::string histNameQ    = "Hist_Q_Pixel_" + std::to_string(i);
    vecOfHist_DelT.push_back(new TH1F(histNameDelT.c_str(), histNameDelT.c_str(), 200, -50., 50.));
    vecOfHist_Z.push_back(new TH1F(histNameZ.c_str(), histNameZ.c_str(), 160, -80., 80.));
    vecOfHist_Q.push_back(new TH1F(histNameQ.c_str(), histNameQ.c_str(), 100, -5., 5.));
  }

  GenerateScintMatrixXYCenters();

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
      bool fingerCondition                      = false;
      fingerCondition =
          (scintStart->GetLayerIndex() < inspectedLayerIndex && scintEnd->GetLayerIndex() < inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scint->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() > inspectedLayerIndex && scintEnd->GetLayerIndex() > inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scint->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() < inspectedLayerIndex && scintEnd->GetLayerIndex() > inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() > inspectedLayerIndex && scintEnd->GetLayerIndex() < inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer());

      if (scint->GetBarIndex() == 13 && fingerCondition) {
        outfile << scint->GetLayerIndex() << "," << scint->GetBarIndex() << "," << scint->GetLogQNearByQFar() << ","
                << scint->GetDelTCorrected() / 1000. << "," << scint->EstimateHitPosition_QParam()->GetX() << ","
                << scintStart->GetBarIndexInLayer() << std::endl;
        unsigned int pixelIndex = scintStart->GetBarIndexInLayer();
        vecOfHist_DelT[pixelIndex]->Fill(scint->GetDelTCorrected() / 1000.);
        vecOfHist_Z[pixelIndex]->Fill(scint->EstimateHitPosition_QParam()->GetX());
        vecOfHist_Q[pixelIndex]->Fill(scint->GetLogQNearByQFar());
      }
    }
  }

  outfile.close();
  std::vector<double> vecOfMean_DelT;
  std::vector<double> vecOfMean_Q;
  std::vector<double> vecOfMean_Z;

  TFile *fp = new TFile(("muonParam_" + vecOfBarsNamess[barIndex] + ".root").c_str(), "RECREATE");

  std::cout << "--------------------------------------" << std::endl;
  std::vector<double> vecOfPixelPos = {-40., -30., -20., -10., 0., 10., 20., 30., 40.};
  for (unsigned int i = 0; i < numOfBarsInEachLayer; i++) {
    vecOfHist_DelT[i]->Write();
    vecOfHist_Z[i]->Write();
    vecOfHist_Q[i]->Write();

    TF1 *formulaDelT = new TF1("FormulaDelT", "gaus", -50., 50.);
    formulaDelT->SetParameters(vecOfHist_DelT[i]->GetMaximum(), vecOfHist_DelT[i]->GetMean(),
                               vecOfHist_DelT[i]->GetRMS());
    vecOfHist_DelT[i]->Fit(formulaDelT, "qn");
    vecOfMean_DelT.push_back(formulaDelT->GetParameter(1));
    std::cout << "DelT Gauss Parameter at pixel : " << i << " : " << formulaDelT->GetParameter(0) << " : "
              << formulaDelT->GetParameter(1) << " : " << formulaDelT->GetParameter(2) << std::endl;

    TF1 *formulaQ = new TF1("FormulaQ", "gaus", -5., 5.);
    formulaQ->SetParameters(vecOfHist_Q[i]->GetMaximum(), vecOfHist_Q[i]->GetMean(), vecOfHist_Q[i]->GetRMS());
    vecOfHist_Q[i]->Fit(formulaQ, "qn");
    vecOfMean_Q.push_back(formulaQ->GetParameter(1));
    std::cout << "Q Gauss Parameter at pixel : " << i << " : " << formulaQ->GetParameter(0) << " : "
              << formulaQ->GetParameter(1) << " : " << formulaQ->GetParameter(2) << std::endl;

    TF1 *formulaZ = new TF1("FormulaZ", "gaus", -50., 50.);
    formulaZ->SetParameters(vecOfHist_Z[i]->GetMaximum(), vecOfHist_Z[i]->GetMean(), vecOfHist_Z[i]->GetRMS());
    vecOfHist_Z[i]->Fit(formulaZ, "qn");
    vecOfMean_Z.push_back(formulaZ->GetParameter(1));
    std::cout << "Z Gauss Parameter at pixel : " << i << " : " << formulaZ->GetParameter(0) << " : "
              << formulaZ->GetParameter(1) << " : " << formulaZ->GetParameter(2) << std::endl;
  }

  std::cout << "--------------------------------------" << std::endl;
  TGraph *grQ_pos = new TGraph(vecOfPixelPos.size(), &vecOfPixelPos[0], &vecOfMean_Q[0]);
  grQ_pos->SetName("Q_as_function_of_Pixel_Position");
  grQ_pos->SetMarkerStyle(4);
  TGraph *grDelT_Z = new TGraph(vecOfMean_DelT.size(), &vecOfMean_DelT[0], &vecOfPixelPos[0]);
  // TGraph *grDelT_Z = new TGraph(vecOfMean_DelT.size(), &vecOfMean_DelT[0], &vecOfMean_Z[0]);
  grDelT_Z->SetName("Z_as_function_of_Mean_DelT");
  grDelT_Z->SetMarkerStyle(4);
  TGraph *grQ_Z = new TGraph(vecOfMean_Q.size(), &vecOfMean_Q[0], &vecOfPixelPos[0]);
  // TGraph *grQ_Z = new TGraph(vecOfMean_Q.size(), &vecOfMean_Q[0], &vecOfMean_Z[0]);
  grQ_Z->SetName("Z_as_function_of_Mean_Q");
  grQ_Z->SetMarkerStyle(4);
  TF1 *formula_DelT_Z = new TF1(("fzparam_Muon_DelT" + vecOfBarsNamess[barIndex]).c_str(), Pol3, -10, 10, 4);
  TF1 *formula_Q_Z    = new TF1(("fzparam_Muon_Q" + vecOfBarsNamess[barIndex]).c_str(), Pol3, -3., 3., 4);
  grDelT_Z->Fit(formula_DelT_Z, "qn");
  grQ_Z->Fit(formula_Q_Z, "qn");

  /*
  Muon Parameterization plots for paper
  */

  std::vector<double> vecOfZError    = {5., 5., 5., 5., 5., 5., 5., 5., 5.};
  std::vector<double> vecOfDelTError = {0., 0., 0., 0., 0., 0., 0., 0., 0.};
  std::vector<double> vecOfQError    = {0., 0., 0., 0., 0., 0., 0., 0., 0.};

  auto grDelT = new TGraphErrors(vecOfMean_DelT.size(), &vecOfMean_DelT[0], &vecOfPixelPos[0], &vecOfDelTError[0],
                                 &vecOfZError[0]);
  grDelT->SetTitle("Cosmic Muons");
  auto grQ = new TGraphErrors(vecOfMean_Q.size(), &vecOfMean_Q[0], &vecOfPixelPos[0], &vecOfQError[0], &vecOfZError[0]);
  grQ->SetTitle("Cosmic Muons");

  TLegend *legendTZ = new TLegend(0.1, 0.7, 0.24, 0.5);
  new TCanvas("Graph of DelT vs Z", "Graph of DelT vs Z");
  grDelT->Draw("ap");
  grDelT->SetMarkerStyle(8);
  TF1 *formulaTZ = new TF1("FormulaTZ", Pol3, -10, 10, 4);
  formulaTZ->SetLineColor(2);
  formulaTZ->SetLineWidth(3);
  grDelT->Fit(formulaTZ, "qn");
  formulaTZ->Draw("same");

  legendTZ->AddEntry(grDelT, "Data Point", "p");
  legendTZ->AddEntry(formulaTZ, "Fit", "l");
  legendTZ->Draw("same");

  TLegend *legendQZ = new TLegend(0.1, 0.7, 0.24, 0.5);
  new TCanvas("Graph of Q vs Z", "Graph of Q vs Z");
  grQ->Draw("ap");
  TF1 *formulaQZ = new TF1("FormulaQZ", Pol3, -3, 3, 4);
  grQ->Fit(formulaQZ, "qn");
  formulaQZ->SetLineWidth(3);
  grQ->SetMarkerStyle(8);
  formulaQZ->Draw("same");

  legendQZ->AddEntry(grQ, "Data Point", "p");
  legendQZ->AddEntry(formulaQZ, "Fit", "l");
  legendQZ->Draw("same");

  /*------------ End --------------------*/

  std::vector<TF1 *> vecOfParam;
  vecOfParam.push_back(formula_DelT_Z);
  vecOfParam.push_back(formula_Q_Z);
  // TFile *fp = new TFile("muonParam.root", "RECREATE");
  fp->cd();
  grQ_pos->Write();
  grDelT_Z->Write();
  grQ_Z->Write();
  formula_DelT_Z->Write();
  formula_Q_Z->Write();
  fp->Close();
  return vecOfParam;
}

/*
Function to generated the training and testing data for ML using cosmic data.
*/
void GenerateTrainingDataUsingCosmicMuon(std::string filename, unsigned int barIndex)
{
  std::ofstream outfile("testOut.txt");
  unsigned int startIndex          = 2;
  unsigned int endIndex            = 0;
  unsigned int inspectedLayerIndex = 1;
  unsigned int numOfEvents         = 0;

  TH1F *histZ_Q    = new TH1F("ZUsingQ", "ZUsingQ", 160, -80., 80.);
  TH1F *histZ_DelT = new TH1F("ZUsingDelT", "ZUsingDelT", 160, -80., 80.);
  GenerateScintMatrixXYCenters();

  std::vector<std::ofstream> vecOfFilePointer;
  std::vector<int> posVec = {-40, -30, -20, -10, 0, 10, 20, 30, 40};
  for (unsigned int i = 0; i < numOfBarsInEachLayer; i++) {
    std::ofstream outfile;
    std::string fileName = "";
    if (posVec[i] >= 0)
      fileName = vecOfBarsNamess[barIndex] + "_+" + std::to_string(posVec[i]) + "_trainingT.txt";
    else
      fileName = vecOfBarsNamess[barIndex] + "_" + std::to_string(posVec[i]) + "_trainingT.txt";
    outfile.open(fileName.c_str());
    vecOfFilePointer.push_back(std::move(outfile));
  }

  TFile *fp        = new TFile("muonParam.root", "r");
  TF1 *delTFormula = (TF1 *)fp->Get(("fzparam_Muon_DelT" + vecOfBarsNamess[barIndex]).c_str());
  TF1 *qFormula    = (TF1 *)fp->Get(("fzparam_Muon_Q" + vecOfBarsNamess[barIndex]).c_str());

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
      bool fingerCondition                      = false;
      fingerCondition =
          (scintStart->GetLayerIndex() < inspectedLayerIndex && scintEnd->GetLayerIndex() < inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scint->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() > inspectedLayerIndex && scintEnd->GetLayerIndex() > inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scint->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() < inspectedLayerIndex && scintEnd->GetLayerIndex() > inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() > inspectedLayerIndex && scintEnd->GetLayerIndex() < inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer());

      if (scint->GetBarIndex() == 13 && fingerCondition) {
        /* outfile << scint->GetLayerIndex() << "," << scint->GetBarIndex() << "," << scint->GetLogQNearByQFar() << ","
                 << scint->GetDelTCorrected() / 1000. << "," << scint->EstimateHitPosition_QParam()->GetX() << ","
                 << scintStart->GetBarIndexInLayer() << std::endl;*/

        unsigned int pixelIndex = scintStart->GetBarIndexInLayer();

        vecOfFilePointer[pixelIndex] << scint->GetLogQNearByQFar() << "," << scint->GetDelTCorrected() << ","
                                     << scint->EstimateHitPosition_QParam()->GetX() << "," << posVec[pixelIndex]
                                     << std::endl;
      }
    }
  }

  for (unsigned int i = 0; i < numOfBarsInEachLayer; i++) {
    vecOfFilePointer[i].close();
  }
}
std::vector<TH1F *> EvaluateMuonParameterization(std::string filename, unsigned int pixelNo, unsigned int barIndex,
                                                 unsigned int numOfEv)
{
  // TApplication *app = new TApplication("Test", NULL, NULL);
  std::ofstream outfile("testOut.txt");
  unsigned int startIndex          = 2;
  unsigned int endIndex            = 0;
  unsigned int inspectedLayerIndex = 1;
  unsigned int numOfEvents         = numOfEv;

  /*#ifdef USE_FOR_SIMULATION
    numOfEvents = 0;
  #else
    numOfEvents = 1000000;
  #endif*/

  TH1F *histDiff = new TH1F("Diff_Hist", "Diff_Hist", 100, -20., 20.);

  TH1F *histZ_Mean         = new TH1F("ZUsingMean", "ZUsingMean", 200, -100., 100.);
  TH1F *histZ_Q            = new TH1F("ZUsingQ", "ZUsingQ", 200, -100., 100.);
  TH1F *histZ_Q_Reweighted = new TH1F("ZUsingQ_Reweighted", "ZUsingQ_Reweighted", 200, -100., 100.);
  TH1F *histZ_DelT         = new TH1F("ZUsingDelT", "ZUsingDelT", 200, -100., 100.);
  TH1F *histQ              = new TH1F("HistQ", "HistQ", 500, -5., 5.);
  GenerateScintMatrixXYCenters();

  // Histograms for nine pixels
  std::vector<TH1F *> vecOfHistOfPixel;
  std::vector<TH1F *> vecOfDelTHistOfPixel;

  double startMean = -0.8;
  for (unsigned int i = 0; i < 9; i++) {
    startMean = -0.8;
    startMean += (i * 0.2);
    std::string pixelStr     = "Pixel_" + std::to_string(i);
    std::string delTPixelStr = "DelT_Pixel_" + std::to_string(i);
    vecOfHistOfPixel.push_back(new TH1F(pixelStr.c_str(), pixelStr.c_str(), 150, -2., 2.));
    vecOfDelTHistOfPixel.push_back(new TH1F(delTPixelStr.c_str(), delTPixelStr.c_str(), 150, -25., 25.));
  }
  vecOfHistOfPixel[0]->SetMarkerStyle(53);
  vecOfHistOfPixel[1]->SetMarkerStyle(54);
  vecOfHistOfPixel[2]->SetMarkerStyle(55);
  vecOfHistOfPixel[3]->SetMarkerStyle(56);
  vecOfHistOfPixel[4]->SetMarkerStyle(57);
  vecOfHistOfPixel[5]->SetMarkerStyle(58);
  vecOfHistOfPixel[6]->SetMarkerStyle(59);
  vecOfHistOfPixel[7]->SetMarkerStyle(65);
  vecOfHistOfPixel[8]->SetMarkerStyle(67);

  vecOfHistOfPixel[0]->SetLineColor(4);
  vecOfHistOfPixel[1]->SetLineColor(3);
  vecOfHistOfPixel[2]->SetLineColor(6);
  vecOfHistOfPixel[3]->SetLineColor(46);
  vecOfHistOfPixel[4]->SetLineColor(1);
  vecOfHistOfPixel[5]->SetLineColor(28);
  vecOfHistOfPixel[6]->SetLineColor(7);
  vecOfHistOfPixel[7]->SetLineColor(9);
  vecOfHistOfPixel[8]->SetLineColor(2);

  vecOfDelTHistOfPixel[0]->SetMarkerStyle(53);
  vecOfDelTHistOfPixel[1]->SetMarkerStyle(54);
  vecOfDelTHistOfPixel[2]->SetMarkerStyle(55);
  vecOfDelTHistOfPixel[3]->SetMarkerStyle(56);
  vecOfDelTHistOfPixel[4]->SetMarkerStyle(57);
  vecOfDelTHistOfPixel[5]->SetMarkerStyle(58);
  vecOfDelTHistOfPixel[6]->SetMarkerStyle(59);
  vecOfDelTHistOfPixel[7]->SetMarkerStyle(65);
  vecOfDelTHistOfPixel[8]->SetMarkerStyle(67);

  vecOfDelTHistOfPixel[0]->SetLineColor(4);
  vecOfDelTHistOfPixel[1]->SetLineColor(3);
  vecOfDelTHistOfPixel[2]->SetLineColor(6);
  vecOfDelTHistOfPixel[3]->SetLineColor(46);
  vecOfDelTHistOfPixel[4]->SetLineColor(1);
  vecOfDelTHistOfPixel[5]->SetLineColor(28);
  vecOfDelTHistOfPixel[6]->SetLineColor(7);
  vecOfDelTHistOfPixel[7]->SetLineColor(9);
  vecOfDelTHistOfPixel[8]->SetLineColor(2);

  TLegend *legendPixel_Q = new TLegend(0.2, 0.2, .8, .8);
  legendPixel_Q->SetHeader("Pixel Center", "C");
  legendPixel_Q->AddEntry(vecOfHistOfPixel[0], "-40 cm", "l");
  legendPixel_Q->AddEntry(vecOfHistOfPixel[1], "-30 cm", "l");
  legendPixel_Q->AddEntry(vecOfHistOfPixel[2], "-20 cm", "l");
  legendPixel_Q->AddEntry(vecOfHistOfPixel[3], "-10 cm", "l");
  legendPixel_Q->AddEntry(vecOfHistOfPixel[4], "0 cm", "l");
  legendPixel_Q->AddEntry(vecOfHistOfPixel[5], "10 cm", "l");
  legendPixel_Q->AddEntry(vecOfHistOfPixel[6], "20 cm", "l");
  legendPixel_Q->AddEntry(vecOfHistOfPixel[7], "30 cm", "l");
  legendPixel_Q->AddEntry(vecOfHistOfPixel[8], "40 cm", "l");

  TLegend *legendPixel_DelT = new TLegend(0.2, 0.2, .8, .8);
  legendPixel_DelT->SetHeader("Pixel Center", "C");
  legendPixel_DelT->AddEntry(vecOfDelTHistOfPixel[0], "-40 cm", "l");
  legendPixel_DelT->AddEntry(vecOfDelTHistOfPixel[1], "-30 cm", "l");
  legendPixel_DelT->AddEntry(vecOfDelTHistOfPixel[2], "-20 cm", "l");
  legendPixel_DelT->AddEntry(vecOfDelTHistOfPixel[3], "-10 cm", "l");
  legendPixel_DelT->AddEntry(vecOfDelTHistOfPixel[4], "0 cm", "l");
  legendPixel_DelT->AddEntry(vecOfDelTHistOfPixel[5], "10 cm", "l");
  legendPixel_DelT->AddEntry(vecOfDelTHistOfPixel[6], "20 cm", "l");
  legendPixel_DelT->AddEntry(vecOfDelTHistOfPixel[7], "30 cm", "l");
  legendPixel_DelT->AddEntry(vecOfDelTHistOfPixel[8], "40 cm", "l");

  /*TFile *fp        = new TFile("muonParam.root", "r");
  TF1 *delTFormula = (TF1 *)fp->Get(("fzparam_Muon_DelT_"+vecOfBarsNamess[barIndex]).c_str());
  TF1 *qFormula    = (TF1 *)fp->Get(("fzparam_Muon_Q_"+vecOfBarsNamess[barIndex]).c_str());*/

  /*std::string barName = vecOfBarsNamess[barIndex];
  TFile *fpCalib    = new TFile("completeCalib2.root", "r");
  TF1 *qFormula = (TF1 *)fpCalib->Get(("fQparam_" + barName).c_str());
  TF1 *delTFormula = (TF1 *)fpCalib->Get(("fzparam_"+barName).c_str());*/

  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  TF1 *delTFormula                                      = calib->GetCalibrationDataOf(barIndex)->fParameterization_F;
  TF1 *qFormula                                         = calib->GetCalibrationDataOf(barIndex)->fQParameterization_F;
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
      bool fingerCondition                      = false;
      fingerCondition =
          (scintStart->GetLayerIndex() < inspectedLayerIndex && scintEnd->GetLayerIndex() < inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scint->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() > inspectedLayerIndex && scintEnd->GetLayerIndex() > inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scint->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() < inspectedLayerIndex && scintEnd->GetLayerIndex() > inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() > inspectedLayerIndex && scintEnd->GetLayerIndex() < inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer());

      // if (scint->GetBarIndex() == 13 && fingerCondition) {
      if (scint->GetBarIndex() == barIndex) {
        if (fingerCondition) {

          // std::cout << "======================================" << std::endl;
          // scint->GetMeanHitPosition()->Print();
#ifdef USE_FOR_SIMULATION

          outfile << scint->GetLayerIndex() << "," << scint->GetBarIndex() << ","
                  << scint->GetLogQNearByQFar_ForSimulation() << "," << scint->GetDelTCorrected() << ","
                  << scint->EstimateHitPosition_QParam()->GetX() << "," << scintStart->GetBarIndexInLayer() << ","
                  << scint->GetExactHitPosition()->GetZ() / 10. << std::endl;

#else

          outfile << scint->GetLayerIndex() << "," << scint->GetBarIndex() << "," << scint->GetLogQNearByQFar() << ","
                  << scint->GetDelTCorrected() << "," << scint->EstimateHitPosition_QParam()->GetX() << ","
                  << scintStart->GetBarIndexInLayer() << std::endl;
#endif
          unsigned int pixelIndex = scintStart->GetBarIndexInLayer();
          if (pixelNo == 10000) {
#ifdef USE_FOR_SIMULATION
            histZ_Q->Fill(qFormula->Eval(scint->GetLogQNearByQFar_ForSimulation()));
            histQ->Fill(scint->GetLogQNearByQFar_ForSimulation());
#else
            double evaluatedZVal = qFormula->Eval(scint->GetLogQNearByQFar());
            histZ_Q->Fill(evaluatedZVal);
            TH1F *histNormalizedWithMaxVal = NormalWithMaxBinCount(histZ_Q);
            // unsigned int numOfBins         = hist->GetNbinsX();
            // double weightFactor =
            // histNormalizedWithMaxVal->GetBinContent(histNormalizedWithMaxVal->GetXaxis()->FindFixBin(evaluatedZVal));
            double weightFactor =
                histNormalizedWithMaxVal->GetBinContent(histNormalizedWithMaxVal->GetXaxis()->FindBin(evaluatedZVal));
            histZ_Q_Reweighted->Fill(evaluatedZVal, 1 / weightFactor);

            // histZ_Q->Fill(qFormula->Eval(scint->GetLogQNearByQFar())-zCorrOffsetVector[pixelIndex]);
            histQ->Fill(scint->GetLogQNearByQFar());
#endif
            histZ_DelT->Fill(delTFormula->Eval(scint->GetDelTCorrected() / 1000.));
            vecOfHistOfPixel[pixelIndex]->Fill(scint->GetLogQNearByQFar());
            vecOfDelTHistOfPixel[pixelIndex]->Fill(scint->GetDelTCorrected() / 1000.);
          } else {
            if (pixelNo == pixelIndex) {
              double zm;
#ifdef USE_FOR_SIMULATION
              zm = qFormula->Eval(scint->GetLogQNearByQFar_ForSimulation());
              // std::cout << __FILE__ << " : " << __LINE__ << " : qval : " << scint->GetLogQNearByQFar_ForSimulation()
              //        << " : ZM : " << zm << std::endl;
              histZ_Q->Fill(qFormula->Eval(scint->GetLogQNearByQFar_ForSimulation()));
              histQ->Fill(scint->GetLogQNearByQFar_ForSimulation());
              histZ_Mean->Fill(scint->GetMeanHitPosition()->GetZ() / 10.);
              histDiff->Fill(qFormula->Eval(scint->GetLogQNearByQFar_ForSimulation()) -
                             scint->GetExactHitPosition()->GetZ() / 10.);
#else
              zm = qFormula->Eval(scint->GetLogQNearByQFar());
              std::cout << __FILE__ << " : " << __LINE__ << " : qval : " << scint->GetLogQNearByQFar()
                        << " : ZM : " << zm << std::endl;
              histZ_Q->Fill(qFormula->Eval(scint->GetLogQNearByQFar()));
              // Apply zOffsetCorrection
              // histZ_Q->Fill(qFormula->Eval(scint->GetLogQNearByQFar())-zCorrOffsetVector[pixelIndex] );
              histQ->Fill(scint->GetLogQNearByQFar());

#endif
              // zm = zm-((((int)zm-1)/5)*5.);
              // histZ_Q->Fill(qFormula->Eval(scint->GetLogQNearByQFar()));
              // histZ_Q->Fill(zm);
              histZ_DelT->Fill(delTFormula->Eval(scint->GetDelTCorrected() / 1000.));
            }
          }
        }
      }
    }
  }
  std::vector<TH1F *> vecOfHist = {histZ_Q, histZ_DelT, histQ, histZ_Mean, histDiff, histZ_Q_Reweighted};
  /*#ifdef USE_FOR_SIMULATION
    std::vector<TH1F *> vecOfHist = {histZ_Q, histZ_DelT, histQ};
  #else
    std::vector<TH1F *> vecOfHist = {histZ_Q, histZ_DelT, histQ, histZ_Mean};

  #endif*/

  new TCanvas("QAtDiffPixels", "QAtDiffPixels");
  for (unsigned int i = 0; i < vecOfHistOfPixel.size(); i++) {
    // vecOfHistOfPixel[i]->Scale(1/vecOfHistOfPixel[i]->Integral());
    vecOfHistOfPixel[i]->SetMarkerSize(1.);
    // if(!(i%2))
    vecOfHistOfPixel[i]->Draw("same");
    vecOfHistOfPixel[i]->SetLineWidth(3);
  }
  legendPixel_Q->Draw("same");

  new TCanvas("DelTAtDiffPixels", "DelTAtDiffPixels");
  for (unsigned int i = 0; i < vecOfDelTHistOfPixel.size(); i++) {
    // vecOfDelTHistOfPixel[i]->Scale(1/vecOfDelTHistOfPixel[i]->Integral());
    vecOfDelTHistOfPixel[i]->SetMarkerSize(1.);
    // if(!(i%2))
    vecOfDelTHistOfPixel[i]->Draw("same");
    vecOfDelTHistOfPixel[i]->SetLineWidth(3);
  }
  legendPixel_DelT->Draw("same");

  //  app->Run();
  return vecOfHist;
}
int GetMuonRateAt(std::vector<lite_interface::SingleMuonTrack *> smtVec, unsigned int layerIndex)
{

  unsigned int counter = 0;
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    std::vector<lite_interface::ScintillatorBar_V2 *> scintVec = smtVec[i]->GetMuonTrack();

    for (unsigned int j = 0; j < scintVec.size(); j++) {
      if (scintVec[j]->GetLayerIndex() == layerIndex) {
        counter++;
        break;
      }
    }
  }
  return (int)counter / 3600;
}

std::vector<int> GetMuonRateAtLayers(std::vector<lite_interface::SingleMuonTrack *> smtVec)
{
  std::vector<int> rateVec = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    std::vector<lite_interface::ScintillatorBar_V2 *> scintVec = smtVec[i]->GetMuonTrack();

    for (unsigned int j = 0; j < scintVec.size(); j++) {
      rateVec[scintVec[j]->GetLayerIndex()]++;
    }
  }

  for (unsigned int i = 0; i < rateVec.size(); i++) {
    rateVec[i] /= 3600;
  }

  return rateVec;
}

/*Function to normalizing the histogram with the maximum value*/
/*TH1F *GetReweightedHist(TH1F *hist)
{

  //  return NormalWithMaxBinCount(hist);

  TH1F *reWeightedHist = (TH1F *)hist->Clone();
  reWeightedHist->Divide(NormalWithMaxBinCount(hist));
  return reWeightedHist;

  TH1F *histNormalizedWithMaxVal = NormalWithMaxBinCount(hist);
  unsigned int numOfBins         = hist->GetNbinsX();
  for (unsigned int binNo = 0; binNo < numOfBins; binNo++) {
    if (reWeightedHist->GetBinContent(binNo) > 0) {
      double reWeightedValue = reWeightedHist->GetBinContent(binNo) / histNormalizedWithMaxVal->GetBinContent(binNo);
      std::cout << RED << "Reweighted Value : " << reWeightedValue << RESET << std::endl;
      reWeightedHist->SetBinContent(binNo, reWeightedValue);
    }
  }
  return reWeightedHist;
}*/

TH1F *NormalWithMaxBinCount(TH1F *hist)
{
  double maxCount        = hist->GetBinContent(hist->GetMaximumBin());
  unsigned int numOfBins = hist->GetNbinsX();

  TH1F *normalizedHist = (TH1F *)hist->Clone();

  for (unsigned int binNo = 0; binNo < numOfBins; binNo++) {
    normalizedHist->SetBinContent(binNo, normalizedHist->GetBinContent(binNo) / maxCount);
  }
  return normalizedHist;
}

unsigned int GetPixelIndexFromValue(double val)
{
  unsigned int pixelIndex = 10000;
  if (val >= -50 && val < -35)
    pixelIndex = 0;
  else {
    if (val >= 35 && val <= 50)
      pixelIndex = 8;
    else {

      for (unsigned int i = 0; i < 7; i++) {
        double start = -35 + (i * 10);
        double end   = -35 + ((i + 1) * 10);
        if (val >= start && val < end) pixelIndex = (i + 1);
      }
    }
  }
  return pixelIndex;
}

float roundoff(float value, unsigned char prec)
{
  float pow_10 = pow(10.0f, (float)prec);
  return round(value * pow_10) / pow_10;
}

float roundoff(float inputValue)
{
  // 37.66666 * 100 =3766.66
  // 3766.66 + .5 =3767.16    for rounding off value
  // then type cast to int so value is 3767
  // then divided by 100 so the value converted into 37.67
  float value = (int)(inputValue * 100 + .5);
  std::cout << BLUE << "Value 1 : " << value << std::endl;
  value /= 100;
  std::cout << BLUE << "Value 2 : " << value << RESET << std::endl;

  return value; //(float)value / 100;
}
void NormalizeHistUsingMaxCount(TH1F *hist)
{
  double maxVal = hist->GetMaximum();
  double nbins  = hist->GetNbinsX();

  for (unsigned int i = 1; i <= nbins; i++) {
    hist->SetBinContent(i, hist->GetBinContent(i) / maxVal);
  }
}
HelperFunctions::HelperFunctions()
{
  // TODO Auto-generated constructor stub
}

HelperFunctions::~HelperFunctions()
{
  // TODO Auto-generated destructor stub
}

OfStream::OfStream(std::string filename)
{
  fOutfile.open(filename); //, std::ios::out);
  // TODO Auto-generated constructor stub
}

std::ofstream OfStream::GetFilePointer() const
{
  //  return fOutfile;
}
OfStream::OfStream()
{
  // TODO Auto-generated constructor stub
}

OfStream::~OfStream()
{
  // TODO Auto-generated destructor stub
}
