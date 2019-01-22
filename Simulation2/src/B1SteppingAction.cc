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

#include "G4SystemOfUnits.hh"

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
  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);  

  G4StepPoint *point1 = step->GetPreStepPoint();
  G4StepPoint *point2 = step->GetPostStepPoint();
  G4TouchableHandle touch1 = point1->GetTouchableHandle();
  G4TouchableHandle touch2 = point2->GetTouchableHandle();

  G4Track* track = step->GetTrack();
  if(stepNum==0){
	B1EventAction::eventEnergy = track->GetKineticEnergy();
	std::cout << "KE : " << track->GetKineticEnergy() << std::endl;
	//B1EventAction::momentum = std::sqrt(2*105.6583745*track->GetKineticEnergy())/1000.;//track->GetMomentum();
	B1EventAction::momentum = std::sqrt(B1EventAction::eventEnergy*B1EventAction::eventEnergy + 2*105.6583745*B1EventAction::eventEnergy);
	G4ThreeVector momen = track->GetMomentum();
	std::cout << "Calculated Momentum : " <<  B1EventAction::momentum << " :: G4ThreeVector Momentum : " << momen.mag() << std::endl;
	B1EventAction::momentum = momen.mag();

	B1EventAction::meanMomentum += B1EventAction::momentum;
}


  bool isPrimary = (track->GetParentID() == 0 );


  {

  	 	//For Top Plane
	  	for(int i = 0 ; i < 8 ; i++){
	  		std::string scintName = "_LE_CH"+std::to_string(i);
	  		//std::cout << "ScintName : "<< scintName << std::endl;
	  		/*if(std::string(touch1->GetVolume()->GetName()) == scintName){
	  			std::cout << "TopPlaneHitted ..." << std::endl;
	  		}*/
	  		if(isPrimary && std::string(touch1->GetVolume()->GetName()) == scintName
	  			&&  point1->GetStepStatus()==fGeomBoundary){
	  			B1EventAction::topPlaneHit = true;
	  			break;
	  		}
	  	}

	  	//For Bottom plane 
	  	for(int i = 8 ; i < 16 ; i++){
	  		std::string scintName = "_LE_CH"+std::to_string(i);
	  		//std::cout << "ScintName : "<< scintName << std::endl;
	  		//std::cout << "ScintName-2 : " << touch1->GetVolume()->GetName() << std::endl;
 	  		if(isPrimary && std::string(touch1->GetVolume()->GetName()) == scintName
	  			&&  point1->GetStepStatus()==fGeomBoundary){
	  			B1EventAction::bottomPlaneHit = true;
	  			break;
	  		}
	  	}

	  // if(isPrimary && touch1->GetVolume()->GetName().substr(0,4) == "_LE_" 
	  // && point1->GetStepStatus()==fGeomBoundary)
	  // 	B1EventAction::bottomPlaneHit = true;

	  	if(isPrimary && touch1->GetVolume()->GetName().substr(0,6) == "Target"
	  			&& point1->GetStepStatus()==fGeomBoundary){
	  		B1EventAction::scattererHitted = 1.;
	  	}

	  if(isPrimary && touch1->GetVolume()->GetName().substr(0,4) == "_LE_" 
		&& point1->GetStepStatus()==fGeomBoundary)
	  {
		  B1EventAction::evMultiplicity++;

		  /* Something unexpected !!! sometimes getting zero step length,
		   * So Currently processing only those events where steplength
		   * is non-zero.
		   */

		  /*if(step->GetStepLength()==0){
			std::cout << "========= Ayush : Step with Zero Step Length ========" << std::endl;
		  }*/


		  if(step->GetStepLength()!=0.)
		//else 
		{	 std::string hittedStripName = ""; 
			 if(touch1->GetVolume(1)->GetName().substr(0,6)=="Module")
			 	hittedStripName = touch1->GetVolume(1)->GetName();
			 if(touch1->GetVolume(2)->GetName().substr(0,6)=="Module")
			 	hittedStripName = touch1->GetVolume(2)->GetName();
			 //std::string hittedStripName = (touch1->GetVolume(2)->GetName()+ touch1->GetVolume()->GetName());
			 hittedStripName += touch1->GetVolume()->GetName();
			 std::cout<< "Hitted Strip : "<< hittedStripName << "  : " ; //std::endl;
			 G4ThreeVector hitLocation = point1->GetPosition();
			 std::cout<< "HitLocation : "<< hitLocation.x() << "  " << hitLocation.y() << "  :  " << hitLocation.z() << std::endl;
			 hitLocation.setZ(0.);

			 /* False represent that muon does not pass through button
			 ** and once USE_SPACERS are enable and muon pass through
			 ** button location, then this will set to true and the muon
			 ** should not get registered
			 */
			 bool button = false;

			 //Filling Vector of Spacers location

#undef USE_SPACERS
#ifdef USE_SPACERS
			 std::vector<G4ThreeVector> buttonLocationVector;
			 	 	 	 buttonLocationVector.push_back(G4ThreeVector(20*cm,-40*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(20*cm,-30*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(20*cm,-20*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(20*cm,-10*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(20*cm,0*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(20*cm,10*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(20*cm,20*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(20*cm,30*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(20*cm,40*cm,0.));

			 	 	 	 buttonLocationVector.push_back(G4ThreeVector(-20*cm,-40*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(-20*cm,-30*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(-20*cm,-20*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(-20*cm,-10*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(-20*cm,0*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(-20*cm,10*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-20*cm,20*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-20*cm,30*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-20*cm,40*cm,0.));


			  			 buttonLocationVector.push_back(G4ThreeVector(40*cm,-40*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(40*cm,-30*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(40*cm,-20*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(40*cm,-10*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(40*cm,0*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(40*cm,10*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(40*cm,20*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(40*cm,30*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(40*cm,40*cm,0.));

			  			 buttonLocationVector.push_back(G4ThreeVector(-40*cm,-40*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-40*cm,-30*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-40*cm,-20*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-40*cm,-10*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-40*cm,0*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-40*cm,10*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-40*cm,20*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-40*cm,30*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-40*cm,40*cm,0.));


			 	 	 	 buttonLocationVector.push_back(G4ThreeVector(-10*cm,-40*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(-10*cm,-30*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(-10*cm,-20*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(-10*cm,-10*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(-10*cm,0*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(-10*cm,10*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-10*cm,20*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-10*cm,30*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-10*cm,40*cm,0.));

			 	 	 	 buttonLocationVector.push_back(G4ThreeVector(10*cm,-40*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(10*cm,-30*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(10*cm,-20*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(10*cm,-10*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(10*cm,0*cm,0.));
			 			 buttonLocationVector.push_back(G4ThreeVector(10*cm,10*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(10*cm,20*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(10*cm,30*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(10*cm,40*cm,0.));


			  			 buttonLocationVector.push_back(G4ThreeVector(30*cm,-40*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(30*cm,-30*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(30*cm,-20*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(30*cm,-10*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(30*cm,0*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(30*cm,10*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(30*cm,20*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(30*cm,30*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(30*cm,40*cm,0.));

			  			 buttonLocationVector.push_back(G4ThreeVector(-30*cm,-40*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-30*cm,-30*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-30*cm,-20*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-30*cm,-10*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-30*cm,0*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-30*cm,10*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-30*cm,20*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-30*cm,30*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(-30*cm,40*cm,0.));


			  			 buttonLocationVector.push_back(G4ThreeVector(0*cm,-40*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(0*cm,-30*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(0*cm,-20*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(0*cm,-10*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(0*cm,0*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(0*cm,10*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(0*cm,20*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(0*cm,30*cm,0.));
			  			 buttonLocationVector.push_back(G4ThreeVector(0*cm,40*cm,0.));


			 for(int i = 0 ; i < buttonLocationVector.size() ; i++){
				 button |= (buttonLocationVector[i] - hitLocation).mag() < 5*mm;
				 if(button)
					 break;
			 }
#endif

			 //ALGO : If muon passes through the button then DO NOT register it
			 if(button){
				//(B1RunAction::brMap[hittedStripName]).push_back((int)Tracking::Global::GenRandomDet(19450, 21000));
				 //std::cout << "Going through the spacer............. Should not get recored !!!! : Branch Name : " << hittedStripName << std::endl;
			 }else {
			 
			 //(B1RunAction::brMap[hittedStripName]).push_back((int)Tracking::Global::GenRandomDet(19450, 21000));
			 /* For the time being moving the above statement in end of event action, where we store it only once
			 ** we get the trigger, otherwise we will not store it. 
			 ** 
			 ** But now we need to store the hitted strip name in hittedStripNameVector of B1EventAction, so that
			 ** it can be accessed there.
			 */
			 B1EventAction::hittedStripNameVector.push_back(hittedStripName);


			 //std::cout<<"Hitted Strip  : " << hittedStripName << std::endl;
			 }
			 B1EventAction::position.push_back(point1->GetPosition());
			  //std::cout<<"Type : " << Tracking::Tree::instance()->GetTTree()->GetBranch(hittedStripName.c_str())->GetClassName() << std::endl;
			// std::cout<<"Entering .... " << touch1->GetVolume(1)->GetName() << std::endl;
		  }



		  B1EventAction::volName.push_back(touch1->GetVolume(2)->GetName());
		  B1EventAction::energy.push_back(track->GetKineticEnergy());
		  B1EventAction::vertexEnergy.push_back(track->GetVertexKineticEnergy());
  }
 
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

