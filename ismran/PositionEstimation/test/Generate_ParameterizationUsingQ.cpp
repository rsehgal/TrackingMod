/*
**	Filename : Test_ParameterizationUsingQ.cpp
**	2021-06-23
**	username : rsehgal
*/
#include <iostream>
#include <string>
#include "CharacterizationFileReader.h"
#include <fstream>
#include <TH1F.h>
#include <TApplication.h>
#include <vector>
#include "HelperFunctions.h"
#include "includes.h"
#include <TGraph.h>

int main(int argc, char *argv[])
{
  TApplication *fApp                = new TApplication("Test", NULL, NULL);
  std::string dirsPath              = argv[1];
  TFile *paramFile                  = new TFile("calibQ.root", "RECREATE");
  std::vector<std::string> vecOfDir = {"PS55_S1AA6646"};//, "PS65_S3AA1782", "PS75_SF887", "PS80_SF877"};
  std::vector<double> vecOfActualPos;
  std::vector<double> vecOfMeanOfQ;
  std::string barName = "";

  for (unsigned int dirIndex = 0; dirIndex < vecOfDir.size(); dirIndex++) {
    std::string currentDir = dirsPath + "/" + vecOfDir[dirIndex];
    vecOfActualPos.clear();
    vecOfMeanOfQ.clear();
    std::vector<std::string> vecOfFileNames = GetVectorOfFiles(currentDir.c_str());
    for (unsigned int i = 0; i < vecOfFileNames.size(); i++) {
      CharacterizationFileReader *obj = new CharacterizationFileReader(currentDir + "/" + vecOfFileNames[i]);
      vecOfActualPos.push_back((float)obj->GetActualPosition());
      vecOfMeanOfQ.push_back(obj->GetMeanOfQValues());
      if (obj->GetActualPosition() == 0) barName = obj->GetBarName();
      delete obj;
    }

    std::cout << "======================================" << std::endl;
    for (unsigned int i = 0; i < vecOfActualPos.size(); i++) {
      std::cout << "Location of actual source pos : " << vecOfActualPos[i] << std::endl;
    }
    std::cout << "======================================" << std::endl;

    TGraph *gr = new TGraph(vecOfActualPos.size(), &vecOfMeanOfQ[0], &vecOfActualPos[0]);
    gr->Draw("ap");
    TF1 *formula = new TF1(Form("fQparam_%s", barName.c_str()), Pol3, -5., 5., 4);
    gr->Fit(formula, "rq");
    paramFile->cd();
    formula->Write();
    gr->Write();
  }
  paramFile->Close();
  fApp->Run();
}
