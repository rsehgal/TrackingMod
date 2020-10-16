
#ifndef B1RunAction_h
#define B1RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"


//#include <fstream>
using namespace std;
class G4Run;
class G4LogicalVolume;
class TH1F;
class TApplication;
class DataTree;
class Calibration;


/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume 
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class B1RunAction : public G4UserRunAction
{

  public:
   //static ofstream *myfile;
    //static ofstream GetEnergyFile(){return myfile;}

	static Calibration *fCalib;

    B1RunAction();
    virtual ~B1RunAction();

    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
    void WriteData();

   private:
   	TApplication *fApp;
   	TH1F *energyHist;

    DataTree *fDataTree;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


