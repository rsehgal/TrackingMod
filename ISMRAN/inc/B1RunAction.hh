
#ifndef B1RunAction_h
#define B1RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
//#include <fstream>
#include <map>

using namespace std;
class G4Run;
class G4LogicalVolume;
class TH1F;
class TH2F;
class TFile;
class TGraph;


class B1RunAction : public G4UserRunAction
{
  public:
   static double fScatteringAngle;
   TH1F *fScatteringHist;
   TFile *fRootFile;
   int counter;
   TH2F *fPocaXYHist;
   //TGraph *fShape;


    B1RunAction();
    virtual ~B1RunAction();

    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

