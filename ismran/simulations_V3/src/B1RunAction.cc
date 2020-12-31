
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
#include "B1RunAction.hh"
#include "TH1F.h"
#include "TApplication.h"
#include "Analyzer.h"
#include "base/Global.h"
#include "Calibration.h"
#include "DataTree.h"
#include "Plotter.h"
#include "MuonReader.h"
#include "cry_manual_interface.h"
#include <vector>

using namespace std;

lite_interface::Calibration* B1RunAction::fCalib;
std::vector<double> B1RunAction::fAngleVec;
unsigned long B1RunAction::fEvNo = 0;
/*TTree* B1RunAction::fExactHitTree;
std::vector<double> B1RunAction::xVec;
std::vector<double> B1RunAction::yVec;
std::vector<double> B1RunAction::zVec;*/
lite_interface::ExactHitDataTree* B1RunAction::fExactHitDataTree;

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

	B1RunAction::fExactHitDataTree  = new lite_interface::ExactHitDataTree;
	/*fExactHitTree = new TTree("ExactHitTree","ExactHitTree");

	fExactHitTree->Branch("xVec",&xVec);//,"xVec/D");
	fExactHitTree->Branch("yVec",&yVec);//,"yVec/D");
	fExactHitTree->Branch("zVec",&zVec);//,"zVec/D");*/

  lite_interface::MuonReader::instance("Muons-50L.root");
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  fApp = new TApplication("Test", NULL, NULL);
  energyHist = new TH1F("EnergyHist","EnergyHist",100,0,50);
  //Tomography::EventBreak::instance()->fEffEvNo = 0;

  fDataTree = new lite_interface::DataTree();
  //fCalib = new lite_interface::Calibration("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib.root");
  fCalib = new lite_interface::Calibration("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib2.root");
}

void B1RunAction::WriteData(){
	std::cout <<"SIZE OF MUON TRACK VECTOR going to be written in ROOT File : " << MySD::muonTrackVec.size() << std::endl;
  for(unsigned int i = 0 ; i < MySD::muonTrackVec.size() ; i++){
    lite_interface::SingleMuonTrack *singleMuonTrack = MySD::muonTrackVec[i];

    /*singleMuonTrack->SetFittedMembers(0);
    singleMuonTrack->SetFittedMembers(1);
    singleMuonTrack->SetFittedMembers(2);*/
    singleMuonTrack->GetFittedTrack(0);
        singleMuonTrack->GetFittedTrack(1);
        singleMuonTrack->GetFittedTrack(2);
    //std::cout << "================= Track Id : " << i << " ================" << std::endl;
    //singleMuonTrack->Print();

    //if(singleMuonTrack->IsThroughTrack())
    {
    for(unsigned int j = 0 ; j < singleMuonTrack->size() ; j++){
      lite_interface::ScintillatorBar_V2 *scint = (singleMuonTrack->fSingleMuonTrack)[j];

      /*fDataTree->Fill((scint->scintName).c_str(),scint->qlongNear,scint->qlongFar,scint->qlongMean,
                      scint->qlongMeanCorrected,scint->tstampNear, scint->tstampFar,
                      scint->tsmallTimeStamp,scint->deltaTstamp,scint->deltaTstampCorrected,
                      scint->barIndex,scint->layerIndex);*/

     /* fDataTree->Fill((scint->scintName).c_str(),scint->qlongNear,scint->qlongFar,scint->qlongMean,
                       scint->qlongMeanCorrected,scint->tstampNear, scint->tstampFar,
                       scint->tsmallTimeStamp,scint->deltaTstamp,scint->deltaTstampCorrected,
                       scint->barIndex,scint->layerIndex,(scint->meanHitPosition).x,(scint->meanHitPosition).y,(scint->meanHitPosition).z);*/
      scint->fMeanHitPosition->Divide(10);
      //fDataTree->Fill(scint->fQlongNear,scint->fQlongMean,scint->fTSmallTimeStamp,scint->fDelTstamp,scint->fBarIndex, scint->fMeanHitPosition);
      fDataTree->Fill(scint->fQlongNear,scint->fQlongMean,scint->fTSmallTimeStamp,scint->fDelTstamp,scint->fBarIndex, scint->fMeanHitPosition, scint->fExactHitPosition);

      /*void Fill(char *scintname,UInt_t qlongnear,UInt_t qlongfar, Double_t qlongmean,
          Double_t qlongmeancorrected, ULong64_t tstampnear, ULong64_t tstampfar,
          ULong64_t tsmalltimestamp, Long64_t deltatstamp, Long64_t deltatstampcorrected,
          unsigned short barindex, unsigned short layerindex);*/
    }
  }

  }
  fDataTree->Write();
  energyHist->Write();
  fDataTree->Close();
  fExactHitDataTree->Write();
  fExactHitDataTree->Close();

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  std::cout  << "====================================================================" << std::endl;
  std::cout << "Total Number of Event : " << nofEvents << std::endl;
  //std::cout << "No of Particles reaches Sensitive Detector Region : " << MySD::numOfParticlesReached << std::endl;
  //std::cout << "No of Particles reaches Sensitive Detector Region : " << Tomography::EventBreak::instance()->fEffEvNo << std::endl;
  std::cout << "====================================================================" << std::endl;
  std::cout << "======= Printing finally stored events ==========" << std::endl;
  
  //Creating energy histogram of Scintillator bar with index 0
  for(unsigned int i = 0 ; i < MySD::eventsVec.size() ; i++){
  	for(unsigned int j =0 ; j < MySD::eventsVec[i].size() ; j++)
  	if(MySD::eventsVec[i][j]->fBarIndex==0){
  		energyHist->Fill(MySD::eventsVec[i][j]->fQlongMean);
  	}
  }
  energyHist->Draw();
  MySD::Print();
  //PrintPsBarVector();

  TH1F *energySum = lite_interface::PlotEnergySum(MySD::muonTrackVec);
  new TCanvas();
  energySum->Draw();

  std::vector<TH1D*> vecHist = lite_interface::PlotEnergyDistributionWithMultiplicity(MySD::muonTrackVec);
  new TCanvas("Energy with multiplicity..","Energy with multiplicity..");
  for(unsigned int i = 0; i < vecHist.size();  i++){

		  vecHist[i]->Draw("same");
  }

  new TCanvas();
  lite_interface::PlotZenithAngle(fAngleVec,4)->Draw();

  /*for(unsigned int  i = 0 ; i < MySD::muonTrackVec.size() ; i++){
	  std::cout << "============ Muon Track : " << i <<" ==========" << std::endl;
	  MySD::muonTrackVec[i]->Print();
  }*/
#if(0)
  //Doing the processing of muon tracks vector using Analyzer.
  Analyzer_V2 v;
  v.CalculateTotalEnergyDepositionForMuonTracks(MySD::muonTrackVec);
  v.PlotEnergyDistributionWithMultiplicity(MySD::muonTrackVec,0);

  TH1D *globalMultiplicityHist = new TH1D("globalMultiplicityHist","globalMultiplicityHist",20,1,21);
  int count = 0;
  for(unsigned int i = 0 ; i < MySD::muonTrackVec.size() ; i++){
	  globalMultiplicityHist->Fill(MySD::muonTrackVec[i]->size());
	  /*
      //NOT MAKING ANY SENSE
      
      if(MySD::muonTrackVec[i]->size() > 9){
		  std::cout <<"======================= Muon Track Length : " << MySD::muonTrackVec[i]->size() <<"  =======================================" << std::endl;
		  count++;
		  //MySD::muonTrackVec[i]->Print();
		  if(count==10)
			  break;
	  }*/
  }
  new TCanvas();
  globalMultiplicityHist->Draw();
  globalMultiplicityHist->Write();



  std::cout << "Total Num of Stopped particles : " << MySD::numOfStoppedParticles << std::endl;
  //std::cout << ((1.0*MySD::numOfStoppedParticles/Tomography::EventBreak::instance()->fEffEvNo)*100)<<" % of Number of particles that reaches the Sensitive detector region" << std::endl;

/* 
 #ifdef USE_CRY
  CryGeantInterface::energyHist->Draw();
#endif
*/

 // WriteData();


#endif
  WriteData();
  fApp->Run();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

