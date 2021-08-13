/*
**	Filename : MuonTrackAnalysis_ParameterizationUsingMuon.cpp
**	2021-07-15
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
#include "HardwareNomenclature.h"
#include <TSpectrum.h>
#include "Histograms.h"
#include <TH2F.h>
#include "ml.h"

TF1 *GetParam(unsigned int barIndex, bool deltParam = true)
{
  lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
  TF1 *delTFormula                   = calib->GetCalibrationDataOf(barIndex)->fParameterization_F;
  TF1 *qFormula                      = calib->GetCalibrationDataOf(barIndex)->fQParameterization_F;
  if (deltParam)
    return delTFormula;
  else
    return qFormula;
}

TH1F *NormalWithMaxBinCount_L(TH1F *hist)
{
  double maxCount        = hist->GetMaximum();//hist->GetBinContent(hist->GetMaximumBin());
  unsigned int numOfBins = hist->GetNbinsX();

  TH1F *normalizedHist = (TH1F *)hist->Clone();

  for (unsigned int binNo = 1; binNo <= numOfBins; binNo++) {
    normalizedHist->SetBinContent(binNo, normalizedHist->GetBinContent(binNo) / maxCount);
  }
  return normalizedHist;
}

int main(int argc, char *argv[])
{
#define MODIFY_SOURCE_PARAM_FOR_MUON
  //  gStyle->SetOptStat(0);
  std::cout << "ARGC : " << argc << std::endl;
  // return 0;
  GenerateScintMatrixXYCenters();
  for (unsigned int i = 0; i < vecOfScintXYCenter.size(); i++) {
    if (!(i % 9)) std::cout << "=================================" << std::endl;
    vecOfScintXYCenter[i].Print();
  }
  // return 0;

  TH1F *histDelT = new TH1F("delTHist", "delTHist", 100, -20, 20);
  TH1F *histz    = new TH1F("histZ", "histZ", 400, -100, 100);
  TH1F *histz_reweight    = new TH1F("histZreweight", "histZ_Reweighted", 400, -100, 100);

  TApplication *fApp                   = new TApplication("Test", NULL, NULL);
  lite_interface::SingleMuonTrack *smt = new lite_interface::SingleMuonTrack;

  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename                                  = argv[1];
  unsigned int numOfEv                                  = std::atoi(argv[2]);
  unsigned int barIndex                                 = std::atoi(argv[3]);
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename, numOfEv);
  std::cout << "$$$$$$$$$$$$$$  Going to Generated Muon Classification data $$$$$$$$$$$$$$$$$$" << std::endl;

  // GenerateParameterizationUsingMuons(filename,13);
  unsigned int pixelNo             = std::atoi(argv[4]);
  unsigned int startIndex          = 2;
  unsigned int endIndex            = 0;
  unsigned int inspectedLayerIndex = 1;
  std::vector<double> zvalVec;
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    unsigned int hittBarIndex = 10000;
    bool check                = smtVec[i]->CheckTrackForLayerNum(startIndex, hittBarIndex);
    lite_interface::ScintillatorBar_V2 *scintStart;
    lite_interface::ScintillatorBar_V2 *scintEnd;
    if (check) {
      scintStart = smtVec[i]->GetScintillator(hittBarIndex);
    }
    check &= smtVec[i]->CheckTrackForLayerNum(endIndex, hittBarIndex);
    if (check) {
      scintEnd = smtVec[i]->GetScintillator(hittBarIndex);
    }
    check &= smtVec[i]->CheckTrackForLayerNum(inspectedLayerIndex, hittBarIndex);
    if (check) {
      lite_interface::ScintillatorBar_V2 *scint = smtVec[i]->GetScintillator(hittBarIndex);
      bool fingerCondition                      = false;
      fingerCondition =
          (scintStart->GetLayerIndex() < inspectedLayerIndex && scintEnd->GetLayerIndex() < inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scint->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() > inspectedLayerIndex && scintEnd->GetLayerIndex() > inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scint->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() < inspectedLayerIndex && scintEnd->GetLayerIndex() > inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer()) ||
          (scintStart->GetLayerIndex() > inspectedLayerIndex && scintEnd->GetLayerIndex() < inspectedLayerIndex &&
           scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer());

      // if (scint->GetBarIndex() == 13 && fingerCondition) {
      if (scint->GetBarIndex() == barIndex /*&& scintStart->GetBarIndexInLayer() == pixelNo*/) 
{
        if (fingerCondition) {

          // std::cout << "BarIndex : " << scint->GetBarIndex() << std::endl;
          // double deltCorr=scint->GetDelTCorrected();
          histDelT->Fill(scint->GetDelTCorrected() / 1000.);
          //bool useDelT = false; 
	  bool useDelT = true;
	
	  /*TF1 *formu   = GetParam(scintStart->GetBarIndex(), useDelT);
          if (useDelT)
            histz->Fill(formu->Eval(scintStart->GetDelTCorrected() / 1000.));*/

          TF1 *formu   = GetParam(barIndex, useDelT);
          if (useDelT){
		double zv = scint->GetDelTCorrected() / 1000.;
		histz->Fill(formu->Eval(zv));
		zvalVec.push_back(formu->Eval(zv));

}
          else
            histz->Fill(formu->Eval(scint->GetLogQNearByQFar()));
        }
      }
    }
  }

/*Logic to Reweight the histogram*/
TH1F *histNormalizedWithMaxVal = NormalWithMaxBinCount_L(histz);
new TCanvas("NOrmalizedWithMaxValue","NOrmalizedWithMaxValue");
histNormalizedWithMaxVal->Draw();


for(unsigned int i = 0 ; i < zvalVec.size() ; i++){
  //double weightFactor = histNormalizedWithMaxVal->GetBinContent(histNormalizedWithMaxVal->GetXaxis()->FindBin(zvalVec[i]));
  int ibin = histNormalizedWithMaxVal->FindBin(zvalVec[i]);
  double weightFactor = histNormalizedWithMaxVal->GetBinContent(ibin);
  histz_reweight->Fill(zvalVec[i],1/weightFactor);
}
new TCanvas("DelT","DelT");
  histDelT->Draw();
  new TCanvas("Z", "Z");
  histz->Draw();

new TCanvas("ReWeighted","ReWeighted");  
histz_reweight->Draw();

  fApp->Run();
  return 0;
}
