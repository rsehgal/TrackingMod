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
#include "base/Global.h"
#include "Analyzer_V2.h"
#include <TH1F.h>
#include "HardwareNomenclature.h"
#include "B1RunAction.hh"

int MySD::stepNum = 0;
int MySD::numOfParticlesReached = 0;
unsigned int MySD::evNo=0;
std::vector< std::vector<ScintillatorBar_V2*> > MySD::eventsVec;
std::vector< SingleMuonTrack* > MySD::muonTrackVec;
unsigned int MySD::numOfStoppedParticles = 0;
//std::vector<ScintillatorBar_V2*> MySD::psBarVec;
unsigned long int MySD::muonNum = -1;

bool verbose = false;
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
  if(verbose)
	  std::cout << "===== Printing Vector of PsBars =======" << std::endl;
  for (unsigned int i = 0 ; i < psBarVec.size() ; i++){
    psBarVec[i]->Print();
  }
}

void MySD::InitializeVectorOfPsBars(){
	if(verbose)
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
			psBarVec.push_back(new ScintillatorBar_V2(barIndex));
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
  //muonNum++;
	if(verbose){
		std::cout <<" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
		std::cout <<"Processing Ev No : " << evNo << std::endl;
		std::cout<<"RAMAN Entered Initialize Of SD" << std::endl;
	}
	InitializeVectorOfPsBars();
	reachedSensitiveRegion = false;

	//Tomography::EventBreak::instance()->fEffEvNo++;
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
  G4TouchableHandle touchable;
  if(isPrimary){

  newHit->SetPosition(aStep->GetPostStepPoint()->GetPosition());
  touchable= aStep->GetPreStepPoint()->GetTouchableHandle();
  newHit->SetName(touchable->GetVolume(0)->GetName());
  newHit->SetCopyNum(touchable->GetVolume(0)->GetCopyNo());
  G4String particleName=track->GetDefinition()->GetParticleName() ;
  if(verbose)
	  std::cout << particleName << "  " << std::endl;
  if(verbose)
  std::cout << "Energy deposited in current step in : "
            << touchable->GetVolume(0)->GetName()
            << " : " << aStep->GetTotalEnergyDeposit() << " : Current Energy : " << track->GetKineticEnergy() <<std::endl;
  newHit->SetEnergyDeposited(aStep->GetTotalEnergyDeposit());
  fHitsCollection->insert( newHit );

	  if(track->GetKineticEnergy() == 0){
		numOfStoppedParticles++;
	  }
  }/*else{
	  std::cout << "Energy deposited by secondary particle in current step in : "
	              << touchable->GetVolume(0)->GetName()
	              << " : " << aStep->GetTotalEnergyDeposit() << " : Current Energy : " << track->GetKineticEnergy() <<std::endl;
  }*/
//  std::cout << "New Hit position : " << newHit->GetPosition() << std::endl;

  return true;
}

void MySD::EndOfEvent(G4HCofThisEvent*)
{
	if(verbose){
  std::cout <<"++++"<<std::endl;
  std::cout << "No of Hits : " << fHitsCollection->entries() << std::endl;
  std::cout<<"RAMAN Entered EndOfEvent Of SD" << std::endl;
	}
  //if ( verboseLevel>1 ) {
  if(1){
     G4int nofHits = fHitsCollection->entries();
     if(verbose)
     G4cout << G4endl
            << "-------->Hits Collection: in this event there are " << nofHits
            << " hits  " << G4endl;
     for ( G4int i=0; i<nofHits; i++ ) {
      (*fHitsCollection)[i]->Print();
      psBarVec[(*fHitsCollection)[i]->GetCopyNum()]->barHitted=true;
      psBarVec[(*fHitsCollection)[i]->GetCopyNum()]->qlongMean += (*fHitsCollection)[i]->GetEnergyDeposited();
      G4ThreeVector hitPosition = (*fHitsCollection)[i]->GetPosition();
     // G4cout << "MuonNum : " << muonNum<< " : BarName : " << vecOfBarsNamess[(*fHitsCollection)[i]->GetCopyNum()]
           //  << " : G4Hit Position : " <<  hitPosition << G4endl;
      (psBarVec[(*fHitsCollection)[i]->GetCopyNum()]->hitsVectorInAnEventInABar).push_back(
        new Point3D(hitPosition.x(),hitPosition.y(),hitPosition.z())
      );


      //hitsVectorInAnEventInABar
    }

     //std::cout << "==============================================================" << std::endl;
    /*-------------------------------------*/
    {
      /*int debugCounter=0;
      for(unsigned int i = 0 ; i < psBarVec.size() ; i++){
        if(psBarVec[i]->barHitted){
          debugCounter++;
          std::cout << "Num of hits in " << i << " bar : " << (psBarVec[i]->hitsVectorInAnEventInABar).size() << " : " << __FILE__ << " : " << __LINE__ << std::endl;
        }
      }
      std::cout << "Number of Hitted bars : " << debugCounter << " : " << __FILE__ <<" : " << __LINE__ << std::endl;
      */
    }    

    /*-------------------------------------*/
    /*
     *Loop to maintain the correct muon numbers
     */
    for(unsigned int i = 0 ; i < psBarVec.size() ; i++){
      if(psBarVec[i]->barHitted){
        muonNum++;
        break;
      }
    }

    //std::cout << "Length of HIT POINTs in an event : " << nofHits << std::endl;
    std::vector<ScintillatorBar_V2*> onlyHittedBarVec;
    for(unsigned int i = 0 ; i < psBarVec.size() ; i++){
      if(psBarVec[i]->barHitted){
    	 // std::cout << "BAR : " << i << "  Hitted : Energy Deposited : " << psBarVec[i]->qlongMean << std::endl;

    	// std::cout <<"Going to insert hitted bar with index : " <<i <<  __FILE__ << " : "  << __LINE__ << std::endl;
        ScintillatorBar_V2 *scintBar = new ScintillatorBar_V2(*psBarVec[i]);
        scintBar->qlongMeanCorrected = scintBar->qlongMean*1000.;
        //scintBar->CalculateVariousPhysicalParameters(muonNum);
        //scintBar->Print();

        /*
         * PUTTING THE CUT ON THE ENERGY DEPOSITED
         *
         * tHIS GIVES SIMILAR MULTIPLICITY PICTURES AS WE ARE GETTING FROM DATA
         */
        if(scintBar->qlongMeanCorrected > 10000 && scintBar->qlongMeanCorrected < 35000){
        	onlyHittedBarVec.push_back(scintBar);
          //scintBar->CalculateVariousPhysicalParameters(muonNum);
        	scintBar->CalculateVariousPhysicalParameters(muonNum,B1RunAction::fCalib);
        }
        if(onlyHittedBarVec.size() > 0){
        	reachedSensitiveRegion |= true;
          
        }
      }
    }

    //std::cout <<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
    //verbose = true;
    if(verbose){
    std::cout<< "********** Printing onlyHittedBarVec ***********" << std::endl;
    for(unsigned int i = 0 ; i < onlyHittedBarVec.size() ; i++){
      onlyHittedBarVec[i]->Print();
    }
    //std::cout << "888888 Printing eventsVec2 888888888" << std::endl;
    std::cout << "***********************************************" << std::endl;
    }
    eventsVec.push_back(onlyHittedBarVec);
    SingleMuonTrack *smt = new SingleMuonTrack(onlyHittedBarVec);
    //std::cout << "Size of Muon Track : " << (smt->fSingleMuonTrack).size() << std::endl;
    if(reachedSensitiveRegion)
    	muonTrackVec.push_back(smt);
    onlyHittedBarVec.clear();
    if(!(Tomography::EventBreak::instance()->fEffEvNo % 10000) &&   Tomography::EventBreak::instance()->fEffEvNo != 0 && reachedSensitiveRegion){
    	std::cout << "Processed : " << Tomography::EventBreak::instance()->fEffEvNo  << "  Events" << std::endl;
    }

  }
  //delete fHitsCollection;
  if(reachedSensitiveRegion)
  Tomography::EventBreak::instance()->fEffEvNo++;

 // std::cout << "DEBUG PRINTING>>>>>>>>>>>>>>>>>>>>>>>J::::::::::::::::K:POIK{PO{O{POL{L{" << std::endl;


}

