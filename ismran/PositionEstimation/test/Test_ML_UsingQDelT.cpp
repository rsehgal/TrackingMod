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
//  TApplication *fApp = new TApplication("Test", NULL, NULL);

  new TCanvas();
  // TLegend *legend = new TLegend();
  std::string filename = argv[1];
  CharacterizationFileReader cfReader(filename);
  //FillAttenCoeffVec();
  std::string barName = cfReader.GetBarName();
  int pos = cfReader.GetActualPosition();
  //cfReader.RandomizeIt();

  TFile *fpCalib    = new TFile("calibQ.root", "r");
  TF1 *paramFormula = (TF1 *)fpCalib->Get(("fQparam_" + barName).c_str());

  std::string subFileName="";
  if(pos >=0 )
	subFileName = barName+"_+"+std::to_string(pos);
  else
	subFileName = barName+"_"+std::to_string(pos);
  std::vector<Event *> eventsVec = cfReader.GetTrainingData(); // AllEvents(0);//std::atoi(argv[2]));
  //std::ofstream outfileTrain(barName+"_"+std::to_string(pos)+"_training.txt");
  std::ofstream outfileTrain(subFileName+"_training.txt");
  for (unsigned int i = 0; i < eventsVec.size(); i++) {
    double logQNearbyQFar = eventsVec[i]->GetLogQNearByQFar();
    double delt           = eventsVec[i]->GetDelT();
    double estimatedZ     = paramFormula->Eval(logQNearbyQFar);
    outfileTrain << logQNearbyQFar << "," << delt << "," << estimatedZ << "," << pos << std::endl;
  }
  outfileTrain.close();

  eventsVec.clear();
  eventsVec = cfReader.GetTestingData();
  //std::ofstream outfileTest(barName+"_"+std::to_string(pos)+"_testing.txt");
  std::ofstream outfileTest(subFileName+"_testing.txt");
  for (unsigned int i = 0; i < eventsVec.size(); i++) {
    double logQNearbyQFar = eventsVec[i]->GetLogQNearByQFar();
    double delt           = eventsVec[i]->GetDelT();
    double estimatedZ     = paramFormula->Eval(logQNearbyQFar);
    outfileTest << logQNearbyQFar << "," << delt << "," << estimatedZ << "," << pos << std::endl;
  }
  outfileTest.close();
  std::cout << "Training and Testing files generated........." << std::endl;
//  fApp->Run(); 
}
