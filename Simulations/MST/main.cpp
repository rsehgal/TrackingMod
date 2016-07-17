#include "G4RunManager.hh"
#include "G4VModularPhysicsList.hh"
#include "G4VUserPhysicsList.hh"
#include "MyPrimaryGeneratorAction.h"
#include "MyDetectorConstruction.h"
#include "QBBC.hh"
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
  G4VModularPhysicsList *phyList = new QBBC;
  //G4VUserPhysicsList* physics = new FTFP_BERT();

  //Detector Construction
  runManager->SetUserInitialization(new MyDetectorConstruction());

  //Physics List
  runManager->SetUserInitialization(phyList);

  //Primary Generator Action
  runManager->SetUserAction(new MyPrimaryGeneratorAction());

  //Initiaizer G4 Kernel
  runManager->Initialize();

  //Cause the run manager to generate a single event using the
  //primary generator action registered above.
  runManager->BeamOn(1);

   GeantVisualizer *visualiser = new GeantVisualizer(argc, argv);
   visualiser->Start();


/*
G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }
  
 G4VisManager* visManager = new G4VisExecutive;
 visManager->Initialize();
 G4UImanager* UImanager = G4UImanager::GetUIpointer();
 // Process macro or start UI session
  //
  if ( ! ui ) { 
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else { 
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }
  //freeing the memory allocated to runManager
  delete visManager;
  
  delete runManager;
*/
  return 0;

}
