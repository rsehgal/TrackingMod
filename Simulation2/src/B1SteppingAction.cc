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
// $Id: B1SteppingAction.cc 74483 2013-10-09 13:37:06Z gcosmo $
//
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "MyDetectorConstruction.h"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
//#include "Tree.h"
#include "B1RunAction.hh"
//#include <experimental/random>
#include <ctime>
#include <cstdlib>
#include "base/Global.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int B1SteppingAction::stepNum = -1;

B1SteppingAction::B1SteppingAction(B1EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
stepNum++;
/*
  if (!fScoringVolume) { 
    const MyDetectorConstruction* detectorConstruction
      = static_cast<const MyDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();   
  }
*/
  // get volume of the current step
  /*
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
      
  // check if we are in scoring volume
  if (volume != fScoringVolume) return;

*/
  // collect energy deposited in this step

  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);  

  G4StepPoint *point1 = step->GetPreStepPoint();
  G4StepPoint *point2 = step->GetPostStepPoint();
  G4TouchableHandle touch1 = point1->GetTouchableHandle();
  G4TouchableHandle touch2 = point2->GetTouchableHandle();

  G4Track* track = step->GetTrack();
  G4ThreeVector hitPre = track->GetPosition();
  G4ThreeVector hitPost = track->GetPosition();
  G4ThreeVector hitBeginOfPost = track->GetPosition();
  if(point1->GetPhysicalVolume()->GetName()!="World")// && point2->GetPhysicalVolume()->GetName()!="World")
  //if(point2->GetPhysicalVolume()->GetName()!="World")
  {

/*
	  if((touch1->GetVolume(2)->GetName()=="E0" &&  point1->GetStepStatus()==fGeomBoundary )||
			  (touch1->GetVolume(2)->GetName()=="E1" &&  point1->GetStepStatus()==fGeomBoundary ) ||
			  (touch1->GetVolume(2)->GetName()=="E2" &&  point1->GetStepStatus()==fGeomBoundary ) ||
			  (touch1->GetVolume(2)->GetName()=="E3" &&  point1->GetStepStatus()==fGeomBoundary ) ||
			  (touch1->GetVolume(2)->GetName()=="E4" &&  point1->GetStepStatus()==fGeomBoundary ))
*/
	//	std::cout<<"Processing Step Num : "<< stepNum  << std::endl;
/*
	  if((touch1->GetVolume(2)->GetName()=="Module2" ||
			  touch1->GetVolume(2)->GetName()=="Module3" ||
			  touch1->GetVolume(2)->GetName()=="Module4" ||
			  touch1->GetVolume(2)->GetName()=="Module5")
			  && point1->GetStepStatus()==fGeomBoundary)
*/
	  if((touch1->GetVolume(2)->GetName()=="Module2" ||
	  			  touch1->GetVolume(2)->GetName()=="Module3" ||
	  			  touch1->GetVolume(2)->GetName()=="Module4" ||
	  			  touch1->GetVolume(2)->GetName()=="Module5")
	  			  && point1->GetStepStatus()==fGeomBoundary)
	  //if(point1->GetStepStatus()==fGeomBoundary)
	  {
		  B1EventAction::evMultiplicity++;

		  /* Something unexpected !!! sometimes getting zero step length,
		   * So Currently processing only those events where steplength
		   * is non-zero.
		   */
		  if(step->GetStepLength()){
			  std::string hittedStripName = (touch1->GetVolume(2)->GetName()+ touch1->GetVolume()->GetName());
			 // std::cout<< "Hitted Strip : "<< hittedStripName << std::endl;
			 (B1RunAction::brMap[hittedStripName]).push_back((int)Tracking::Global::GenRandomDet(19450, 21000));

			  //std::cout<<"Type : " << Tracking::Tree::instance()->GetTTree()->GetBranch(hittedStripName.c_str())->GetClassName() << std::endl;
		  }



			  B1EventAction::volName.push_back(touch1->GetVolume(2)->GetName());
		  B1EventAction::energy.push_back(track->GetKineticEnergy());
		  B1EventAction::vertexEnergy.push_back(track->GetVertexKineticEnergy());
		  B1EventAction::position.push_back(point1->GetPosition());
		//  std::cout<<"GrandMa : "<< touch1->GetVolume(2)->GetName() << " :: StepStatus : " << point1->GetStepStatus() << std::endl;



/*

		  std::cout<<"PreStep : "<< hitPre.x()<<", "<<hitPre.y()<<", "<<hitPre.z()
		   <<" :: PostStep : "<< hitBeginOfPost.x()<<", "<<hitBeginOfPost.y()<<", "<<hitBeginOfPost.z()
		   <<" :: StepLength : " << step->GetStepLength()
		   <<" :: EnergyDeposited in Step: "<< step->GetTotalEnergyDeposit()
		   <<" :: PhysicalName : "<<  point1->GetPhysicalVolume()->GetName()
  	  	   <<" :: Energy-Remaining : "<< point1->GetKineticEnergy()
		   <<" :: GrandParentName Prestep: " << touch1->GetVolume(2)->GetName()  <<  std::endl;
*/



		   //<<" :: GrandParentName Prestep: " << touch1->GetVolume(2)->GetName()
		   //<<" :: TrackID : "<< step->GetTrack()->GetTrackID() <<  std::endl;
		   //<<" :: GrandParentName Poststep: " << touch2->GetVolume(2)->GetName() << std::endl;
	  }
 
  }


  /*if(B1EventAction::evMultiplicity > 1){
	  B1EventAction::genuineEventCounter++;
	  std::cout<<"Event Num : "<< fEventAction->GetEventNum() <<" :: EventMultiplicity : " << B1EventAction::evMultiplicity << std::endl;
  }*/

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

