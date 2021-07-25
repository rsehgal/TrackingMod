/*
**	Filename : MuonTrackAnalysis_For_Chi2_Test.cpp
**	2021-07-25
**	username : rsehgal
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
#include "includes.h"
#include "HardwareNomenclature.h"
#include <TSpectrum.h>
#include "Histograms.h"
#include <TH2F.h>
#include "colors.h"
int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  TH1F *histChi2 = new TH1F("Chi2Hist","Chi2Hist",100,-10,10.);
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename                                  = argv[1];
  unsigned int numOfTracks                              = 0;
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename, numOfTracks);

  std::vector<unsigned int> subsetLayer = {0, 1, 2, 3};
  unsigned int counter                  = 0;
  for (unsigned int i = 0; i < smtVec.size(); i++) {
if(!(i%100000))
std::cout << "Processed : " << i << " tracks......" << std::endl;
/*    std::cout << RED;
    std::cout << "======================================" << std::endl;
    smtVec[i]->Print();
    std::cout << RESET << std::endl;
*/
    lite_interface::SingleMuonTrack *smt = smtVec[i]->GetTrackSubset(subsetLayer);
//    if (counter < 5) 
{
//      std::cout << "======================================" << std::endl;
      if (smt != NULL) {
        /*std::cout << MAGENTA;
        smt->Print();
        std::cout << RESET << std::endl;*/

	//std::vector<lite_interface::Point3D*> vecOfPoint3D = smt->Get3dHitPointVector_QParam();
	double chi2byNdf = smt->GetChisquareByNDF(subsetLayer,false);
	histChi2->Fill(chi2byNdf);
//	std::cout << "Chi2ByNDF : " << chi2byNdf << std::endl;
	counter++;	
      }
    }
  }
histChi2->Draw();
fApp->Run();
return 0;
}
