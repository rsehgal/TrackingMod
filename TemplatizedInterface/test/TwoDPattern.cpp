#include "GlassRpc.h"
#include "TriggeringPlane.h"
#include <iostream>
#include <TCanvas.h>
#include "Tree.h"
#include "TH2F.h"
#include "TFile.h"

typedef Tomography::Properties Detector;
using namespace Tomography;

void GenerateTimingHistogram();

int main() {

  /*
    RPC rpc(2,96,"FirstRpc");
    rpc.PrintStripNames();
    RPC rpc2(3,64,"FirstRpc");
    rpc2.PrintStripNames();
  */

  GenerateTimingHistogram();
}

void GenerateTimingHistogram() {
  Scintillator::SetStartingId(-1);
  // ScintillatorPlane topPlane(2,8,"Top-Plane");
  // ScintillatorPlane bottomPlane(2,8,"Bottom-Plane");
  Detector *topPlane = new TriggeringPlane(2, "TopPlane", -150, -1);
  Detector *bottomPlane = new TriggeringPlane(2, "BottomPlane", 150, 7);
  int nxbins = 1000;
  int xlow = 19500;
  int xhigh = 21000;
  int nybins = 150;
  int ylow = -10;
  int yhigh = 140;
  Tracking::Channel *trig = 0;
  Tracking::Channel *ch = 0;

  TCanvas *c2 = new TCanvas("c2", "Timing-Info", 200, 10, 700, 500);
  c2->Divide(1, 1);
  c2->cd(1);
  Tracking::Tree t("6742.root", "BSC_DATA_TREE");
  int numOfEvents = t.GetNumOfEvents();

  // TH1F *hTrig = new TH1F("hTrig","TEST",100,20000,21000);
  TH2F *h2d = new TH2F("h2d", "Timing", nxbins, xlow, xhigh, nybins, ylow, yhigh);
  ScintillatorPlane *scintPlane = topPlane->GetPlane(0);

  for (int evNo = 0; evNo < numOfEvents; evNo++) {
    trig = t.GetEntry("Module2_LE_CH31", evNo);
    h2d->Fill(trig->at(0), 31);
    // for (int i = 0; i < fScintillatorPlane.size(); i++) {
    for (int i = 0; i < scintPlane->GetNumOfScintillators(); i++) {
      ch = t.GetEntry(scintPlane->GetScintVector()[i]->GetName(), evNo);
      if (ch->size()) {
        for (int j = 0; j < ch->size(); j++) {
          h2d->Fill(ch->at(j), scintPlane->GetScintVector()[i]->GetScintId());
        }
      }
    }
  }

  scintPlane = bottomPlane->GetPlane(0);
  for (int evNo = 0; evNo < numOfEvents; evNo++) {
    for (int i = 0; i < scintPlane->GetNumOfScintillators(); i++) {
      ch = t.GetEntry(scintPlane->GetScintVector()[i]->GetName(), evNo);

      if (ch->size()) {
        for (int j = 0; j < ch->size(); j++) {
          h2d->Fill(ch->at(j), scintPlane->GetScintVector()[i]->GetScintId());
        }
      }
    }
  }

  // Now Creating Real RPC
  Detector *rpc = new GlassRpc(2, "SecondGlassRpc", 120, 31);
  for (int evNo = 0; evNo < numOfEvents; evNo++) {
    for (int plNum = 0; plNum < rpc->GetNumOfPlanes(); plNum++) {
      for (int i = 0; i < rpc->GetPlane(plNum)->GetNumOfScintillators(); i++) {
        // std::cout<<"Name : "<<scintPlane[i]->GetName()<<std::endl;
        ch = t.GetEntry(rpc->GetPlane(plNum)->GetScintVector()[i]->GetName(), evNo);
        if (ch->size()) {
          // std::cout<<"-- Data Found --"<<std::endl;
          for (int j = 0; j < ch->size(); j++) {
            h2d->Fill(ch->at(j), rpc->GetPlane(plNum)->GetScintVector()[i]->GetScintId());
          }
        }
      }
    }
  }

  h2d->Draw();
  h2d->Print();
  // TFile f("hTrig.root","recreate"); //Open file, then write histo to it.
  TFile::Open("hTrig.root", "RECREATE");
  h2d->Write();
  c2->Modified();
  c2->Update();
}
