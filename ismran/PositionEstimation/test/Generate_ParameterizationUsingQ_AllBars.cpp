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
#include "colors.h"
int main(int argc, char *argv[])
{
  // TApplication *fApp                = new TApplication("Test", NULL, NULL);
  std::string dirsPath              = argv[1];
  unsigned int start                = std::atoi(argv[2]);
  unsigned int jump                 = 9;
  std::string qfileName             = "calibQ_" + std::to_string(start / jump) + ".root";
  TFile *paramFile                  = new TFile(qfileName.c_str(), "RECREATE");
  std::vector<std::string> vecOfDir = vecOfBarsNamess; //{"PS55_S1AA6646", "PS65_S3AA1782", "PS75_SF887", "PS80_SF877"};
  std::vector<double> vecOfActualPos;
  std::vector<double> vecOfMeanOfQ;
  std::string barName = "";

  for (unsigned int dirIndex = start; dirIndex < (start + jump); dirIndex++) {
    std::string currentDir = dirsPath + "/" + vecOfDir[dirIndex];
    std::cout << BOLDRED;
    std::cout << "======================================================================" << std::endl;
    std::cout << "Generating Charge Parameterization for BAR : " << vecOfDir[dirIndex] << std::endl;
    std::cout << "======================================================================" << std::endl;
    std::cout << RESET;
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

    TGraph *gr = new TGraph(vecOfActualPos.size(), &vecOfMeanOfQ[0], &vecOfActualPos[0]);
    // gr->Draw("ap");
    TF1 *formula = new TF1(Form("fQparam_%s", barName.c_str()), Pol3, -5., 5., 4);
    gr->Fit(formula, "rq");
    paramFile->cd();
    formula->Write();
  }
  paramFile->Close();
  //  fApp->Run();
}
