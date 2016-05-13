#include "Scintillator.h"
#include <iostream>
#include <TApplication.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <vector>
#include <TFile.h>
using namespace Tracking;

void GenerateTimingHistogram();

int main(){

//  RPC rpc(2,96,"FirstRpc");
//  rpc.PrintStripNames();
//  RPC rpc2(3,64,"FirstRpc");
//  rpc2.PrintStripNames();

  GenerateTimingHistogram();

}

void GenerateTimingHistogram(){
  TApplication *fApp = new TApplication("Test", NULL, NULL); 
  //Scintillator::SetStartingId(-1);
  ScintillatorPlane topPlane(2,8,-105.,false,"TopPlane");
  ScintillatorPlane bottomPlane(2,8,105.,false,"BottomPlane");
  int nxbins = 8;
  int xlow = 0;
  int xhigh = 8;
  int nybins = 8;
  int ylow = 0;
  int yhigh = 8;
  Channel *trig = 0;
  Channel *ch1 = 0;
  Channel *ch2 = 0;

  TCanvas *c2 = new TCanvas("c2", "Timing-Info", 200, 10, 700, 500);
  c2->Divide(1, 1);
  c2->cd(1);
  Tree t("3476.root", "BSC_DATA_TREE");
  int numOfEvents = t.GetNumOfEvents();

  // TH1F *hTrig = new TH1F("hTrig","TEST",100,20000,21000);
  TH2F *h2d = new TH2F("h2d", "Timing", nxbins, xlow, xhigh, nybins, ylow, yhigh);
  std::vector<Tracking::Scintillator*> scintPlane1 = topPlane.GetScintillatorPlane();
  std::vector<Tracking::Scintillator*> scintPlane2 = bottomPlane.GetScintillatorPlane();


  for (int evNo = 0; evNo < numOfEvents; evNo++) {
   for (int i = 0; i < scintPlane1.size(); i++) {
      ch1 = t.GetEntry(scintPlane1[i]->GetName(), evNo);
      if (ch1->size()) {
        for (int j = 0; j < scintPlane2.size(); j++) {
          ch2 = t.GetEntry(scintPlane2[j]->GetName(), evNo);
          if(ch2->size())
            h2d->Fill(i, j);
        }
        }
      }
    }


  //h2d->Draw();
  h2d->Draw("LEGO2");
  fApp->Run();
  h2d->Print();
  // TFile f("hTrig.root","recreate"); //Open file, then write histo to it.
  TFile::Open("hTrig.root", "RECREATE");
  h2d->Write();
  c2->Modified();
  c2->Update();
}
