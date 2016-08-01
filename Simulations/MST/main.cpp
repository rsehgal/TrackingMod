#include "G4RunManager.hh"
#include "G4VModularPhysicsList.hh"
#include "G4VUserPhysicsList.hh"
#include "MyPrimaryGeneratorAction.h"
#include "MyDetectorConstruction.h"
#include "QBBC.hh"
#include "FTFP_BERT.hh"
#include "MyPhysics.h"
#include "B1ActionInitialization.hh"

//#include "PhysicsList.h"
#include "HodoScope.h"
//#include "QGSP_BIC_EMY.hh"
/*
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
*/

#include "GeantVisualizer.h"


#include "Randomize.hh"


int main(int argc, char *argv[]){

  

  G4RunManager *runManager = new G4RunManager;
  //G4VUserPhysicsList* phyList = new MyPhysics;

  //Detector Construction
  //runManager->SetUserInitialization(new MyDetectorConstruction());
  runManager->SetUserInitialization(new HodoScope());

  //Physics List
  //runManager->SetUserInitialization(phyList);
  //runManager->SetUserInitialization(new PhysicsList);
  runManager->SetUserInitialization(new MyPhysics);

  //Primary Generator Action
  //runManager->SetUserAction(new MyPrimaryGeneratorAction());

  // User action initialization
  runManager->SetUserInitialization(new B1ActionInitialization());

  //Initiaizer G4 Kernel
  //runManager->Initialize();

  //Cause the run manager to generate a single event using the
  //primary generator action registered above.
  //runManager->BeamOn(1);

  //read a macro file of commands
  #if (1)
  G4UImanager* UI = G4UImanager::GetUIpointer();
  G4String command = "/control/execute ";
  G4String fileName = argv[1];
  UI->ApplyCommand(command+fileName);
  #else
   GeantVisualizer *visualizer = new GeantVisualizer(argc, argv);
   //visualizer->Start(argv[1]);
    if(argc==1)
      visualizer->Start();
    else
      visualizer->Start(argv[1]);
  #endif

  delete runManager;

  return 0;

}
