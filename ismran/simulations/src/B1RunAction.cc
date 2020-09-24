
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
//#include "B1EventAction.hh"
#include "MySD.h"
#include "TH1F.h"
#include "TApplication.h"
#include "Analyzer_V2.h"
#include "base/Global.h"

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//ofstream* B1RunAction::myfile=NULL;

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
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  fApp = new TApplication("Test", NULL, NULL);
  energyHist = new TH1F("EnergyHist","EnergyHist",1000,0,250000);
  Tomography::EventBreak::instance()->fEffEvNo = 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  std::cout  << "====================================================================" << std::endl;
  std::cout << "Total Number of Event : " << nofEvents << std::endl;
  //std::cout << "No of Particles reaches Sensitive Detector Region : " << MySD::numOfParticlesReached << std::endl;
  std::cout << "No of Particles reaches Sensitive Detector Region : " << Tomography::EventBreak::instance()->fEffEvNo << std::endl;
  std::cout << "====================================================================" << std::endl;
  std::cout << "======= Printing finally stored events ==========" << std::endl;
  
  //Creating energy histogram of Scintillator bar with index 0
  for(unsigned int i = 0 ; i < MySD::eventsVec.size() ; i++){
  	for(unsigned int j =0 ; j < MySD::eventsVec[i].size() ; j++)
  	if(MySD::eventsVec[i][j]->barIndex==0){
  		energyHist->Fill(MySD::eventsVec[i][j]->qlongMean*1000.);
  	}
  }
  energyHist->Draw();
  MySD::Print();
  //PrintPsBarVector();

  //Doing the processing of muon tracks vector using Analyzer.
  Analyzer_V2 v;
  v.CalculateTotalEnergyDepositionForMuonTracks(MySD::muonTrackVec);
  v.PlotEnergyDistributionWithMultiplicity(MySD::muonTrackVec,0);

  TH1D *globalMultiplicityHist = new TH1D("globalMultiplicityHist","globalMultiplicityHist",20,1,21);
  for(unsigned int i = 0 ; i < MySD::muonTrackVec.size() ; i++){
	  globalMultiplicityHist->Fill(MySD::muonTrackVec[i]->size());
	  if(MySD::muonTrackVec[i]->size() > 9){
		 // std::cout <<"======================= Muon Track Length : " << MySD::muonTrackVec[i]->size() <<"  =======================================" << std::endl;
		 // MySD::muonTrackVec[i]->Print();
	  }
  }
  new TCanvas();
  globalMultiplicityHist->Draw();


  fApp->Run();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

