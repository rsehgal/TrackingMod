
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
//class DataTree;
#include "DataTree_V2.h"
//class Calibration;
#include "Calibration.h"
#include <TTree.h>
#include "ExactHitDataTree.h"


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

	static lite_interface::Calibration *fCalib;

    B1RunAction();
    virtual ~B1RunAction();

    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
    void WriteData();

   private:
   	TApplication *fApp;
   	TH1F *energyHist;

    lite_interface::DataTree *fDataTree;



   public:
    static std::vector<double> fAngleVec;
    static unsigned long fEvNo;

    static lite_interface::ExactHitDataTree* fExactHitDataTree;

   /* static TTree* fExactHitTree;
    static std::vector<double> xVec;
    static std::vector<double> yVec;
    static std::vector<double> zVec;*/

    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


