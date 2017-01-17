#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4UnitsTable.hh"

#define PI 3.14159265359

class BallsInABox{
   G4LogicalVolume* fLogicTargetPb;
public:
  G4LogicalVolume* GetTarget(){return fLogicTargetPb; }

  BallsInABox(){
G4NistManager* nist = G4NistManager::Instance();
	G4Box *targetPb = new G4Box("TargetPb",10.*cm,10.*cm,10.*cm);
  fLogicTargetPb = new G4LogicalVolume(targetPb,nist->FindOrBuildMaterial("G4_Fe"),"LogicalTargetPbBlock");
/* fPhyTargetPbBlock = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(),
                            fLogicTargetPb,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
*/
bool checkOverlaps = true;
G4Box *targetVac = new G4Box("TargetPb",9.*cm,9.*cm,9.*cm);
  //G4LogicalVolume *logicalLeadBlock = new G4LogicalVolume(leadBlock,nist->FindOrBuildMaterial("G4_Pb"),"LogicalLeadBlock");
  G4LogicalVolume* fLogicTargetVac = new G4LogicalVolume(targetVac,nist->FindOrBuildMaterial("G4_Pb"),"LogicalTargetPbBlock");
  G4VPhysicalVolume *phyTargetVacBlock = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(),
                            fLogicTargetVac,
                            "PhysicalWorld",
			    fLogicTargetPb,
                            //logicWorld,
                            false,
                            0,
                            checkOverlaps);

 
  G4Material* matU = nist->FindOrBuildMaterial("G4_U");
G4Sphere *targetU = new G4Sphere("TargetU",0., 5.*cm, 0., 2*PI, 0. , PI);
 G4LogicalVolume* fLogicTargetU = new G4LogicalVolume(targetU,nist->FindOrBuildMaterial("G4_U"),"LogicalTargetUBlock");
  G4VPhysicalVolume *phyTargetUBlock = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(-5*cm,0.,0.),
                            fLogicTargetU,
                            "PhysicalWorld",
                            fLogicTargetVac,
                            //logicWorld,
                            false,
                            0,
                            checkOverlaps);

 G4VPhysicalVolume *phyTargetUBlock2 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(5*cm,0.,0.),
                            fLogicTargetU,
                            "PhysicalWorld",
			    fLogicTargetVac,
                            //logicWorld,
                            false,
                            0,
                            checkOverlaps);
  }
};
