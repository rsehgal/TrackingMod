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
#include "G4GDMLParser.hh"
#include "Voxelator_Evolution.h"
//#include "Scatterers.h"
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
  G4Material *galactic = nist->FindOrBuildMaterial("G4_Galactic");

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

 
  //Adding RPC layer, considering that it is made up of galactic, to avoid interaction in RPC itself
  G4Box *rpc = new G4Box("RPC",50.*cm,50.*cm,0.5*cm);
  G4LogicalVolume *logicalRPC = new G4LogicalVolume(rpc,galactic,"LogicalRPC");
  int rpcNum = -1;
	for (int k = 0; k < 8; k++) {
		if (k == 0 || k == 3 || k == 4 || k == 7)
			continue;
		rpcNum++;
		G4VPhysicalVolume *planeEPhy = new G4PVPlacement(0,
														 G4ThreeVector(0., 0., (-105 + 30 * k) * cm),
														 logicalRPC,
														 "RPC" + std::to_string(rpcNum),
														 logicWorld,
														 false,
														 rpcNum,
														 checkOverlaps);
	}





/*
  //Lets try to build material from NIST database
  G4Box *leadBlock = new G4Box("LeadBlock",50.*cm,50.*cm,5.*cm);
  G4Material *Pb=nist->FindOrBuildMaterial("G4_Pb");
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
*/





/*
	 G4Material *Pb=nist->FindOrBuildMaterial("G4_Pb");
	    G4Material *Fe=nist->FindOrBuildMaterial("G4_Fe");
	    G4Material *Al=nist->FindOrBuildMaterial("G4_Al");
	    G4Material *U=nist->FindOrBuildMaterial("G4_U");


	 //   G4NistManager* nist = G4NistManager::Instance();
	     G4Box *target = new G4Box("Target",5*cm,5*cm,8*cm);
	      G4LogicalVolume *fLogicTarget = new G4LogicalVolume(target,Pb,"LogicalTargetBlock");
	      G4LogicalVolume *fLogicTargetFe = new G4LogicalVolume(target,Pb,"LogicalTargetFeBlock");

	      G4Box *targetAl = new G4Box("Target",5*cm,5*cm,18*cm);
	      G4Box *subtargetU = new G4Box("Target",3*cm,3*cm,7*cm);
	      G4LogicalVolume *fLogicTargetAl = new G4LogicalVolume(targetAl,Al,"LogicalTargetAlBlock");
	      G4LogicalVolume *fLogicTargetU = new G4LogicalVolume(target,U,"LogicalTargetUBlock");
	      G4LogicalVolume *fLogicSubTargetU = new G4LogicalVolume(subtargetU,U,"LogicalSubTargetUBlock");




  G4VPhysicalVolume *phyTargetBlock = new G4PVPlacement(0,
                              //G4ThreeVector(),
                              //G4ThreeVector(15*cm,-15*cm,12*cm),
  		  	  	  	  	    G4ThreeVector(15*cm,-15*cm,0*cm),//-39*cm),
                             fLogicTarget,
                              "TargetPhysical_Pb",
                              //world->GetLogicalVolume(),//
							  logicWorld,

                              false,
                              0,
                              checkOverlaps);

    G4VPhysicalVolume *phyTargetBlock2 = new G4PVPlacement(0,
                                //G4ThreeVector(),
                                //G4ThreeVector(-15*cm,15*cm,12*cm),
  		  	  	  	  	  	  G4ThreeVector(-15*cm,15*cm,0*cm),//-39*cm),
                               fLogicTargetU,
                                "TargetPhysical_U",
                                //world->GetLogicalVolume(),//
								logicWorld,
                                false,
                                0,
                                checkOverlaps);

    G4VPhysicalVolume *phyTargetBlock3 = new G4PVPlacement(0,
                                  //G4ThreeVector(),
                                  //G4ThreeVector(-15*cm,-15*cm,12*cm),
  		  	  	  	  	  	    G4ThreeVector(-15*cm,-15*cm,0*cm),//-39*cm),
                                 fLogicTargetAl,
                                  "TargetPhysical_Al",
                                  //world->GetLogicalVolume(),//
								  logicWorld,
                                  false,
                                  0,
                                  checkOverlaps);

    G4VPhysicalVolume *phyTargetBlock4 = new G4PVPlacement(0,
                                    //G4ThreeVector(),
                                    //G4ThreeVector(25*cm,25*cm,12*cm),
  		  	  	  	  	  	  	  G4ThreeVector(25*cm,25*cm,0*cm),//-39*cm),
                                   fLogicTargetFe,
                                    "TargetPhysical_Fe",
                                    //world->GetLogicalVolume(),//
									logicWorld,
                                    false,
                                    0,
                                    checkOverlaps);

    G4VPhysicalVolume *phySubTargetBlockU = new G4PVPlacement(0,
                                    //G4ThreeVector(),
                                    //G4ThreeVector(-15*cm,-15*cm,12*cm),
    		  	  	  	  	  	    G4ThreeVector(0*cm,0*cm,0*cm),//-39*cm),
  								fLogicSubTargetU,
                                    "TargetSubPhysical_U",
                                    //world->GetLogicalVolume(),//logicWorld,
  								  fLogicTargetAl,

                                    false,
                                    0,
                                    checkOverlaps);
*/






/*


    G4Box *targetOuterBox = new G4Box("TargetOuterBox",16.*cm,16.*cm,16.*cm);
    G4LogicalVolume* fLogicTargetOuterBox = new G4LogicalVolume(targetOuterBox,Al,"LogicalTargetOuterPbox");
    G4VPhysicalVolume *phyTargetOuterBox = new G4PVPlacement(0,
                                G4ThreeVector(),
                                fLogicTargetOuterBox,
                                "PhysicalOuterBox",
								logicWorld,
                                false,
                                0,
                                checkOverlaps);



    //G4Sphere *targetU = new G4Sphere("TargetU",0., 5.*cm, 0., 2*M_PI*rad, 0. , M_PI*rad);
    G4Box *targetU = new G4Box("InnerBox",5.*cm,5.*cm,5.*cm);
    G4LogicalVolume* fLogicTargetSphereU = new G4LogicalVolume(targetU,U,"LogicalTargetUBlock");
    G4VPhysicalVolume *phyTargetUSphere1 = new G4PVPlacement(0,
                                G4ThreeVector(-5*cm,0.,0.),
                                fLogicTargetSphereU,
                                "PhysicalUBall1",
								fLogicTargetOuterBox,
                                false,
                                0,
                                checkOverlaps);

     G4VPhysicalVolume *phyTargetUSphere2 = new G4PVPlacement(0,
                                G4ThreeVector(5*cm,0.,0.),
                                fLogicTargetSphereU,
                                "PhysicalUBall2",
								fLogicTargetOuterBox,
                                false,
                                0,
                                checkOverlaps);

*/


//#define VOXELIZE
#ifdef VOXELIZE
   //Trying to use Voxelator to visualize the VoxelizedVolume
   Tomography::evolution::Voxelator *v = Tomography::evolution::Voxelator::Create(50*cm,50*cm,42*cm,20*cm,20*cm,16.8*cm);
   //v->SetVoxelator(50*cm,50*cm,40*cm,20*cm,20*cm,16*cm);
   v->CalculateVoxelCenters();
   std::vector<Tracking::Vector3D<double>> voxelCenters = v->GetVoxelCenters();
   Tracking::Vector3D<int> voxDim = v->GetEachVoxelDim();
   //Block *voxel = new Block("Voxel",voxDim.x()/2.,voxDim.y()/2.,voxDim.z()/2.,"G4_Galactic");
   G4Box *voxel = new G4Box("Voxel",voxDim.x()/2.,voxDim.y()/2.,voxDim.z()/2.);
   G4LogicalVolume *logicalVoxel = new G4LogicalVolume(rpc,galactic,"LogicalVoxel");
   //int i = 5;
   for(int i = 0 ; i< voxelCenters.size() ; i++){
	//  voxelCenters[i].Print();
   G4VPhysicalVolume *voxPhy = new G4PVPlacement(0,
   	                               //G4ThreeVector(),
   	                               G4ThreeVector(voxelCenters[i].x(),voxelCenters[i].y(),voxelCenters[i].z()),
   	                               //voxel->GetLogicalVolume(),
   	                               logicalVoxel,
   	                               "VoxelPhysical"+std::to_string(i),
   	                               logicWorld,//world->GetLogicalVolume(),//logicWorld,
   	                               false,
   	                               i,
   	                               checkOverlaps);
   }
#endif


  G4GDMLParser parser;
  parser.Write("Hodoscope.gdml", physWorld);



	return physWorld;

}
