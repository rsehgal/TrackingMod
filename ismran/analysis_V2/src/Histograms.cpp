/*
 * Histograms.cpp
 *
 *  Created on: 04-Sep-2020
 *      Author: rsehgal
 */

#include "Histograms.h"
#include "ScintillatorBar_V2.h"
#include "TPad.h"

Histograms::Histograms()
{
  // TODO Auto-generated constructor stub
  padHeight = 150;
  padWidth  = 200;
}

void Histograms::Save()
{
  std::string filename = fBarName + ".pdf";

  fCanvas->cd(1);
  fhistDelTCorrected->Draw();

  (fCanvas->cd(2))->SetLogy();
  fhistQNear->Draw();
  fhistQFar->SetLineColor(6);
  fhistQFar->Draw("same");

  (fCanvas->cd(3))->SetLogy();
  fhistQMean->Draw();

  (fCanvas->cd(4))->SetLogy();
  fhistQMeanCorrected->Draw();

  fCanvas->cd(5);
  fhistDelTvsZ->Draw("colz");

  //

  // fCanvas->cd(4);

  fCanvas->Print(filename.c_str());
}

Histograms::Histograms(std::string barName, unsigned int barIndex)
{

  fBarName                      = barName;
  fBarIndex                     = barIndex;
  int lqstart                   = 0;
  int lqend                     = 32000;
  int lnbins                    = 32000;
  unsigned short int numOfPlots = 4;
  gStyle->SetOptStat(0);
  /*fhistQNearFarPad = new TPad( (barName+"-NearFarPad").c_str(),(barName+"-NearFarPad").c_str(),
                 0,barIndex*padHeight,
                 padWidth,(barIndex+1)*padHeight);
  fhistQNearFarPad->Draw();*/

  fCanvas = new TCanvas(barName.c_str(), barName.c_str());
  fCanvas->Divide(3, 2);
  fhistQNear = new TH1D((barName + "-Near").c_str(), (barName + "-Near").c_str(), nbins, lqstart, lqend);
  fhistQNear->SetLineColor(1);
  fhistQFar = new TH1D((barName + "-Far").c_str(), (barName + "-Far").c_str(), nbins, lqstart, lqend);
  fhistQFar->SetLineColor(2);

  /*fhistQMeanPad = new TPad( (barName+"-MeanPad").c_str(),(barName+"-MeanPad").c_str(),
                 padWidth,barIndex*padHeight,
                 2*padWidth,(barIndex+1)*padHeight);
  fhistQMeanPad->Draw();*/
  fhistQMean = new TH1D((barName + "-QMean").c_str(), (barName + "-QMean").c_str(), nbins, lqstart, lqend);

  /*fhistDelTPad = new TPad( (barName+"-DelTPad").c_str(),(barName+"-DelTPad").c_str(),
                 2*padWidth,barIndex*padHeight,
                 3*padWidth,(barIndex+1)*padHeight);
  fhistDelTPad->Draw();*/
  fhistDelT = new TH1D((barName + "-DelT").c_str(), (barName + "-DelT").c_str(), 200, -25000, 25000);

  /*fhistDelTCorrectedPad = new TPad( (barName+"-DelTCorrectedPad").c_str(),(barName+"-DelTCorrectedPad").c_str(),
                3*padWidth,barIndex*padHeight,
                4*padWidth,(barIndex+1)*padHeight);
  fhistDelTCorrectedPad->Draw();*/
  fhistDelTCorrected =
      new TH1D((barName + "-DelTCorrected").c_str(), (barName + "-DelTCorrected").c_str(), 200, -25000, 25000);

  fhistQMeanCorrected =
      new TH1D((barName + "-QMeanCorrected").c_str(), (barName + "-QMeanCorrected").c_str(), nbins, 0, 40);
  fhistDelTvsZ =
      new TH2F((barName + "-DelTvsZ").c_str(), (barName + "-DelTvsZ").c_str(), 500, -25., 25., 500, -100., 100.);
}

void Histograms::FillHistogram(lite_interface::ScintillatorBar_V2 *scintBar)
{
  fhistQNear->Fill(scintBar->fQlongNear);
  fhistQFar->Fill(scintBar->GetQLongFar());
  fhistQMean->Fill(scintBar->fQlongMean);
  fhistDelT->Fill(scintBar->fDelTstamp);
  fhistDelTCorrected->Fill(scintBar->GetDelTCorrected());
  fhistQMeanCorrected->Fill(scintBar->GetQMeanCorrected());
  fhistDelTvsZ->Fill(scintBar->GetDelTCorrected()/1000., scintBar->EstimateHitPosition_Param()->GetZ());
}
#if (0)
// Working
void Histograms::FillCorrectedQMean()
{
  fhistQMeanCorrected = new TH1D((fBarName + "-QMeanCorrected").c_str(), (fBarName + "-QMeanCorrected").c_str(), nbins,
                                 qstart + fEnergyCalibrationFactor, qend + fEnergyCalibrationFactor);
  // fhistQMeanCorrected = fhistQMean*fEnergyCalibrationFactor;//->Fill(scintBar->qlongMeanCorrected);
  for (int i = 1; i <= fhistQMean->GetXaxis()->GetNbins(); i++) {

    // double temp = fhistQMean->GetBinContent(i);
    /*if(i==10){
         std::cout << "For Bar : " << fBarName <<" : UnCorrected : " << fhistQMean->GetBinContent(i) <<" : Corrected : "
     << temp << std::endl;
     }*/

    double xval = fhistQMean->GetXaxis()->GetBinCenter(i);
    // xval += fEnergyCalibrationFactor;
    int binNum = fhistQMean->GetXaxis()->FindBin(xval);
    fhistQMeanCorrected->SetBinContent(binNum, fhistQMean->GetBinContent(i));
  }
}
#endif

Histograms::~Histograms()
{
  // TODO Auto-generated destructor stub
}

void Histograms::DoSinglePointEnergyCalibrationForMuon()
{
  int binmax = fhistQMean->GetMaximumBin();
  double bin = fhistQMean->GetXaxis()->GetBinCenter(binmax);

  int revBin               = fhistQMean->GetXaxis()->FindBin(bin);
  fEnergyCalibrationFactor = (1.0 * muonEnergyPeak) - bin; //(1.0*muonEnergyPeak)/(double)bin;
  std::cout << "Energy Calibration factor of " << fBarName << " : PeakAt : " << bin << " : BinMax :" << binmax
            << " : RevBinMax : " << revBin << " : " << fEnergyCalibrationFactor << std::endl;
}
