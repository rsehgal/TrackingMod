/*
 * TestMuonTrackTimeEstimation.cpp
 *
 *  Created on: 01-Mar-2021
 *      Author: rsehgal
 */

#include "PairFinder.h"
#include "Analyzer.h"
#include "Calibration.h"
#include <TApplication.h>
#include <TCanvas.h>
#include "Plotter.h"
#include "includes.hh"
#include "HardwareNomenclature.h"

int main(int argc, char *argv[])
{
  lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
  gStyle->SetOptStat(0);
  TApplication *fApp = new TApplication("Test", NULL, NULL);
//  lite_interface::Analyzer analyzerObj(argv[1],true,20000);
  lite_interface::Analyzer analyzerObj(argv[1],true,0);
  std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = analyzerObj.GetVectorOfScintillators();
  //ULong64_t timeWindow                                          = GetMuonTrackTimeWindow(scintBarVec);
  TH1F *hist = new TH1F("Timing","Timing",5000,0,5000);
  for(unsigned int i = 0 ; i < scintBarVec.size() ; i++){
	hist->Fill(scintBarVec[i]->GetTAverage()/1e+14);
	} 

Long64_t tdiff = scintBarVec[scintBarVec.size()-1]->GetTAverage() - scintBarVec[0]->GetTAverage();
std::cout << "Total time : " << (tdiff/1e+12) << std::endl;
hist->Draw();
  fApp->Run();

  return 0;
}
