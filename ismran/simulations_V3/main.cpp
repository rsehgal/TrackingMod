#include "B1ActionInitialization.hh"
#include "FTFP_BERT.hh"
#include "G4RunManager.hh"
#include "G4VModularPhysicsList.hh"
#include "G4VUserPhysicsList.hh"
#include "MyDetectorConstruction.h"
#include "MyPrimaryGeneratorAction.h"
#include "QBBC.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "FTFP_BERT.hh"
#include "G4UImanager.hh"

#include "Analyzer.h"
#include "HardwareNomenclature.h"
#include "includes.hh"
///#include "PsBar.h"

//#include "PhysicsList.h"
//#include "HodoScope.h"
//#include "QGSP_BIC_EMY.hh"
/*
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
*/

//#include "GeantVisualizer.h"
#include "Calibration.h"

#include "Randomize.hh"

int main(int argc, char *argv[]) {
G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  GenerateScintMatrixXYCenters();
  IsSimulation = true;

CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  long seeds[2]={12334457,1239075};
  time_t systime = time(NULL);
    seeds[0] = (long) systime;
    seeds[1] = (long) (systime*G4UniformRand());

  G4cout << "seed1: " << seeds[0] << "; seed2: " << seeds[1] << G4endl;
  CLHEP::HepRandom::setTheSeeds(seeds);
  CLHEP::HepRandom::showEngineStatus();

  // lite_interface::Calibration *calib =
  // lite_interface::Calibration::instance("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib.root");
  lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");

  G4RunManager *runManager = new G4RunManager;
  // G4VUserPhysicsList* phyList = new MyPhysics;

  // Detector Construction
  runManager->SetUserInitialization(new MyDetectorConstruction());
  // runManager->SetUserInitialization(new HodoScope());

  // Physics List
  // For user define PhysicsList uncomment the line below
  // runManager->SetUserInitialization(new MyPhysics);

  // Using Pre defined PhysicsList
  runManager->SetUserInitialization(new FTFP_BERT);

  // Primary Generator Action
  // runManager->SetUserAction(new MyPrimaryGeneratorAction());

  // User action initialization
  runManager->SetUserInitialization(new B1ActionInitialization());
//#ifdef G4VIS_USE
  // Visualization manager construction
  G4VisManager *visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
//#endif

  // Get the pointer to the User Interface manager
  G4UImanager *UImanager = G4UImanager::GetUIpointer();
/*
  if (argc > 1) {
    // execute an argument macro file if exist
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  } else {
    // Needed with Qt sessions
    // Initialize Geant4 kernel
    runManager->Initialize();

    // start interactive session
#ifdef G4UI_USE
    G4UIExecutive *ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac");
#else
    UImanager->ApplyCommand("/control/execute init.mac");
#endif
    if (ui->IsGUI())
      UImanager->ApplyCommand("/control/execute gui.mac");
    ui->SessionStart();
    delete ui;
#endif
  }
*/

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

  delete runManager;
  delete visManager;
  return 0;
}
