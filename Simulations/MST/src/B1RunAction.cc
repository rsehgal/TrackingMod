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
#include "HodoScope2.h"
#include "B1Run.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "TFile.h"
#include "TROOT.h"

#include "Voxelator.h"


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
using Tomography::Voxelator;
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
  fScatteringHist = new TH1F("fScatteringHist","Scattering Histogram",100,0.,100.);
  
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

  const HodoScope2* detectorConstruction
   = static_cast<const HodoScope2*>
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
  fs << particleEnergy << " " << detectorConstruction->GetTargetMaterial() << " " << detectorConstruction->GetTargetThickness() << " ";
  for(int i = 0 ;i < b1Run->NumOfTracks() ; i++){
    if(verbose){
    G4cout << "----- Printing Event No : " << i+1 << "  --------" << G4endl;

    for(int j = 0 ; j < b1Run->GetPhysicalTrackVector()[i].size() ; j++){
      b1Run->GetPhysicalTrackVector()[i][j].Print();
    }
    std::cout<<"scattering Angle : " << b1Run->GetScatteringAngleVector()[i] << std::endl;
  }

  }

  //Logic to store the scattering angle of every event into "run.txt"
  std::cout<<"@@@@@@@@@@@ Scattering Angle Vector size : "<< b1Run->GetScatteringAngleVector().size() << " @@@@@@@@@@@@@@" << std::endl;
   for(int i=0 ; i < b1Run->GetScatteringAngleVector().size() ; i++){
   	fs << b1Run->GetScatteringAngleVector()[i] << " " ;
   }


for (auto &PocaPt : b1Run->GetPocaPtVector()){
	ftrack<< PocaPt.x() <<" "<<PocaPt.y() << " "<< PocaPt.z() << " " << PocaPt.GetColor() << std::endl;
	fScatteringHist->Fill(PocaPt.GetColor());
	//fs << PocaPt.GetColor() << " " ;
    }

//Now trying to voxelate PocaVector

Voxelator vox;
vox.Insert(b1Run->GetPocaPtVector()); //Voxelized Poca Ready
std::ofstream voxTrack;
voxTrack.open("VoxelizedTracks.txt");
Vector3D<int> voxelatorDim = vox.GetVoxelatorDim();
std::cout<< " -+-+-+-+--+-+-+--+--+-+--+-------+---++- " << std::endl;
voxelatorDim.Print();
Vector3D<int> voxelizedVolumeDim = vox.GetVoxelizedVolumeDim();
Vector3D<int> voxelDim = vox.GetEachVoxelDim();
for(int x = 0 ; x < voxelatorDim.x()-1 ; x++){
    for(int y = 0 ; y < voxelatorDim.y()-1 ; y++){
      for(int z = 0 ; z < voxelatorDim.z()-1 ; z++){
	if(vox.GetVoxelizedCount()->GetBinContent(x,y,z))
	//voxTrack << x << " " << y << " " << z << " " <<  vox.GetVoxelizedHist()->GetBinContent(x,y,z) << std::endl;
	voxTrack << (-voxelizedVolumeDim.x()+x*voxelDim.x()) << " " << (-voxelizedVolumeDim.y()+y*voxelDim.y())  << " " << (-voxelizedVolumeDim.z()+z*voxelDim.z())  << " " <<  vox.GetVoxelizedHist()->GetBinContent(x,y,z) << std::endl;
      }
    }
  }

voxTrack.close();



/*
TFile *f = new TFile();//"histos.root","new");
f->SetName("histos.root");
f->SetOption("recreate");
//fScatteringHist->Write();
//vox.GetVoxelizedHist()->Write();
f->Write();
*/

//f.close();

//TFile f("histos.root","recreate");
//fScatteringHist->Write();
//f.Write();
delete fScatteringHist;
//auto xmax = max_element(std::begin(cloud), std::end(cloud)); // c++11



  //v.Show();
  //gEve->DoRedraw3D();
//  fApp->Run();

  fs << std::endl;

  fs.close();
  ftrack.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
