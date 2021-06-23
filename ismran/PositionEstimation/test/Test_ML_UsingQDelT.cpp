/*
**	Filename : Test_ML_UsingQDelT.cpp
**	2021-06-23
**	username : rsehgal
*/
#include <iostream>
#include "includes.hh"
#include <TFitResult.h>
#include <TH1F.h>
#include <TTree.h>
#include <TFile.h>
#include <vector>
#include <TApplication.h>
#include <TStyle.h>
#include "includes.h"
#include "CharacterizationFileReader.h"
int main(int argc, char *argv[])
{
  gStyle->SetOptStat(0);
  TApplication *fApp = new TApplication("Test", NULL, NULL);

  new TCanvas();
  // TLegend *legend = new TLegend();
  std::string filename = argv[1];
  CharacterizationFileReader cfReader(filename);
  FillAttenCoeffVec();
  std::string barName = cfReader.GetBarName();
  cfReader.RandomizeIt();

  TFile *fpCalib    = new TFile("calibQ.root", "r");
  TF1 *paramFormula = (TF1 *)fpCalib->Get(("fQparam_" + barName).c_str());

  std::vector<Event *> eventsVec = cfReader.GetTrainingData(); // AllEvents(0);//std::atoi(argv[2]));
  std::ofstream outfileTrain("ml_q_delt_training.txt");
  for (unsigned int i = 0; i < eventsVec.size(); i++) {
    double logQNearbyQFar = eventsVec[i]->GetLogQNearByQFar();
    double delt           = eventsVec[i]->GetDelT();
    double estimatedZ     = paramFormula->Eval(logQNearbyQFar);
    outfileTrain << logQNearbyQFar << "," << delt << "," << estimatedZ << std::endl;
  }
  outfileTrain.close();

  eventsVec.clear();
  eventsVec = cfReader.GetTestingData();
  std::ofstream outfileTest("ml_q_delt_testing.txt");
  for (unsigned int i = 0; i < eventsVec.size(); i++) {
    double logQNearbyQFar = eventsVec[i]->GetLogQNearByQFar();
    double delt           = eventsVec[i]->GetDelT();
    double estimatedZ     = paramFormula->Eval(logQNearbyQFar);
    outfileTest << logQNearbyQFar << "," << delt << "," << estimatedZ << std::endl;
  }
  outfileTest.close();
  std::cout << "Training and Testing files generated........." << std::endl;
  fApp->Run(); 
}
