
#include "B1RunAction.hh"
#include "MyPrimaryGeneratorAction.h"
#include "MyDetectorConstruction.h"
#include "B1Run.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include <G4FastStep.hh>
#include <string.h>
#include "B1EventAction.hh"
#include <vector>

//ROOT specific header files
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TFile.h>

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

double B1RunAction::fScatteringAngle = 0;

#ifdef STORE
//TTree* B1RunAction::fTree=NULL;
#endif

B1RunAction::B1RunAction()
: G4UserRunAction()
{ 
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::~B1RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* B1RunAction::GenerateRun()
{
  return new B1Run; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::BeginOfRunAction(const G4Run*)
{
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

fScatteringHist = new TH1F("ScatteringHistogram","ScatteingHistogram",100,-250,250);
fRootFile = new TFile("data.root","RECREATE","SNP 2019 Geant4 Simulation");
fPocaXYHist = new TH2F("Hist of PoCA XY","Hist of PoCA XY",1000,-500.,500.,1000,-500.,500.);

 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run* run)
{

  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;
  
  const B1Run* b1Run = static_cast<const B1Run*>(run);


  const MyDetectorConstruction* detectorConstruction
   = static_cast<const MyDetectorConstruction*>
     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const MyPrimaryGeneratorAction* generatorAction
   = static_cast<const MyPrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
          
  // Print
  //  
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }

  //Writing run.txt file
  ofstream fs("run.txt");
  std::vector<double> scatteringAngleVector = b1Run->GetScatteringAngleVector();
  std::vector<G4ThreeVector> pocaPointVector = b1Run->GetPocaPtVector();
  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> z;
  for(int i = 0 ; i < scatteringAngleVector.size() ; i++){
	  fs << scatteringAngleVector[i] << std::endl;
	  fScatteringHist->Fill(scatteringAngleVector[i]*1000.);
    fPocaXYHist->Fill(pocaPointVector[i].x(),pocaPointVector[i].y());
    x.push_back(pocaPointVector[i].x());
    y.push_back(pocaPointVector[i].y());
    z.push_back(pocaPointVector[i].z());
  }
  fScatteringHist->Draw();
  fScatteringHist->Write();
  fPocaXYHist->Draw();
  fPocaXYHist->Write();

  TGraph *fShape = new TGraph(x.size(),&x[0],&y[0]);
  fShape->SetLineStyle(0);
  fShape->SetLineColor(kWhite);
  fShape->Draw("p");
  fShape->Write();
  

  TGraph *fShapeYZ = new TGraph(y.size(),&y[0],&z[0]);
  fShapeYZ->SetLineStyle(0);
  fShapeYZ->SetLineColor(kWhite);
  fShapeYZ->Draw("p");
  fShapeYZ->Write();

  fRootFile->Close();
  delete fRootFile;
  fs.close();

  std::cout <<"=========================================" << std::endl;
  std::cout << "Length of Position Vector : " << B1EventAction::position.size() << std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
