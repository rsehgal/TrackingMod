#include "MyDetectorConstruction.h"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include <G4Tubs.hh>
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
#include <cmath>
#include <iostream>


int MyDetectorConstruction::stripNum = 0;
int MyDetectorConstruction::blockNum=-1;
int MyDetectorConstruction::eBlockNum=1;


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
  
 G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.8*world_sizeXYZ, 0.8*world_sizeXYZ, 0.8*world_sizeXYZ);     //its size

     
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


 G4Material *Si=nist->FindOrBuildMaterial("G4_AIR");
//  G4LogicalVolume *planeLogical = GetBlock("planeDeltaE",2.5*cm,2.5*cm,0.0025*cm,Si,16,0.0025*cm,1);

  G4LogicalVolume *planeE1Logical = GetBlock("planeE1",50*cm,50*cm,0.5*cm,Si,32,0.5*cm,1);
  G4LogicalVolume *planeE2Logical = GetBlock("planeE2",50*cm,50*cm,0.5*cm,Si,32,0.5*cm,2);
  G4LogicalVolume *planeE = CreateEBlock(planeE1Logical, planeE2Logical, 1.*cm, Si);

for(int k=0; k < 8 ; k++){
if(k==0 || k==3 || k==4 || k==7)
	continue;
 eBlockNum++;
 G4VPhysicalVolume *planeEPhy = new G4PVPlacement(0,
                            G4ThreeVector(0.,0.,(-105+30*k)*cm),
                            planeE,
                            "Module"+std::to_string(eBlockNum),
                            logicWorld,
                            false,
			    eBlockNum,
                           checkOverlaps);
}


//Trying to create scattering of collection of bricks of dimension 10cm X 5cm X 3cm
G4LogicalVolume *brickLane = GetBlock("brickLane",15*cm,2.5*cm,1.5*cm,Si,3,1.5*cm,1);
G4LogicalVolume *brickLanePlane = GetBlock("brickLanePlane",15*cm,15*cm,1.5*cm,Si,brickLane,6,2);
G4LogicalVolume *brickLanePlaneBlock = GetBlock("brickLanePlaneBlock",15*cm,15*cm,15*cm,Si,brickLanePlane,10,3);
G4VPhysicalVolume *brickLanePhy = new G4PVPlacement(0,
                            G4ThreeVector(0.,0.,0.),
                            brickLanePlaneBlock,
                            //"Module"+std::to_string(eBlockNum),
							"LanePlaneBlock1",
                            logicWorld,
                            false,
							//eBlockNum,
							0,
                           checkOverlaps);

	return physWorld;
}


//Helper functions for Geometry creation.
G4LogicalVolume* MyDetectorConstruction::GetBlock(std::string name,double halfX,double halfY, double halfZ,
		G4Material *mat,int numOfStrips, double stripHalfZ,int dir){

  G4bool checkOverlaps = true;
  double stripLen = 2*halfX/numOfStrips;
  double stripHalfLen = 0.5*stripLen;
  G4LogicalVolume *tempLogical =  new G4LogicalVolume( (new G4Box(name,halfX,halfY,halfZ)),mat, name+"Logical" );
  if(dir==1)
	  stripNum = 32;
  else
	  stripNum = 64;
  for(int i = 0 ; i < numOfStrips ; i++){

        G4ThreeVector temp(0.,0.,0.);
        
        if(dir==1){
	temp.setX(-halfX + stripHalfLen + i*stripLen);
	G4VPhysicalVolume *blockPhy = new G4PVPlacement(0,//yRot,
		            temp,		   
                            GetStrip("strip",stripHalfLen,halfY,stripHalfZ,mat),
							"_LE_CH"+std::to_string(stripNum),
                            //tempLogical->GetName()+"Strip"+std::to_string(i),
                            tempLogical,
                            false,
                            i,
                           checkOverlaps);
	}
	else{
	temp.setY(-halfY + stripHalfLen + i*stripLen);
	G4VPhysicalVolume *blockPhy = new G4PVPlacement(0,//yRot,
		            temp,		   
                            GetStrip("strip",halfX,stripHalfLen,stripHalfZ,mat),
							"_LE_CH"+std::to_string(stripNum),
                            //tempLogical->GetName()+"Strip"+std::to_string(i),
                            tempLogical,
                            false,
                            i,
                           checkOverlaps);
	
	}
        stripNum++;
  }

  return tempLogical;
}

G4LogicalVolume* MyDetectorConstruction::GetBlock(std::string name,double halfX,double halfY, double halfZ, G4Material *mat,
		G4LogicalVolume* logVol,int numOfLogVol,int dir){
	G4bool checkOverlaps = true;
	G4LogicalVolume *tempLogical =  new G4LogicalVolume( (new G4Box(name,halfX,halfY,halfZ)),mat, name+"Logical" );
	for(int i = 0  ; i < numOfLogVol ; i++){
		G4ThreeVector temp(0.,0.,0.);
		if(dir==1){ //X Direction
			double stripLen = 2*halfX/numOfLogVol;
			double stripHalfLen = 0.5*stripLen;
			temp.setX(-halfX + stripHalfLen + i*stripLen);

		}else if(dir==2){
			//Y Direction
			double stripLen = 2*halfY/numOfLogVol;
			double stripHalfLen = 0.5*stripLen;
			temp.setY(-halfY + stripHalfLen + i*stripLen);

		}else if(dir==3){
			//Z Direction
			double stripLen = 2*halfZ/numOfLogVol;
			double stripHalfLen = 0.5*stripLen;
			temp.setZ(-halfZ + stripHalfLen + i*stripLen);
		}
		G4VPhysicalVolume *blockPhy = new G4PVPlacement(
														0, //yRot,
														temp,
														logVol,
														"brickLane"+std::to_string(i),
														//"_LE_CH" + std::to_string(stripNum),
														//tempLogical->GetName()+"Strip"+std::to_string(i),
														tempLogical,
														false,
														i,
														checkOverlaps);
	}

	return tempLogical;
}

G4LogicalVolume* MyDetectorConstruction::GetStrip(std::string name,double halfX,double halfY, double halfZ, G4Material *mat){

return (new G4LogicalVolume( (new G4Box(name+std::to_string(stripNum),halfX,halfY,halfZ)),mat, name+std::to_string(stripNum)+"Logical" ));

}

G4LogicalVolume* MyDetectorConstruction::CreateEBlock(G4LogicalVolume* E1, G4LogicalVolume* E2, double halfZ, G4Material *mat){
  G4bool checkOverlaps = true;
G4LogicalVolume* EBlockLogical = GetStrip("XYBlock",50.*cm,50.*cm,halfZ,mat);
G4ThreeVector temp1(0.,0.,-halfZ/2.);

G4VPhysicalVolume *blockEPhy1 = new G4PVPlacement(0,//yRot,
		            temp1,		   
                            E1,
                            "PlaneX",
                            EBlockLogical,
                            false,
                            0,
                           checkOverlaps);


G4ThreeVector temp2(0.,0.,halfZ/2.);
G4VPhysicalVolume *blockEPhy2 = new G4PVPlacement(0,//yRot,
		            temp2,		   
                            E2,
                            "PlaneY",
                            EBlockLogical,
                            false,
                            0,
                           checkOverlaps);

return EBlockLogical;

}
