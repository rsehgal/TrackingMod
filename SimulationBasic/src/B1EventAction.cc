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
#include "Track.h"
#include "CommonFunc.h"
//#include <TVector3.h>

//Trying to use new Architecture
#include "EventHelper.h"
#include "Voxelator_Evolution.h"
#include "Imaging.h"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
int B1EventAction::evNo = 0;
//int B1EventAction::evMultiplicity = 0;
//int B1EventAction::genuineEventCounter = 0;
//std::vector<G4String> B1EventAction::volName({"Hello"});
//std::vector<G4double> B1EventAction::energy({0.});
//std::vector<G4double> B1EventAction::vertexEnergy({0.});
std::vector<G4ThreeVector> B1EventAction::position({G4ThreeVector(0.,0.,0.)});

B1EventAction::B1EventAction()
: G4UserEventAction(),
  fEdep(0.)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::~B1EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event*)
{ evNo++;
if(!(evNo%10000))
  std::cout << "========  " << evNo << "  Events over... =======" << std::endl;
  position.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::EndOfEventAction(const G4Event*)
{  
  // accumulate statistics in B1Run
  B1Run* run 
    = static_cast<B1Run*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  run->AddEdep(fEdep);
 // Tracking::Tree::instance()->Fill();

 Tomography::Track ref(G4ThreeVector(0.,0.,0.),G4ThreeVector(0.,0.,1.));
 Tomography::Track incoming(position[0],position[1]);
 Tomography::Track outgoing(position[2],position[3]);
 double angleIncoming = CommonFunc::Functions::instance()->GetAngleInRadian(incoming,ref);
 double angleOutgoing = CommonFunc::Functions::instance()->GetAngleInRadian(outgoing,ref);
 double diff = angleOutgoing-angleIncoming;
 //if(diff < 0.)
	// std::cout<<"Negative comes............" << std::endl;
 run->FillScatteringAngleVector(diff);//angleOutgoing-angleIncoming);
 run->FillInComingAngleVector(angleIncoming);
 Tomography::EventHelper u(incoming,outgoing);
 //run->FillScatteringAngleVector(CommonFunc::Functions::GetAngleInRadian(incoming,outgoing));
 //std::cout << "======== Event no : "<< evNo << "  ended  =======" << std::endl;

  //std::cout<<"-----------------------------------------------------" << std::endl;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
