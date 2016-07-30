#ifndef HodoScope_h
#define HodoScope_h

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VUserDetectorConstruction;
class G4VPhysicalVolume;
class G4LogicalVolume;

class HodoScope : public G4VUserDetectorConstruction
{
G4LogicalVolume* fScoringVolume;
public:
	HodoScope();
	~HodoScope();
	G4VPhysicalVolume* Construct();
        G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
};
#endif
