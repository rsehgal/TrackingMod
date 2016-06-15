/*
 * DetectorInfo.cpp
 *
 *  Created on: Jun 8, 2016
 *      Author: rsehgal
 */

#include "GlassRpc.h"
#include "Properties.h"
#include <iostream>
#include "Tree.h"
#include "base/Global.h"
#include "TriggeringPlane.h"
#include <TApplication.h>
#include <TH2F.h>
#include <TCanvas.h>
typedef Tomography::Properties Detector;
using namespace Tomography;
int main() {

  TApplication *fApp = new TApplication("Test", NULL, NULL);
  TCanvas *c1 = new TCanvas("c2", "Timing-Info", 200, 10, 700, 500);
  int nxbins = 10;
  int xlow = 0;
  int xhigh = 10;
  int nybins = 10;
  int ylow = 0;
  int yhigh = 10;
  TH2F *h2d = new TH2F("h2d", "Timing", nxbins, xlow, xhigh, nybins, ylow, yhigh);

  //Tracking::Tree tr("6702.root", "BSC_DATA_TREE", 0);

//  Tracking::Tree::instance()->ReadTree("6702.root", "BSC_DATA_TREE", 0);
  Tracking::Tree::instance()->ReadTree("6853.root", "BSC_DATA_TREE", 0);
  int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
  std::cout << "Total Num of Events : " << numOfEvents << std::endl;
  Detector *trigger1 = new Tomography::TriggeringPlane(2, "UpperPlane", 120, -1);
  Detector *trigger2 = new Tomography::TriggeringPlane(2, "LowerPalne", -120, 7);
  // numOfEvents = 10;
  int count[8][8];
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      count[i][j] = 0;
  Tracking::Channel *ch2 = 0;
  Tracking::Channel *ch0 = 0;
  Tracking::Channel *ch1 = 0;

  for (int evNo = 0; evNo < numOfEvents; evNo++) {
    // for(int i=0 ; i < trigger1->GetNumOfPlanes() ;i++)

    ch0 = Tracking::Tree::instance()->GetEntry("Module2_LE_CH31", evNo);
    for (int i = 0; i < trigger1->GetPlane(0)->GetNumOfScintillators(); i++) {
      std::string brName = trigger1->GetPlane(0)->GetScintVector()[i]->GetName();
      ch1 = Tracking::Tree::instance()->GetEntry(brName, evNo);
      if (ch1->size()) {
        for (int j = 0; j < trigger2->GetPlane(0)->GetNumOfScintillators(); j++) {
          std::string brName1 = trigger2->GetPlane(0)->GetScintVector()[j]->GetName();
          ch2 = Tracking::Tree::instance()->GetEntry(brName1, evNo);

          if (ch2->size()) {
            // if(ch1->at(0) - ch2->at(0) > 500)
            // continue;
            if (ch1->size() && ch2->size()) {
              //	if(ch1->at(0) > 19500 && ch1->at(0) < 20500 && ch2->at(0) > 19500 && ch2->at(0) < 20500)
              count[i][j] += 1;
              h2d->Fill(i + 1, j + 1);
            } // std::cout<< "ChannelName : " << brName  << " : Data : " << ch->at(0) << std::endl;
            // if(ch2->size())
            //			std::cout<<ch2->at(0)<<std::endl;
          }
        }
      }
    }
    // std::cout<<ch0->at(0)<<std::endl;
    //	std::cout<<"==================================================="<<std::endl;
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++)
      std::cout << count[i][j] << " ";

    std::cout << "" << std::endl;
  }

  h2d->Draw("lego2");
  // h2d->Draw();
  c1->Modified();
  c1->Update();
  fApp->Run();

  return 0;
}
