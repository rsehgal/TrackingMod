#ifndef HodoScope_h
#define HodoScope_h

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VUserDetectorConstruction;
class G4VPhysicalVolume;
class G4LogicalVolume;
class B2aDetectorMessenger;
class G4Material;

class HodoScope : public G4VUserDetectorConstruction
{
G4LogicalVolume* fScoringVolume;
B2aDetectorMessenger*  fMessenger;   // messenger
G4LogicalVolume*   fLogicTarget;
G4Material*        fTargetMaterial; 


public:
	HodoScope();
	~HodoScope();
	G4VPhysicalVolume* Construct();
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
    void SetTargetMaterial (G4String );
};
#endif
