
#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "MyDetectorConstruction.h"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
//#include "Tree.h"
//#include "B1RunAction.hh"
//#include <experimental/random>
#include <ctime>
#include <cstdlib>
#include "../base/Global.h"
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
  G4StepPoint *point1 = step->GetPreStepPoint();
  G4StepPoint *point2 = step->GetPostStepPoint();
  G4TouchableHandle touch1 = point1->GetTouchableHandle();
  G4TouchableHandle touch2 = point2->GetTouchableHandle();

  G4Track* track = step->GetTrack();
  G4ThreeVector hitPre = track->GetPosition();
  G4ThreeVector hitPost = track->GetPosition();
  G4ThreeVector hitBeginOfPost = track->GetPosition();

  G4String particleName=(track->GetParticleDefinition())->GetParticleName();
  bool isPrimary = (track->GetParentID() == 0 );

  if(isPrimary && touch1->GetVolume()->GetName().substr(0,13) == "PhysicalPsBar"
                && point1->GetStepStatus()==fGeomBoundary)
//if(isPrimary && point1->GetStepStatus()==fGeomBoundary)
 {
	std::cout << "Hitted Detector Name : "<< touch1->GetVolume()->GetName() << " : Particle Name : " << particleName << std::endl;
	G4ThreeVector hitLocation = point1->GetPosition();
	std::cout << "HitLocation : " << hitLocation << std::endl;
	B1EventAction::position.push_back(hitLocation);
	//Generating the smeared hit location. Make sure the smeared location does not lie outside the length of PsBar
	G4ThreeVector smearedHitLocation = G4ThreeVector(hitLocation.x(),Tracking::Global::GetSmeared(hitLocation.y(),5.*cm),hitLocation.z());
	B1EventAction::smearedPosition.push_back(smearedHitLocation);
	std::cout << "SmearedHitLocation : " << smearedHitLocation << std::endl;
  }



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

