/*
 * SingleMuonTrack.cpp
 *
 *  Created on: 10-Nov-2020
 *      Author: rsehgal
 */

#include "SingleMuonTrack.h"
#include "ScintillatorBar_V2.h"
#include <iterator>
#include "PsBar.h"
#include "Point3D.h"
#include "HardwareNomenclature.h"
#include <TVector3.h>
#include "Plotter.h"
#include <algorithm>
#include <TF1.h>
#ifndef FOR_SIMULATION
ClassImp(lite_interface::SingleMuonTrack)
#endif
    namespace lite_interface
{

  unsigned long int SingleMuonTrack::wrongTrackCounter = 0;

  ScintillatorBar_V2 *SingleMuonTrack::GetScintillator(unsigned int barIndex)
  {
    for (unsigned int i = 0; i < size(); i++) {
      if (fSingleMuonTrack[i]->GetBarIndex() == barIndex) return fSingleMuonTrack[i];
    }
  }

  SingleMuonTrack::SingleMuonTrack()
  {
    // TODO Auto-generated constructor stub
    // std::cout << "SINGLEMUONTRACK : DEfault Contructor called" << std::endl;
    fHitInAllLayers = false;
    // fHitInRequiredLayers = false;
  }

  SingleMuonTrack::SingleMuonTrack(std::vector<ScintillatorBar_V2 *> vecOfScintBars)
  {
    fHitInAllLayers = true;
    // fHitInRequiredLayers = true;
    std::vector<unsigned int> vecOfHittedBarIndex;
    for (unsigned int i = 0; i < numOfLayers; i++) {
      vecOfHittedBarIndex.push_back(0);
    }
    fSingleMuonTrack = vecOfScintBars;
    for (unsigned int i = 0; i < vecOfScintBars.size(); i++) {
      vecOfHittedBarIndex[vecOfScintBars[i]->GetLayerIndex()]++;
    }

    // std::cout << "====================================" << std::endl;
    for (unsigned int i = 0; i < numOfLayers; i++) {
      fHitInAllLayers &= (vecOfHittedBarIndex[i] > 0);
      /*
          if(vecOfHittedBarIndex[i] > 0){
            std::cout << "HIT FOUND IN LAYER  " << i << std::endl;
          }
          if(fHitInAllLayers)
          fHitInAllLayers &= (vecOfHittedBarIndex[i] > 0);
          else
            fHitInAllLayers |= (vecOfHittedBarIndex[i] > 0);
      */
    }

    // std::cout << "*************************************" << std::endl;
    for (unsigned int i = 0; i < vecOfRequiredLayers.size(); i++) {
      /*if(vecOfHittedBarIndex[vecOfRequiredLayers[i]] > 0){
        std::cout <<"hit found in layer : " << i << std::endl;
      }*/
      // fHitInRequiredLayers &= (vecOfHittedBarIndex[vecOfRequiredLayers[i]] > 0);
    }
  }

  SingleMuonTrack::SingleMuonTrack(const SingleMuonTrack &smt)
  {

    // std::cout << "SINGLEMUONTRACK : Copy Contructor called" << std::endl;
    // smt.GetMuonTrack()[0]->Print();
    fHitInAllLayers = smt.fHitInAllLayers;
    // fHitInRequiredLayers = smt.fHitInRequiredLayers;
    for (unsigned int i = 0; i < smt.GetMuonTrack().size(); i++) {
      // fSingleMuonTrack.push_back(new ScintillatorBar_V2(*smt.GetMuonTrack()[i]));
      fSingleMuonTrack.push_back(new lite_interface::ScintillatorBar_V2(*(smt.GetMuonTrack()[i])));
    }
  }

  SingleMuonTrack::~SingleMuonTrack()
  {
    // TODO Auto-generated destructor stub
  }

  void SingleMuonTrack::push_back(ScintillatorBar_V2 * hittedBar) { fSingleMuonTrack.push_back(hittedBar); }

  bool SingleMuonTrack::CompareBarIndexInScintillator(ScintillatorBar_V2 * i1, ScintillatorBar_V2 * i2)
  {
    return (i1->fBarIndex > i2->fBarIndex);
  }

  void SingleMuonTrack::Sort()
  {
    std::sort(fSingleMuonTrack.begin(), fSingleMuonTrack.end(), CompareBarIndexInScintillator);
  }

  void SingleMuonTrack::clear() { fSingleMuonTrack.clear(); }

  unsigned long int SingleMuonTrack::size() { return fSingleMuonTrack.size(); }

  unsigned int SingleMuonTrack::NumOfHitsInLayer(unsigned int layerNum)
  {
    unsigned int noOfHits = 0;
    for (unsigned int i = 0; i < size(); i++) {
      if (fSingleMuonTrack[i]->GetLayerIndex() == layerNum) {
        noOfHits++;
      }
    }
    return noOfHits;
  }

  bool SingleMuonTrack::HitInAllLayers()
  {
    // return fHitInAllLayers;
    std::vector<unsigned int> vecOfHittedLayers;
    for (unsigned int i = 0; i < numOfLayers; i++) {
      vecOfHittedLayers.push_back(0);
    }
    // fSingleMuonTrack = vecOfScintBars;
    for (unsigned int i = 0; i < size(); i++) {
      vecOfHittedLayers[fSingleMuonTrack[i]->GetLayerIndex()]++;
    }

    std::cout << "====================================" << std::endl;
    bool hitInAllLayers = true;
    for (unsigned int i = 0; i < numOfLayers; i++) {
      hitInAllLayers &= (vecOfHittedLayers[i] > 0);
    }

    /*
        bool hitInRequiredLayers = true;
        std::cout << "*************************************" << std::endl;
        for(unsigned int i = 0 ; i < vecOfRequiredLayers.size() ; i++){
          hitInRequiredLayers &= (vecOfHittedLayers[vecOfRequiredLayers[i]] > 0);
        }
    */
    return hitInAllLayers;
  }

  bool SingleMuonTrack::HitInRequiredLayers()
  {
    // return fHitInRequiredLayers;
    std::vector<unsigned int> vecOfHittedLayers;
    for (unsigned int i = 0; i < numOfLayers; i++) {
      vecOfHittedLayers.push_back(0);
    }
    // fSingleMuonTrack = vecOfScintBars;
    for (unsigned int i = 0; i < size(); i++) {
      vecOfHittedLayers[fSingleMuonTrack[i]->GetLayerIndex()]++;
    }

    /*std::cout << "====================================" << std::endl;
    bool hitInAllLayers = true;
    for(unsigned int i = 0 ; i < numOfLayers; i++){
      hitInAllLayers &= (vecOfHittedLayers[i] > 0);
    }*/

    bool hitInRequiredLayers = true;
    // std::cout << "*************************************" << std::endl;
    for (unsigned int i = 0; i < vecOfRequiredLayers.size(); i++) {
      hitInRequiredLayers &= (vecOfHittedLayers[vecOfRequiredLayers[i]] > 0);
    }

    return hitInRequiredLayers;
  }

#if (0)
  bool SingleMuonTrack::HitInRequiredLayers(std::vector<unsigned int> reqLayersVec)
  {
    // return fHitInRequiredLayers;
    std::vector<unsigned int> vecOfHittedLayers;
    for (unsigned int i = 0; i < numOfLayers; i++) {
      vecOfHittedLayers.push_back(0);
    }
    // fSingleMuonTrack = vecOfScintBars;
    for (unsigned int i = 0; i < size(); i++) {
      vecOfHittedLayers[fSingleMuonTrack[i]->GetLayerIndex()]++;
    }

    /*std::cout << "====================================" << std::endl;
    bool hitInAllLayers = true;
    for(unsigned int i = 0 ; i < numOfLayers; i++){
      hitInAllLayers &= (vecOfHittedLayers[i] > 0);
    }*/

    bool hitInRequiredLayers = true;
    // std::cout << "*************************************" << std::endl;
    for (unsigned int i = 0; i < reqLayersVec.size(); i++) {
      hitInRequiredLayers &= (vecOfHittedLayers[reqLayersVec[i]] > 0);
    }

    return hitInRequiredLayers;
  }
#endif
  bool SingleMuonTrack::HitInRequiredLayers(std::vector<unsigned int> reqLayersVec)
  {
    bool hitInRequiredLayers = true;
    for (unsigned int i = 0; i < reqLayersVec.size(); i++) {
      bool hitInCurrentLayer = false;
      for (unsigned int index = 0; index < size(); index++) {
        hitInCurrentLayer |= (fSingleMuonTrack[index]->GetLayerIndex() == reqLayersVec[i]);
        if (hitInCurrentLayer) break;
      }

      hitInRequiredLayers &= hitInCurrentLayer;
      if (!hitInRequiredLayers) break;
    }

    return hitInRequiredLayers;
  }

  bool SingleMuonTrack::SingleHitInLayer(unsigned int layerIndex)
  {
    std::vector<unsigned int> vecOfNumOfHits;
    for (unsigned int i = 0; i < numOfLayers; i++) {
      vecOfNumOfHits.push_back(NumOfHitsInLayer(i));
    }

    /*
     * If number of hits in each layer is 0 or 1
     * then return true else false
     */
    bool singleHit = true;
    /*for(unsigned int i = 0 ; i < numOfLayers ; i++){
      singleHit &= (vecOfNumOfHits[i]==1);
    }*/

    singleHit &= (vecOfNumOfHits[layerIndex] == 1);
    return singleHit;
  }

  bool SingleMuonTrack::SingleHitInEachLayer()
  {
    std::vector<unsigned int> vecOfNumOfHits;
    for (unsigned int i = 0; i < numOfLayers; i++) {
      vecOfNumOfHits.push_back(NumOfHitsInLayer(i));
    }

    /*
     * If number of hits in each layer is 0 or 1
     * then return true else false
     */
    bool singleHit = true;
    for (unsigned int i = 0; i < numOfLayers; i++) {
      singleHit &= (vecOfNumOfHits[i] <= 1);
    }
    return singleHit;
  }

  SingleMuonTrack *SingleMuonTrack::GetIncomingTrack()
  {
    std::vector<ScintillatorBar_V2 *> vecOfScintBars;
    if (SingleHitInEachLayer()) {
      for (unsigned int i = 0; i < size(); i++) {
        /*if(fSingleMuonTrack[i]->GetLayerIndex()==5 ||
           fSingleMuonTrack[i]->GetLayerIndex()==6 ||
           fSingleMuonTrack[i]->GetLayerIndex()==7)*/
        if (std::count(incomingLayerIndices.begin(), incomingLayerIndices.end(),
                       fSingleMuonTrack[i]->GetLayerIndex())) {
          vecOfScintBars.push_back(fSingleMuonTrack[i]);
        }
      }
      return (new SingleMuonTrack(vecOfScintBars));
    }
    return NULL;
  }

  SingleMuonTrack *SingleMuonTrack::GetOutgoingTrack()
  {
    std::vector<ScintillatorBar_V2 *> vecOfScintBars;
    if (SingleHitInEachLayer()) {
      for (unsigned int i = 0; i < size(); i++) {
        if (std::count(outgoingLayerIndices.begin(), outgoingLayerIndices.end(),
                       fSingleMuonTrack[i]->GetLayerIndex())) {
          vecOfScintBars.push_back(fSingleMuonTrack[i]);
        }
      }
      return (new SingleMuonTrack(vecOfScintBars));
    }
    return NULL;
  }

  void SingleMuonTrack::Print()
  {
    // if(fHitInRequiredLayers)
    {
      // std::cout << "==== Print from SingleMuonTrack  : size : " << fSingleMuonTrack.size() << " : Zenith Angle : " <<
      // GetZenithAngle(4) <<"  ===== : Hit_In_Required_Layers : " << fHitInRequiredLayers <<  std::endl;
      for (unsigned int j = 0; j < fSingleMuonTrack.size(); j++) {
        fSingleMuonTrack[j]->Print();
      }
    }
    // std::cout << "@@@@@@@@@2 Total Energy Deposited : " << fTotalEnergyDeposited << " @@@@@@@@@" <<  std::endl;
  }

  std::vector<ScintillatorBar_V2 *> SingleMuonTrack::GetMuonTrack() const { return fSingleMuonTrack; }

  std::vector<unsigned short> SingleMuonTrack::GetLayersHitCountVector()
  {
    std::vector<unsigned short> layerHitCountVector;
    for (unsigned short i = 0; i < numOfLayers; i++)
      layerHitCountVector.push_back(0);

    std::vector<ScintillatorBar_V2 *>::iterator itr;
    for (itr = fSingleMuonTrack.begin(); itr != fSingleMuonTrack.end(); itr++) {
      layerHitCountVector[(*itr)->GetLayerIndex()]++;
    }
    return layerHitCountVector;
  }

  bool SingleMuonTrack::IsClearTrack()
  {
    std::vector<unsigned short> layerHitCountVector = GetLayersHitCountVector();
    std::vector<unsigned short>::iterator itr;
    bool clear = true;
    for (itr = layerHitCountVector.begin(); itr != layerHitCountVector.end(); itr++) {
      clear &= ((*itr) <= 1);
      if (!clear) break;
    }
    return clear;
  }

//#ifdef USE_FOR_SIMULATION
#if defined(USE_FOR_SIMULATION) || defined(FOR_SIMULATION)
  std::vector<lite_interface::Point3D *> SingleMuonTrack::GetMean3DHitPointVector()
  {
    std::vector<lite_interface::Point3D *> vectorOf3DHitPoint;
    std::vector<ScintillatorBar_V2 *>::iterator itr;
    // int count = 0 ;
    for (itr = fSingleMuonTrack.begin(); itr != fSingleMuonTrack.end(); itr++) {

      vectorOf3DHitPoint.push_back((*itr)->GetMeanHitPosition());
      // vectorOf3DHitPoint[count]->Print();
      // count++;
    }
    return vectorOf3DHitPoint;
  }

  std::vector<lite_interface::Point3D *> SingleMuonTrack::GetExact3DHitPointVector()
  {
    std::vector<lite_interface::Point3D *> vectorOf3DHitPoint;
    std::vector<ScintillatorBar_V2 *>::iterator itr;
    // int count = 0 ;
    for (itr = fSingleMuonTrack.begin(); itr != fSingleMuonTrack.end(); itr++) {

      vectorOf3DHitPoint.push_back((*itr)->GetExactHitPosition());
    }
    return vectorOf3DHitPoint;
  }

#endif
  std::vector<lite_interface::Point3D *> SingleMuonTrack::GetSmeared3DHitPointVector()
  {
    std::vector<lite_interface::Point3D *> vectorOf3DHitPoint;
    std::vector<ScintillatorBar_V2 *>::iterator itr;
    // int count = 0 ;
    for (itr = fSingleMuonTrack.begin(); itr != fSingleMuonTrack.end(); itr++) {

      vectorOf3DHitPoint.push_back((*itr)->GetSmearedHitPosition());
    }
    return vectorOf3DHitPoint;
  }
  std::vector<lite_interface::Point3D *> SingleMuonTrack::Get3DHitPointVector()
  {
    std::vector<lite_interface::Point3D *> vectorOf3DHitPoint;
    std::vector<ScintillatorBar_V2 *>::iterator itr;
    for (itr = fSingleMuonTrack.begin(); itr != fSingleMuonTrack.end(); itr++) {
      vectorOf3DHitPoint.push_back((*itr)->EstimateHitPosition());
    }
    return vectorOf3DHitPoint;
  }

  std::vector<lite_interface::Point3D *> SingleMuonTrack::Get3DHitPointVector_Param()
  {
    std::vector<lite_interface::Point3D *> vectorOf3DHitPoint;
    std::vector<ScintillatorBar_V2 *>::iterator itr;
    for (itr = fSingleMuonTrack.begin(); itr != fSingleMuonTrack.end(); itr++) {
      vectorOf3DHitPoint.push_back((*itr)->EstimateHitPosition_Param());
    }
    return vectorOf3DHitPoint;
  }

  std::vector<lite_interface::Point3D *> SingleMuonTrack::GetFittedTrack(int opt)
  {
    std::vector<lite_interface::Point3D *> fittedTrack;
    if (opt == 0) {
      // For Linear
      fittedTrack = lite_interface::CreateFittedTrack(Get3DHitPointVector());
      //#ifdef USE_FOR_SIMULATION
      /*for(unsigned int i = 0 ; i < size() ; i++){
        fSingleMuonTrack[i]->fittedLinear = fittedTrack[i];
      }*/
      ///#endif
    }
    if (opt == 1) {
      // For Param
      fittedTrack = lite_interface::CreateFittedTrack(Get3DHitPointVector_Param());
      //#ifdef USE_FOR_SIMULATION
      /*for(unsigned int i = 0 ; i < size() ; i++){
        fSingleMuonTrack[i]->fittedParam = fittedTrack[i];
      }*/
      //#endif
    }
    /*
    #ifdef USE_FOR_SIMULATION
      if(opt == 2){
        //For Mean
        fittedTrack = lite_interface::CreateFittedTrack(GetMean3DHitPointVector());
        for(unsigned int i = 0 ; i < size() ; i++){
          fSingleMuonTrack[i]->fittedMean = fittedTrack[i];
        }
      }
    #endif
    */
    return fittedTrack;
  }

  double SingleMuonTrack::GetEnergySum()
  {
    double energySum = 0;
    std::vector<ScintillatorBar_V2 *>::iterator itr;
    // std::cout <<"==================================" << std::endl;
    // std::cout << "Size of SingleMuon Track : " << size() << std::endl;

    for (itr = fSingleMuonTrack.begin(); itr != fSingleMuonTrack.end(); itr++) {
      double ener = (*itr)->GetQMeanCorrected();
      // std::cout << "Energy :  "<< ener << std::endl;
      energySum += ener;
    }
    // std::cout << "Energy Sum : " << energySum << std::endl;
    return energySum;
  }

  std::vector<std::string> SingleMuonTrack::GetBarNamesVector()
  {
    std::vector<std::string> barNamesVector;
    std::vector<ScintillatorBar_V2 *>::iterator itr;
    for (itr = fSingleMuonTrack.begin(); itr != fSingleMuonTrack.end(); itr++) {
      barNamesVector.push_back(vecOfBarsNamess[(*itr)->GetBarIndex()]);
    }
    return barNamesVector;
  }

  std::vector<double> SingleMuonTrack::GetDepositedEnergyVector()
  {
    std::vector<double> energyVec;
    std::vector<ScintillatorBar_V2 *>::iterator itr;
    for (itr = fSingleMuonTrack.begin(); itr != fSingleMuonTrack.end(); itr++) {
      energyVec.push_back((*itr)->GetQMeanCorrected());
    }
    return energyVec;
  }

  double SingleMuonTrack::GetZenithAngle(std::vector<lite_interface::Point3D *> vecOfPoint3D)
  {
    TVector3 ref(0., -1., 0.);
    Point3D *startPoint = vecOfPoint3D[0];
    Point3D *endPoint   = vecOfPoint3D[vecOfPoint3D.size() - 1];
    TVector3 muonDir(TVector3(endPoint->GetX(), endPoint->GetY(), endPoint->GetZ()) -
                     TVector3(startPoint->GetX(), startPoint->GetY(), startPoint->GetZ()));
    return muonDir.Angle(ref);
  }

  bool SingleMuonTrack::IsThroughTrack()
  {

    if ((fSingleMuonTrack[0]->GetLayerIndex() == (numOfLayers - 1)) &&
        (fSingleMuonTrack[size() - 1]->GetLayerIndex() == 0))
      return true;
  }

  double SingleMuonTrack::GetZenithAngle(int opt)
  {
    if (opt == 1) {
      std::vector<lite_interface::Point3D *> pt3DVec = Get3DHitPointVector();
      // return GetZenithAngle(CreateFittedTrack(Get3DHitPointVector()));
      if (pt3DVec.size() > 2)
        return GetZenithAngle(CreateFittedTrack(pt3DVec));
      else
        return -1.;
    }
    if (opt == 2) {
      std::vector<lite_interface::Point3D *> pt3DVec = Get3DHitPointVector_Param();
      // return GetZenithAngle(CreateFittedTrack(Get3DHitPointVector_Param()));
      if (pt3DVec.size() > 2)
        return GetZenithAngle(CreateFittedTrack(pt3DVec));
      else
        return -1.;
    }
//#ifdef USE_FOR_SIMULATION
#if defined(USE_FOR_SIMULATION) || defined(FOR_SIMULATION)
    if (opt == 3) {
      std::vector<lite_interface::Point3D *> pt3DVec = GetMean3DHitPointVector();
      /*std::cout << "========= Mean Hit Points of a track ========== " << std::endl;
      for(unsigned int j = 0 ; j < pt3DVec.size() ; j++){
        pt3DVec[j]->Print();
      }*/

      // std::cout << "Printing just before the calculation of Zenith angle using Mean hit point............" <<
      // std::endl;
      if (pt3DVec.size() > 2)
        return GetZenithAngle(CreateFittedTrack(pt3DVec));
      else
        return -1.;
    }

    if (opt == 4) {
      int count                                      = 0;
      std::vector<lite_interface::Point3D *> pt3DVec = GetExact3DHitPointVector();
      if (pt3DVec.size() > 2) {

        /*std::cout << "========= Exact Hit Points of a track ========== " << std::endl;
        for(unsigned int j = 0 ; j < pt3DVec.size() ; j++){
          pt3DVec[j]->Print();
        }*/

        /*if(pt3DVec[0]->GetY() < pt3DVec[pt3DVec.size()-1]->GetY()){
          std::cout << "WRONG TRACK ...." << std::endl;
          wrongTrackCounter++;
          return -1.;
        }*/
      }
      /*std::cout <<"================== Printing Vector of Exact Hit point ===== : Size : " << pt3DVec.size() << "
      ==========" << std::endl; for(unsigned int i = 0 ; i < pt3DVec.size() ; i++){ pt3DVec[i]->Print();
      }*/
      // std::cout << "Printing just before the calculation of Zenith angle using Exact hit point............" <<
      // std::endl;
      if (pt3DVec.size() > 2)
        return GetZenithAngle(CreateFittedTrack(pt3DVec));
      else
        return -1.;
      // return GetZenithAngle(pt3DVec);
    }
#endif
  }
  double SingleMuonTrack::GetZenithAngle_Linear() { return GetZenithAngle(1); }
  double SingleMuonTrack::GetZenithAngle_Param() { return GetZenithAngle(2); }

  bool SingleMuonTrack::IfPassThroughOneOrMoreOfScintillators(std::vector<unsigned int> vecOfScintBarIndex)
  {
    bool exist = false;
    for (unsigned int barIdIndex = 0; barIdIndex < vecOfScintBarIndex.size(); barIdIndex++) {

      for (unsigned int index = 0; index < size(); index++) {
        exist |= (fSingleMuonTrack[index]->GetBarIndex() == vecOfScintBarIndex[barIdIndex]);
        if (exist) return exist;
      }
    }
    return exist;
  }

  unsigned short SingleMuonTrack::GetNumOfHittedScintillatorsInLayer(unsigned short layerIndex)
  {
    unsigned short hitCounter = 0;
    for (unsigned int index = 0; index < size(); index++) {
      if (fSingleMuonTrack[index]->GetLayerIndex() == layerIndex) hitCounter++;
    }
    return hitCounter;
  }

  /*
   * Will be used to do Anti-Coincidence
   */
  bool SingleMuonTrack::NoHitInScintillators(std::vector<unsigned int> vecOfScintBarIndex)
  {
    bool hitFound = false;
    for (unsigned int barIdIndex = 0; barIdIndex < vecOfScintBarIndex.size(); barIdIndex++) {
      for (unsigned int index = 0; index < size(); index++) {
        hitFound = (fSingleMuonTrack[index]->GetBarIndex() == vecOfScintBarIndex[barIdIndex]);
        if (hitFound) {
          return !hitFound;
        }
      }
    }
    return !hitFound;
  }

  bool SingleMuonTrack::CheckTrackForRequiredScintillators(std::vector<unsigned int> vecOfScintBarIndex)
  {
    bool exist = true;
    for (unsigned int barIdIndex = 0; barIdIndex < vecOfScintBarIndex.size(); barIdIndex++) {
      bool existLocal = false;
      for (unsigned int index = 0; index < size(); index++) {
        existLocal |= (fSingleMuonTrack[index]->GetBarIndex() == vecOfScintBarIndex[barIdIndex]);
        if (existLocal) break;
      }
      exist &= existLocal;
      if (!exist) break;
    }
    return exist;
  }

  bool SingleMuonTrack::HitInAnyScintillators(std::vector<unsigned int> vecOfScintBarIndex)
  {
    bool hitFound = false;
    for (unsigned int i = 0; i < vecOfScintBarIndex.size(); i++) {
      hitFound |= CheckTrackForScintillator(vecOfScintBarIndex[i]);
      if (hitFound) return hitFound;
    }
    return hitFound;
  }

  bool SingleMuonTrack::CheckTrackForScintillator(unsigned int barIndex)
  {
    bool exist = false;
    for (unsigned int index = 0; index < size(); index++) {
      exist |= (fSingleMuonTrack[index]->GetBarIndex() == barIndex);
      if (exist) return exist;
    }
    return exist;
  }
  bool SingleMuonTrack::CheckTrackForLayerNum(unsigned int layerIndex, unsigned int &hittedBarIndex)
  {
    bool exist = false;
    for (unsigned int barGeomIndex = layerIndex * numOfBarsInEachLayer;
         barGeomIndex < (layerIndex + 1) * numOfBarsInEachLayer; barGeomIndex++) {
      for (unsigned int index = 0; index < size(); index++) {
        exist |= (fSingleMuonTrack[index]->GetBarIndex() == barGeomIndex);
        if (exist) {
          hittedBarIndex = barGeomIndex;
          return exist;
        }
      }
    }
    hittedBarIndex = 10000;
    return exist;
  }

  /*
   * Return vector of hitted Scintillators IF muon is detected by specified layers
   *
   * The calling function should check the size of returned vector.
   * IF size is not equal to size specified scintillator layers then it means
   * some of the layers has not detected the muon.
   */
  std::vector<unsigned int> SingleMuonTrack::CheckTrackForRequiredLayers(std::vector<unsigned int> vecOfLayerIndex)
  {

    std::vector<unsigned int> vecOfHittedBarIndex;
    bool hitted = true;
    for (unsigned int i = 0; i < vecOfLayerIndex.size(); i++) {
      unsigned int hittedBarIndex = 10000;
      bool hittedLocal            = false;
      hittedLocal                 = CheckTrackForLayerNum(vecOfLayerIndex[i], hittedBarIndex);
      if (hittedLocal) {
        vecOfHittedBarIndex.push_back(hittedBarIndex);
      }
      hitted &= hittedLocal;
      if (!hitted) break;
    }
    return vecOfHittedBarIndex;
  }

  int SingleMuonTrack::GetTraversalTime()
  {
    lite_interface::ScintillatorBar_V2 *scintStart = fSingleMuonTrack[0];
    lite_interface::ScintillatorBar_V2 *scintEnd   = fSingleMuonTrack[size() - 1];

    return (scintEnd->GetTSmallTimestamp() - scintStart->GetTSmallTimestamp());
  }

  TF1 *SingleMuonTrack::GetFitFormula(unsigned short trackType, bool planeType)
  {
    TF1 *formula;
    std::vector<lite_interface::Point3D *> vecOfPoint3D;
    if (trackType == exactTrack) {
      vecOfPoint3D = InCm(GetExact3DHitPointVector());
      if (planeType)
        formula = lite_interface::GetFittedMuonTrackFormulaXY(vecOfPoint3D, true);
      else
        formula = lite_interface::GetFittedMuonTrackFormulaZY(vecOfPoint3D, true);
    } else {
      if (trackType == smearedTrack) {
        vecOfPoint3D = InCm(GetSmeared3DHitPointVector());
        if (planeType)
          formula = lite_interface::GetFittedMuonTrackFormulaXY(vecOfPoint3D, false);
        else
          formula = lite_interface::GetFittedMuonTrackFormulaZY(vecOfPoint3D, false);
      }
    }
    return formula;
  }

//#ifdef USE_FOR_SIMULATION
#if defined(USE_FOR_SIMULATION) || defined(FOR_SIMULATION)
  double SingleMuonTrack::GetZenithAngle_MeanHitPoint() { return GetZenithAngle(3); }
  double SingleMuonTrack::GetZenithAngle_ExactHitPoint() { return GetZenithAngle(4); }
#endif

} /* namespace lite_interface */
