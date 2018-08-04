#ifndef DetectorHelpers_h
#define DetectorHelpers_h 

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;

class DetectorHelpers 
{
static int stripNum;
static int blockNum;
static int eBlockNum;
public:
	DetectorHelpers();
	~DetectorHelpers();
	G4LogicalVolume* GetBlock(std::string name,double halfX,double halfY, double halfZ, G4Material *mat,
        int numOfStrips,double stripHalfZ,int dir,int start = 32,std::string defaultName = "_LE_CH");
    //G4LogicalVolume* GetBlock(std::string name,double halfX,double halfY, double halfZ, G4Material *mat,int numOfStrips,double stripHalfZ,int dir,std::string defaultName = "_LE_CH");
	G4LogicalVolume* GetBlock(std::string name,double halfX,double halfY, double halfZ, G4Material *mat, G4LogicalVolume* logVol,int numOfLogVol,int dir);
	G4LogicalVolume* GetStrip(std::string name,double halfX,double halfY, double halfZ, G4Material *mat);
	G4LogicalVolume* CreateEBlock(G4LogicalVolume* E1, G4LogicalVolume* E2, double halfZ, G4Material *mat);
	//G4VPhysicalVolume* Construct();
};
#endif
