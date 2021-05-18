/*
 * TestSystemHealth.cpp
 *
 *  Created on: 10-Feb-2021
 *      Author: rsehgal
 */

#include <iostream>
#include <vector>
#include "PairFinder.h"
#include "Analyzer.h"
#include "Calibration.h"
#include <TApplication.h>
#include <TCanvas.h>
#include "Plotter.h"
#include "includes.hh"
#include "HardwareNomenclature.h"
#include <TSpectrum.h>
#include "Histograms.h"

std::string getFileName(const std::string &s)
{

  char sep = '/';

#ifdef _WIN32
  sep = '\\';
#endif

  size_t i = s.rfind(sep, s.length());
  if (i != std::string::npos) {
    return (s.substr(i + 1, s.length() - i));
  }

  return ("");
}

int main(int argc, char *argv[])
{
  /*std::string outputRootFileName = "SystemHealth_"+getFileName(std::string(argv[1]));
  std::cout << "Output file : " << outputRootFileName << std::endl;
  return 0;
   */

  GenerateScintMatrixXYCenters();
  std::string outputRootFileName = "SystemHealth_" + getFileName(std::string(argv[1]));
  std::string delFile            = "rm -rf " + outputRootFileName + ".pdf";
  system(delFile.c_str());

  std::string allfilenames           = "";
  std::string command                = "/usr/bin/pdfunite ";
  TApplication *fApp                 = new TApplication("Test", NULL, NULL);
  lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename               = argv[1];
  // ushort barIndex = std::atoi(argv[2]);

  std::vector<Histograms *> vecOfHists;
  for (unsigned int i = 0; i < vecOfBarsNamess.size(); i++) {
    vecOfHists.push_back(new Histograms(vecOfBarsNamess[i], i));
  }

  lite_interface::Analyzer analyzerObj(filename);
  std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = analyzerObj.GetVectorOfScintillators();

  std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;

  for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {

    vecOfHists[(*itr)->GetBarIndex()]->FillHistogram(*itr);
  }

  for (unsigned int i = 0; i < vecOfBarsNamess.size(); i++) {

    vecOfHists[i]->Save();
    if (i == 0)
      allfilenames += (vecOfBarsNamess[i] + ".pdf");
    else
      allfilenames += (" " + vecOfBarsNamess[i] + ".pdf");
  }

  command += allfilenames;
  command += (" " + outputRootFileName + ".pdf");
  std::cout << "================ Exectung command ============= " << std::endl;
  std::cout << command << std::endl;

  system(command.c_str());

  std::string delfiles = ("rm -rf " + allfilenames);
  std::cout << "================ Exectung command ============= " << std::endl;
  std::cout << delfiles << std::endl;
  system(delfiles.c_str());

  TFile *f = new TFile(outputRootFileName.c_str(), "RECREATE");
  f->cd();
  for (unsigned int i = 0; i < vecOfBarsNamess.size(); i++) {
    vecOfHists[i]->fhistQNear->Write();
    vecOfHists[i]->fhistQFar->Write();
    vecOfHists[i]->fhistQMean->Write();
    vecOfHists[i]->fhistQMeanCorrected->Write();
    vecOfHists[i]->fhistDelT->Write();
    vecOfHists[i]->fhistDelTCorrected->Write();
    vecOfHists[i]->fhistDelTvsZ->Write();
  }
  f->Close();

  return 0;
}
