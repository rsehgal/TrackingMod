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
#include "colors.h"
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

std::vector<double> GetNormalizedWeightingFactorForPixels(std::vector<TH1F *> vecOfHist)
{

  std::vector<double> vecOfMaxCount;
  vecOfMaxCount.resize(vecOfHist.size());
  for (unsigned int i = 0; i < vecOfHist.size(); i++) {
    vecOfMaxCount[i] = vecOfHist[i]->GetMaximum();
  }
  double max = *max_element(vecOfMaxCount.begin(), vecOfMaxCount.end());

  for (unsigned int i = 0; i < vecOfMaxCount.size(); i++) {
    vecOfMaxCount[i] /= max;
  }
  return vecOfMaxCount;
}
void NormalizeHistogramUsingOneWeightingFactor(TH1F *hist, double weightingFactor)
{

  unsigned int numOfBins = hist->GetNbinsX();
  for (unsigned int binNo = 1; binNo <= numOfBins; binNo++) {
    hist->SetBinContent(binNo, hist->GetBinContent(binNo) / weightingFactor);
  }
}

void NormalizeHistograms(std::vector<TH1F *> vecOfHist)
{
  std::vector<double> normalizingVector = GetNormalizedWeightingFactorForPixels(vecOfHist);

  for (unsigned int i = 0; i < vecOfHist.size(); i++) {
    NormalizeHistogramUsingOneWeightingFactor(vecOfHist[i], normalizingVector[i]);
  }
}

double GetWeightFactor(std::vector<double> vecOfWeightFactor, double val)
{
  short index = -1;
  if (val < -35) index = 0;

  for (unsigned int i = 1; i < vecOfWeightFactor.size() - 1; i++) {
    if (val >= (-35 + (10 * (i - 1))) && (val < -35 + (10 * i))) {
      index = i;
      break;
    }
  }

  if (val >= 35) index = vecOfWeightFactor.size() - 1;

  return vecOfWeightFactor[index];
}

TH1F *NormalWithMaxBinCount_L(TH1F *hist)
{
  double maxCount        = hist->GetMaximum(); // hist->GetBinContent(hist->GetMaximumBin());
  unsigned int numOfBins = hist->GetNbinsX();

  TH1F *normalizedHist = (TH1F *)hist->Clone();

  for (unsigned int binNo = 1; binNo <= numOfBins; binNo++) {
    normalizedHist->SetBinContent(binNo, normalizedHist->GetBinContent(binNo) / maxCount);
  }
  return normalizedHist;
}

int main(int argc, char *argv[])
{
  //#define MODIFY_SOURCE_PARAM_FOR_MUON
   gStyle->SetOptStat(0);
  std::cout << "ARGC : " << argc << std::endl;
  // return 0;
  GenerateScintMatrixXYCenters();
  for (unsigned int i = 0; i < vecOfScintXYCenter.size(); i++) {
    if (!(i % 9)) std::cout << "=================================" << std::endl;
    vecOfScintXYCenter[i].Print();
  }
  // return 0;

  TH1F *histDelT            = new TH1F("delTHist", "delTHist", 100, -20, 20);
  TH1F *histz               = new TH1F("histZ", "histZ", 400, -100, 100);
  TH1F *histz_reweight      = new TH1F("histZreweight", "histZ_Reweighted", 400, -100, 100);
  TH1F *histz_withoutFinger = new TH1F("histZWithoutFingers", "", 400, -100, 100);

  /*Creating vector of histogram for 9 pixels*/
  std::vector<TH1F *> vecOfHistForPixels;
  for (unsigned int i = 0; i < 9; i++) {
    std::string name = "Hist_Pixel_" + std::to_string(i);
    vecOfHistForPixels.push_back(new TH1F(name.c_str(), "", 400, -100., 100.));
  }

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
	 //bool useDelT = false;
          bool useDelT = true;
          TF1 *formu = GetParam(barIndex, useDelT);

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
      if (scint->GetBarIndex() == barIndex /*&& scintStart->GetBarIndexInLayer() == pixelNo*/) {
#ifdef USE_CRITICAL_COND
        if (fingerCondition) 
#endif
{

          // std::cout << "BarIndex : " << scint->GetBarIndex() << std::endl;
          // double deltCorr=scint->GetDelTCorrected();
          histDelT->Fill(scint->GetDelTCorrected() / 1000.);
         
	 // bool useDelT = false;
         /* bool useDelT = true;
          TF1 *formu = GetParam(barIndex, useDelT);*/

          /*TF1 *formu   = GetParam(scintStart->GetBarIndex(), useDelT);
                if (useDelT)
                  histz->Fill(formu->Eval(scintStart->GetDelTCorrected() / 1000.));*/

          if (useDelT) {
            double zv = scint->GetDelTCorrected() / 1000.;
		double zval = formu->Eval(zv);
		std::cout << RED << "DelTCorrected (ns) : " << zv << " : ZVal : " << zval << std::endl;
            histz->Fill(zval);
            zvalVec.push_back(zval);//formu->Eval(zv));
            vecOfHistForPixels[scintStart->GetBarIndexInLayer()]->Fill(zval);//formu->Eval(zv));

          } else {
#ifdef USE_FOR_SIMULATION
            double zv = scint->GetLogQNearByQFar_ForSimulation();
#else
            double zv = scint->GetLogQNearByQFar();
#endif

            // histz->Fill(formu->Eval());
            histz->Fill(formu->Eval(zv));
            zvalVec.push_back(formu->Eval(zv));
            vecOfHistForPixels[scintStart->GetBarIndexInLayer()]->Fill(formu->Eval(zv));
          }
        }
        /*Block for without finger condition*/
        {
          if (useDelT) {
            double zv = scint->GetDelTCorrected() / 1000.;
            //histz->Fill(formu->Eval(zv));
            //zvalVec.push_back(formu->Eval(zv));
            histz_withoutFinger->Fill(formu->Eval(zv));

          } else {
#ifdef USE_FOR_SIMULATION
            double zv = scint->GetLogQNearByQFar_ForSimulation();
#else
            double zv = scint->GetLogQNearByQFar();
#endif
            // histz->Fill(formu->Eval());
            //histz->Fill(formu->Eval(zv));
            //zvalVec.push_back(formu->Eval(zv));
            histz_withoutFinger->Fill(formu->Eval(zv));
          }
        }
      }
    }
  }

  /*Logic to Reweight the histogram*/
  TH1F *histNormalizedWithMaxVal = NormalWithMaxBinCount_L(histz);
  new TCanvas("NOrmalizedWithMaxValue", "NOrmalizedWithMaxValue");
  histNormalizedWithMaxVal->Draw();

  for (unsigned int i = 0; i < zvalVec.size(); i++) {
    // double weightFactor =
    // histNormalizedWithMaxVal->GetBinContent(histNormalizedWithMaxVal->GetXaxis()->FindBin(zvalVec[i]));
    int ibin            = histNormalizedWithMaxVal->FindBin(zvalVec[i]);
    double weightFactor = histNormalizedWithMaxVal->GetBinContent(ibin);
    histz_reweight->Fill(zvalVec[i], 1 / weightFactor);
  }
  new TCanvas("DelT", "DelT");
  histDelT->Draw();
  new TCanvas("Z", "Z");
  histz->Draw();

  new TCanvas("ReWeighted", "ReWeighted");
  histz_reweight->Draw();

  NormalizeHistograms(vecOfHistForPixels);
  new TCanvas("NormalizedAndWeightedHist", "Normalized And Weighted Hist");

  for (unsigned int i = 0; i < vecOfHistForPixels.size(); i++) {
    vecOfHistForPixels[i]->SetLineColor(i + 1);
    vecOfHistForPixels[i]->SetLineWidth(2);
    if (i == 0)
      vecOfHistForPixels[i]->Draw();
    else
      vecOfHistForPixels[i]->Draw("sames");
  }

  std::cout << RED << "No. of pix histograms : " << vecOfHistForPixels.size() << RESET << std::endl;
  TList *list = new TList;

  for (unsigned int i = 0; i < vecOfHistForPixels.size(); i++) {
    list->Add(vecOfHistForPixels[i]);
  }

  TH1F *histz_reweight_2 = new TH1F("histZreweight_2", "histZ_Reweighted_2", 200, -100, 100);
  histz_reweight_2->Merge(list);
  new TCanvas("ReWeightedMerged", "ReWeighted Merged");
  histz_reweight_2->Draw();

  new TCanvas("ZwithoutFinger","ZwithoutFinger");
  histz_withoutFinger->Draw();
  fApp->Run();
  return 0;
}
