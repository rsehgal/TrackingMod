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


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
int B1EventAction::evNo = 0;
int B1EventAction::evMultiplicity = 0;
int B1EventAction::genuineEventCounter = 0;
std::vector<G4String> B1EventAction::volName({"Hello"});
std::vector<G4double> B1EventAction::energy({0.});
std::vector<G4double> B1EventAction::vertexEnergy({0.});
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
  //std::cout << "======== Event no : "<< evNo << "  started =======" << std::endl;
  fEdep = 0.;
  evMultiplicity=0;
  volName.clear();
  energy.clear();
  position.clear();


  B1RunAction::fScatteringAngle = 0.;
  InitializeBranchMap();
  (B1RunAction::brMap["Module2_LE_CH31"]).push_back((int)Tracking::Global::GenRandomDet(19000, 24000));


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::EndOfEventAction(const G4Event*)
{  
 /* double E1,E2,th1,th2,phi1,phi2,erel,c12;   //declaration of energy and theta_phi
  th1=acos(z1/r);
  th2=acos(z2/r);
  phi1=atan(y1/x1);
  phi2=atan(y2/x2);
  c12=cos(th1)*cos(th2)+(sin(th1)*sin(th2))*cos(phi1-phi2);
  erel=(1/2)*(E1+E2-2*sqrt(E1*E2*C12));      // definition of relative energy
*/
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

  G4ThreeVector incoming = position[1]-position[0];
  G4ThreeVector outgoing = position[3]-position[2];

  //Filling Tracks
  B1RunAction::fIncomingTrack.SetP1(Tracking::Vector3D<double>(position[0].x(),position[0].y(),position[0].z()));
  B1RunAction::fIncomingTrack.SetP2(Tracking::Vector3D<double>(position[1].x(),position[1].y(),position[1].z()));
  B1RunAction::fOutgoingTrack.SetP1(Tracking::Vector3D<double>(position[2].x(),position[2].y(),position[2].z()));
  B1RunAction::fOutgoingTrack.SetP2(Tracking::Vector3D<double>(position[3].x(),position[3].y(),position[3].z()));

  TVector3 incomingR(incoming.x(),incoming.y(),incoming.z());
  TVector3 outgoingR(outgoing.x(),outgoing.y(),outgoing.z());
  B1RunAction::fScatteringAngle = outgoingR.Angle(incomingR);
  //std::cout<<"Scattering Angle : " << B1RunAction::fScatteringAngle << std::endl;

#ifdef STORE
  B1RunAction::fTree->Fill();
#endif

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
