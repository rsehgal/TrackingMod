#include "DetectorHelpers.h"
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


int DetectorHelpers::stripNum = 0;
int DetectorHelpers::blockNum=-1;
int DetectorHelpers::eBlockNum=1;


DetectorHelpers::DetectorHelpers(){

}

DetectorHelpers::~DetectorHelpers(){}


//Helper functions for Geometry creation.
G4LogicalVolume* DetectorHelpers::GetBlock(std::string name,double halfX,double halfY, double halfZ,
		G4Material *mat,int numOfStrips, double stripHalfZ,int dir,int start, std::string defaultName){

  G4bool checkOverlaps = true;
  double stripLen = 2*halfX/numOfStrips;
  double stripHalfLen = 0.5*stripLen;
  G4LogicalVolume *tempLogical =  new G4LogicalVolume( (new G4Box(name,halfX,halfY,halfZ)),mat, name+"Logical" );
  if(dir==1)
	  //stripNum = 32;
      stripNum = start;
  else
	  stripNum = 64;
  for(int i = 0 ; i < numOfStrips ; i++){

        G4ThreeVector temp(0.,0.,0.);
        
        if(dir==1){
	temp.setX(-halfX + stripHalfLen + i*stripLen);
	G4VPhysicalVolume *blockPhy = new G4PVPlacement(0,//yRot,
		            temp,		   
                            GetStrip("strip",stripHalfLen,halfY,stripHalfZ,mat),
							defaultName+std::to_string(stripNum),
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
							defaultName+std::to_string(stripNum),
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

G4LogicalVolume* DetectorHelpers::GetBlock(std::string name,double halfX,double halfY, double halfZ, G4Material *mat,
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
														"brick"+std::to_string(i),
														//"_LE_CH" + std::to_string(stripNum),
														//tempLogical->GetName()+"Strip"+std::to_string(i),
														tempLogical,
														false,
														i,
														checkOverlaps);
	}

	return tempLogical;
}

G4LogicalVolume* DetectorHelpers::GetStrip(std::string name,double halfX,double halfY, double halfZ, G4Material *mat){

return (new G4LogicalVolume( (new G4Box(name+std::to_string(stripNum),halfX,halfY,halfZ)),mat, name+std::to_string(stripNum)+"Logical" ));

}

G4LogicalVolume* DetectorHelpers::CreateEBlock(G4LogicalVolume* E1, G4LogicalVolume* E2, double halfZ, G4Material *mat){
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
