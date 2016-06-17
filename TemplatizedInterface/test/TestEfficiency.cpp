/*
 *  Created on: June 15, 2016
 *      Author: rsehgal
 */

#include "GlassRpc.h"
#include "CmsRpc.h"
#include "TriggeringPlane.h"
#include <vector>
#include <iostream>
#include "VisualizationHelper.h"
#include "SetupManager.h"
#include <TApplication.h>
#include "Update.h"
#include "Tree.h"
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TLegend.h>
typedef Tomography::Properties Detector;
using namespace Tomography;

int main(int argc, char *argv[]) {
  bool verbose = false;
  int x[20] = {8000, 8200, 8400, 8600, 8800, 9000,  9100,  9200,  9300,  9400,
               9500, 9600, 9700, 9800, 9900, 10000, 10100, 10200, 10300, 10400};
  int size = 20;
  std::vector<std::string> flName;

  TApplication *app = new TApplication("Random", NULL, NULL);
  TCanvas *c = new TCanvas("object", "Manual Data", 800, 600);
  c->Divide(1, 3);
  Detector *rpc1 = new GlassRpc(2, "FirstGlassRpc", 30, 31);
  Detector *rpc2 = new GlassRpc(4, "SecondGlassRpc", -30, 31);
  TMultiGraph *mg1 = new TMultiGraph();
  mg1->SetTitle("Both X and Y ON");
  TMultiGraph *mg2 = new TMultiGraph();
  mg2->SetTitle("Only Y plane (ie. TOP Gap) ON");
  TMultiGraph *mg3 = new TMultiGraph();
  mg3->SetTitle("Only X plane (ie. BOTTOM Gap) ON");

  // Trying to add Legends
  TLegend *leg = new TLegend(0.1, 0.7, 0.48, 0.9);
  leg->SetHeader("The Legend Title");
  leg->AddEntry("h1", "Histogram filled with random numbers", "f");
  leg->AddEntry("f1", "Function abs(#frac{sin(x)}{x})", "l");
  leg->AddEntry("gr", "Graph with error bars", "lep");
  leg->Draw();

  TGraph *both_on1 = new TGraph();
  both_on1->SetTitle("w.r.t both");
  both_on1->SetMarkerStyle(21);
  both_on1->SetLineColor(kRed);
  both_on1->SetLineWidth(2);
  both_on1->GetXaxis()->SetRangeUser(8000, 11000);
  TGraph *top_on1 = new TGraph();
  top_on1->SetTitle("w.r.t TOP");
  top_on1->SetMarkerStyle(21);
  top_on1->SetLineColor(kGreen);
  top_on1->SetLineWidth(2);
  top_on1->GetXaxis()->SetRangeUser(8000, 11000);
  TGraph *bottom_on1 = new TGraph();
  bottom_on1->SetTitle("w.r.t BOTTOM");
  bottom_on1->SetMarkerStyle(21);
  bottom_on1->SetLineColor(kBlue);
  bottom_on1->SetLineWidth(2);
  bottom_on1->GetXaxis()->SetRangeUser(8000, 11000);

  TGraph *both_on2 = new TGraph();
  both_on2->SetTitle("w.r.t both");
  both_on2->SetMarkerStyle(21);
  both_on2->SetLineColor(kRed);
  both_on2->SetLineWidth(2);
  both_on2->GetXaxis()->SetRangeUser(8000, 11000);
  TGraph *top_on2 = new TGraph();
  top_on2->SetTitle("w.r.t TOP");
  top_on2->SetMarkerStyle(21);
  top_on2->SetLineColor(kGreen);
  top_on2->SetLineWidth(2);
  top_on2->GetXaxis()->SetRangeUser(8000, 11000);
  TGraph *bottom_on2 = new TGraph();
  bottom_on2->SetTitle("w.r.t BOTTOM");
  bottom_on2->SetMarkerStyle(21);
  bottom_on2->SetLineColor(kBlue);
  bottom_on2->SetLineWidth(2);
  bottom_on2->GetXaxis()->SetRangeUser(8000, 11000);

  TGraph *both_on3 = new TGraph();
  both_on3->SetTitle("w.r.t both");
  both_on3->SetMarkerStyle(21);
  both_on3->SetLineColor(kRed);
  both_on3->SetLineWidth(2);
  both_on3->GetXaxis()->SetRangeUser(8000, 11000);
  TGraph *top_on3 = new TGraph();
  top_on3->SetTitle("w.r.t TOP");
  top_on3->SetMarkerStyle(21);
  top_on3->SetLineColor(kGreen);
  top_on3->SetLineWidth(2);
  top_on3->GetXaxis()->SetRangeUser(8000, 11000);
  TGraph *bottom_on3 = new TGraph();
  bottom_on3->SetTitle("w.r.t BOTTOM");
  bottom_on3->SetMarkerStyle(21);
  bottom_on3->SetLineColor(kBlue);
  bottom_on3->SetLineWidth(2);
  bottom_on3->GetXaxis()->SetRangeUser(8000, 11000);

  std::vector<std::string> bothOn;
  int temp1 = atoi(argv[1]); // 6794;
  std::vector<std::string> topOn;
  int temp2 = temp1 + 20;
  std::vector<std::string> bottomOn;
  int temp3 = temp2 + 20;

  std::vector<std::vector<std::string>> filetype;

  // Logic to Get the ROOT file in order
  std::string temp_str;
  for (int i = 0; i < size; i++) {
    temp_str = std::to_string(temp1);
    temp_str += ".root";
    bothOn.push_back(temp_str);
    temp1++;

    temp_str = std::to_string(temp2);
    temp_str += ".root";
    topOn.push_back(temp_str);
    temp2++;

    temp_str = std::to_string(temp3);
    temp_str += ".root";
    bottomOn.push_back(temp_str);
    temp3++;
  }

  filetype.push_back(bothOn);
  filetype.push_back(topOn);
  filetype.push_back(bottomOn);

  Scintillator::SetWindowStartEnd(19450, 22500);
  for (int i = 0; i < 3; i++) {
    std::cout << "////////////////////" << std::endl;
    std::cout << "////////////////////" << std::endl;
    for (int j = 0; j < 20; j++) {

      std::cout << "===========================================================================" << std::endl;
      Tracking::Tree::instance()->ReadTree(filetype[i][j].c_str(), "BSC_DATA_TREE", 0);
      std::cout << filetype[i][j] << std::endl;
      std::cout << "===========================================================================" << std::endl;
      rpc1->SetEfficiency();
      rpc1->GetPlane(0)->SetEfficiency();
      rpc1->GetPlane(1)->SetEfficiency();
      if (verbose) {
        std::cout << "Eff of Det-1 : " << rpc1->GetEfficiency() << std::endl;
        std::cout << "Eff w.r.t Plane 1 : " << rpc1->GetPlane(0)->GetEfficiency() << std::endl;
        std::cout << "Eff w.r.t Plane 2 : " << rpc1->GetPlane(1)->GetEfficiency() << std::endl;
        std::cout << "--------------------------------------" << std::endl;
      }
      rpc2->SetEfficiency();
      rpc2->GetPlane(0)->SetEfficiency();
      rpc2->GetPlane(1)->SetEfficiency();
      if (verbose) {
        std::cout << "Eff of Det-2 : " << rpc2->GetEfficiency() << std::endl;
        std::cout << "Eff w.r.t Plane 1 : " << rpc2->GetPlane(0)->GetEfficiency() << std::endl;
        std::cout << "Eff w.r.t Plane 2 : " << rpc2->GetPlane(1)->GetEfficiency() << std::endl;
      }
      if (i == 0) {
        // both_on->SetPoint(temp1,j+1,rpc2->GetEfficiency());
        both_on1->SetPoint(temp1, x[j], rpc2->GetEfficiency());
        top_on1->SetPoint(temp2, x[j], rpc2->GetPlane(0)->GetEfficiency());
        bottom_on1->SetPoint(temp3, x[j], rpc2->GetPlane(1)->GetEfficiency());
        temp1++;
        temp2++;
        temp3++;
      }
      if (i == 1) {
        // top_on->SetPoint(temp2,j+1,rpc2->GetEfficiency());
        both_on2->SetPoint(temp1, x[j], rpc2->GetEfficiency());
        top_on2->SetPoint(temp2, x[j], rpc2->GetPlane(1)->GetEfficiency());
        bottom_on2->SetPoint(temp3, x[j], rpc2->GetPlane(0)->GetEfficiency());
        temp1++;
        temp2++;
        temp3++;
      }
      if (i == 2) {
        // bottom_on->SetPoint(temp3,j+1,rpc2->GetEfficiency());
        both_on3->SetPoint(temp3, x[j], rpc2->GetEfficiency());
        top_on3->SetPoint(temp2, x[j], rpc2->GetPlane(1)->GetEfficiency());
        bottom_on3->SetPoint(temp3, x[j], rpc2->GetPlane(0)->GetEfficiency());
        temp1++;
        temp2++;
        temp3++;
      }
    }
  }

  mg1->Add(both_on1);
  mg1->Add(top_on1);
  mg1->Add(bottom_on1);

  mg2->Add(both_on2);
  mg2->Add(top_on2);
  mg2->Add(bottom_on2);

  mg3->Add(both_on3);
  mg3->Add(top_on3);
  mg3->Add(bottom_on3);

  c->cd(1);
  mg1->Draw("ACP");
  c->GetPad(2)->BuildLegend();
  c->cd(2);
  mg2->Draw("ACP");
  c->cd(3);
  mg3->Draw("ACP");
  // c->BuildLegend();
  mg1->GetXaxis()->SetLimits(8000., 11000.);
  mg2->GetXaxis()->SetLimits(8000., 11000.);
  mg3->GetXaxis()->SetLimits(8000., 11000.);
  app->Run();
  return 0;
}
