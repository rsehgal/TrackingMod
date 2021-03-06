#ifndef GeantVisualizer_h
#define GeantVisualizer_h

#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4String.hh"

class GeantVisualizer
{
public:
	GeantVisualizer(int argc, char **argv);
	~GeantVisualizer();
	void Start();
	void Start(G4String filename);

private:
	G4VisManager* fVisManager;
	G4UImanager* fUImanager;
	G4UIExecutive* fUi;
	
};

GeantVisualizer::GeantVisualizer(int argc, char **argv){
	fVisManager = new G4VisExecutive;
	fUImanager = G4UImanager::GetUIpointer();
	fUi = new G4UIExecutive(argc,argv);
}

GeantVisualizer::~GeantVisualizer(){
	delete fUi;
	delete fUImanager;
	delete fVisManager;
}

void GeantVisualizer::Start(){

	fVisManager->Initialize();
	fUImanager->ApplyCommand("/control/execute init_vis.mac");
    fUi->SessionStart();
}
  
void GeantVisualizer::Start(G4String filename){

        fVisManager->Initialize();
        fUImanager->ApplyCommand("/control/execute init_vis.mac");
        fUImanager->ApplyCommand("/control/execute "+filename);
    fUi->SessionStart();
}
#endif
