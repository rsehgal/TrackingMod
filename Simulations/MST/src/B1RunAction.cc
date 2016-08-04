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
//#include "MyDetectorConstruction.hh"
#include "HodoScope.h"
#include "B1Run.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>
// #ifdef USE_EVE
// #include "VisualizationHelper.h"
// #include "Imaging.h"
// #endif
//#include <TROOT.h>
//#include <TApplication.h>

//using Tomography::VisualizationHelper;
#include "Imaging.h"
using Tracking::ImageReconstruction;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::B1RunAction()
: G4UserRunAction(),verbose(false)
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
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  fs.open("run.txt", std::ios::app);
  ftrack.open("tracks.txt",std::ios::app);
  /*#ifdef USE_EVE
  VisualizationHelper v;
  #endif*/

// TApplication *fApp = new TApplication("Test", NULL, NULL);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run* run)
{
  // std::ofstream fs;
  // fs.open("run.txt", std::ios::app);

//TApplication *fApp = new TApplication("Test", NULL, NULL);
/*
#ifdef USE_EVE
  VisualizationHelper v;
#endif
*/  
  ImageReconstruction im;

  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;
  
  const B1Run* b1Run = static_cast<const B1Run*>(run);

  // Compute dose
  //
  G4double edep  = b1Run->GetEdep();
  G4double edep2 = b1Run->GetEdep2();
  G4double rms = edep2 - edep*edep/nofEvents;
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;

  const HodoScope* detectorConstruction
   = static_cast<const HodoScope*>
     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4double mass = detectorConstruction->GetScoringVolume()->GetMass();
  G4double dose = edep/mass;
  G4double rmsDose = rms/mass;

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const MyPrimaryGeneratorAction* generatorAction
   = static_cast<const MyPrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  G4double particleEnergy ;
  const G4ParticleGun* particleGun ;
  if (generatorAction)
  {
    particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }
        
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
  
  G4cout
     << G4endl
     << " The run consists of " << nofEvents << " "<< runCondition
     << G4endl
     << " Dose in scoring volume : " 
     << G4BestUnit(dose,"Dose") << " +- " << G4BestUnit(rmsDose,"Dose")
     << G4endl
     << "------------------------------------------------------------"
     << G4endl
     << G4endl;

  G4cout << "Num of Tracks : " 
         << b1Run->NumOfTracks()
         << G4endl;

  if(verbose)
  G4cout << "------ Printing Tracking info at the end of Run ------- " << G4endl;
  fs << particleEnergy << " " << detectorConstruction->GetTargetMaterial() << " ";
  for(int i = 0 ;i < b1Run->NumOfTracks() ; i++){
    if(verbose){
    G4cout << "----- Printing Event No : " << i+1 << "  --------" << G4endl;
    for(int j = 0 ; j < b1Run->GetPhysicalTrackVector()[i].size() ; j++){
      b1Run->GetPhysicalTrackVector()[i][j].Print();
    }
    std::cout<<"scattering Angle : " << b1Run->GetScatteringAngleVector()[i] << std::endl;
  }

    //Logic to store the scattering angle of every event into "run.txt"
    fs << b1Run->GetScatteringAngleVector()[i] << " " ;
    
    //std::cout<<"GetIncomingTrackVector Size : " << b1Run->GetIncomingTrackVector().size() << std::endl;

if(1){
    //fs << "Hello ";
    Tracking::Vector3D<double> p1(0.,0.,0.), q1(0.,0.,0.);
    Tracking::Vector3D<double>  pocaPt = im.POCA(b1Run->GetIncomingTrackVector()[i].GetP1(),
                         b1Run->GetIncomingTrackVector()[i].GetDirCosine(),
                         b1Run->GetOutgoingTrackVector()[i].GetP1(),
                         b1Run->GetOutgoingTrackVector()[i].GetDirCosine(),p1,q1);

    //pocaPt.Print();
    ftrack<< pocaPt.x() <<" "<<pocaPt.y() << " "<< pocaPt.z() << std::endl;
  }

    //v.Register(pocaPt);
   

  }

  //v.Show();
  //gEve->DoRedraw3D();
//  fApp->Run();

  fs << std::endl;

  fs.close();
  ftrack.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
