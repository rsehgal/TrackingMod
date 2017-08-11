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
#include "Voxelator.h"
//#include "base/Global.h"
#define PI 3.14159265359

HodoScope2::HodoScope2(){
double density = 2.700*g/cm3;
double a = 26.98*g/mole;
double z;
char *name="";
fMessenger = new B2aDetectorMessenger(this);
fTargetMaterial = new G4Material(name="Aluminum", z=13., a, density);
fTargetThickness = 0.5*cm;
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
  Block *world = new Block("World",0.75*world_sizeXYZ,"G4_Galactic");
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
  Block *scintPlane = new Block("TriggeringPlane",0.60*world_sizeXYZ, 0.60*world_sizeXYZ, 1.*cm,"G4_Galactic");
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
 Block *shelf = new Block("Shelf",0.250*world_sizeXYZ, 0.250*world_sizeXYZ, 0.25*mm,"G4_Galactic");
  double bottomShelfZ = -75*cm;
  for(int i = -3 ; i < 3; i++ ){

	  if(i==-1 || i==0 /*|| i==2*/) continue;
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



  G4Material *Pb=nist->FindOrBuildMaterial("G4_Pb");
    G4Material *Fe=nist->FindOrBuildMaterial("G4_Fe");
    G4Material *Al=nist->FindOrBuildMaterial("G4_Al");
    G4Material *U=nist->FindOrBuildMaterial("G4_U");


    G4Box *targetKnown = new G4Box("Target",50*cm,50*cm,5*cm);
    G4LogicalVolume *fLogicTargetKnown = new G4LogicalVolume(targetKnown,Fe,"LogicalKnownBlock");
    G4VPhysicalVolume *phyKnownTargetBlock = new G4PVPlacement(0,
                                G4ThreeVector(),
                                //G4ThreeVector(15*cm,-15*cm,12*cm),
    		  	  	  	  	    //G4ThreeVector(15*cm,-15*cm,0*cm),//-39*cm),
                               fLogicTargetKnown,
                                "TargetPhysical_Known",
                                world->GetLogicalVolume(),//logicWorld,
                                false,
                                0,
                                checkOverlaps);



//Generalized target which can be changed from UI, default material is set to Aluminium
  //target = new G4Box("Target",0.250*world_sizeXYZ, 0.250*world_sizeXYZ,fTargetThickness);
#if(0)
  G4NistManager* nist = G4NistManager::Instance();
  target = new G4Box("Target",5*cm,5*cm,8*cm);

  fLogicTarget = new G4LogicalVolume(target,Pb,"LogicalTargetBlock");
  G4LogicalVolume *fLogicTargetFe = new G4LogicalVolume(target,Pb,"LogicalTargetFeBlock");

  G4Box *targetAl = new G4Box("Target",5*cm,5*cm,18*cm);
  G4Box *subtargetU = new G4Box("Target",3*cm,3*cm,7*cm);
  G4LogicalVolume *fLogicTargetAl = new G4LogicalVolume(targetAl,Al,"LogicalTargetAlBlock");
  G4LogicalVolume *fLogicTargetU = new G4LogicalVolume(target,U,"LogicalTargetUBlock");
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

  G4VPhysicalVolume *phyTargetBlock = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            //G4ThreeVector(15*cm,-15*cm,12*cm),
		  	  	  	  	    G4ThreeVector(15*cm,-15*cm,0*cm),//-39*cm),
                           fLogicTarget,
                            "TargetPhysical_Pb",
                            world->GetLogicalVolume(),//logicWorld,
                            false,
                            0,
                            checkOverlaps);

  G4VPhysicalVolume *phyTargetBlock2 = new G4PVPlacement(0,
                              //G4ThreeVector(),
                              //G4ThreeVector(-15*cm,15*cm,12*cm),
		  	  	  	  	  	  G4ThreeVector(-15*cm,15*cm,0*cm),//-39*cm),
                             fLogicTargetU,
                              "TargetPhysical_U",
                              world->GetLogicalVolume(),//logicWorld,
                              false,
                              0,
                              checkOverlaps);

  G4VPhysicalVolume *phyTargetBlock3 = new G4PVPlacement(0,
                                //G4ThreeVector(),
                                //G4ThreeVector(-15*cm,-15*cm,12*cm),
		  	  	  	  	  	    G4ThreeVector(-15*cm,-15*cm,0*cm),//-39*cm),
                               fLogicTargetAl,
                                "TargetPhysical_Al",
                                world->GetLogicalVolume(),//logicWorld,
                                false,
                                0,
                                checkOverlaps);

  G4VPhysicalVolume *phyTargetBlock4 = new G4PVPlacement(0,
                                  //G4ThreeVector(),
                                  //G4ThreeVector(25*cm,25*cm,12*cm),
		  	  	  	  	  	  	  G4ThreeVector(25*cm,25*cm,0*cm),//-39*cm),
                                 fLogicTargetFe,
                                  "TargetPhysical_Fe",
                                  world->GetLogicalVolume(),//logicWorld,
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


  G4VPhysicalVolume *phySubTargetBlockU2 = new G4PVPlacement(0,
                                    //G4ThreeVector(),
                                    //G4ThreeVector(-15*cm,-15*cm,12*cm),
    		  	  	  	  	  	    G4ThreeVector(0*cm,0*cm,0*cm),//-39*cm),
  								fLogicSubTargetU,
                                    "TargetSubPhysical_U",
                                    //world->GetLogicalVolume(),//logicWorld,
  								  fLogicTarget,

                                    false,
                                    0,
                                    checkOverlaps);
#endif


	//fScoringVolume = logicalLeadBlock;
   fScoringVolume = shelf->GetLogicalVolume();//logicalShelf;


#ifdef VOXELIZE
   //Trying to use Voxelator to visualize the VoxelizedVolume
   Tomography::Voxelator v;
   v.SetVoxelator(50*cm,50*cm,40*cm,20*cm,20*cm,16*cm);
   v.CalculateVoxelCenters();
   std::vector<Tracking::Vector3D<double>> voxelCenters = v.GetVoxelCenters();
   Tracking::Vector3D<int> voxDim = v.GetEachVoxelDim();
   Block *voxel = new Block("Voxel",voxDim.x()/2.,voxDim.y()/2.,voxDim.z()/2.,"G4_Galactic");
   //int i = 5;
   for(int i = 0 ; i< voxelCenters.size() ; i++){
	//  voxelCenters[i].Print();
   G4VPhysicalVolume *voxPhy = new G4PVPlacement(0,
   	                               //G4ThreeVector(),
   	                               G4ThreeVector(voxelCenters[i].x(),voxelCenters[i].y(),voxelCenters[i].z()),
   	                               voxel->GetLogicalVolume(),
   	                               "VoxelPhysical",
   	                               world->GetLogicalVolume(),//logicWorld,
   	                               false,
   	                               0,
   	                               checkOverlaps);
   }
#endif

   /*for(int i = 0 ; i < voxelCenters.size() ; i++){
	   G4VPhysicalVolume *voxPhy = new G4PVPlacement(0,
	                               //G4ThreeVector(),
	                               G4ThreeVector(voxelCenters[i].x()*cm,voxelCenters[i].y()*cm,voxelCenters[i].z()*cm),
	                               voxel->GetLogicalVolume(),
	                               "PhysicalWorld",
	                               world->GetLogicalVolume(),//logicWorld,
	                               false,
	                               0,
	                               checkOverlaps);

   }
*/

G4GDMLParser parser;
parser.Write("Hodoscope.gdml", physWorld);

//WriteVoxelizedVolume(0.250*world_sizeXYZ, 0.250*world_sizeXYZ,45*cm);

	return physWorld;

}


void HodoScope2::WriteVoxelizedVolume(double halfXWidth, double halfYWidth, double halfZWidth, double posX ,double posY, double posZ){


	bool checkOverlaps = true;


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


	G4NistManager* nist = G4NistManager::Instance();
    G4Material *Galactic=nist->FindOrBuildMaterial("G4_Galactic");
	G4Box *voxelizedVolume = new G4Box("Target",halfXWidth,halfYWidth,halfZWidth);
	G4LogicalVolume *voxelizedVolumeLogical = new G4LogicalVolume(voxelizedVolume,Galactic,"voxelizedVolumeLogical");
	G4VPhysicalVolume *voxelizedVolumePhy = new G4PVPlacement(0,
	                            //G4ThreeVector(),
	                            G4ThreeVector(posX,posY,posZ),
	                            voxelizedVolumeLogical,
	                            "voxelizedVolumePhysical",
	                            world->GetLogicalVolume(),//logicWorld,
								//0,
	                            false,
	                            0,
	                            checkOverlaps);



	G4GDMLParser parser;
	//parser.Write("VoxelizedVolume.gdml", voxelizedVolumePhy );
	parser.Write("VoxelizedVolume.gdml", physWorld );
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
