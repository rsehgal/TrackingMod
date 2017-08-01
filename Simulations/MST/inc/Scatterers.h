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

G4NistManager* nist = G4NistManager::Instance();

class Block{
G4LogicalVolume* fLogicTarget;
std::string fName;
//G4VPhyscialVolume *physicalPlacedTarget;
public : 
  G4LogicalVolume *GetTarget(){return fLogicTarget;}
  G4LogicalVolume *GetLogicalVolume(){return fLogicTarget;}
  std::string GetName(){return fName;}
  Block(){
    
  }

  Block(float side,std::string nistMaterial){
    G4Material *mat = nist->FindOrBuildMaterial(nistMaterial);
    std::string objName = nistMaterial+"-Target";
    G4Box *target = new G4Box(objName,side,side,side);
    std::string logicalObjectName = objName+"-Logical";
    fLogicTarget = new G4LogicalVolume(target,mat,logicalObjectName);
//    PlaceObject(position);
    
  }

  //This definition is required if we want to define target that can be changed from outside macro
  Block(float side,G4Material *mat){
      //G4Material *mat = nist->FindOrBuildMaterial(nistMaterial);
      //std::string objName = nistMaterial+"-Target";
	  std::string objName = "TestTarget";
      G4Box *target = new G4Box(objName,side,side,side);
      std::string logicalObjectName = objName+"-Logical";
      fLogicTarget = new G4LogicalVolume(target,mat,logicalObjectName);
  //    PlaceObject(position);

    }

  Block(std::string name, float side,std::string nistMaterial){
    G4Material *mat = nist->FindOrBuildMaterial(nistMaterial);
    fName = nistMaterial+"-"+name;
    G4Box *target = new G4Box(fName,side,side,side);
    std::string logicalObjectName = fName+"-Logical";
    fLogicTarget = new G4LogicalVolume(target,mat,logicalObjectName);
//    PlaceObject(position);

  }

  Block(std::string name, double halfX, double halfY, double halfZ,std::string nistMaterial){
    G4Material *mat = nist->FindOrBuildMaterial(nistMaterial);
    fName = nistMaterial+"-"+name;
    G4Box *target = new G4Box(fName,halfX,halfY,halfZ);
    std::string logicalObjectName = fName+"-Logical";
    fLogicTarget = new G4LogicalVolume(target,mat,logicalObjectName);
//    PlaceObject(position);

  }


  void PlaceObject(G4ThreeVector position){
/*
  G4VPhyscialVolume  *physicalPlacedTarget = new G4PVPlacement(0,
                            position,
                            fLogicTarget,
                            "PhysicalWorld",
                            fLogicTarget,
                            //logicWorld,
                            false,
                            0,
                            checkOverlaps);
*/

  }

};

class BallsInABox{
   G4LogicalVolume* fLogicTargetPb;
public:
  G4LogicalVolume* GetTarget(){return fLogicTargetPb; }

  BallsInABox(){
G4NistManager* nist = G4NistManager::Instance();
	G4Box *targetPb = new G4Box("TargetPb",10.*cm,10.*cm,10.*cm);
  fLogicTargetPb = new G4LogicalVolume(targetPb,nist->FindOrBuildMaterial("G4_WATER"),"LogicalTargetPbBlock");
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
  G4LogicalVolume* fLogicTargetVac = new G4LogicalVolume(targetVac,nist->FindOrBuildMaterial("G4_Al"),"LogicalTargetPbBlock");
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
