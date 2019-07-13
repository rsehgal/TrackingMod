//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B1EventAction.cc 75117 2013-10-28 09:38:37Z gcosmo $
//
/// \file B1EventAction.cc
/// \brief Implementation of the B1EventAction class

#include "B1EventAction.hh"
#include "B1Run.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
//#include "Tree.h"
#include "B1RunAction.hh"
#include <TVector3.h>

//Trying to use new Architecture
#include "EventHelper.h"
#include "Voxelator_Evolution.h"
#include "Imaging.h"
#include "CommonFunc.h"
#include "DetectorMapping.h"

#include "Files.h"
#include "B1SteppingAction.hh"

#include "base/Global.h"

#include "Coordinates.h"

#include "Fit2DLinear.h"

#include "TrackFinder.h"

//Included to check for NAN
#include <cmath>
#include <cfloat>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
int B1EventAction::noTrigger = 0;
int B1EventAction::evNo = 0;
int B1EventAction::effEvNo = -1;
int B1EventAction::evMultiplicity = 0;
int B1EventAction::genuineEventCounter = 0;
std::vector<G4String> B1EventAction::volName({"Hello"});
std::vector<G4double> B1EventAction::energy({0.});
std::vector<G4double> B1EventAction::vertexEnergy({0.});
std::vector<G4ThreeVector> B1EventAction::position({G4ThreeVector(0.,0.,0.)});
double B1EventAction::eventEnergy = -1.;
double B1EventAction::scattererHitted = 0.;

bool B1EventAction::topPlaneHit = false;
bool B1EventAction::bottomPlaneHit = false;
std::vector<std::string> B1EventAction::hittedStripNameVector({"StripName"});

double B1EventAction::momentum = 0.;
double B1EventAction::meanMomentum = 0.;

B1EventAction::B1EventAction()
: G4UserEventAction(),
  fEdep(0.)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::~B1EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event*)
{ //evNo++;
//std::cout << "-------------------------------------------------------"<< std::endl;
  //Settig topPlaneHit and bottomPlaneHit to false
  // if(effEvNo > 100)
  //   return;
  if(Tomography::EventBreak::instance()->BreakSimulation()){
     //G4RunManager::GetRunManager()->TerminateEventLoop();
     //unsigned long long int numOfEvents = G4RunManager::GetRunManager()->GetNumberOfEventsToBeProcessed();
     //std::cout << "SHACHI : numOfEvents before : " << numOfEvents << std::endl;
     //G4RunManager::GetRunManager()->SetNumberOfEventsToBeProcessed(Tomography::EventBreak::instance()->fNumOfTrigger);
     //numOfEvents = G4RunManager::GetRunManager()->GetNumberOfEventsToBeProcessed();
     //std::cout << "SHACHI : numOfEvents after : " << numOfEvents << std::endl;
     G4RunManager::GetRunManager()->AbortRun();
    return;
  }

  topPlaneHit = false;
  bottomPlaneHit = false;
  hittedStripNameVector.clear();

  std::cout << "========  " << evNo << "  Started... =======" << std::endl;
  B1SteppingAction::stepNum = -1;
//if(!(evNo%10000))
  //std::cout << "========  " << evNo << "  Started... =======" << std::endl;
  fEdep = 0.;
  evMultiplicity=0;
  volName.clear();
  energy.clear();
  position.clear();


  B1RunAction::fScatteringAngle = 0.;
  InitializeBranchMap();
  //(B1RunAction::brMap["Module2_LE_CH31"]).push_back((int)Tracking::Global::GenRandomDet(19000, 24000));
  (B1RunAction::brMap["Module2_LE_CH31"]).push_back(
    (int)Tracking::Global::GenRandomDet(Tomography::start, Tomography::end));

  //TODO : Create branches for TopPlane and BottomPlane
  /*for(int i = 0 ; i  < 16 ; i++){
  (B1RunAction::brMap["Module2_LE_CH"+std::to_string(i)]).push_back(
    (int)Tracking::Global::GenRandomDet(Tomography::start, Tomography::end));}*/


  //Setting scattererHitted to false
  scattererHitted = 0.;


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::EndOfEventAction(const G4Event*)
{  
//	ofstream outfile("Hits.txt",std::ios::app);
 // Tomography::Files::instance()->Write("test3.txt",3, 5.,6.,7.);
 
 if(Tomography::EventBreak::instance()->BreakSimulation())
        return;
  if(effEvNo == Tomography::EventBreak::instance()->fNumOfEvents){
    std::cout <<"***********************************" << std::endl;
    std::cout <<"========= Raman100 events done=========" << std::endl;
    std::cout <<"***********************************" << std::endl;
 }
  // accumulate statistics in B1Run
  B1Run* run 
    = static_cast<B1Run*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  run->AddEdep(fEdep);
 // Tracking::Tree::instance()->Fill();
 // std::cout << "======== Event no : "<< evNo << "  ended  =======" << std::endl;
  if(evMultiplicity == 2){
      genuineEventCounter++;
      std::cout<<"Event Num : "<< evNo <<" :: EventMultiplicity : " << evMultiplicity << " :: DetectorNames : "
          << volName[0] << " : " << volName[1]
        << " :: Position : " << position[0] <<" : "<< position[1]
        << " :: VertexEnergy : "<< vertexEnergy[0] <<" : Energy " << energy[0]
        << " :: VertexEnergy : "<< vertexEnergy[1] <<" : Energy " << energy[1] << std::endl;
    }

Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::instance();
//std::vector<std::string> detNamesVector = detectorMap->GetDetectorNamesVector();
std::vector<Tomography::Mapping::Detector*> detectorVector = detectorMap->GetDetectorVector();

/*if(evNo != effEvNo){
  std::cout<<"Ayush : " << evNo << std::endl;
}*/

/* Changing the below mentioned condition, coz its not necessary that all the detectors
** will alway fire, it totally depends on efficieny of detector in real experiment.
**
** But what is must is that trigger should certainly come, then only we should store
** data in ROOT tree
*/
//if(position.size() == (detNamesVector.size()-1)*2  && evNo==effEvNo){
Tomography::Coordinates c;
if(topPlaneHit && bottomPlaneHit) { // logic for two fold coincidence
  std::vector<Vector3D<double>> pixelCentreCoordVector;
  std::vector<Vector3D<double>> hitPointVector;

  Tomography::EventBreak::instance()->fNumOfTrigger++;

//HARDCODING BE CAREFULLLLLLLLLL............
int numOfDetectors = Tomography::DetectorMapping::instance()->GetNumOfDetectors();
numOfDetectors -= 3;
//if(position.size() == 14)
if(position.size() == (2*numOfDetectors+2))
{


	for(int i = 0 ; i < position.size() ; ){
		//outfile << "Event No : " << effEvNo <<" : " << position[i].x() << "  " << position[i].y() << "  " << position[i].z() << std::endl;
     Tomography::Files::instance()->Write("Hits.txt",3, position[i].x(), position[i].y(), position[i].z());
     int xstripNum = c.GetStripNum(position[i].x());
     int ystripNum = c.GetStripNum(position[i].y());
     Vector3D<double> pixelCentreCoord = c.GetStripCoordinate(xstripNum,ystripNum,position[i].z());
     pixelCentreCoordVector.push_back(pixelCentreCoord);
     hitPointVector.push_back(Vector3D<double>(position[i].x(), position[i].y(), position[i].z()));
     //std::cout << "-------------------------" << std::endl;
     //Vector3D<double>(position[i].x(),position[i].y(),position[i].z()).Print(); 
     //std::cout << "XStripNum : " << xstripNum <<" : YStripNum : " << ystripNum << std::endl;
     //pixelCentreCoord.Print();
     if(i==0)
      i++;
     else
		  i += 2;
	}

  //Taking pixelCentreCoor only for RPC, by leaving first and last ScintPlane
  std::vector<Vector3D<double>> pixelCentreCoordVectorRpc;
  for(int i = 1 ; i < pixelCentreCoordVector.size()-1 ; i++){
    pixelCentreCoordVectorRpc.push_back(pixelCentreCoordVector[i]);
  }

  //Taking hitPointVector only for RPC, by leaving first and last ScintPlane
  std::vector<Vector3D<double>> hitPointVectorRpc;
  for(int i = 1 ; i < hitPointVector.size()-1 ; i++){
    hitPointVectorRpc.push_back(hitPointVector[i]);
  }

  int hitPointSize = pixelCentreCoordVectorRpc.size();
  std::vector<Vector3D<double>> incomingHitPtVector;
  std::vector<Vector3D<double>> outgoingHitPtVector;
  std::vector<Vector3D<double>> incomingPixelHitPtVector;
  std::vector<Vector3D<double>> outgoingPixelHitPtVector;

  //Filling incoming hit point vector using top half detectors
  for(int i = 0 ; i < hitPointSize/2 ; i++){
    incomingPixelHitPtVector.push_back(pixelCentreCoordVectorRpc[i]);
    incomingHitPtVector.push_back(hitPointVectorRpc[i]);
  }

  //Filling outgoing hit point vector using bottom half detectors
  for(int i = hitPointSize/2; i < hitPointSize ; i++){
    outgoingPixelHitPtVector.push_back(pixelCentreCoordVectorRpc[i]);
    outgoingHitPtVector.push_back(hitPointVectorRpc[i]);
  }

  //Try to get fitted Hit point vector from pixel center points
  Tomography::Fit2DLinear fitter;
  //std::vector<Tracking::Vector3D<double>> fittedIncomingHitPointVector = fitter.GetFittedTrack(incomingHitPtVector); //incomingPixelHitPtVector);
  //std::vector<Tracking::Vector3D<double>> fittedIncomingHitPointVector = fitter.GetFittedTrack(fitter.EstimatePreFitter(incomingPixelHitPtVector));
  std::vector<Tracking::Vector3D<double>> fittedIncomingHitPointVector = fitter.GetFittedTrack(incomingPixelHitPtVector);
  //std::vector<Tracking::Vector3D<double>> fittedOutgoingHitPointVector = fitter.GetFittedTrack(outgoingHitPtVector); //outgoingPixelHitPtVector);
  std::vector<Tracking::Vector3D<double>> fittedOutgoingHitPointVector = fitter.GetFittedTrack(outgoingPixelHitPtVector);

  //Using sampling from Pixel and getting a Track from TrackFinder
  Tomography::TrackFinder trackFinder;
  trackFinder.SetPixelCenterVector(incomingPixelHitPtVector);
  std::vector<Tracking::Vector3D<double>> fittedSampledIncomingHitPointVector = trackFinder.GetFittedTrackPointVector();
  trackFinder.SetPixelCenterVector(outgoingPixelHitPtVector);
  std::vector<Tracking::Vector3D<double>> fittedSampledOutgoingHitPointVector = trackFinder.GetFittedTrackPointVector();

  for(int i = 0 ; i < incomingHitPtVector.size() ; i++){
    Tomography::Files::instance()->Write("ActualAndSampledFittedHits.txt",6,
                                         incomingHitPtVector[i].x(),
                                         incomingHitPtVector[i].y(),
                                         incomingHitPtVector[i].z(),
                                         fittedSampledIncomingHitPointVector[i].x(),
                                         fittedSampledIncomingHitPointVector[i].y(),
                                         fittedSampledIncomingHitPointVector[i].z()
                                         );
   }

    for(int i = 0 ; i < outgoingHitPtVector.size() ; i++){
    Tomography::Files::instance()->Write("ActualAndSampledFittedHits.txt",6,
                                         outgoingHitPtVector[i].x(),
                                         outgoingHitPtVector[i].y(),
                                         outgoingHitPtVector[i].z(),
                                         fittedSampledOutgoingHitPointVector[i].x(),
                                         fittedSampledOutgoingHitPointVector[i].y(),
                                         fittedSampledOutgoingHitPointVector[i].z()
                                         );
   }


  bool goahead = true;
  for(int j = 0 ; j < fittedIncomingHitPointVector.size() ; j++){
	goahead &= (fittedIncomingHitPointVector[j].GetColor() != -100000) ;
  }
  if(goahead)
  {
  for(int i = 0 ; i < incomingHitPtVector.size() ; i++){
  Tomography::Files::instance()->Write("ActualAndFittedHits.txt",6, 
                                       incomingHitPtVector[i].x(), 
                                       incomingHitPtVector[i].y(), 
                                       incomingHitPtVector[i].z(),
                                       fittedIncomingHitPointVector[i].x(),
                                       fittedIncomingHitPointVector[i].y(),
                                       fittedIncomingHitPointVector[i].z()
                                       );
 }
 }
  goahead = true;
    for(int j = 0 ; j < fittedOutgoingHitPointVector.size() ; j++){
  	goahead &= (fittedOutgoingHitPointVector[j].GetColor() != -100000) ;
    }
    if(goahead)
    {
  for(int i = 0 ; i < outgoingHitPtVector.size() ; i++){
  Tomography::Files::instance()->Write("ActualAndFittedHits.txt",6, 
                                       outgoingHitPtVector[i].x(), 
                                       outgoingHitPtVector[i].y(), 
                                       outgoingHitPtVector[i].z(),
                                       fittedOutgoingHitPointVector[i].x(),
                                       fittedOutgoingHitPointVector[i].y(),
                                       fittedOutgoingHitPointVector[i].z()
                                       );
 }
    }

 for(int i = 0 ; i < incomingHitPtVector.size() ; i++){
 Tomography::Files::instance()->Write("ActualHitAndPixelCenter.txt",6, 
                                       incomingHitPtVector[i].x(), 
                                       incomingHitPtVector[i].y(), 
                                       incomingHitPtVector[i].z(),
                                       incomingPixelHitPtVector[i].x(),
                                       incomingPixelHitPtVector[i].y(),
                                       incomingPixelHitPtVector[i].z()
                                       );
 }

 for(int i = 0 ; i < incomingHitPtVector.size() ; i++){
 Tomography::Files::instance()->Write("ActualHitAndPixelCenter.txt",6, 
                                       outgoingHitPtVector[i].x(),
                                       outgoingHitPtVector[i].y(),
                                       outgoingHitPtVector[i].z(),
                                       outgoingPixelHitPtVector[i].x(),
                                       outgoingPixelHitPtVector[i].y(),
                                       outgoingPixelHitPtVector[i].z()
                                       );
 } 
    

  std::cout<<"======== Sehgal100 Writing to file : " << effEvNo << " =============" << std::endl;
  Tomography::Files::instance()->Write("Hits.txt","===================================");
	effEvNo++;
  Tomography::effEvNo++;
  //Tomography::Event::fEffEvNo++;
  //Tomography::EventBreak::fEffEvNo++;
  Tomography::EventBreak::instance()->fEffEvNo++;

  Tomography::Track ref(G4ThreeVector(0.,0.,0.),G4ThreeVector(0.,0.,-1.));
  //Generating incoming track from exact hit point of first two detectors
  // Tomography::Track incoming(CommonFunc::Functions::instance()->ConvertToVector3D(position[0]),
  //                             CommonFunc::Functions::instance()->ConvertToVector3D(position[2]));
  Tomography::Track incoming(incomingHitPtVector[0],
                               incomingHitPtVector[incomingHitPtVector.size()-1]);

  //Generating outgoing track from exact hit point of last two detectors
  // Tomography::Track outgoing(CommonFunc::Functions::instance()->ConvertToVector3D(position[position.size()-3]),
  //                             CommonFunc::Functions::instance()->ConvertToVector3D(position[position.size()-1]));
  Tomography::Track outgoing(outgoingHitPtVector[0],
                               outgoingHitPtVector[outgoingHitPtVector.size()-1]);

   
  /* In addition to get Poca from exact hit point, let see the results of Poca from
   ** fitted hit points
   */
   Tomography::Track fittedIncoming(fittedIncomingHitPointVector[0],
                                    fittedIncomingHitPointVector[fittedIncomingHitPointVector.size()-1]);
   Tomography::Track fittedOutgoing(fittedOutgoingHitPointVector[0],
                                    fittedOutgoingHitPointVector[fittedOutgoingHitPointVector.size()-1]);
   //Tomography::EventHelper u2(fittedIncoming,fittedOutgoing,"PocaFromFittedHit.txt");


   Tomography::Track fittedSampledIncomingTrack(fittedSampledIncomingHitPointVector[0],
                                       fittedSampledIncomingHitPointVector[fittedSampledIncomingHitPointVector.size()-1]);
   Tomography::Track fittedSampledOutgoingTrack(fittedSampledOutgoingHitPointVector[0],
                                       fittedSampledOutgoingHitPointVector[fittedSampledOutgoingHitPointVector.size()-1]);

   //Tomography::EventHelper u3(fittedSampledIncomingTrack,fittedSampledOutgoingTrack,"PocaFromFittedSampledHit.txt");


   double angleIncoming = CommonFunc::Functions::instance()->GetAngleInRadian(incoming,ref);
   double angleOutgoing = CommonFunc::Functions::instance()->GetAngleInRadian(outgoing,ref);
   double diff = angleOutgoing-angleIncoming;

   double angleFittedIncoming = CommonFunc::Functions::instance()->GetAngleInRadian(fittedIncoming,ref);
   double angleFittedOutgoing = CommonFunc::Functions::instance()->GetAngleInRadian(fittedOutgoing,ref);
   double diffFitted = angleFittedOutgoing-angleFittedIncoming;

   double angleSampledFittedIncoming = CommonFunc::Functions::instance()->GetAngleInRadian(fittedSampledIncomingTrack,ref);
   double angleSampledFittedOutgoing = CommonFunc::Functions::instance()->GetAngleInRadian(fittedSampledOutgoingTrack,ref);
   double diffSampledFitted = angleSampledFittedOutgoing-angleSampledFittedIncoming;

   bool isNan = std::isnan(angleIncoming)
   	   	   	    || std::isnan(angleOutgoing)
   	   	   	    || std::isnan(angleFittedIncoming)
   	   	   	    || std::isnan(angleFittedOutgoing)
   	   	   	    || std::isnan(angleSampledFittedIncoming)
   	    	    || std::isnan(angleSampledFittedOutgoing);

   if(!isNan){

   Tomography::Files::instance()->Write("StatsFromEventAction.txt",10, angleIncoming,angleOutgoing,diff,
                                        angleFittedIncoming,angleFittedOutgoing,diffFitted,
                                        angleSampledFittedIncoming,angleSampledFittedOutgoing,
                                        diffSampledFitted,eventEnergy);

   //if(diff < 0.)
   // std::cout<<"Negative comes............" << std::endl;
   run->FillScatteringAngleVector(diff);//angleOutgoing-angleIncoming);
   //Tomography::EventHelper u(incoming,outgoing,"PocaFromExactHit.txt");
   Tomography::Files::instance()->Write("TrackExact.txt",14,
		   	   	   	   	   	   	   	    incoming.GetP1().x(),incoming.GetP1().y(),incoming.GetP1().z(),
		   	   	   	   	   	   	   	    incoming.GetP2().x(),incoming.GetP2().y(),incoming.GetP2().z(),
		   	   	   	   	   	   	   	    outgoing.GetP1().x(),outgoing.GetP1().y(),outgoing.GetP1().z(),
		   	   	   	   	   	   	   	    outgoing.GetP2().x(),outgoing.GetP2().y(),outgoing.GetP2().z(),scattererHitted,B1EventAction::momentum);

   //Tomography::EventHelper u2(fittedIncoming,fittedOutgoing,"PocaFromFittedHit.txt");
   Tomography::Files::instance()->Write("TrackFitted.txt",14,
		   	   	   	   	   	   	   	   fittedIncoming.GetP1().x(),fittedIncoming.GetP1().y(),fittedIncoming.GetP1().z(),
		   	   	   	   	   	   	   	   fittedIncoming.GetP2().x(),fittedIncoming.GetP2().y(),fittedIncoming.GetP2().z(),
		   	   	   	   	   	   	   	   fittedOutgoing.GetP1().x(),fittedOutgoing.GetP1().y(),fittedOutgoing.GetP1().z(),
		   	   	   	   	   	   	   	   fittedOutgoing.GetP2().x(),fittedOutgoing.GetP2().y(),fittedOutgoing.GetP2().z(),scattererHitted,B1EventAction::momentum);

   //Tomography::EventHelper u3(fittedSampledIncomingTrack,fittedSampledOutgoingTrack,"PocaFromFittedSampledHit.txt");
   Tomography::Files::instance()->Write("TrackSampledFitted.txt",14,
		   	   	   	   	   	   	   	   fittedSampledIncomingTrack.GetP1().x(),fittedSampledIncomingTrack.GetP1().y(),fittedSampledIncomingTrack.GetP1().z(),
		   	   	   	   	   	   	   	   fittedSampledIncomingTrack.GetP2().x(),fittedSampledIncomingTrack.GetP2().y(),fittedSampledIncomingTrack.GetP2().z(),
		   	   	   	   	   	   	   	   fittedSampledOutgoingTrack.GetP1().x(),fittedSampledOutgoingTrack.GetP1().y(),fittedSampledOutgoingTrack.GetP1().z(),
		   	   	   	   	   	   	   	   fittedSampledOutgoingTrack.GetP2().x(),fittedSampledOutgoingTrack.GetP2().y(),fittedSampledOutgoingTrack.GetP2().z(),scattererHitted
									   ,B1EventAction::momentum);

   /* In addition to get Poca from exact hit point, let see the results of Poca from
   ** fitted hit points
   */
   
/*
   Tomography::Track fittedIncoming(fittedIncomingHitPointVector[0],
                                    fittedIncomingHitPointVector[fittedIncomingHitPointVector.size()-1]);
   Tomography::Track fittedOutgoing(fittedOutgoingHitPointVector[0],
                                    fittedOutgoingHitPointVector[fittedOutgoingHitPointVector.size()-1]);
   Tomography::EventHelper u2(fittedIncoming,fittedOutgoing,"PocaFromFittedHit.txt");
*/


   //Generating the data for ROOT tree, which corresponds to TDC value
  for(int i = 0 ; i < hittedStripNameVector.size() ; i++ ){
   (B1RunAction::brMap[hittedStripNameVector[i]]).push_back((int)Tracking::Global::GenRandomDet(19450, 21000));
  }

   } // end of NAN
} // end of if for positionSize check, currently checked for 18
else{

	B1EventAction::meanMomentum -= B1EventAction::momentum;
}


}
else{
	B1EventAction::meanMomentum -= B1EventAction::momentum;
  evNo++;
  noTrigger++;
  std::cout<<"Returning for EvNo : " << evNo << std::endl;
	return;
}

#ifdef STORE
  B1RunAction::fTree->Fill();
#endif

evNo++;
}

void B1EventAction::InitializeBranchMap(){

	for(auto it = B1RunAction::brMap.begin(); it != B1RunAction::brMap.end(); ++it)
	{
	     B1RunAction::brMap[(*it).first].clear();
//	     B1RunAction::brMap[(*it).first].push_back(1);
//	     B1RunAction::brMap[(*it).first].push_back(2);
//	     B1RunAction::brMap[(*it).first].push_back(3);
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
