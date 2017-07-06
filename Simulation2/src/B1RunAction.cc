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
// $Id: B1RunAction.cc 87359 2014-12-01 16:04:27Z gcosmo $
//
/// \file B1RunAction.cc
/// \brief Implementation of the B1RunAction class

#include "B1RunAction.hh"
#include "MyPrimaryGeneratorAction.h"
#include "MyDetectorConstruction.h"
#include "B1Run.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include <G4FastStep.hh>
#include <string.h>
#include "B1EventAction.hh"
//#include "Data.h"
#include <vector>

//#include "Tree.h"

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ofstream* B1RunAction::myfile=NULL;

std::map<std::string,Tracking::Channel> B1RunAction::brMap;

#ifdef STORE
TTree* B1RunAction::fTree=NULL;
#endif

B1RunAction::B1RunAction()
: G4UserRunAction()
{ 
  // add new units for dose
  //
  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;  
  const G4double picogray  = 1.e-12*gray;
   
  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray);        
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::~B1RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* B1RunAction::GenerateRun()
{
  return new B1Run; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::BeginOfRunAction(const G4Run*)
{
  myfile = new ofstream(); 
  myfile->open("energy.txt");
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

#ifdef STORE
  fRootFile = new TFile ("1234.root","RECREATE","My GEANT4 simulation") ;
  fTree = new TTree ("BSC_DATA_TREE","My GEANT4 simulation") ;
#endif


  //Tracking::Channel b;
  //InitializeTree();

  //std::vector<Tracking::Channel> b;
  //b.resize(100);

  //Creating branch for Trigger
  brMap["Module2_LE_CH31"] = *(new Tracking::Channel());
#ifdef STORE
  fTree->Branch("Module2_LE_CH31","Module2_LE_CH31",&brMap["Module2_LE_CH31"]);
#endif
  counter = 0;
  for(int i = 2 ; i < 6 ; i++){

  	for(int brNum = 32 ; brNum < 96 ; brNum++){
  		counter++;

  	  //Tracking::Channel *br = new Tracking::Channel();
  	  std::string bName = "Module"+std::to_string(i)+"_LE_CH"+std::to_string(brNum);
  	  brMap[bName] = *(new Tracking::Channel());

#ifdef STORE
  	  fTree->Branch(bName.c_str(),bName.c_str(),&brMap[bName]);
#endif



  	  //Tracking::Tree::instance()->CreateBranch<decltype(brMap[bName])>(bName.c_str(),brMap[bName]);
  	  //CreateBranch(bName.c_str(),brMap[bName]);

  		//Tracking::Channel b;

  		//std::string bName = "Module"+std::to_string(i)+"_LE_CH"+std::to_string(brNum);
  //		tree->CreateBranch<decltype(b)>(bName.c_str(),b);
  		//CreateBranch(bName.c_str(),b);
  	}

  	}

  for(int i = 0  ; i< 10 ; i++){
	 // CreateBranch(("TestBranch_"+std::to_string(i)).c_str(), b.at(i) );
	 // Tracking::Channel *br = new Tracking::Channel();
	 // CreateBranch(("TestBrerer"+std::to_string(i)).c_str(),*(new Tracking::Channel()));
  }

  //Tracking::Channel *br = new Tracking::Channel();
  //CreateBranch("TestBrerer",*br);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;
  
  const B1Run* b1Run = static_cast<const B1Run*>(run);

  // Compute dose
  //
  G4double edep  = b1Run->GetEdep();
  G4double edep2 = b1Run->GetEdep2();
  G4double rms = edep2 - edep*edep/nofEvents;
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;

  const MyDetectorConstruction* detectorConstruction
   = static_cast<const MyDetectorConstruction*>
     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  //G4double mass = detectorConstruction->GetScoringVolume()->GetMass();
  //G4double dose = edep/mass;
  //G4double rmsDose = rms/mass;

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const MyPrimaryGeneratorAction* generatorAction
   = static_cast<const MyPrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
/*
  if (generatorAction)
  {
    const G4ParticleGun* particleGun1 = generatorAction->GetParticleGun1();
    runCondition += particleGun1->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun1->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy\n");

    const G4ParticleGun* particleGun2 = generatorAction->GetParticleGun2();
        runCondition += particleGun2->GetParticleDefinition()->GetParticleName();
        runCondition += " of ";
        particleEnergy = particleGun2->GetParticleEnergy();
        runCondition += G4BestUnit(particleEnergy,"Energy");
  }
*/
          
  // Print
  //  
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }

//  ofstream myfile;

  std::cout<<"Total Number of Genuine Events : "<< B1EventAction::genuineEventCounter << std::endl;

 // Tracking::Tree::instance()->Fill();
 // Tracking::Tree::instance()->WriteToFile();
 // Tracking::Tree::instance()->Close();

#ifdef STORE
  fTree->Write();
  fRootFile->Close();
#endif

 std::cout << "Total number of branches : " << brMap.size() << " : counter  : "<< counter << std::endl;
  myfile->close();
    delete myfile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
