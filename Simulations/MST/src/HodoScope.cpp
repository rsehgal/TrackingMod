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

HodoScope::HodoScope(){

}

HodoScope::~HodoScope(){}

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
/*


  G4Box* hodoscope =
    new G4Box("hodoscope",                       //its name
       0.5*world_sizeXYZ, 0.5*world_sizeXYZ, 0.5*world_sizeXYZ);     //its size

  G4LogicalVolume* logicHodoscope =
    new G4LogicalVolume(hodoscope,          //its solid
                        world_mat,           //its material
                        "HodoScope");            //its name

  G4VPhysicalVolume* physHodoscope =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicHodoscope,            //its logical volume
                      "PHYHodoscope",               //its name
                      logicWorld,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking


*/

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
  //RPC setup of 1mx1m
            /*
  G4Box *rpc = new G4Box("RPC",0.25*world_sizeXYZ, 0.25*world_sizeXYZ, 2.*cm);
  G4LogicalVolume *logicalRpc = new G4LogicalVolume(rpc,orb_material,"LogicalRpc");
  //G4LogicalVolume *logicalRpc = new G4LogicalVolume(rpc,C2H2F4,"LogicalRpc");
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
*/


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
  ele_enrichedU-> AddIsotope (iso_U235, abundance=20.*perCent);
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
