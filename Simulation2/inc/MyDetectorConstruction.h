#ifndef MyDetectorConstruction_h
#define MyDetectorConstruction_h 

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VUserDetectorConstruction;
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
static int stripNum;
static int blockNum;
static int eBlockNum;
public:
	MyDetectorConstruction();
	~MyDetectorConstruction();
	G4LogicalVolume* GetBlock(std::string name,double halfX,double halfY, double halfZ, G4Material *mat,int numOfStrips,double stripHalfZ,int dir);
	G4LogicalVolume* GetStrip(std::string name,double halfX,double halfY, double halfZ, G4Material *mat);
	G4LogicalVolume* CreateEBlock(G4LogicalVolume* E1, G4LogicalVolume* E2, double halfZ, G4Material *mat);
	G4VPhysicalVolume* Construct();
};
#endif
