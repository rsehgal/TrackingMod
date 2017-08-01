#ifndef HodoScope2_h
#define HodoScope2_h

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include <G4Material.hh>
#include <G4Box.hh>
#include "G4RunManager.hh"

class G4VUserDetectorConstruction;
class G4VPhysicalVolume;
class G4LogicalVolume;
class B2aDetectorMessenger;
class G4Box;
//class G4Material;

class HodoScope2 : public G4VUserDetectorConstruction
{
G4LogicalVolume* fScoringVolume;
B2aDetectorMessenger*  fMessenger;   // messenger
G4LogicalVolume*   fLogicTarget;
G4Box *target;
G4Material*        fTargetMaterial;
double fTargetThickness; 
//G4Box *target;


public:
	HodoScope2();
	~HodoScope2();
	G4VPhysicalVolume* Construct();
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
    void SetTargetMaterial (G4String );
    void SetTargetThickness (double val){fTargetThickness = val; G4cout<<"Target Thickness Set to : " << fTargetThickness << G4endl;
    //target->SetXHalfLength(fTargetThickness);
    //target->SetYHalfLength(fTargetThickness);
    target->SetZHalfLength(fTargetThickness);
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    }
    double GetTargetThickness() const {return fTargetThickness;}
    G4String GetTargetMaterial() const {return fTargetMaterial->GetName();}
    void WriteVoxelizedVolume(double halfXWidth, double halfYWidth, double halfZWidth, double posX = 0. ,double posY=0., double posZ=0.);
};
#endif
