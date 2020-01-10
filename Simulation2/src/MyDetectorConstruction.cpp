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
#include "DetectorMapping.h"
#include <G4GDMLParser.hh>
#include "Voxelator_Evolution.h"


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
  G4double world_sizeXYZ = 300*cm;
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  
 G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

/*
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
      world_sizeXYZ, world_sizeXYZ, world_sizeXYZ);     //its size
       //0.8*world_sizeXYZ, 0.8*world_sizeXYZ, 0.8*world_sizeXYZ);     //its size

     
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
*/


 G4Material *Si=nist->FindOrBuildMaterial("G4_AIR");
 G4Material *galactic=nist->FindOrBuildMaterial("G4_Galactic");
 //  G4LogicalVolume *planeLogical = GetBlock("planeDeltaE",2.5*cm,2.5*cm,0.0025*cm,Si,16,0.0025*cm,1);

 Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::create("testMapping.txt");
 int numberOfStripsInEachPlane = detectorMap->GetNumberOfStripsInEachPlane();

  G4LogicalVolume *planeE1Logical = GetBlock("planeE1",50*cm,50*cm,0.5*cm,Si,numberOfStripsInEachPlane,0.5*cm,1);
  G4LogicalVolume *planeE2Logical = GetBlock("planeE2",50*cm,50*cm,0.5*cm,Si,numberOfStripsInEachPlane,0.5*cm,2);
  G4LogicalVolume *planeE = CreateEBlock(planeE1Logical, planeE2Logical, 1.*cm, Si);

/*for(int k=0; k < 8 ; k++){
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
*/
  //Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::create("testMapping.txt");
  // std::vector<std::string> detectorNamesVector = detectorMap->GetDetectorNamesVector();
  // std::vector<int> startChannelVector = detectorMap->GetStartingChannelVector();
  // std::vector<int> moduleVector = detectorMap->GetModuleVector();
  // std::vector<double> zcoordinateVector = detectorMap->GetZCoordinateVector();

  double worldHalfLength = detectorMap->GetWorldHalfLength();
  G4Box* solidWorld =
    new G4Box("World",                       //its name
      worldHalfLength,worldHalfLength,worldHalfLength);     //its size

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

  std::vector<Tomography::Mapping::Detector*>  detectorVector = detectorMap->GetDetectorVector();

  //std::cout << "Length of DetectorMapping from DetectorConstruction : " << detectorNamesVector.size() << std::endl;
  std::cout << "Length of DetectorMapping from DetectorConstruction : " << detectorVector.size() << std::endl;

  for(int i = 0 ; i < detectorVector.size()-1; i++){
    if(detectorVector[i]->sDetectorType == "GLASS"){
    G4VPhysicalVolume *planeEPhy = new G4PVPlacement(0,
                            G4ThreeVector(0.,0., detectorVector[i]->sZCoordinate),
                            planeE,
                            "Module"+std::to_string(detectorVector[i]->sModule),
                            logicWorld,
                            false,
                            detectorVector[i]->sModule,
                            checkOverlaps);
  }
  }

  G4LogicalVolume *scintTopPlaneLogical = GetBlock("TopScintPlane",9*8*cm,90*cm,0.5*cm,Si,8,0.5*cm,1,0);
  G4LogicalVolume *scintBottomPlaneLogical = GetBlock("BottomScintPlane",9*8*cm,90*cm,0.5*cm,Si,8,0.5*cm,1,8);

  int plane = -1;
  for(int i = 0 ; i < detectorVector.size()-1; i++){
    if(detectorVector[i]->sDetectorType == "TRG"){
    plane++;
    if(plane == 0){
    G4VPhysicalVolume *planeScintPhy = new G4PVPlacement(0,
                            G4ThreeVector(0.,0., detectorVector[i]->sZCoordinate),
                            scintTopPlaneLogical,
                            "Module"+std::to_string(detectorVector[i]->sModule),
                            logicWorld,
                            false,
                            detectorVector[i]->sModule,
                            checkOverlaps);
  }

  if(plane == 1){
    G4VPhysicalVolume *planeScintPhy = new G4PVPlacement(0,
                            G4ThreeVector(0.,0., detectorVector[i]->sZCoordinate),
                            scintBottomPlaneLogical,
                            "Module"+std::to_string(detectorVector[i]->sModule),
                            logicWorld,
                            false,
                            detectorVector[i]->sModule,
                            checkOverlaps);
  }
  }
  }

  /*for(int i = 0 ; i < detectorNamesVector.size()-1; i++){
    std::cout << "Raman : DetZ : " << zcoordinateVector[i] << std::endl;

    G4VPhysicalVolume *planeEPhy = new G4PVPlacement(0,
                            G4ThreeVector(0.,0.,zcoordinateVector[i]),
                            planeE,
                            "Module"+std::to_string(moduleVector[i]),
                            logicWorld,
                            false,
                            moduleVector[i],
                           checkOverlaps);
  }*/


/*G4Material *Pb=nist->FindOrBuildMaterial("G4_U");
//Trying to create scattering of collection of bricks of dimension 10cm X 5cm X 3cm
G4LogicalVolume *brickLane = GetBlock("brickLane",15*cm,2.5*cm,1.5*cm,Pb,3,1.5*cm,1);
G4LogicalVolume *brickLanePlane = GetBlock("brickLanePlane",15*cm,15*cm,1.5*cm,Pb,brickLane,6,2);
G4LogicalVolume *brickLanePlaneBlock = GetBlock("brickLanePlaneBlock",15*cm,15*cm,15*cm,Pb,brickLanePlane,10,3);
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
*/

/*
    G4Material *Pb=nist->FindOrBuildMaterial("G4_Pb");
    G4Box *leadBox = new G4Box("LeadBox",10*cm,10*cm,10*cm);
    G4LogicalVolume *logicalLeadBox = new G4LogicalVolume(leadBox,Pb,"LogicalLeadBox");
    G4VPhysicalVolume *phyicalLeadBox = new G4PVPlacement(0,
                            G4ThreeVector(0.,0.,0.),
                            logicalLeadBox,
                            "PhysicalLeadBox",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);  
*/

    G4Material *Pb=nist->FindOrBuildMaterial("G4_Pb");
    G4Material *Fe=nist->FindOrBuildMaterial("G4_Fe");
    G4Material *Al=nist->FindOrBuildMaterial("G4_Al");
    G4Material *U=nist->FindOrBuildMaterial("G4_U");

#if(1)

    G4Box *target = new G4Box("Target",5*cm,5*cm,5*cm);

  G4LogicalVolume *fLogicTargetPb = new G4LogicalVolume(target,Pb,"LogicalTargetBlock");
  G4LogicalVolume *fLogicTargetFe = new G4LogicalVolume(target,Fe,"LogicalTargetFeBlock");
  G4LogicalVolume *fLogicTargetAl = new G4LogicalVolume(target,Al,"LogicalTargetAlBlock");
  G4LogicalVolume *fLogicTargetU = new G4LogicalVolume(target,U,"LogicalTargetUBlock");

  //G4Box *targetAl = new G4Box("Target",7*cm,7*cm,18*cm);
  G4Box *subtargetU = new G4Box("Target",3*cm,3*cm,7*cm);
  //G4LogicalVolume *fLogicTargetAl = new G4LogicalVolume(targetAl,Al,"LogicalTargetAlBlock");


  G4LogicalVolume *fLogicSubTargetU = new G4LogicalVolume(subtargetU,U,"LogicalSubTargetUBlock");

/*G4VPhysicalVolume *phyTargetBlock = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(25*cm,15.*cm,13.5*cm),
                           fLogicTarget,
                            "TargetPhysical",
                            world->GetLogicalVolume(),//logicWorld,
                            false,
                            0,
                            checkOverlaps);
*/


/*
 // WILL TRY TO READ SCATTERER INFORMATION FROM DetectorMapping.txt AS BELOW
  G4VPhysicalVolume *phyTargetBlock = new G4PVPlacement(0,
                            G4ThreeVector(15*cm,-15*cm,0*cm),//-39*cm),
                           fLogicTargetPb,
                            "TargetPhysical_Pb",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);

  G4VPhysicalVolume *phyTargetBlock2 = new G4PVPlacement(0,
                              G4ThreeVector(-15*cm,15*cm,0*cm),//-39*cm),
                             fLogicTargetU,
                              "TargetPhysical_U",
                             logicWorld,
                              false,
                              0,
                              checkOverlaps);

  G4VPhysicalVolume *phyTargetBlock3 = new G4PVPlacement(0,
                                G4ThreeVector(-15*cm,-15*cm,0*cm),//-39*cm),
                               fLogicTargetAl,
                                "TargetPhysical_Al",
                               logicWorld,
                                false,
                                0,
                                checkOverlaps);

  G4VPhysicalVolume *phyTargetBlock4 = new G4PVPlacement(0,
                                 G4ThreeVector(25*cm,25*cm,0*cm),//-39*cm),
                                 fLogicTargetFe,
                                  "TargetPhysical_Fe",
                                  logicWorld,
                                  false,
                                  0,
                                  checkOverlaps);
*/

  std::vector<Tomography::Mapping::Scatterer*>  scattererVector = detectorMap->GetScattererVector();
  for(int i = 0 ; i < scattererVector.size() ; i++){
	  Tomography::Mapping::Scatterer *scatterer = scattererVector[i];
	  G4Material *material=nist->FindOrBuildMaterial((scatterer->sMaterial).c_str());
	  std::string scattererName = scatterer->sName;
	  G4Box *target = new G4Box(scattererName,(scatterer->sDim).x(),(scatterer->sDim).y(),(scatterer->sDim).z());
	  G4LogicalVolume *logicalScattererer = new G4LogicalVolume(target,material,("Logical-"+scattererName).c_str());
	  G4VPhysicalVolume *physicalTarget = new G4PVPlacement(0,
	                               G4ThreeVector((scatterer->sLocation).x(),(scatterer->sLocation).y(),(scatterer->sLocation).z()),
	                               logicalScattererer,
	                               scattererName.c_str(),
	                               logicWorld,
	                                false,
	                                0,
	                                checkOverlaps);
  }


  //Different Scatterers for Reconstruction. A Box, Tube, Sphere
/*
  G4Box *boxScatterer = new G4Box("BoxScatterer",10*cm,10*cm,10*cm);
  G4LogicalVolume *boxLogical = new G4LogicalVolume(boxScatterer,Fe,"BoxLogical");
  G4VPhysicalVolume *boxPhysical = new G4PVPlacement(0,
  	                               G4ThreeVector(15.*cm,15.*cm,0.),
								   boxLogical,
  	                               "BoxPhysical",
  	                               logicWorld,
  	                                false,
  	                                0,
  	                                checkOverlaps);

  G4Orb *orbScatterer = new G4Orb("OrbScatterer",5*cm);
  G4LogicalVolume *orbLogical = new G4LogicalVolume(orbScatterer,Pb,"OrbLogical");
  G4VPhysicalVolume *orbPhysical = new G4PVPlacement(0,
  	                               G4ThreeVector(0.,0.,0.),
								   orbLogical,
  	                               "OrbPhysical",
								   logicWorld,
  	                                false,
  	                                0,
  	                                checkOverlaps);

  G4Tubs *tubsScatterer = new G4Tubs("TubsScatterer", 8*cm,10*cm,10*cm,0.,2*M_PI);
  G4LogicalVolume *tubsLogical = new G4LogicalVolume(tubsScatterer,Fe,"TubsLogical");
  G4VPhysicalVolume *tubsPhysical = new G4PVPlacement(0,
  	                               G4ThreeVector(20.*cm,-15.*cm,25.*cm),
								   tubsLogical,
  	                               "TubsPhysical",
  	                               logicWorld,
  	                                false,
  	                                0,
  	                                checkOverlaps);

*/



/*
G4VPhysicalVolume *phySubTargetBlockU = new G4PVPlacement(0,
                               G4ThreeVector(0*cm,0*cm,0*cm),//-39*cm),
								fLogicSubTargetU,
                                  "TargetSubPhysical_U",
                                  fLogicTargetAl,

                                  false,
                                  0,
                                  checkOverlaps);


  G4VPhysicalVolume *phySubTargetBlockU2 = new G4PVPlacement(0,
                                   G4ThreeVector(0*cm,0*cm,0*cm),//-39*cm),
  								fLogicSubTargetU,
                                    "TargetSubPhysical_U",
                                    //world->GetLogicalVolume(),//logicWorld,
  								  fLogicTarget,

                                    false,
                                    0,
                                    checkOverlaps);
*/
#endif

#undef VOXELIZE

#ifdef VOXELIZE
   //Tomography::evolution::Voxelator *v =

   Tomography::evolution::Voxelator::Create(50*cm,50*cm,43*cm,10*cm,10*cm,8.6*cm);
   Tomography::evolution::Voxelator *v = Tomography::evolution::Voxelator::instance();
   v->CalculateVoxelCenters();
   std::vector<Tracking::Vector3D<double>> voxelCenters = v->GetVoxelCenters();
   Tracking::Vector3D<double> voxDim = v->GetEachVoxelDim();
   G4Box *voxel = new G4Box("Voxel",voxDim.x()/2.,voxDim.y()/2.,voxDim.z()/2.);
   G4LogicalVolume *logicalVoxel = new G4LogicalVolume(voxel,galactic,"LogicalVoxel");
   for(int i = 0 ; i< voxelCenters.size() ; i++){
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
  parser.Write("Hodoscope.gdml",physWorld);
	return physWorld;
}
/*
** Helper functions for Geometry creation will be taken from DetectorHelpers
*/
