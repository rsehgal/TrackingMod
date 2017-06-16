#include "HodoScope2.h"
#include "G4RunManager.hh"
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
#include "B2aDetectorMessenger.hh"

#include "G4GDMLParser.hh"

#include "Scatterers.h"
//#include "base/Global.h"
#define PI 3.14159265359

HodoScope2::HodoScope2(){
double density = 2.700*g/cm3;
double a = 26.98*g/mole;
double z;
char *name="";
fMessenger = new B2aDetectorMessenger(this);
fTargetMaterial = new G4Material(name="Aluminum", z=13., a, density);
fTargetThickness = 1*cm;
}

HodoScope2::~HodoScope2(){
delete fMessenger;
}

// This Construct function creates 2 Scintillator Planes, 6 shelves with 1 mm Al
// and one targets of 10cm X 10cm X 10cm. 
// this is written with a view to run in batch mode, where the target material 
// and gun energy can be changed via macro file.
// So that we should be able to take different runs with different target material
// and different energy beams.
// Each run should create its histogram or any other required plot
G4VPhysicalVolume* HodoScope2::Construct(){
  
  //G4NistManager* nist = G4NistManager::Instance();
  G4bool checkOverlaps = true;

  G4double world_sizeXYZ = 200*cm;

  //World
  Block *world = new Block("World",0.75*world_sizeXYZ,"G4_AIR");
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      world->GetLogicalVolume(),            //its logical volume
                      "PhysicalWorld",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  //Bottom and Top Scintillator plane
  Block *scintPlane = new Block("TriggeringPlane",0.60*world_sizeXYZ, 0.60*world_sizeXYZ, 1.*cm,"G4_AIR");
  std::string planeName;
  for(int i = -1 ; i < 2 ; i=i+2){
  if(i == -1)
	  planeName = "Bottom";
  else
	  planeName = "Top";
  G4VPhysicalVolume *phyTopScintPlane = new G4PVPlacement(0,
                                G4ThreeVector(0.,0.,i*105.*cm),
                                scintPlane->GetLogicalVolume(),
                                scintPlane->GetName()+"-"+planeName+"-Physical",
                                world->GetLogicalVolume(),
                                false,
                                0,
                                checkOverlaps);
  }

  //6 Shelves starting from Bottom
  Block *shelf = new Block("Shelf",0.50*world_sizeXYZ, 0.50*world_sizeXYZ, 0.5*mm,"G4_Al");
  double bottomShelfZ = -75*cm;
  for(int i = -3 ; i < 3; i++ ){
	  //if(i==-1 || i==0) continue;
      //Now physically placing Six such shelves for Six Rpc's
	  int shelfNum = i+3;
      G4VPhysicalVolume *phyFirstShelf = new G4PVPlacement(0,
                                      //G4ThreeVector(),
                                      G4ThreeVector(0.,0.,bottomShelfZ+(shelfNum*30*cm)),
                                      shelf->GetLogicalVolume(),
                                      shelf->GetName()+"-"+std::to_string(shelfNum+1)+"-Physical",
                                      world->GetLogicalVolume(),
                                      false,
                                      0,
                                      checkOverlaps);
  }
/*

//BallsInABox *scatterer = new BallsInABox();
//Block *scatterer = new Block(5.*cm,"G4_Pb");
  Block *scatterer = new Block(5.*cm,fTargetMaterial);
G4VPhysicalVolume *phyTargetPbBlock = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(),
                            scatterer->GetTarget(),
                            "Scatterer",
                            world->GetLogicalVolume(),
                            false,
                            0,
                            checkOverlaps);
*/

//Generalized target which can be changed from UI, default material is set to Aluminium
  target = new G4Box("Target",0.50*world_sizeXYZ, 0.50*world_sizeXYZ,fTargetThickness);
  fLogicTarget = new G4LogicalVolume(target,fTargetMaterial,"LogicalTargetBlock");
  G4VPhysicalVolume *phyTargetBlock = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(),
                            fLogicTarget,
                            "PhysicalWorld",
                            world->GetLogicalVolume(),//logicWorld,
                            false,
                            0,
                            checkOverlaps);



	//fScoringVolume = logicalLeadBlock;
   fScoringVolume = shelf->GetLogicalVolume();//logicalShelf;

G4GDMLParser parser;
parser.Write("Hodoscope.gdml", physWorld);

	return physWorld;

}

void HodoScope2::SetTargetMaterial(G4String materialName)
{
  G4NistManager* nistManager = G4NistManager::Instance();

  G4Material* pttoMaterial =
              nistManager->FindOrBuildMaterial(materialName);

  if (fTargetMaterial != pttoMaterial) {
     if ( pttoMaterial ) {
        fTargetMaterial = pttoMaterial;
        if (fLogicTarget) fLogicTarget->SetMaterial(fTargetMaterial);
        G4cout
          << G4endl
          << "----> The target is made of " << materialName << G4endl;
         // << "----> Target Z Half Length : " << target->GetZHalfLength() << G4endl;


     } else {
        G4cout
          << G4endl
          << "-->  WARNING from SetTargetMaterial : "
          << materialName << " not found" << G4endl;
     }
  }
}
