
#ifndef B1EventAction_h
#define B1EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>
#include <G4String.hh>
#include <G4ThreeVector.hh>
/// Event action class
///

class B1EventAction : public G4UserEventAction
{
static int evNo;
public:
static std::vector<G4ThreeVector> position;
static std::vector<G4ThreeVector> smearedPosition;


  public:
	static int GetEventNum(){return evNo;}
    B1EventAction();
    virtual ~B1EventAction();
    G4ThreeVector POCA(G4ThreeVector p, G4ThreeVector u, G4ThreeVector q, G4ThreeVector v);
    
    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
