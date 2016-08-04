#include "GlassRpc.h"
#include "TriggeringPlane.h"
#include <iostream>
#include <TCanvas.h>
#include "Tree.h"
#include "TH2F.h"
#include "TFile.h"
#include <TApplication.h>

typedef Tomography::Properties Detector;
using namespace Tomography;

void GenerateTimingHistogram();

int main() {
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	GenerateTimingHistogram();
        fApp->Run();
}

void GenerateTimingHistogram() {
  Scintillator::SetStartingId(-1);
  // ScintillatorPlane topPlane(2,8,"Top-Plane");
  // ScintillatorPlane bottomPlane(2,8,"Bottom-Plane");
  Detector *topPlane = new TriggeringPlane(2, "TopPlane", -150, -1);
  Detector *bottomPlane = new TriggeringPlane(2, "BottomPlane", 150, 7);
  int nxbins = 1000;
  int xlow = 19500;
  int xhigh = 23000;
  int nybins = 150;
  int ylow = -10;
  int yhigh = 140;
  Tracking::Channel *trig = 0;
  Tracking::Channel *ch = 0;

  TCanvas *c2 = new TCanvas("c2", "Timing-Info", 200, 10, 700, 500);
  c2->Divide(1, 1);
  c2->cd(1);
  // Tracking::Tree t("6742.root", "BSC_DATA_TREE");
  Tracking::Tree::instance()->ReadTree("6853.root", "BSC_DATA_TREE", 0);
  Tracking::Tree *t = Tracking::Tree::instance()->GetTree();
  int numOfEvents = t->GetNumOfEvents();

  // TH1F *hTrig = new TH1F("hTrig","TEST",100,20000,21000);
  TH2F *h2d = new TH2F("h2d", "Timing", nxbins, xlow, xhigh, nybins, ylow, yhigh);
  ScintillatorPlane *scintPlane = topPlane->GetPlane(0);

  for (int evNo = 0; evNo < numOfEvents; evNo++) {
    trig = t->GetEntry("Module2_LE_CH31", evNo);
#define TRIG
#ifdef TRIG
    if(trig->size())
    	h2d->Fill(trig->at(0), 31);
#endif
#define TOP
#ifdef TOP

    // for (int i = 0; i < fScintillatorPlane.size(); i++) {
    for (int i = 0; i < scintPlane->GetNumOfScintillators(); i++) {
      ch = t->GetEntry(scintPlane->GetScintVector()[i]->GetName(), evNo);
      if (ch->size()) {
    	  h2d->Fill(ch->at(0), scintPlane->GetScintVector()[i]->GetScintId());
     }
    }
#endif
  }

#define BOTTOM
#ifdef BOTTOM
  scintPlane = bottomPlane->GetPlane(0);
  for (int evNo = 0; evNo < numOfEvents; evNo++) {
    for (int i = 0; i < scintPlane->GetNumOfScintillators(); i++) {
      ch = t->GetEntry(scintPlane->GetScintVector()[i]->GetName(), evNo);

      if (ch->size()) {
    	  h2d->Fill(ch->at(0), scintPlane->GetScintVector()[i]->GetScintId());
      }
    }
  }
#endif

#define RPC
#ifdef RPC
  // Now Creating Real RPC
  Detector *rpc = new GlassRpc(4, "SecondGlassRpc", 120, 31);
  std::vector<std::string> firedNameVector;
  std::vector<int> firedIDVector;
  int count[]={0,0};
  int countBoth = 0;
  for (int evNo = 0; evNo < numOfEvents; evNo++) {
	  rpc->SetFiredStripsVector(evNo);

	  if(rpc->GetPlane(0)->GetFiredStripsNameVector().size() &&
			  rpc->GetPlane(1)->GetFiredStripsNameVector().size() ){
	     countBoth++;
	     //std::cout<<"EveNO : " << evNo << std::endl;
	  }

    for (int plNum = 0; plNum < rpc->GetNumOfPlanes(); plNum++) {

    	firedNameVector.clear();
    	firedIDVector.clear();
    	firedNameVector = rpc->GetPlane(plNum)->GetFiredStripsNameVector();
    	firedIDVector = rpc->GetPlane(plNum)->GetFiredStripsIDVector();

    	if(firedNameVector.size()){
    		count[plNum]++;
    		for(int i = 0 ; i < firedNameVector.size() ; i++){
    			ch = t->GetEntry(firedNameVector[i], evNo);
    			if (ch->size()) {
    				h2d->Fill(ch->at(0), firedIDVector[i]+plNum*32);
    			}
    		}
    	}
    }
  }

  std::cout<<"countBoth : " << countBoth <<std::endl;
  std::cout<<"Count-0 : " << count[0] << std::endl;
  std::cout<<"Count-1 : " << count[1] << std::endl;


//#define DISS
#ifdef DISS
  Detector *rpc = new GlassRpc(4, "SecondGlassRpc", 120, -1);
    for (int evNo = 0; evNo < numOfEvents; evNo++) {
      //for (int plNum = 0; plNum < rpc->GetNumOfPlanes(); plNum++) {
    	plNum = 1;
        for (int i = 0; i < rpc->GetPlane(plNum)->GetNumOfScintillators(); i++) {
          // std::cout<<"Name : "<<scintPlane[i]->GetName()<<std::endl;
          ch = t->GetEntry(rpc->GetPlane(plNum)->GetScintVector()[i]->GetName(), evNo);
          if (ch->size()) {
          	if(ch->at(0) > 19450 && ch->at(0) < 20550)
          	h2d->Fill(ch->at(0), rpc->GetPlane(plNum)->GetScintVector()[i]->GetScintId());

          }
        }
     // }
    }
#endif
#endif
  h2d->Draw();
  h2d->Print();
  // TFile f("hTrig.root","recreate"); //Open file, then write histo to it.
//  TFile::Open("hTrig.root", "RECREATE");
//  h2d->Write();
  c2->Modified();
  c2->Update();
}


/*
          for (int j = 0; j < ch->size(); j++) {
            h2d->Fill(ch->at(j), rpc->GetPlane(plNum)->GetScintVector()[i]->GetScintId());
          }
*/

