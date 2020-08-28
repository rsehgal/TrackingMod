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
#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4UnitsTable.hh"
#include "G4SDManager.hh"
#include "MySD.h"
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

 

  //Lets try to build material from NIST database
  /*G4Box *leadBlock = new G4Box("LeadBlock",50.*cm,50.*cm,5.*cm);
  G4Material *Pb=nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");
  G4LogicalVolume *logicalLeadBlock = new G4LogicalVolume(leadBlock,Pb,"LogicalLeadBlock");
  MySD* mySD = new MySD("MySensitiveDetector", "MyBlockHitsCollection");
  G4SDManager *sdman = G4SDManager::GetSDMpointer();
  sdman->AddNewDetector(mySD);
  logicalLeadBlock->SetSensitiveDetector(mySD);
  G4VPhysicalVolume *phyLeadBlock1 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(),
                            logicalLeadBlock,
                            "block1",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
*/
  //G4_Galactic is chosen as PsBar material
  G4Material* bar_mat = nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");

  double halfXOneBar = 5.*cm;
  double halfYOneBar = 50.*cm;
  double halfZOneBar = 5.*cm;
  //Lets try to build one bar of 10cm X 10cm X 10cm
  G4Box *psBar = new G4Box("PsBar",halfXOneBar,halfYOneBar,halfZOneBar);
  G4LogicalVolume *logicalPsBar = new G4LogicalVolume(psBar,bar_mat,"LogicalPsBar");
  MySD* mySD = new MySD("MySensitiveDetector", "MyBlockHitsCollection");
  G4SDManager *sdman = G4SDManager::GetSDMpointer();
  sdman->AddNewDetector(mySD);
  logicalPsBar->SetSensitiveDetector(mySD);
  /*
   // Example to Place one PsBar at origin
   G4VPhysicalVolume *phyPsBar = new G4PVPlacement(0,
                            G4ThreeVector(),
                            logicalPsBar,
                            "PhysicalPsBar",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
  */
  //Lets try to build the full matrix of PsBars
  int barInX=9;
  int barInZ=3;
  double yval=0;
  double xstart=-1*(barInX/2)*(2*halfXOneBar);
  double zstart=-1*(barInZ/2)*(2*halfZOneBar);
  std::cout <<"XStart : " << xstart <<" : ZStart : " << zstart << std::endl;
  int counter=0;
  for(unsigned int zindex = 0 ; zindex < barInZ ; zindex++ ){
    double zval = zstart + (2*zindex+1)*halfZOneBar;
    for(unsigned int xindex = 0 ; xindex < barInX ; xindex++ ){
      double xval = xstart + (2*xindex+1)*halfXOneBar;
      std::cout <<xval<<","<<yval<<","<<zval<<std::endl;
      new G4PVPlacement(0,
                                  G4ThreeVector(xval,yval,zval),
                                  logicalPsBar,
                                  "PhysicalPsBar-"+std::to_string(xindex)+"-"+std::to_string(zindex),
                                  logicWorld,
                                  false,
                                  counter,
                                  checkOverlaps);
      counter++;
    }
  }

/*G4Box *testBox = new G4Box("testBox",50*cm,50*cm,0.5*cm);
  G4LogicalVolume *logicalTestBox = new G4LogicalVolume(testBox,bar_mat,"LogicalTestBox");
  new G4PVPlacement(0,
                                    G4ThreeVector(0,0,70*cm),
                                    logicalTestBox,
                                    "TestBoxTop",
                                    logicWorld,
                                    false,
                                    0,
                                    checkOverlaps);

  new G4PVPlacement(0,
                                      G4ThreeVector(0,0,-70*cm),
                                      logicalTestBox,
                                      "TestBoxBottom",
                                      logicWorld,
                                      false,
                                      0,
                                      checkOverlaps);

*/  /*G4VPhysicalVolume *phyLeadBlock2 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(0,0,-15*cm),
                            logicalLeadBlock,
                            "block2",
                            logicWorld,
                            false,
                            1,
                            checkOverlaps);*/



	return physWorld;

}