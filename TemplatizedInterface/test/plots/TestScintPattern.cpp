#include "GlassRpc.h"
#include "CmsRpc.h"
#include "TriggeringPlane.h"
#include <vector>
#include <iostream>
//#include "VisualizationHelper.h"
#include "SetupManager.h"
#include <TApplication.h>
//#include "Update.h"
#include "Tree.h"
#include "Statistics.h"
#include <TStyle.h>
#include "GenericXYDetector.h"
#include "TriggeringPlane.h"
#include <TH2F.h>
#include <TFile.h>
typedef Tomography::Properties Detector;
using namespace Tomography;

int main(int argc, char *argv[]) {
  std::string temp_str = std::to_string(atoi(argv[1]));
  temp_str += ".root";
  int clusterSize = 1;
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  //gStyle->SetPalette(1);
  Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);

#if(1) //using concept of Detector
  Detector *trgPlanes = new GenericXYDetector(2,"XYTriggeringPlanes",0,-1,8,100.,100.,1);
  trgPlanes->SetClusterSize(clusterSize);
  SetupManager::instance()->Register(trgPlanes);
  trgPlanes->GetStripsHitPlot3D();
#endif

#if(0) // Old Traditional Way
  Channel *ch1 = 0;
  Channel *ch2 = 0;

  int nxbins = 8;
  int xlow = 0;
  int xhigh = 8;
  int nybins = 8;
  int ylow = 0;
  int yhigh = 8;
  TCanvas *c2 = new TCanvas("c2", "Timing-Info", 200, 10, 700, 500);
  TH2F *h2d = new TH2F("h2d", "Timing", nxbins, xlow, xhigh, nybins, ylow, yhigh);

  int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
  Tomography::TriggeringPlane topPlane(2,"TopPlane",-105,-1);
  Tomography::TriggeringPlane bottomPlane(2,"TopPlane",105,7);

  std::vector<Tomography::Scintillator*> scintPlane1 = topPlane.GetPlane(0)->GetScintVector();
  std::vector<Tomography::Scintillator*> scintPlane2 = bottomPlane.GetPlane(0)->GetScintVector();

  for (int evNo = 0; evNo < numOfEvents; evNo++) {
   for (int i = 0; i < scintPlane1.size(); i++) {
      ch1 = Tracking::Tree::instance()->GetEntry(scintPlane1[i]->GetName(), evNo);
      if (ch1->size()) {
        for (int j = 0; j < scintPlane2.size(); j++) {
          ch2 = Tracking::Tree::instance()->GetEntry(scintPlane2[j]->GetName(), evNo);
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




#endif
  fApp->Run();
}
