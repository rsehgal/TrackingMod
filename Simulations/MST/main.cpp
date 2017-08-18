#include "G4RunManager.hh"
#include "G4VModularPhysicsList.hh"
#include "G4VUserPhysicsList.hh"
#include "MyPrimaryGeneratorAction.h"
#include "MyDetectorConstruction.h"
#include "QBBC.hh"
#include "FTFP_BERT.hh"
#include "QGSP_BIC.hh"
//#include "MyPhysics.h"
#include "B1ActionInitialization.hh"

//#include "PhysicsList.h"
#include "HodoScope2.h"
//#include "QGSP_BIC_EMY.hh"
/*
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
*/

#include "GeantVisualizer.h"


#include "Randomize.hh"

//Some image reconstruction related stuff
//#include <TApplication.h>
//#include "VisualizationHelper.h"
#include "base/Vector3D.h"
//using Tomography::VisualizationHelper;


int main(int argc, char *argv[]){
// TApplication *fApp = new TApplication("Test", NULL, NULL);
//   VisualizationHelper v;


  G4RunManager *runManager = new G4RunManager;
  //G4VUserPhysicsList* phyList = new MyPhysics;

  //Detector Construction
  //runManager->SetUserInitialization(new MyDetectorConstruction());
  runManager->SetUserInitialization(new HodoScope2());

  //Physics List
  //runManager->SetUserInitialization(phyList);
  //runManager->SetUserInitialization(new PhysicsList);
  //runManager->SetUserInitialization(new MyPhysics);
  runManager->SetUserInitialization(new FTFP_BERT);
  //runManager->SetUserInitialization(new QGSP_BIC);

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
/*
  #if (0)
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
*/
#ifdef G4VIS_USE
    // Visualization manager construction
    G4VisManager* visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();
#endif

    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (argc>1) {
        // execute an argument macro file if exist
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    else {
        //Needed with Qt sessions
        // Initialize Geant4 kernel
        runManager->Initialize();

        // start interactive session
#ifdef G4UI_USE
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
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


  delete runManager;


  // v.Show();
  //   gEve->DoRedraw3D(); 
  //   fApp->Run();

  return 0;

}
