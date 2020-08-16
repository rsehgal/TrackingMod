/*
 * MySD.cpp
 *
 *  Created on: Dec 8, 2017
 *      Author: rsehgal
 */

#include "MySD.h"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SteppingManager.hh"
#include "G4UnitsTable.hh"
#include "G4VProcess.hh"
#include "PsBar.h"

int MySD::stepNum = 0;
int MySD::numOfParticlesReached = 0;
unsigned int MySD::evNo=0;
std::vector< std::vector<ScintillatorBar*> > MySD::eventsVec;


// std::vector<ScintillatorBar*> MySD::eventsVec2;

/*
 * One may refer to following presentation, particularly slide 16 to get info about touchables
 * http://geant4.in2p3.fr/2005/Workshop/ShortCourse/session3/M.Asai1.pdf
 */

/*MySD::MySD() {
	// TODO Auto-generated constructor stub

}*/

MySD::~MySD() {
	// TODO Auto-generated destructor stub
}

void MySD::PrintVectorOfPsBars(){
  std::cout << "===== Printing Vector of PsBars =======" << std::endl;
  for (unsigned int i = 0 ; i < psBarVec.size() ; i++){
    psBarVec[i]->Print();
  }
}

void MySD::InitializeVectorOfPsBars(){

	std::cout << "Initializing Vector of Scintillator Bars for the current event........." << std::endl;
    if(psBarVec.size()){
  	for(unsigned int i = 0 ; i < psBarVec.size() ; i++){
  		delete psBarVec[i];
  	}
  	psBarVec.clear();
  }
	for (unsigned int layerNum = 0 ; layerNum < numOfLayers ; layerNum++){
		for(unsigned int index = 0 ;  index < numOfBarsInEachLayer ; index++){
			unsigned int barIndex = numOfBarsInEachLayer*layerNum+index;
			psBarVec.push_back(new ScintillatorBar(barIndex));
		}
	}
}

MySD::MySD(const G4String& name, const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name),
   fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

void MySD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
	std::cout <<" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
	std::cout <<"Processing Ev No : " << evNo << std::endl;
	InitializeVectorOfPsBars();
	std::cout<<"RAMAN Entered Initialize Of SD" << std::endl;
  numOfParticlesReached++;
  fHitsCollection
    = new MyHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce

  G4int hcID
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection );

  evNo++;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MySD::ProcessHits(G4Step* aStep,
                                     G4TouchableHistory*)
{
//	std::cout<<"SEHGAL Entered ProcessHits Of SD" << std::endl;
  // energy deposit


  //if (edep==0.) return false;

  MyHit* newHit = new MyHit();

  G4Track* track = aStep->GetTrack();
  bool isPrimary = (track->GetParentID() == 0 );
/*
  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetChamberNb(aStep->GetPreStepPoint()->GetTouchableHandle()
                                               ->GetCopyNumber());
  newHit->SetEdep(edep);
  newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());
*/
  if(isPrimary){
  newHit->SetPosition(aStep->GetPostStepPoint()->GetPosition());
  G4TouchableHandle touchable = aStep->GetPreStepPoint()->GetTouchableHandle();
  newHit->SetName(touchable->GetVolume(0)->GetName());
  newHit->SetCopyNum(touchable->GetVolume(0)->GetCopyNo());
  G4String particleName=track->GetDefinition()->GetParticleName() ;
  std::cout << particleName << "  " << std::endl;
  std::cout << "Energy deposited in current step in : "
            << touchable->GetVolume(0)->GetName()
            << " : " << aStep->GetTotalEnergyDeposit() << std::endl;
  newHit->SetEnergyDeposited(aStep->GetTotalEnergyDeposit());
  fHitsCollection->insert( newHit );
  }
//  std::cout << "New Hit position : " << newHit->GetPosition() << std::endl;

  return true;
}

void MySD::EndOfEvent(G4HCofThisEvent*)
{
  std::cout <<"++++"<<std::endl;
  std::cout << "No of Hits : " << fHitsCollection->entries() << std::endl;
  std::cout<<"RAMAN Entered EndOfEvent Of SD" << std::endl;
  //if ( verboseLevel>1 ) {
  if(1){
     G4int nofHits = fHitsCollection->entries();
     G4cout << G4endl
            << "-------->Hits Collection: in this event there are " << nofHits
            << " hits  " << G4endl;
     for ( G4int i=0; i<nofHits; i++ ) {
      (*fHitsCollection)[i]->Print();
      psBarVec[(*fHitsCollection)[i]->GetCopyNum()]->barHitted=true;
      psBarVec[(*fHitsCollection)[i]->GetCopyNum()]->qlongMean += (*fHitsCollection)[i]->GetEnergyDeposited();
      
    }

    std::vector<ScintillatorBar*> onlyHittedBarVec;
    for(unsigned int i = 0 ; i < psBarVec.size() ; i++){
      if(psBarVec[i]->barHitted){
        //onlyHittedBarVec.push_back(psBarVec[i]);
        ScintillatorBar *scintBar = new ScintillatorBar(*psBarVec[i]);
        //eventsVec2.push_back(psBarVec[i]);
        //eventsVec2.push_back(scintBar);
        onlyHittedBarVec.push_back(scintBar);
        //delete scintBar;
      }
    }

    std::cout<< "********** Printing onlyHittedBarVec ***********" << std::endl;
    for(unsigned int i = 0 ; i < onlyHittedBarVec.size() ; i++){
      onlyHittedBarVec[i]->Print();
    }
    //std::cout << "888888 Printing eventsVec2 888888888" << std::endl;
    std::cout << "***********************************************" << std::endl;
    eventsVec.push_back(onlyHittedBarVec);
    /*if(onlyHittedBarVec.size()){
      for(unsigned int i = 0 ; i < onlyHittedBarVec.size() ; i++){
        delete onlyHittedBarVec[i];
      }
    }*/
    onlyHittedBarVec.clear();

    //PrintVectorOfPsBars();
  }
  //delete fHitsCollection;
}

