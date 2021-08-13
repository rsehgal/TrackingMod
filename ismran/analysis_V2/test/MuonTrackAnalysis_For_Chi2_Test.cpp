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
#include <TMath.h>
int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  std::string calibType = "Source"; 
  TFile *fp = new TFile(("Chi2"+calibType+".root").c_str(),"RECREATE");
  TH1F *histChi2 = new TH1F(("Chi2Hist_"+calibType).c_str(),("Chi2Hist_"+calibType).c_str(),50,0,10.);
  TH1F *histChi2Prob = new TH1F(("Chi2ProbHist_"+calibType).c_str(),("Chi2ProbHist_"+calibType).c_str(),50,0,1.);
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
 	std::vector<double> chi2ndf=smt->GetChisquareAndNDF(subsetLayer,false);
	histChi2->Fill(chi2byNdf);
 	double prob=TMath::Prob(chi2ndf[0],(int)chi2ndf[1]);
	//std::cout<< BLUE << "Chi2Prob : " << prob << RESET << std::endl;
	histChi2Prob->Fill(prob);
//	std::cout << "Chi2ByNDF : " << chi2byNdf << std::endl;
	counter++;	
      }
    }
  }
histChi2->Draw();
fp->cd();
histChi2->Write();
histChi2Prob->Write();
fp->Close();
fApp->Run();
return 0;
}
