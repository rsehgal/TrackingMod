#include "MyDetectorConstruction.h"
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

MyDetectorConstruction::MyDetectorConstruction(){

}

MyDetectorConstruction::~MyDetectorConstruction(){}

G4VPhysicalVolume* MyDetectorConstruction::Construct(){
  
  G4NistManager* nist = G4NistManager::Instance();
  G4bool checkOverlaps = true;

  
  // Envelope params
  //
  G4double env_sizeXY = 20*m, env_sizeZ = 30*m;
  //G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");
  
  //     
  // World
  //
  G4double world_sizeXYZ = 200*cm;
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXYZ, 0.5*world_sizeXYZ, 0.5*world_sizeXYZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  G4Material *orb_material = nist->FindOrBuildMaterial("G4_AIR");

  //RPC setup of 1mx1m
  
  G4Box *rpc = new G4Box("RPC",0.25*world_sizeXYZ, 0.25*world_sizeXYZ, 2.*cm);
  G4LogicalVolume *logicalRpc = new G4LogicalVolume(rpc,orb_material,"LogicalRpc");
  G4ThreeVector firstRpc(0.,0.,60*cm);
  G4ThreeVector secondRpc(0.,0.,30*cm);
  G4ThreeVector thirdRpc(0.,0.,-30*cm);
  G4ThreeVector fourthRpc(0.,0.,-60*cm);
  

  // G4Orb *orb = new G4Orb("simpleOrb",0.2*world_sizeXY);
  // G4LogicalVolume *logicalOrb = new G4LogicalVolume(orb,orb_material,"LogicalOb");
  G4VPhysicalVolume *phyRpc1 = new G4PVPlacement(0,
                          //G4ThreeVector(),
                          firstRpc,
                          logicalRpc,
                          "PhysicalWorld",
                          logicWorld,
                          false,
                          0,
                          checkOverlaps);

  G4VPhysicalVolume *phyRpc2 = new G4PVPlacement(0,
                          //G4ThreeVector(),
                          secondRpc,
                          logicalRpc,
                          "PhysicalWorld",
                          logicWorld,
                          false,
                          0,
                          checkOverlaps);

  G4VPhysicalVolume *phyRpc3 = new G4PVPlacement(0,
                          //G4ThreeVector(),
                          thirdRpc,
                          logicalRpc,
                          "PhysicalWorld",
                          logicWorld,
                          false,
                          0,
                          checkOverlaps);

  G4VPhysicalVolume *phyRpc4 = new G4PVPlacement(0,
                          //G4ThreeVector(),
                          fourthRpc,
                          logicalRpc,
                          "PhysicalWorld",
                          logicWorld,
                          false,
                          0,
                          checkOverlaps);

  G4double a; // atomic mass
  G4double z; // atomic number
  G4double density;

  G4Material* Pb =
    new G4Material("Lead", z= 82., a= 207.19*g/mole, density= 11.35*g/cm3);
  G4Box *leadBlock = new G4Box("LeadBlock",5.*cm,5.*cm,5.*cm);
  //G4LogicalVolume *logicalLeadBlock = new G4LogicalVolume(leadBlock,nist->FindOrBuildMaterial("G4_Pb"),"LogicalLeadBlock");
  G4LogicalVolume *logicalLeadBlock = new G4LogicalVolume(leadBlock,Pb,"LogicalLeadBlock");
  G4VPhysicalVolume *phyLeadBlock = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(),
                            logicalLeadBlock,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);

  /*G4VPhysicalVolume *phyRpc1 = new G4PVPlacement(0.,
                                                firstRpc,
                                                G4ThreeVector(),
                                                "FirstRpc",
                                                logicWorld,
                                                false,
                                                0,
                                                checkOverlaps);
*/
  /*  G4VPhysicalVolume *phyRpc2 = new G4PVPlacement(0.,
                                                secondRpc,
                                                logicalRpc,
                                                "FirstRpc",
                                                logicWorld,
                                                false,
                                                0,
                                                checkOverlaps);
*/

  //Considering Square RPCs



	return physWorld;

}
