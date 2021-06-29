/*
**	Filename : MuonTrackAnalysis_GenerateDataFor_Classification.cpp
**	2021-06-29
**	username : rsehgal
*/

// DOCBEGIN
/*
**  Test program to generate the data for classification.
**  This will generated the output file with  12 columns
**  ---------------------------------------------------------------------------------------------------------------
**  [1] BarIndex_OfStart 	 [2] LogOfQNearByQFar_OfStart 	  [3] CorrectedDelT_OfStart 	[4] zQ_OfStart
**  [5] BarIndex_OfInspected 	 [6] LogOfQNearByQFar_OfInspected [7] CorrectedDelT_OfInspected [8] zQ_OfInspected
**  [9] BarIndex_OfEnd 	 	 [10] LogOfQNearByQFar_OfEnd 	  [11] CorrectedDelT_OfEnd 	[12] zQ_OfEnd
**  ---------------------------------------------------------------------------------------------------------------
*/
// DOCEND

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
#include <TH2F.h>
#include "ml.h"

int main(int argc, char *argv[])
{
  GenerateScintMatrixXYCenters();
  for (unsigned int i = 0; i < vecOfScintXYCenter.size(); i++) {
    if (!(i % 9)) std::cout << "=================================" << std::endl;
    vecOfScintXYCenter[i].Print();
  }
  //  TApplication *fApp                   = new TApplication("Test", NULL, NULL);
  lite_interface::SingleMuonTrack *smt = new lite_interface::SingleMuonTrack;

  lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename               = argv[1];

  std::cout << "$$$$$$$$$$$$$$$ Going to Generate data for Classification $$$$$$$$$$$$$$$$$$$" << std::endl;
  GenerateDataForClassification(filename, std::atoi(argv[2]), std::atoi(argv[3]), std::atoi(argv[4]));

  //  fApp->Run();
}
