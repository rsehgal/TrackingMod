#include "HodoScope.h"
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

HodoScope::HodoScope(){
double density = 2.700*g/cm3;
double a = 26.98*g/mole;
double z;
char *name="";
fMessenger = new B2aDetectorMessenger(this);
fTargetMaterial = new G4Material(name="Aluminum", z=13., a, density);
}

HodoScope::~HodoScope(){
delete fMessenger;
}

// This Construct function creates 2 Scintillator Planes, 6 shelves with 1 mm Al
// and one targets of 10cm X 10cm X 10cm. 
// this is written with a view to run in batch mode, where the target material 
// and gun energy can be changed via macro file.
// So that we should be able to take different runs with different target material
// and different energy beams.
// Each run should create its histogram or any other required plot
G4VPhysicalVolume* HodoScope::Construct(){
  
  G4NistManager* nist = G4NistManager::Instance();
  G4bool checkOverlaps = true;

  G4double world_sizeXYZ = 200*cm;
//  G4double world_sizeXY = 1.2*env_sizeXY;
//  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.75*world_sizeXYZ, 0.75*world_sizeXYZ, 0.75*world_sizeXYZ);     //its size
      
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



  G4double a; // atomic mass
    G4double z; // atomic number
    G4double density;



  //Trying to create gas R134a (C2H2F4)
    // define Elements
    char *name="";
    char *symbol="";
    int natoms=0;
    int ncomponents;

    density = 2.700*g/cm3;
          a = 26.98*g/mole;
          G4Material* Al = new G4Material(name="Aluminum", z=13., a, density);

    a = 1.01*g/mole;
    G4Element* elH  = new G4Element(name="Hydrogen",symbol="H" , z= 1., a);

    a = 12.01*g/mole;
    G4Element* elC  = new G4Element(name="Carbon"  ,symbol="C" , z= 6., a);

    a = 19.00*g/mole;
    G4Element* elF  = new G4Element(name="Fluorine"  ,symbol="F" , z= 9., a);

    density = 8.280*g/cm3;
    G4Material* C2H2F4= new G4Material(name="C2H2F4", density, ncomponents=3);
    C2H2F4->AddElement(elC , natoms=2);
    C2H2F4->AddElement(elH , natoms=2);
    C2H2F4->AddElement(elF, natoms=4);


    //Top and bottom Scintillator plane
    G4Box *scintPlane = new G4Box("ScintPlane",0.60*world_sizeXYZ, 0.60*world_sizeXYZ, 2.*cm);
      G4LogicalVolume *logicalScintPlane = new G4LogicalVolume(scintPlane,orb_material,"LogicalScintPlane");
      G4VPhysicalVolume *phyTopScintPlane = new G4PVPlacement(0,
                                //G4ThreeVector(),
                                G4ThreeVector(0.,0.,105.*cm),
                                logicalScintPlane,
                                "PhysicalTopPlane",
                                logicWorld,
                                false,
                                0,
                                checkOverlaps);

      G4VPhysicalVolume *phyBottomScintPlane = new G4PVPlacement(0,
                                      //G4ThreeVector(),
                                      G4ThreeVector(0.,0.,-105.*cm),
                                      logicalScintPlane,
                                      "PhysicalBottomPlane",
                                      logicWorld,
                                      false,
                                      0,
                                      checkOverlaps);

      G4Box *shelf = new G4Box("Shelf",0.50*world_sizeXYZ, 0.50*world_sizeXYZ, 0.5*mm);
      G4LogicalVolume *logicalShelf = new G4LogicalVolume(shelf,Al,"LogicalShelf");
      //Now physically placing Six such shelves for Six Rpc's
      G4VPhysicalVolume *phyFirstShelf = new G4PVPlacement(0,
                                      //G4ThreeVector(),
                                      G4ThreeVector(0.,0.,75.*cm),
                                      logicalShelf,
                                      "PhysicalFirstShelf",
                                      logicWorld,
                                      false,
                                      0,
                                      checkOverlaps);
      G4VPhysicalVolume *phySecondShelf = new G4PVPlacement(0,
                                            //G4ThreeVector(),
                                            G4ThreeVector(0.,0.,45.*cm),
                                            logicalShelf,
                                            "PhysicalSecondShelf",
                                            logicWorld,
                                            false,
                                            0,
                                            checkOverlaps);

      G4VPhysicalVolume *phyThirdShelf = new G4PVPlacement(0,
                                            //G4ThreeVector(),
                                            G4ThreeVector(0.,0.,15.*cm),
                                            logicalShelf,
                                            "PhysicalThirdShelf",
                                            logicWorld,
                                            false,
                                            0,
                                            checkOverlaps);

      G4VPhysicalVolume *phyFourthShelf = new G4PVPlacement(0,
                                            //G4ThreeVector(),
                                            G4ThreeVector(0.,0.,-15.*cm),
                                            logicalShelf,
                                            "PhysicalFourthShelf",
                                            logicWorld,
                                            false,
                                            0,
                                            checkOverlaps);
            G4VPhysicalVolume *phyFifthShelf = new G4PVPlacement(0,
                                                  //G4ThreeVector(),
                                                  G4ThreeVector(0.,0.,-45.*cm),
                                                  logicalShelf,
                                                  "PhysicalFifthShelf",
                                                  logicWorld,
                                                  false,
                                                  0,
                                                  checkOverlaps);

            G4VPhysicalVolume *phySixthShelf = new G4PVPlacement(0,
                                                  //G4ThreeVector(),
                                                  G4ThreeVector(0.,0.,-75.*cm),
                                                  logicalShelf,
                                                  "PhysicalSixthShelf",
                                                  logicWorld,
                                                  false,
                                                  0,
                                                  checkOverlaps);
  

/*
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

  G4Box *alBlock = new G4Box("AlBlock",5.*cm,5.*cm,5.*cm);
    //G4LogicalVolume *logicalLeadBlock = new G4LogicalVolume(leadBlock,nist->FindOrBuildMaterial("G4_Pb"),"LogicalLeadBlock");
    G4LogicalVolume *logicalAlBlock = new G4LogicalVolume(alBlock,Al,"LogicalLeadBlock");

  G4VPhysicalVolume *phyAlBlock = new G4PVPlacement(0,
                              //G4ThreeVector(),
                              G4ThreeVector(50.*cm,0.,0.),
                              logicalAlBlock,
                              "PhysicalAlBlock",
                              logicWorld,
                              false,
                              0,
                              checkOverlaps);

  //Trying to create enriched uranium
  double fractionmass;
  double iz,ia;
  double abundance;
  G4Isotope* iso_U235 = new  G4Isotope ("U235",iz=92, ia=235, a=235.0439242*g/mole);
  G4Isotope* iso_U238 = new  G4Isotope ("U238", iz=92, ia=238, a=238.0507847 *g/mole);
  G4Element* ele_enrichedU = new  G4Element("enriched U", symbol="U" , ncomponents=2);
  ele_enrichedU-> AddIsotope (iso_U235, abundance=80.*perCent);
  ele_enrichedU-> AddIsotope (iso_U238, abundance=20.*perCent);
  G4Material* mat_enrichedU =   new G4Material ("U for nuclear  power generation" , density= 19.050*g/cm3 ,  ncomponents = 1 , kStateSolid );
  mat_enrichedU-> AddElement( ele_enrichedU , fractionmass = 1 );

  G4Box *urBlock = new G4Box("UrBlock",5.*cm,5.*cm,5.*cm);
      //G4LogicalVolume *logicalLeadBlock = new G4LogicalVolume(leadBlock,nist->FindOrBuildMaterial("G4_Pb"),"LogicalLeadBlock");
      G4LogicalVolume *logicalUrBlock = new G4LogicalVolume(urBlock,mat_enrichedU,"LogicalLeadBlock");

    G4VPhysicalVolume *phyUrBlock = new G4PVPlacement(0,
                                //G4ThreeVector(),
                                G4ThreeVector(-50.*cm,0.,0.),
                                logicalUrBlock,
                                "PhysicalAlBlock",
                                logicWorld,
                                false,
                                0,
                                checkOverlaps);

*/

//Generalized target which can be changed from UI, default material is set to Aluminium
/*
G4Box *target = new G4Box("Target",5.*cm,5.*cm,5.*cm);
  //G4LogicalVolume *logicalLeadBlock = new G4LogicalVolume(leadBlock,nist->FindOrBuildMaterial("G4_Pb"),"LogicalLeadBlock");
  fLogicTarget = new G4LogicalVolume(target,fTargetMaterial,"LogicalTargetBlock");
  G4VPhysicalVolume *phyTargetBlock = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(),
                            fLogicTarget,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
*/
/*
//Alphabet 'P'
G4Material* matPb = nist->FindOrBuildMaterial("G4_Pb");
G4Material* matU = nist->FindOrBuildMaterial("G4_U");
G4Box *target1 = new G4Box("Target1",5.*cm,2.*cm,5.*cm);
G4LogicalVolume* fLogicTarget1 = new G4LogicalVolume(target1,matPb,"LogicalTargetPbBlock1");
G4Box *target2 = new G4Box("Target2",2.*cm,5.*cm,5.*cm);
G4LogicalVolume* fLogicTarget2 = new G4LogicalVolume(target2,matU,"LogicalTargetPbBlock2");
G4VPhysicalVolume *phyTarget11 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(5.*cm,10*cm,0.),
                            fLogicTarget1,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
G4VPhysicalVolume *phyTarget12 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(5.*cm,0*cm,0.),
                            fLogicTarget1,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
G4VPhysicalVolume *phyTarget21 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(10.*cm,2.5*cm,0.),
                            fLogicTarget2,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);

G4Box *target3 = new G4Box("Target2",2.*cm,10.*cm,5.*cm);
G4LogicalVolume* fLogicTarget3 = new G4LogicalVolume(target3,matPb,"LogicalTargetPbBlock3");
G4VPhysicalVolume *phyTarget22 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(0.*cm,0.,0.),
                            fLogicTarget3,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);

//Alphabet 'C'
G4VPhysicalVolume *phyTarget23 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(20.*cm,0.,0.),
                            fLogicTarget3,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
G4VPhysicalVolume *phyTarget111 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(25.*cm,10*cm,0.),
                            fLogicTarget1,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
G4VPhysicalVolume *phyTarget112 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(25.*cm,-10*cm,0.),
                            fLogicTarget1,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
*/
/*
G4Box *target4 = new G4Box("Target4",3.*cm,2.*cm,5.*cm);
G4LogicalVolume* fLogicTarget4 = new G4LogicalVolume(target4,matPb,"LogicalTargetPbBlock4");
G4VPhysicalVolume *phyTarget31 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(-8.*cm,0.,0.),
                            fLogicTarget4,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
G4VPhysicalVolume *phyTarget32 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(-6.*cm,-2*cm,0.),
                            fLogicTarget4,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
G4VPhysicalVolume *phyTarget33 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(-4.*cm,-4.*cm,0.),
                            fLogicTarget4,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
G4VPhysicalVolume *phyTarget34 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(-2.*cm,-6.*cm,0.),
                            fLogicTarget4,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
G4VPhysicalVolume *phyTarget35 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(0.*cm,-8.*cm,0.),
                            fLogicTarget4,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
*/

//Trying to create P
/*
G4VPhysicalVolume *phyTargetP1 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(22.*cm,0.,0.),
                            fLogicTarget3,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
G4VPhysicalVolume *phyTargetP2 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(22.*cm,10*cm,0.),
                            fLogicTarget1,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
G4VPhysicalVolume *phyTargetP3 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(22.*cm,0*cm,0.),
                            fLogicTarget1,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
G4VPhysicalVolume *phyTargetP4 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(22.*cm,0*cm,0.),
                            fLogicTarget2,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
*/
/*
G4VPhysicalVolume *phyTargetP1 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(10.*cm,5*cm,0.),
                            fLogicTarget1,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
G4VPhysicalVolume *phyTargetP2 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(10.*cm,0*cm,0.),
                            fLogicTarget1,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
G4VPhysicalVolume *phyTargetP3 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(15.*cm,2.5*cm,0.),
                            fLogicTarget2,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
G4VPhysicalVolume *phyTargetP4 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(5.*cm,0.,0.),
                            fLogicTarget3,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
                            */

//Now trying to create C
/*
G4VPhysicalVolume *phyTargetC1 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(42.*cm,10*cm,0.),
                            fLogicTarget1,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
G4VPhysicalVolume *phyTargetC2 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(42.*cm,0*cm,0.),
                            fLogicTarget2,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
G4VPhysicalVolume *phyTargetC3 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(42.*cm,0*cm,0.),
                            fLogicTarget1,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);
                            */

BallsInABox *scatterer = new BallsInABox();
G4VPhysicalVolume *phyTargetPbBlock = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(),
                            scatterer->GetTarget(),
                            "Scatterer",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);

/*
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

G4Box *targetPb = new G4Box("TargetPb",10.*cm,10.*cm,10.*cm);
  //G4LogicalVolume *logicalLeadBlock = new G4LogicalVolume(leadBlock,nist->FindOrBuildMaterial("G4_Pb"),"LogicalLeadBlock");
  G4LogicalVolume* fLogicTargetPb = new G4LogicalVolume(targetPb,nist->FindOrBuildMaterial("G4_Al"),"LogicalTargetPbBlock");
  G4VPhysicalVolume *phyTargetPbBlock = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(),
                            fLogicTargetPb,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);


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
//G4Box *targetU = new G4Box("TargetU",5.*cm,5.*cm,5.*cm);
G4Sphere *targetU = new G4Sphere("TargetU",0., 2.*cm, 0., 2*PI, 0. , PI);
  //G4LogicalVolume *logicalLeadBlock = new G4LogicalVolume(leadBlock,nist->FindOrBuildMaterial("G4_Pb"),"LogicalLeadBlock");
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

//++++++++++++++++++++++++++++++++++++++++++++++++++
*/


/*

    G4Material* matAm = nist->FindOrBuildMaterial("G4_Am");
//G4Box *targetU = new G4Box("TargetU",5.*cm,5.*cm,5.*cm);
G4Sphere *targetAm = new G4Sphere("TargetAm",0., 2.*cm, 0., 2*PI, 0. , PI);
  //G4LogicalVolume *logicalLeadBlock = new G4LogicalVolume(leadBlock,nist->FindOrBuildMaterial("G4_Pb"),"LogicalLeadBlock");
 G4LogicalVolume* fLogicTargetAm = new G4LogicalVolume(targetAm,matAm,"LogicalTargetAmBlock");
  G4VPhysicalVolume *phyTargetAmBlock = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(-50.*cm,0.,-10.*cm),
                            fLogicTargetAm,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);

    G4VPhysicalVolume *phyTargetAmBlock2 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(-20.*cm,0.,-10.*cm),
                            fLogicTargetAm,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);

      G4VPhysicalVolume *phyTargetAmBlock3 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(40.*cm,0.,+10.*cm),
                            fLogicTargetAm,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);

        G4VPhysicalVolume *phyTargetAmBlock4 = new G4PVPlacement(0,
                            //G4ThreeVector(),
                            G4ThreeVector(-10.*cm,40.,20.*cm),
                            fLogicTargetAm,
                            "PhysicalWorld",
                            logicWorld,
                            false,
                            0,
                            checkOverlaps);

     */
  //Considering Square RPCs


	//fScoringVolume = logicalLeadBlock;
   fScoringVolume = logicalShelf;

G4GDMLParser parser;
parser.Write("Hodoscope.gdml", physWorld);

	return physWorld;

}


/* 
//This Construct function creates 2 Scintillator Planes, 6 shelves with 1 mm Al
//and three targets of 10cm X 10cm X 10cm
G4VPhysicalVolume* HodoScope::Construct(){
  
  G4NistManager* nist = G4NistManager::Instance();
  G4bool checkOverlaps = true;

  G4double world_sizeXYZ = 200*cm;
//  G4double world_sizeXY = 1.2*env_sizeXY;
//  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.75*world_sizeXYZ, 0.75*world_sizeXYZ, 0.75*world_sizeXYZ);     //its size
      
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



  G4double a; // atomic mass
    G4double z; // atomic number
    G4double density;



  //Trying to create gas R134a (C2H2F4)
    // define Elements
    char *name="";
    char *symbol="";
    int natoms=0;
    int ncomponents;

    density = 2.700*g/cm3;
          a = 26.98*g/mole;
          G4Material* Al = new G4Material(name="Aluminum", z=13., a, density);

    a = 1.01*g/mole;
    G4Element* elH  = new G4Element(name="Hydrogen",symbol="H" , z= 1., a);

    a = 12.01*g/mole;
    G4Element* elC  = new G4Element(name="Carbon"  ,symbol="C" , z= 6., a);

    a = 19.00*g/mole;
    G4Element* elF  = new G4Element(name="Fluorine"  ,symbol="F" , z= 9., a);

    density = 8.280*g/cm3;
    G4Material* C2H2F4= new G4Material(name="C2H2F4", density, ncomponents=3);
    C2H2F4->AddElement(elC , natoms=2);
    C2H2F4->AddElement(elH , natoms=2);
    C2H2F4->AddElement(elF, natoms=4);


    //Top and bottom Scintillator plane
    G4Box *scintPlane = new G4Box("ScintPlane",0.40*world_sizeXYZ, 0.40*world_sizeXYZ, 2.*cm);
      G4LogicalVolume *logicalScintPlane = new G4LogicalVolume(scintPlane,orb_material,"LogicalScintPlane");
      G4VPhysicalVolume *phyTopScintPlane = new G4PVPlacement(0,
                                //G4ThreeVector(),
                                G4ThreeVector(0.,0.,105.*cm),
                                logicalScintPlane,
                                "PhysicalTopPlane",
                                logicWorld,
                                false,
                                0,
                                checkOverlaps);

      G4VPhysicalVolume *phyBottomScintPlane = new G4PVPlacement(0,
                                      //G4ThreeVector(),
                                      G4ThreeVector(0.,0.,-105.*cm),
                                      logicalScintPlane,
                                      "PhysicalBottomPlane",
                                      logicWorld,
                                      false,
                                      0,
                                      checkOverlaps);

      G4Box *shelf = new G4Box("Shelf",0.40*world_sizeXYZ, 0.40*world_sizeXYZ, 0.5*mm);
      G4LogicalVolume *logicalShelf = new G4LogicalVolume(shelf,Al,"LogicalShelf");
      //Now physically placing Six such shelves for Six Rpc's
      G4VPhysicalVolume *phyFirstShelf = new G4PVPlacement(0,
                                      //G4ThreeVector(),
                                      G4ThreeVector(0.,0.,75.*cm),
                                      logicalShelf,
                                      "PhysicalFirstShelf",
                                      logicWorld,
                                      false,
                                      0,
                                      checkOverlaps);
      G4VPhysicalVolume *phySecondShelf = new G4PVPlacement(0,
                                            //G4ThreeVector(),
                                            G4ThreeVector(0.,0.,45.*cm),
                                            logicalShelf,
                                            "PhysicalSecondShelf",
                                            logicWorld,
                                            false,
                                            0,
                                            checkOverlaps);

      G4VPhysicalVolume *phyThirdShelf = new G4PVPlacement(0,
                                            //G4ThreeVector(),
                                            G4ThreeVector(0.,0.,15.*cm),
                                            logicalShelf,
                                            "PhysicalThirdShelf",
                                            logicWorld,
                                            false,
                                            0,
                                            checkOverlaps);

      G4VPhysicalVolume *phyFourthShelf = new G4PVPlacement(0,
                                            //G4ThreeVector(),
                                            G4ThreeVector(0.,0.,-15.*cm),
                                            logicalShelf,
                                            "PhysicalFourthShelf",
                                            logicWorld,
                                            false,
                                            0,
                                            checkOverlaps);
            G4VPhysicalVolume *phyFifthShelf = new G4PVPlacement(0,
                                                  //G4ThreeVector(),
                                                  G4ThreeVector(0.,0.,-45.*cm),
                                                  logicalShelf,
                                                  "PhysicalFifthShelf",
                                                  logicWorld,
                                                  false,
                                                  0,
                                                  checkOverlaps);

            G4VPhysicalVolume *phySixthShelf = new G4PVPlacement(0,
                                                  //G4ThreeVector(),
                                                  G4ThreeVector(0.,0.,-75.*cm),
                                                  logicalShelf,
                                                  "PhysicalSixthShelf",
                                                  logicWorld,
                                                  false,
                                                  0,
                                                  checkOverlaps);

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

  G4Box *alBlock = new G4Box("AlBlock",5.*cm,5.*cm,5.*cm);
    //G4LogicalVolume *logicalLeadBlock = new G4LogicalVolume(leadBlock,nist->FindOrBuildMaterial("G4_Pb"),"LogicalLeadBlock");
    G4LogicalVolume *logicalAlBlock = new G4LogicalVolume(alBlock,Al,"LogicalLeadBlock");

  G4VPhysicalVolume *phyAlBlock = new G4PVPlacement(0,
                              //G4ThreeVector(),
                              G4ThreeVector(50.*cm,0.,0.),
                              logicalAlBlock,
                              "PhysicalAlBlock",
                              logicWorld,
                              false,
                              0,
                              checkOverlaps);

  //Trying to create enriched uranium
  double fractionmass;
  double iz,ia;
  double abundance;
  G4Isotope* iso_U235 = new  G4Isotope ("U235",iz=92, ia=235, a=235.0439242*g/mole);
  G4Isotope* iso_U238 = new  G4Isotope ("U238", iz=92, ia=238, a=238.0507847 *g/mole);
  G4Element* ele_enrichedU = new  G4Element("enriched U", symbol="U" , ncomponents=2);
  ele_enrichedU-> AddIsotope (iso_U235, abundance=80.*perCent);
  ele_enrichedU-> AddIsotope (iso_U238, abundance=20.*perCent);
  G4Material* mat_enrichedU =   new G4Material ("U for nuclear  power generation" , density= 19.050*g/cm3 ,  ncomponents = 1 , kStateSolid );
  mat_enrichedU-> AddElement( ele_enrichedU , fractionmass = 1 );

  G4Box *urBlock = new G4Box("UrBlock",5.*cm,5.*cm,5.*cm);
      //G4LogicalVolume *logicalLeadBlock = new G4LogicalVolume(leadBlock,nist->FindOrBuildMaterial("G4_Pb"),"LogicalLeadBlock");
      G4LogicalVolume *logicalUrBlock = new G4LogicalVolume(urBlock,mat_enrichedU,"LogicalLeadBlock");

    G4VPhysicalVolume *phyUrBlock = new G4PVPlacement(0,
                                //G4ThreeVector(),
                                G4ThreeVector(-50.*cm,0.,0.),
                                logicalUrBlock,
                                "PhysicalAlBlock",
                                logicWorld,
                                false,
                                0,
                                checkOverlaps);




  //fScoringVolume = logicalLeadBlock;
   fScoringVolume = logicalShelf;

  return physWorld;

}
*/

void HodoScope::SetTargetMaterial(G4String materialName)
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
     } else {
        G4cout
          << G4endl
          << "-->  WARNING from SetTargetMaterial : "
          << materialName << " not found" << G4endl;
     }
  }
}
