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
#include "CommonFunc.h"

//Trying to use Evolved Voxelator
#include "Voxelator_Evolution.h"
//#include "Data.h"
#include <vector>

//Trying to use RunHelper
#include "RunHelper.h"
#include "EventHelper.h"
//#include "Tree.h"

//Trying to use DetectorMapping
#include "DetectorMapping.h"

//Trying to File Mechanism
#include "Files.h"

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ofstream* B1RunAction::myfile=NULL;

std::map<std::string,Tracking::Channel> B1RunAction::brMap;
double B1RunAction::fScatteringAngle = 0;
Tomography::Track B1RunAction::fIncomingTrack(Tracking::Vector3D<double>(0.,0.,0.),Tracking::Vector3D<double>(0.,0.,0.));
Tomography::Track B1RunAction::fOutgoingTrack(Tracking::Vector3D<double>(0.,0.,0.),Tracking::Vector3D<double>(0.,0.,0.));

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

 // Tomography::evolution::Voxelator::instance()->Create(50*cm,50*cm,45*cm,20*cm,20*cm,18*cm);
  Tomography::evolution::Voxelator::Create(50*cm,50*cm,45*cm,5*cm,5*cm,4.5*cm);

  //Tracking::Channel b;
  //InitializeTree();

  //std::vector<Tracking::Channel> b;
  //b.resize(100);

  //Creating branch for Trigger
  brMap["Module2_LE_CH31"] = *(new Tracking::Channel());
#ifdef STORE
  fTree->Branch("IncomingTrack", &fIncomingTrack);
  fTree->Branch("OutgoingTrack", &fOutgoingTrack);
  fTree->Branch("ScatteringAngle",&fScatteringAngle);
  fTree->Branch("Module2_LE_CH31","Module2_LE_CH31",&brMap["Module2_LE_CH31"]);
  
  //Creating branches for Scintillator planes
  for(int brNum = 0 ; brNum < 16 ; brNum++){
    std::string bName = "Module2_LE_CH"+std::to_string(brNum);
      brMap[bName] = *(new Tracking::Channel());
      fTree->Branch(bName.c_str(),bName.c_str(),&brMap[bName]);
  }

#endif
  counter = 0;

  //Detector Mapping should be created here
  Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::instance();
  //std::vector<std::string> detectorNamesVector = detectorMap->GetDetectorNamesVector();
  //std::vector<int> startChannelVector = detectorMap->GetStartingChannelVector();
  //std::vector<int> moduleVector = detectorMap->GetModuleVector();
  std::vector<Tomography::Mapping::Detector*> detectorVector = detectorMap->GetDetectorVector();
  //std::vector<double> zcoordinateVector = detectorMap->GetZCoordinateVector();

  //Reading numberOfStripsInEachPlane from DetectorMapping
  int numberOfStripsInEachPlane = detectorMap->GetNumberOfStripsInEachPlane();
  int startStripNum = detectorMap->GetStartStripNumOfEachDetector();
  //for(int i = 0 ; i < moduleVector.size()-1; i++){
  for(int i = 0 ; i < detectorVector.size()-1; i++){

    //for(int brNum = 32 ; brNum < 96 ; brNum++){
	  for(int brNum = startStripNum ; brNum < (startStripNum + 2*numberOfStripsInEachPlane) ; brNum++){
      counter++;

      //Tracking::Channel *br = new Tracking::Channel();
      //std::string bName = "Module"+std::to_string(moduleVector[i])+"_LE_CH"+std::to_string(brNum);
      std::string bName = "Module"+std::to_string(detectorVector[i]->sModule)+"_LE_CH"+std::to_string(brNum);
      brMap[bName] = *(new Tracking::Channel());

#ifdef STORE
      fTree->Branch(bName.c_str(),bName.c_str(),&brMap[bName]);
#endif


Tomography::Files::instance()->Open("Hits.txt",Tomography::operation::write);
Tomography::Files::instance()->Open("StatsFromGenerator.txt",Tomography::operation::write);
Tomography::Files::instance()->Open("StatsFromEventAction.txt",Tomography::operation::write);
Tomography::Files::instance()->Open("PocaFromExactHit.txt",Tomography::operation::write);
Tomography::Files::instance()->Open("ActualAndFittedHits.txt",Tomography::operation::write);
Tomography::Files::instance()->Open("ActualHitAndPixelCenter.txt",Tomography::operation::write);
Tomography::Files::instance()->Open("PocaFromFittedHit.txt",Tomography::operation::write);
Tomography::Files::instance()->Open("ActualAndSampledFittedHits.txt",Tomography::operation::write);
Tomography::Files::instance()->Open("PocaFromFittedSampledHit.txt",Tomography::operation::write);

//Writing Track info to file for offline analysis
Tomography::Files::instance()->Open("TrackExact.txt",Tomography::operation::write);
Tomography::Files::instance()->Open("TrackFitted.txt",Tomography::operation::write);
Tomography::Files::instance()->Open("TrackSampledFitted.txt",Tomography::operation::write);
    }
  }

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
  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const MyPrimaryGeneratorAction* generatorAction
   = static_cast<const MyPrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
          
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

  //Writing run.txt file
  ofstream fs("run.txt");
  fs << 3000 << " " << "G4_Fe" << " " << 100 << " ";
  for(int i = 0 ; i < b1Run->GetScatteringAngleVector().size() ; i++){
	  fs << b1Run->GetScatteringAngleVector()[i] << " " ;
  }
  fs.close();


#ifdef STORE
  fTree->Write();
  fRootFile->Close();
#endif

 std::cout << "Total number of branches : " << brMap.size() << " : counter  : "<< counter << std::endl;
  myfile->close();

  std::cout << "ScatteringAngleVector Size : " << b1Run->GetScatteringAngleVector().size() << std::endl;
    delete myfile;

  // Tomography::Files::instance()->Close("Hits.txt");
  // Tomography::Files::instance()->Close("StatsFromGenerator.txt");
  //Below line will close all the registered file in one shot.
  Tomography::Files::instance()->Close();

  std::cerr << "Total Num Of Events : " << nofEvents << std::endl;
  std::cerr << "Effective Num Of Events : " << B1EventAction::GetEffectiveEventNumbers() << std::endl;
  std::cerr << "Mean Momentum : " << ((B1EventAction::meanMomentum / 1000. ) / B1EventAction::GetEffectiveEventNumbers()) << std::endl;

  /* Creating RunHelper, because its constructor will automatically  
  ** call the functions in desired sequence to do filteration stuff
  */


 /*
  Tomography::RunHelper *runHelper = new Tomography::RunHelper("Exact");
  //Now trying to calculate Radiation for the whole run
  std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
  std::cout<<"\033[1;31m                              SD : " << CommonFunc::Functions::instance()->StandardDeviation(runHelper->GetScatteringAngleVector()) << "  radians for 2 sigma  \033[0m\n" << std::endl;
  //std::cout<<"                              SD : " << CommonFunc::Functions::instance()->StandardDeviation(b1Run->GetScatteringAngleVector()) << std::endl;
  std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
  std::cout<<"\033[1;31m                              RL : " << CommonFunc::Functions::instance()->RadiationLength(runHelper->GetScatteringAngleVector(),10) << "  cms  \033[0m\n" <<  std::endl;
  //std::cout<<"                              RL : " << CommonFunc::Functions::instance()->RadiationLength(b1Run->GetScatteringAngleVector(),10) << std::endl;
  std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
  
  std::cout << std::endl << "================================================================" << std::endl;
  std::cout << "========== No of Missed Trigger : " << B1EventAction::noTrigger << "  =========" << std::endl;
*/


/*
  std::cout << "======================================================================================" << std::endl;
  std::cout << "================= @@@@@@@@@@@@@@  Trying NEW Stuff  @@@@@@@@@@@@@@@===================" << std::endl;
  std::cout << "======================================================================================" << std::endl;
  {
  	Tomography::EventHelper u("TrackExact.txt","PocaPtEventHelperTrackExact.txt");
  	Tomography::RunHelper r("Exact");
  }

  {
  	Tomography::EventHelper u("TrackFitted.txt","PocaPtEventHelperTrackFitted.txt");
  	Tomography::RunHelper r("Fitted");
  }

  {
  	Tomography::EventHelper u("TrackSampledFitted.txt","PocaPtEventHelperTrackSampledFitted.txt");
  	Tomography::RunHelper r("SampledFitted");
  }

*/



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
