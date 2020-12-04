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
//#include "Analyzer_V2.h"
//#include "Analyzer.h"
#include <TH1F.h>
#include "HardwareNomenclature.h"
#include "B1RunAction.hh"
#include "ScintillatorBar_V2.h"
#include "G4SystemOfUnits.hh"
#include <iostream>
#include <utility>

int MySD::stepNum = 0;
int MySD::numOfParticlesReached = 0;
unsigned int MySD::evNo=0;
std::vector< std::vector<lite_interface::ScintillatorBar_V2*> > MySD::eventsVec;
std::vector< lite_interface::SingleMuonTrack* > MySD::muonTrackVec;
unsigned int MySD::numOfStoppedParticles = 0;
//std::vector<ScintillatorBar_V2*> MySD::psBarVec;
unsigned long int MySD::muonNum = -1;
std::vector<G4ThreeVector> MySD::exactHitVector;
double MySD::initialEnergy = 0;
double MySD::depositedEnergy = 0;
bool MySD::enteredMatrix = false;
std::vector<std::pair<std::string,G4ThreeVector>> MySD::vecOfPairs;

G4ThreeVector FindExactHitPoint(std::string barName){
	for(unsigned short i = 0 ; i < MySD::vecOfPairs.size() ; i++){
		if(MySD::vecOfPairs[i].first == barName){
			return MySD::vecOfPairs[i].second;
		}
	}
}

bool verbose = false;
// std::vector<ScintillatorBar*> MySD::eventsVec2;

/*
 * One may refer to following presentation, particularly slide 16 to get info about touchables
 * http://geant4.in2p3.fr/2005/Workshop/ShortCourse/session3/M.Asai1.pdf
 */

/*MySD::MySD() {
	// TODO Auto-generated constructor stub

}*/

void MySD::Print(){
		if(0){
		std::cout << "9999999999 Printing eventsVec 999999999" << std::endl;
		for(unsigned int i = 0 ; i < eventsVec.size() ; i++){
			std::cout << "@@@@@@@ Printing event num : " << i <<" @@@@@@@" << std::endl;
			for(unsigned int j = 0 ; j < eventsVec[i].size() ; j++){
				eventsVec[i][j]->Print();
			}
		}
		}
	}

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
			psBarVec.push_back(new lite_interface::ScintillatorBar_V2(barIndex));
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
	//if(verbose){
	exactHitVector.clear();
	initialEnergy = 0.;
	depositedEnergy = 0.;
	enteredMatrix = false;

	if(false){
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

	//std::cout << "EvNo. : " << evNo <<" : Injected Energy : " << initialEnergy << std::endl;

  //if (edep==0.) return false;

  MyHit* newHit = new MyHit();

  G4Track* track = aStep->GetTrack();
  //initialEnergy = track->GetVertexKineticEnergy();
  //initialEnergy = track->GetTotalEnergy();
  bool isPrimary = (track->GetParentID() == 0 );

  G4TouchableHandle touchable;
  G4TouchableHandle touchable1;
  if(isPrimary){

  newHit->SetPosition(aStep->GetPostStepPoint()->GetPosition());
  touchable= aStep->GetPreStepPoint()->GetTouchableHandle();
  touchable1= aStep->GetPostStepPoint()->GetTouchableHandle();
  std::string psBar = std::string(touchable1->GetVolume(0)->GetName());
  std::string psSubBarName = psBar.substr(0,13);

  if(0){
	  std::cout <<" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
	  std::cout <<" @@@@@@ RAMAN Processing Ev No : " << evNo << " @@@@@@@@@@" << std::endl;
	  std::cout <<" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
	  std::cout << "SUBSTR : " << psSubBarName << " : " << touchable->GetVolume(0)->GetName() << std::endl;
  }


  //if(std::string(touchable->GetVolume(0)->GetName()) ==  "World" && psSubBarName=="PhysicalPsBar"){
  if(psSubBarName=="PhysicalPsBar"){
	  if(!enteredMatrix)
		  //std::cout << "================== Entering in Top layer.............. With Energy : " << track->GetKineticEnergy() <<  " : ===============  : Prestep is at " << touchable->GetVolume(0)->GetName() << std::endl;
	  	  initialEnergy = track->GetKineticEnergy();
	  	  enteredMatrix = true;
  }
  newHit->SetName(touchable->GetVolume(0)->GetName());
  newHit->SetCopyNum(touchable->GetVolume(0)->GetCopyNo());
  G4String particleName=track->GetDefinition()->GetParticleName() ;

  if(aStep->GetPreStepPoint()->GetStepStatus()==fGeomBoundary){
	  G4ThreeVector hitPt = aStep->GetPreStepPoint()->GetPosition();
	  exactHitVector.push_back(hitPt);
	  vecOfPairs.push_back(std::pair<std::string,G4ThreeVector>(std::string(touchable->GetVolume(0)->GetName()),hitPt));
//	 / std::cout <<  hitPt << std::endl;

  }

  if(verbose)
  //if(evNo < 20)
	  std::cout << particleName << "  : MeV : " << MeV << " : KeV : " << keV << " : GeV : " << GeV << std::endl;
  //if(verbose)
  if(evNo < 20)
	  std::cout << "Energy deposited in current step in : "
	  	  	  << touchable->GetVolume(0)->GetName()
			  << " : " << aStep->GetTotalEnergyDeposit() << " : Current Energy : " << track->GetKineticEnergy() <<std::endl;
  newHit->SetEnergyDeposited(aStep->GetTotalEnergyDeposit());
  fHitsCollection->insert( newHit );

	  if(track->GetKineticEnergy() == 0){
		numOfStoppedParticles++;
	  }
  }
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
      psBarVec[(*fHitsCollection)[i]->GetCopyNum()]->fBarHitted=true;


      G4ThreeVector firstExactEntryHit =  FindExactHitPoint(std::string((*fHitsCollection)[i]->GetName()));
      /*psBarVec[(*fHitsCollection)[i]->GetCopyNum()]->exactHitX = firstExactEntryHit.getX();
      psBarVec[(*fHitsCollection)[i]->GetCopyNum()]->exactHitY = firstExactEntryHit.getY();
      psBarVec[(*fHitsCollection)[i]->GetCopyNum()]->exactHitZ = firstExactEntryHit.getZ();*/
      psBarVec[(*fHitsCollection)[i]->GetCopyNum()]->fExactHitPosition->SetXYZ(firstExactEntryHit.getX(),firstExactEntryHit.getY(),firstExactEntryHit.getZ());

      psBarVec[(*fHitsCollection)[i]->GetCopyNum()]->fQlongMean += (*fHitsCollection)[i]->GetEnergyDeposited();
      G4ThreeVector hitPosition = (*fHitsCollection)[i]->GetPosition();
      //G4cout << "MuonNum : " << muonNum<< " : BarName : " << vecOfBarsNamess[(*fHitsCollection)[i]->GetCopyNum()]
         //    << " : G4Hit Position : " <<  hitPosition << G4endl;
      (psBarVec[(*fHitsCollection)[i]->GetCopyNum()]->hitsVectorInAnEventInABar).push_back(
        new lite_interface::Point3D(hitPosition.x(),hitPosition.y(),hitPosition.z())
      );

    }

    /*-------------------------------------*/
    /*
     *Loop to maintain the correct muon numbers
     */
    for(unsigned int i = 0 ; i < psBarVec.size() ; i++){
      if(psBarVec[i]->fBarHitted){
        muonNum++;
        break;
      }
    }

    //std::cout << "Length of HIT POINTs in an event : " << nofHits << std::endl;
    std::vector<lite_interface::ScintillatorBar_V2*> onlyHittedBarVec;
    //std::cout << "==================================" << std::endl;
    for(unsigned int i = 0 ; i < psBarVec.size() ; i++){
      if(psBarVec[i]->fBarHitted){
    	 // std::cout << "BAR : " << i << "  Hitted : Energy Deposited : " << psBarVec[i]->GetQLongMean() << std::endl;

    	// std::cout <<"Going to insert hitted bar with index : " <<i <<  __FILE__ << " : "  << __LINE__ << std::endl;
        lite_interface::ScintillatorBar_V2 *scintBar = new lite_interface::ScintillatorBar_V2(*psBarVec[i]);
        /*
         * PUTTING THE CUT ON THE ENERGY DEPOSITED
         *
         * tHIS GIVES SIMILAR MULTIPLICITY PICTURES AS WE ARE GETTING FROM DATA
         */
        //if(scintBar->qlongMeanCorrected > 10000 && scintBar->qlongMeanCorrected < 35000){

        //DO WE REALLY NEED BELOW MENTIONED IF CONDITION, OR IT SHOULD BE HANDLED AT ANALYSIS LEVEL
        //if(scintBar->GetQMeanCorrected() > 10 && scintBar->GetQMeanCorrected() < 35)
        {
        	onlyHittedBarVec.push_back(scintBar);

        	//std::cout << "@@@@@@@@@@ Printing ScintBar : " ; scintBar->Print();
#ifdef USE_CALIBRATION
       	scintBar->CalculateVariousPhysicalParameters(muonNum,B1RunAction::fCalib);
#else
        scintBar->CalculateVariousPhysicalParameters(muonNum);
#endif
          //scintBar->CalculateVariousPhysicalParameters(muonNum,B1RunAction::fCalib);
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
    lite_interface::SingleMuonTrack *smt = new lite_interface::SingleMuonTrack(onlyHittedBarVec);
    depositedEnergy = smt->GetEnergySum();
    //std::cout << "Size of Muon Track : " << (smt->fSingleMuonTrack).size() << std::endl;
    if(reachedSensitiveRegion)
    	muonTrackVec.push_back(smt);
    onlyHittedBarVec.clear();
    /*if(!(Tomography::EventBreak::instance()->fEffEvNo % 10000) &&   Tomography::EventBreak::instance()->fEffEvNo != 0 && reachedSensitiveRegion){

    	std::cout << "Processed : " << Tomography::EventBreak::instance()->fEffEvNo  << "  Events" << std::endl;
    }*/

  }

  std::vector<double> xvec;
  std::vector<double> yvec;
  std::vector<double> zvec;
  for(unsigned int i = 0 ; i < exactHitVector.size(); i++){
	  xvec.push_back(exactHitVector[i].getX());
	  yvec.push_back(exactHitVector[i].getY());
	  zvec.push_back(exactHitVector[i].getZ());

	  /*B1RunAction::xVec.push_back(exactHitVector[i].getX());
	  B1RunAction::yVec.push_back(exactHitVector[i].getY());
	  B1RunAction::zVec.push_back(exactHitVector[i].getZ());*/

  }
  //B1RunAction::fExactHitDataTree->Fill(xvec,yvec,zvec);
  //B1RunAction::fExactHitDataTree->Fill(xvec,yvec,zvec,initialEnergy,depositedEnergy);
  if(evNo < 20.){
	  std::cout << "EvNo. : " << evNo <<" : Injected Energy : " << initialEnergy << std::endl;
  }
  B1RunAction::fExactHitDataTree->Fill(xvec,yvec,zvec,initialEnergy,depositedEnergy,evNo);




  //delete fHitsCollection;
  //if(reachedSensitiveRegion)
  //Tomography::EventBreak::instance()->fEffEvNo++;

 // std::cout << "DEBUG PRINTING>>>>>>>>>>>>>>>>>>>>>>>J::::::::::::::::K:POIK{PO{O{POL{L{" << std::endl;


}

