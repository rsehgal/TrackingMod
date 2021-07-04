#include "Plotter.h"
#include <iterator>
#include "HardwareNomenclature.h"
#include "HelperFunctions.h"
#include "Analyzer.h"
#include "ScintillatorBar_V2.h"
#include "SingleMuonTrack.h"
#include <TGraph.h>
#include <TVector3.h>

namespace lite_interface {

TH1F *PlotZenithAngle(std::vector<std::vector<lite_interface::Point3D *>> vecOfVecOfPoint3D)
{
  TVector3 ref(0., -1., 0.);
  int numOfBins = 50;
  std::vector<double> vecOfZenithAngle;
  for (unsigned int i = 0; i < vecOfVecOfPoint3D.size(); i++) {
    vecOfZenithAngle.push_back(M_PI - GetZenithAngle(vecOfVecOfPoint3D[i]));
  }

  return PlotZenithAngle(vecOfZenithAngle, 5);
}

ULong64_t GetMuonTrackTimeWindow(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec)
{
  ULong64_t timeWindow = 20000;
  ULong64_t startTime  = 0;
  ULong64_t endTime    = 0;
  std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
  bool startFound      = false;
  bool endFound        = false;
  TH1F *timeWindowHist = new TH1F("TimeWindowHist", "TimeWindowHist", 50, 0, 30);
  int counter          = 0;
  for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
    /*if(!startFound && !endFound)
      std::cout << "@@@@ Correctly set to False values @@@@" << std::endl;*/

    // if((*itr)->GetLayerIndex() == 0){
    if ((*itr)->GetLayerIndex() == (numOfLayers - 1)) {
      startFound = true;
      // endTime = (*itr)->GetTSmallTimestamp();
      startTime = (*itr)->GetTSmallTimestamp();
    }

    // if((*itr)->GetLayerIndex() == (numOfLayers-1)){
    if ((*itr)->GetLayerIndex() == 0) {
      endFound = true;
      // startTime = (*itr)->GetTSmallTimestamp();
      endTime = (*itr)->GetTSmallTimestamp();
    }

    if (startFound && endFound) {
      startFound = false;
      endFound   = false;
      counter++;
      Long64_t delt = (endTime - startTime);
      std::cout << "STart Time : " << startTime << " : EndTime : " << endTime << " :  (EndTime - StartTime) : " << delt
                << std::endl;

      timeWindow = delt / 1000.;
      timeWindowHist->Fill(timeWindow);
    }
  }
  // GetTSmallTimestamp

  TFile *fp = new TFile("PassageTime.root", "RECREATE");
  fp->cd();
  timeWindowHist->Write();
  fp->Close();

  new TCanvas("TimeWindowHist", "TimeWindowHist");
  timeWindowHist->Draw();

  std::cout << "@@@@@@@@ COUNTER Val : " << counter << " @@@@@@@@" << std::endl;

  return timeWindow;
}

TH1F *PlotQ(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec, ushort barIndex)
{
  TH1F *hist = new TH1F("RawHist", "RawHist", 40000, 0, 40000);
  std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
  for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
    if ((*itr)->fBarIndex == barIndex) {
      hist->Fill((*itr)->GetQLongMean());
    }
  }
  return hist;
}

TH1F *PlotQ_0123(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec, ushort barIndex, ushort opt,
                 bool energKev)
{
  TH1F *hist;
  unsigned int nbins = 1000;
  unsigned int start = 0;
  unsigned int end   = 40;
  if (opt == 0) {
    std::string barName = vecOfBarsNamess[barIndex].substr(0, 4) + "_QMean";
    // hist = new TH1F("HistQMean","HistQMean",nbins,start,end);
    // hist = new TH1F(barName.c_str(),barName.c_str(),nbins,start,end);

    if (energKev)
      hist = new TH1F(barName.c_str(), barName.c_str(), nbins, start * 1000, end * 1000);
    else
      hist = new TH1F(barName.c_str(), barName.c_str(), nbins, start, end);

    std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
    for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
      if (IsSimulation) {
        if (barIndex == 65535) {
          hist->Fill((*itr)->GetQLongMean());
        } else {
          if ((*itr)->fBarIndex == barIndex) {
            hist->Fill((*itr)->GetQLongMean());
          }
        }
      } else {
        if (barIndex == 65535) {
          hist->Fill((*itr)->GetQLongMean() / 1000.);
        } else {
          if ((*itr)->fBarIndex == barIndex) {
            if (energKev)
              hist->Fill((*itr)->GetQLongMean());
            else
              hist->Fill((*itr)->GetQLongMean() / 1000.);
          }
        }
      }
    }
  }

  if (opt == 1) {
    std::string barName = vecOfBarsNamess[barIndex].substr(0, 4) + "_QNear";
    // hist = new TH1F("HistQNear","HistQNear",nbins,start,end);
    hist = new TH1F(barName.c_str(), barName.c_str(), nbins, start, end);
    std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
    for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
      if (barIndex == 65535) {
        hist->Fill((*itr)->GetQLongNear() / 1000.);
      } else {
        if ((*itr)->fBarIndex == barIndex) {
          hist->Fill((*itr)->GetQLongNear() / 1000.);
        }
      }
    }
  }

  if (opt == 2) {
    std::string barName = vecOfBarsNamess[barIndex].substr(0, 4) + "_QFar";
    // hist = new TH1F("HistQFar","HistQFar",nbins,start,end);
    hist = new TH1F(barName.c_str(), barName.c_str(), nbins, start, end);
    std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
    for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
      if (barIndex == 65535) {
        hist->Fill((*itr)->GetQLongFar() / 1000.);
      } else {
        if ((*itr)->fBarIndex == barIndex) {
          hist->Fill((*itr)->GetQLongFar() / 1000.);
        }
      }
    }
  }
  if (opt == 3) {
    std::string barName = vecOfBarsNamess[barIndex].substr(0, 4) + "_QMean" + "_Corr";
    // hist = new TH1F("HistQMeanCorrected","HistQMeanCorrected",nbins,start,end);
    hist = new TH1F(barName.c_str(), barName.c_str(), nbins, start, end);
    std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
    for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
      if (barIndex == 65535) {
        // std::cout << (*itr)->GetBarIndex() << std::endl;
        hist->Fill((*itr)->GetQMeanCorrected());
      } else {
        if ((*itr)->fBarIndex == barIndex) {
          hist->Fill((*itr)->GetQMeanCorrected());
        }
      }
    }
  }
  hist->Scale(1 / hist->Integral());
  return hist;
}

TH2F *PlotHitPointOnLayer(std::vector<lite_interface::SingleMuonTrack *> smtVec, unsigned int inspectedLayerIndex)
{
  std::string name   = "HitPointOnLayer_" + std::to_string(inspectedLayerIndex);
  TH2F *hist2D_Layer = new TH2F(name.c_str(), name.c_str(), 200, -50, 50, 200, -50, 50);
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    unsigned int hittBarIndex = 10000;
    ushort startIndex         = GetStartIndex(inspectedLayerIndex);
    ushort endIndex           = GetEndIndex(inspectedLayerIndex);
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
      lite_interface::Point3D *hitPoint = Get3DHitPointOnLayer(smtVec[i], inspectedLayerIndex);
      if (hitPoint->GetX() < 9000. && hitPoint->GetZ() < 9000 &&
          scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer()) {
        hist2D_Layer->Fill(hitPoint->GetX(), hitPoint->GetZ());
      }
    }
  }
  return hist2D_Layer;
}

#ifdef USE_FOR_SIMULATION
TH2F *PlotExactHitPointOnLayer(std::vector<lite_interface::SingleMuonTrack *> smtVec, unsigned int inspectedLayerIndex)
{
  std::string name   = "ExactHitPointOnLayer_" + std::to_string(inspectedLayerIndex);
  TH2F *hist2D_Layer = new TH2F(name.c_str(), name.c_str(), 200, -50, 50, 200, -50, 50);
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    unsigned int hittBarIndex = 10000;
    ushort startIndex         = GetStartIndex(inspectedLayerIndex);
    ushort endIndex           = GetEndIndex(inspectedLayerIndex);
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
      lite_interface::Point3D *hitPoint =
          scint->GetExactHitPosition(); // Get3DHitPointOnLayer(smtVec[i], inspectedLayerIndex);
      if (hitPoint->GetX() < 9000. && hitPoint->GetZ() < 9000 &&
          scintStart->GetBarIndexInLayer() == scintEnd->GetBarIndexInLayer()) {
        hist2D_Layer->Fill(hitPoint->GetX(), hitPoint->GetZ());
      }
    }
  }
  return hist2D_Layer;
}
#endif

TH1F *PlotQMeanCorrected_V2(std::vector<ScintillatorBar_V2> scintBarVec, ushort barIndex)
{
  unsigned int nbins = 1000;
  unsigned int start = 0;
  unsigned int end   = 40;
  TH1F *hist         = new TH1F("HistQMeanCorrected", "HistQMeanCorrected", nbins, start, end);
  std::vector<ScintillatorBar_V2>::iterator itr;
  for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
    if (barIndex == 65535) {
      // std::cout << (*itr).GetBarIndex() << std::endl;
      hist->Fill((*itr).GetQMeanCorrected());
    } else {
      if ((*itr).fBarIndex == barIndex) {
        hist->Fill((*itr).GetQMeanCorrected());
      }
    }
  }
  return hist;
}

TH1F *PlotQMean(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort barIndex)
{
  unsigned int nbins  = 1000;
  unsigned int start  = 0;
  unsigned int end    = 40;
  std::string barName = vecOfBarsNamess[barIndex].substr(0, 4) + "_QMean_Corr";
  TH1F *hist          = new TH1F(barName.c_str(), barName.c_str(), nbins, start, end);
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = smtVec[i]->GetMuonTrack();
    std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
    for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
      // if((*itr)->GetQMeanCorrected() > 15)
      // if((*itr)->GetQLongMean() > 5000 && (*itr)->GetQLongMean() < 13000 )
      {
        if ((*itr)->fBarIndex == barIndex) {
          hist->Fill((*itr)->GetQLongMean() / 1000.);
          // hist->Fill((*itr)->GetQLongMean()/1000.);
          /*if(IsSimulation)
            hist->Fill((*itr)->GetDelTCorrected()/1000.);
          else
            hist->Fill((*itr)->GetDelTCorrected()/1000.);*/
        }
      }
    }
  }

  return hist;
}

TH1F *PlotQMeanCorrected(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort barIndex)
{
  unsigned int nbins  = 1000;
  unsigned int start  = 0;
  unsigned int end    = 40;
  std::string barName = vecOfBarsNamess[barIndex].substr(0, 4) + "_QMean_Corr";
  TH1F *hist          = new TH1F(barName.c_str(), barName.c_str(), nbins, start, end);
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = smtVec[i]->GetMuonTrack();
    std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
    for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
      // if((*itr)->GetQMeanCorrected() > 15)
      // if((*itr)->GetQLongMean() > 5000 && (*itr)->GetQLongMean() < 13000 )
      {
        if ((*itr)->fBarIndex == barIndex) {
          hist->Fill((*itr)->GetQMeanCorrected());
          // hist->Fill((*itr)->GetQLongMean()/1000.);
          /*if(IsSimulation)
            hist->Fill((*itr)->GetDelTCorrected()/1000.);
          else
            hist->Fill((*itr)->GetDelTCorrected()/1000.);*/
        }
      }
    }
  }

  return hist;
}

TH1F *PlotQMeanCorrected(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec, ushort barIndex)
{
  return PlotQ_0123(scintBarVec, barIndex, 3);
}

TH1F *PlotQMean(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec, ushort barIndex, bool energKev)
{
  return PlotQ_0123(scintBarVec, barIndex, 0, energKev);
}

TH1F *PlotQNear(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec, ushort barIndex)
{
  return PlotQ_0123(scintBarVec, barIndex, 1);
}

TH1F *PlotQFar(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec, ushort barIndex)
{
  return PlotQ_0123(scintBarVec, barIndex, 2);
}
TH1F *PlotDelT(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec, ushort barIndex)
{
  std::string barName = vecOfBarsNamess[barIndex].substr(0, 4) + "_DelT";
  // TH1F *hist = new TH1F("HistDelT","HistDelT",200,-25,25); //Histogram with entries in nanosecond
  TH1F *hist = new TH1F(barName.c_str(), barName.c_str(), 200, -25, 25); // Histogram with entries in nanosecond
  std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
  for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
    if ((*itr)->fBarIndex == barIndex) {
      // std::cout << "BarIndex : " << (*itr)->fBarIndex << " : DelT : " << (*itr)->GetDelT()/1000. << std::endl;
      if (IsSimulation)
        hist->Fill((*itr)->GetDelT() / 1000.);
      else
        hist->Fill((*itr)->GetDelT() / 1000.);
    }
  }
  return hist;
}

TH1F *PlotPixelDelTCorrected(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort barIndex1, ushort barIndex2)
{
  std::string barName = vecOfBarsNamess[barIndex2].substr(0, 4) + "_DelT" + "_Corr";
  TH1F *hist = new TH1F(barName.c_str(), barName.c_str(), 200, -25, 25); // Histogram with entries in nanosecond
  std::vector<unsigned int> vecOfScintId = {barIndex1, barIndex2};
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    if (smtVec[i]->CheckTrackForRequiredScintillators(vecOfScintId)) {
      std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = smtVec[i]->GetMuonTrack();
      std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
      for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
        if ((*itr)->GetQMeanCorrected() > 15) {
          if ((*itr)->fBarIndex == barIndex2) {
            hist->Fill((*itr)->GetDelTCorrected() / 1000.);
          }
        }
      }
    }
  }

  return hist;
}

TH1F *PlotPixelDelTBetweenBars(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort barIndex1,
                               ushort barIndex2)
{
  std::string barName = vecOfBarsNamess[barIndex2].substr(0, 4) + "_DelT" + "_Corr";
  TH1F *hist = new TH1F(barName.c_str(), barName.c_str(), 100, -15000, 15000); // Histogram with entries in nanosecond
  std::vector<unsigned int> vecOfScintId = {barIndex1, barIndex2};
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    if (smtVec[i]->CheckTrackForRequiredScintillators(vecOfScintId)) {
      std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = smtVec[i]->GetMuonTrack();
      std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
      ULong64_t first = 100000, second = 0;
      for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
        if ((*itr)->GetQMeanCorrected() > 15) {
          if ((*itr)->fBarIndex == barIndex1) {
            // hist->Fill((*itr)->GetDelTCorrected()/1000.);
            first = (*itr)->GetTNearCorr();
          }
          if ((*itr)->fBarIndex == barIndex2) {
            // hist->Fill((*itr)->GetDelTCorrected()/1000.);
            second = (*itr)->GetTNearCorr();
          }
        }
      }
      Long64_t diff = first - second;
      hist->Fill(diff);
    }
  }

  return hist;
}

TH1F *PlotQMeanCorrectedOfFirstBarWithRespectToSecond(std::vector<lite_interface::SingleMuonTrack *> smtVec,
                                                      ushort barIndex1, ushort barIndex2)
{
  unsigned int nbins  = 40;
  unsigned int start  = 0;
  unsigned int end    = 40;
  std::string barName = vecOfBarsNamess[barIndex2].substr(0, 4) + "_QMean_Corr";
  TH1F *hist          = new TH1F(barName.c_str(), barName.c_str(), nbins, start, end);
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = smtVec[i]->GetMuonTrack();
    std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
    double enerFirst = 0., enerSecond = 0.;
    for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
      // if((*itr)->GetQMeanCorrected() > 15)
      {
        if ((*itr)->fBarIndex == barIndex1) {
          enerFirst = (*itr)->GetQMeanCorrected();
        }

        if ((*itr)->fBarIndex == barIndex2) {
          enerSecond = (*itr)->GetQMeanCorrected();
        }
      }
    }
    // std::cout << "EnerFirst : " << enerFirst <<" : EnerSecond : " << enerSecond << std::endl;
    if (enerFirst > 10 && enerSecond > 10.) {

      hist->Fill(enerSecond);
    }
  }

  return hist;
}

TH2F *PlotQMeanCorrectedCorrelationOfFirstBarWithRespectToSecond(std::vector<lite_interface::SingleMuonTrack *> smtVec,
                                                                 ushort barIndex1, ushort barIndex2)
{
  unsigned int nbins  = 1000;
  unsigned int start  = 0;
  unsigned int end    = 50;
  std::string barName = vecOfBarsNamess[barIndex2].substr(0, 4) + "_QMean_Corr";
  std::string title   = "EnergyCorrelationPlot_" + std::to_string(barIndex1) + "_" + std::to_string(barIndex2);
  TH2F *hist          = new TH2F(title.c_str(), title.c_str(), nbins, start, end, nbins, start, end);
  std::ofstream outfile("debug.txt");
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = smtVec[i]->GetMuonTrack();
    std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
    double enerFirst = 0., enerSecond = 0.;
    for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
      // if((*itr)->GetQMeanCorrected() > 15)
      {
        if ((*itr)->fBarIndex == barIndex1) {
          enerFirst = (*itr)->GetQMeanCorrected();
          // enerFirst = (*itr)->GetQLongMean()/1000.;
        }

        if ((*itr)->fBarIndex == barIndex2) {
          enerSecond = (*itr)->GetQMeanCorrected();
          // enerSecond = (*itr)->GetQLongMean()/1000.;
        }
      }
    }
    // std::cout << "EnerFirst : " << enerFirst <<" : EnerSecond : " << enerSecond << std::endl;
    // if(enerFirst >= 5. && enerSecond >= 5. && enerFirst <= 15. && enerSecond <= 15. )
    if (enerFirst > 0. && enerSecond > 0.) {
      outfile << "Track Num : " << i << " : EnerFirst : " << enerFirst << " : EnerSecond : " << enerSecond << std::endl;
      hist->Fill(enerFirst, enerSecond);
    }
    /*if(enerFirst > 10 && enerSecond > 10.){

      hist->Fill(enerSecond);
    }*/
    // hist->Fill(enerFirst,enerSecond);
  }
  outfile.close();

  return hist;
}

TH1F *PlotDelTCorrected(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort barIndex)
{
  std::string barName = vecOfBarsNamess[barIndex].substr(0, 4) + "_DelT" + "_Corr";
  TH1F *hist = new TH1F(barName.c_str(), barName.c_str(), 200, -25, 25); // Histogram with entries in nanosecond

  for (unsigned int i = 0; i < smtVec.size(); i++) {

    std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = smtVec[i]->GetMuonTrack();
    std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
    for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
      // if((*itr)->GetQMeanCorrected() > 15)
      {
        if ((*itr)->fBarIndex == barIndex) {
          hist->Fill((*itr)->GetDelTCorrected() / 1000.);
          /*if(IsSimulation)
            hist->Fill((*itr)->GetDelTCorrected()/1000.);
          else
            hist->Fill((*itr)->GetDelTCorrected()/1000.);*/
        }
      }
    }
  }

  return hist;
}

TH1F *PlotDelTCorrected(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec, ushort barIndex)
{
  std::string barName = vecOfBarsNamess[barIndex].substr(0, 4) + "_DelT" + "_Corr";

  TH1F *hist = new TH1F(barName.c_str(), barName.c_str(), 200, -25, 25); // Histogram with entries in nanosecond
  std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
  for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
    if ((*itr)->fBarIndex == barIndex) {
      if (IsSimulation)
        hist->Fill((*itr)->GetDelTCorrected() / 1000.);
      else
        hist->Fill((*itr)->GetDelTCorrected() / 1000.);
    }
  }
  return hist;
}

TH1F *PlotBarsProfile(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec)
{
  // std::string barName = vecOfBarsNamess[barIndex];
  TH1F *hist = new TH1F("Bars Profile", "Bars Profile", numOfLayers * numOfBarsInEachLayer, 0,
                        numOfLayers * numOfBarsInEachLayer); // Histogram with entries in nanosecond
  std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
  for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {

    hist->Fill((*itr)->GetBarIndex());
  }
  return hist;
}

TH2F *PlotHitPointsOnBar(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec, ushort barIndex)
{
  ushort nbinsx    = 9;
  ushort nbinsz    = 10;
  std::string name = "Hit points on " + vecOfBarsNamess[barIndex];
  TH2F *hist       = new TH2F(name.c_str(), name.c_str(), nbinsx, -45, 45, nbinsz, -50, 50);
  std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
  for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
    if ((*itr)->fBarIndex == barIndex) {
      lite_interface::Point3D *hitPt = (*itr)->EstimateHitPosition();
      // hitPt->Print();
      hist->Fill(hitPt->GetX(), hitPt->GetZ());
    }
  }
  return hist;
}

TH2F *PlotHitPointsOnBar(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort barIndex)
{
  ushort nbinsx    = 9;
  ushort nbinsz    = 10;
  std::string name = "Hit points on " + vecOfBarsNamess[barIndex];
  TH2F *hist       = new TH2F(name.c_str(), name.c_str(), nbinsx, -45, 45, nbinsz, -50, 50);

  for (unsigned int i = 0; i < smtVec.size(); i++) {
    std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = smtVec[i]->GetMuonTrack();
    std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
    for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
      if ((*itr)->fBarIndex == barIndex) {
        lite_interface::Point3D *hitPt = (*itr)->EstimateHitPosition();
        // hitPt->Print();
        hist->Fill(hitPt->GetX(), hitPt->GetZ());
      }
    }
  }
  return hist;
}

TGraph *PlotHitPointsOnBar(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort barIndex1, ushort barIndex2)
{
  std::string barName = vecOfBarsNamess[barIndex2].substr(0, 4) + "_DelT" + "_Corr";
  // TH1F *hist = new TH1F(barName.c_str(),barName.c_str(),200,-25,25); //Histogram with entries in nanosecond
  std::vector<unsigned int> vecOfScintId = {barIndex1, barIndex2};
  std::vector<double> xvec;
  std::vector<double> yvec;
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    if (smtVec[i]->CheckTrackForRequiredScintillators(vecOfScintId)) {
      std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = smtVec[i]->GetMuonTrack();
      std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
      for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
        if ((*itr)->GetQMeanCorrected() > 15) {
          if ((*itr)->fBarIndex == barIndex1) {
            xvec.push_back((*itr)->EstimateHitPosition_Param()->GetZ());
          }
          if ((*itr)->fBarIndex == barIndex2) {
            yvec.push_back((*itr)->EstimateHitPosition_Param()->GetZ());
          }
        }
      }
    }
  }

  TGraph *gr = new TGraph(xvec.size(), &xvec[0], &yvec[0]);
  return gr;
}

TH2F *PlotHitPointsOnBarHist(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort barIndex1, ushort barIndex2)
{
  std::vector<unsigned int> vecOfScintId = {barIndex1, barIndex2};
  /*std::string barName = vecOfBarsNamess[barIndex2].substr(0,4)+"_HitPoint_Histogram";
  TH2F *hist = new TH2F(barName.c_str(),barName.c_str(),200,-100,100,200,-100,100); //Histogram with entries in
  nanosecond

  std::vector<double> xvec;
  std::vector<double> yvec;
  for(unsigned int i = 0 ; i < smtVec.size() ; i++){
    if(smtVec[i]->CheckTrackForRequiredScintillators(vecOfScintId)){
      std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec = smtVec[i]->GetMuonTrack();
      std::vector<lite_interface::ScintillatorBar_V2*>::iterator itr;
      double xval=-100,yval=-100;
      for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
        if((*itr)->GetQMeanCorrected() > 15){
          if((*itr)->fBarIndex == barIndex1){
            xvec.push_back((*itr)->EstimateHitPosition_Param()->GetZ());
            xval = (*itr)->EstimateHitPosition_Param()->GetZ();
          }
          if((*itr)->fBarIndex == barIndex2){
            yvec.push_back((*itr)->EstimateHitPosition_Param()->GetZ());
            yval = (*itr)->EstimateHitPosition_Param()->GetZ();
          }
        }
      }
      hist->Fill(xval,yval);
    }
  }*/

  TH2F *hist = PlotHitPointsOnBarHist(smtVec, vecOfScintId);

  return hist;
}

TH2F *PlotHitPointsOnBarHist(std::vector<lite_interface::SingleMuonTrack *> smtVec,
                             std::vector<unsigned int> vecOfScintId)
{
  std::string barName = vecOfBarsNamess[vecOfScintId[vecOfScintId.size() - 1]].substr(0, 4) + "_HitPoint_Histogram";
  TH2F *hist          = new TH2F(barName.c_str(), barName.c_str(), 200, -100, 100, 200, -100,
                        100); // Histogram with entries in nanosecond
  // std::vector<unsigned int> vecOfScintId = {barIndex1,barIndex2};
  std::vector<double> xvec;
  std::vector<double> yvec;
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    if (smtVec[i]->CheckTrackForRequiredScintillators(vecOfScintId)) {
      std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = smtVec[i]->GetMuonTrack();
      std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
      double xval = -100, yval = -100;
      for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
        if ((*itr)->GetQMeanCorrected() > 15) {
          // if((*itr)->fBarIndex == barIndex1){
          if ((*itr)->fBarIndex == vecOfScintId[0]) {
            xvec.push_back((*itr)->EstimateHitPosition_Param()->GetZ());
            xval = (*itr)->EstimateHitPosition_Param()->GetZ();
          }
          // if((*itr)->fBarIndex == barIndex2){
          if ((*itr)->fBarIndex == vecOfScintId[1]) {
            yvec.push_back((*itr)->EstimateHitPosition_Param()->GetZ());
            yval = (*itr)->EstimateHitPosition_Param()->GetZ();
          }
        }
      }
      hist->Fill(xval, yval);
    }
  }

  return hist;
}

TH1F *PlotStripProfileOfLayer(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec, ushort layerIndex)
{
  std::string layerName = "Layer_" + std::to_string(layerIndex);
  TH1F *hist = new TH1F(layerName.c_str(), layerName.c_str(), numOfBarsInEachLayer, layerIndex * numOfBarsInEachLayer,
                        (layerIndex + 1) * numOfBarsInEachLayer);

  std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
  for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
    if ((*itr)->GetQMeanCorrected() > 15) {
      if (((*itr)->fBarIndex >= layerIndex * numOfBarsInEachLayer) &&
          ((*itr)->fBarIndex < (layerIndex + 1) * numOfBarsInEachLayer)) {
        // std::cout << "BAR INDEX : " << (*itr)->fBarIndex << std::endl;
        hist->Fill((*itr)->fBarIndex);
      }
    }
  }

  return hist;
}

TH1F *PlotStripProfileOfVectorOfLayer(std::vector<lite_interface::SingleMuonTrack *> smtVec,
                                      std::vector<unsigned short> vecOfLayerIndices)
{
  std::string name = "Strip_profile_using_";
  for (unsigned int i = 0; i < vecOfLayerIndices.size(); i++) {
    name += (std::to_string(vecOfLayerIndices[i]) + "_");
  }
  name += "Layers";
  TH1F *stripProfile = new TH1F(name.c_str(), name.c_str(), 90, 0, 90);
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = smtVec[i]->GetMuonTrack();
    std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
    for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
      if ((*itr)->GetQMeanCorrected() > 15) {
        // if(((*itr)->fBarIndex >= layerIndex*numOfBarsInEachLayer) &&
        //	((*itr)->fBarIndex < (layerIndex+1)*numOfBarsInEachLayer))
        if (std::find(vecOfLayerIndices.begin(), vecOfLayerIndices.end(), (*itr)->GetLayerIndex()) !=
            vecOfLayerIndices.end()) {

          // std::cout << "BAR INDEX : " << (*itr)->fBarIndex << std::endl;
          stripProfile->Fill((*itr)->fBarIndex);
        }
      }
    }
  }

  return stripProfile;
}

TH1F *PlotStripProfileOfLayer(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort layerIndex)
{
  std::string layerName = "Layer_" + std::to_string(layerIndex);
  TH1F *hist = new TH1F(layerName.c_str(), layerName.c_str(), numOfBarsInEachLayer, layerIndex * numOfBarsInEachLayer,
                        (layerIndex + 1) * numOfBarsInEachLayer);
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = smtVec[i]->GetMuonTrack();
    std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
    for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
      if ((*itr)->GetQMeanCorrected() > 15) {
        if (((*itr)->fBarIndex >= layerIndex * numOfBarsInEachLayer) &&
            ((*itr)->fBarIndex < (layerIndex + 1) * numOfBarsInEachLayer)) {
          // std::cout << "BAR INDEX : " << (*itr)->fBarIndex << std::endl;
          hist->Fill((*itr)->fBarIndex);
        }
      }
    }
  }

  return hist;
}

TH1F *PlotHitMultiplicityOfLayer(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort layerIndex)
{
  std::string layerName = "HitMultiplicity_Layer_" + std::to_string(layerIndex);
  TH1F *hist = new TH1F(layerName.c_str(), layerName.c_str(), numOfBarsInEachLayer, 1, numOfBarsInEachLayer + 1);
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    hist->Fill(smtVec[i]->GetNumOfHittedScintillatorsInLayer(layerIndex));
  }
  return hist;
}

std::vector<double> GetXYonPixel(lite_interface::SingleMuonTrack *smt, std::vector<unsigned int> vecOfScintId)
{

  std::vector<double> xy;

  if (smt->CheckTrackForRequiredScintillators(vecOfScintId)) {
    std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = smt->GetMuonTrack();
    std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
    double xval = -100, yval = -100;
    for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
      bool xval_bool = false;
      bool yval_bool = false;
      if ((*itr)->GetQMeanCorrected() > 15) {
        // if((*itr)->fBarIndex == barIndex1){
        if ((*itr)->fBarIndex == vecOfScintId[0]) {
          xval_bool = true;
          xval      = (*itr)->EstimateHitPosition_Param()->GetZ();
          xy.push_back(xval);
        }
        // if((*itr)->fBarIndex == barIndex2){
        if ((*itr)->fBarIndex == vecOfScintId[1]) {
          yval_bool = true;
          yval      = (*itr)->EstimateHitPosition_Param()->GetZ();
          xy.push_back(yval);
        }
      }
    }
    return xy;
  }
}

/*
 * This function will generate the Hit point on the pixel specified layer.
 *
 * Layer next to specified one, will be used to form the pixel
 */
TH2F *PlotHitPointsOnLayerHist(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort layerIndex)
{
  std::vector<unsigned int> vecOfLayers;
  vecOfLayers.push_back(layerIndex);
  vecOfLayers.push_back(layerIndex + 1);
  std::string Layer = "Layer_" + std::to_string(layerIndex);
  TH2F *hist = new TH2F(Layer.c_str(), Layer.c_str(), 10, -50, 50, 10, -50, 50); // Histogram with entries in nanosecond
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    // double xval=-100,yval=-100;
    std::vector<unsigned int> hittedBarVec = smtVec[i]->CheckTrackForRequiredLayers(vecOfLayers);
    if (hittedBarVec.size() == vecOfLayers.size()) {
      std::vector<double> xy = GetXYonPixel(smtVec[i], hittedBarVec);
      if (xy.size() > 0) {
        if (xy[0] >= -50 && xy[0] <= 50 && xy[1] >= -50 && xy[1] <= 50 && std::fabs(xy[1]) > 0.01)
          hist->Fill(xy[0], xy[1]);
      }
    }
  }

  return hist;
}

TH1F *CalculateZResolution(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec, ushort barIndex)
{

  TF1 *param = lite_interface::Calibration::instance()->GetCalibrationDataOf(barIndex)->fParameterization_F;

  std::string barName = vecOfBarsNamess[barIndex].substr(0, 4) + "_Resolution";
  TH1F *hist          = new TH1F(barName.c_str(), barName.c_str(), 100, -50, 50);
  std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
  for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {

    if ((*itr)->fBarIndex == barIndex) {
      double delT_NS = (*itr)->GetDelTCorrected() / 1000.;
      // if(delT_NS > -2 && delT_NS < 2)
      {
        double estZ = param->Eval(delT_NS);
        hist->Fill(estZ);
      }
    }
  }
  return hist;
}

TGraph *PlotDelTvsZ(unsigned int barIndex, std::vector<float> delTVec, std::vector<float> zVec)
{
  std::string barName = vecOfBarsNamess[barIndex].substr(0, 4) + "_DelTvsZ";
  TGraph *delTvsZ     = new TGraph(delTVec.size(), &delTVec[0], &zVec[0]);
  delTvsZ->SetTitle(barName.c_str());
  return delTvsZ;
}

//#ifdef USE_FOR_SIMULATION
#if (0)
TGraph *PlotDelTvsZ(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec, ushort barIndex, bool linear)
{
  ushort nbinsx = 9;
  ushort nbinsz = 10;
  // std::string name = "DelT Vs Z : "+vecOfBarsNamess[barIndex];
  std::string barName = vecOfBarsNamess[barIndex].substr(0, 4) + "_DelTvsZ";
  std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
  std::vector<double> zVec;
  std::vector<double> delTVec;
  for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
    if ((*itr)->fBarIndex == barIndex) {
#ifdef USE_FOR_SIMULATION
      zVec.push_back((*itr)->hitZ);
#else
      if (linear)
        zVec.push_back((*itr)->EstimateHitPosition()->GetZ());
      else
        zVec.push_back((*itr)->EstimateHitPosition_Param()->GetZ());
#endif
      delTVec.push_back((*itr)->fDelTstamp / 1000.);
    }
  }
  TGraph *delTvsZ = new TGraph(delTVec.size(), &delTVec[0], &zVec[0]);
  delTvsZ->SetTitle(barName.c_str());
  return delTvsZ;
}
#endif
//#endif

TH2F *PlotDelTvsZ(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec, ushort barIndex, bool linear)
{
  ushort nbinsx = 9;
  ushort nbinsz = 10;
  // std::string name = "DelT Vs Z : "+vecOfBarsNamess[barIndex];
  std::string barName = vecOfBarsNamess[barIndex].substr(0, 4) + "_DelTvsZ";
  std::vector<lite_interface::ScintillatorBar_V2 *>::iterator itr;
  std::vector<double> zVec;
  std::vector<double> delTVec;

  // TH2F *delTvsZ = new TH2F("DeltVsZ","DeltVsZ",1000,-30,30,1000,-500,500);
  TH2F *delTvsZ = new TH2F("DeltVsZ", "DeltVsZ", 1000, -10, 10, 1000, -60, 60);
  for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {
    if ((*itr)->fBarIndex == barIndex) {
#ifdef USE_FOR_SIMULATION
      zVec.push_back((*itr)->hitZ);
#else
      if (linear)
        zVec.push_back((*itr)->EstimateHitPosition()->GetZ());
      else
        zVec.push_back((*itr)->EstimateHitPosition_Param()->GetZ());
#endif
      delTVec.push_back((*itr)->fDelTstamp / 1000.);

      float zval = (*itr)->EstimateHitPosition_Param()->GetZ();
      if (zval > -50 && zval < 50) delTvsZ->Fill((*itr)->GetDelTCorrected() / 1000., zval);
    }
  }
  // TGraph *delTvsZ = new TGraph(delTVec.size(),&delTVec[0],&zVec[0]);
  // delTvsZ->SetTitle(barName.c_str());
  return delTvsZ;
}

TGraphErrors *PlotMuonTrack(lite_interface::SingleMuonTrack *smt, int opt)
{
  return PlotMuonTrack(smt->Get3DHitPointVector(), opt);
}

TGraphErrors *PlotMuonTrack(std::vector<lite_interface::Point3D *> vecOfPoint3D, int opt)
{
  std::vector<Double_t> xVec, yVec, zVec;
  std::vector<Double_t> xVecErr, yVecErr, zVecErr;
  std::vector<lite_interface::Point3D *>::iterator itr;
  for (itr = vecOfPoint3D.begin(); itr != vecOfPoint3D.end(); itr++) {
    // std::cout <<"=================" << std::endl;
    //(*itr)->Print();
    xVec.push_back((*itr)->GetX());
    xVecErr.push_back(5);
    yVec.push_back((*itr)->GetY());
    yVecErr.push_back(5.);
    zVec.push_back((*itr)->GetZ());
    zVecErr.push_back(5.);
  }
  /*TGraphErrors *grxy = new TGraphErrors(xVec.size(), &xVec[0], &yVec[0],&xVecErr[0],&yVecErr[0]);
  grxy->SetMarkerStyle(8);
  grxy->SetMarkerColor(kMagenta);
  TGraphErrors *grzy = new TGraphErrors(zVec.size(), &zVec[0], &yVec[0], &zVecErr[0], &yVecErr[0]);
  grzy->SetMarkerStyle(8);
  grzy->SetMarkerColor(kBlue);
  */
  if (opt == 1) {
    TGraphErrors *grxy = PlotMuonTrack(xVec, yVec);
    grxy->SetMarkerColor(kMagenta);
    return grxy;
  }
  if (opt == 2) {
    TGraphErrors *grzy = PlotMuonTrack(zVec, yVec);
    grzy->SetMarkerColor(kBlue);
    return grzy;
  }
}

TGraphErrors *PlotMuonTrack(std::vector<Double_t> xVec, std::vector<Double_t> yVec)
{
  std::vector<Double_t> xVecErr, yVecErr; //, zVecErr;

  for (unsigned int i = 0; i < xVec.size(); i++) {
    xVecErr.push_back(5);
    yVecErr.push_back(5.);
    // zVecErr.push_back(5.);
  }

  TGraphErrors *grxy = new TGraphErrors(xVec.size(), &xVec[0], &yVec[0], &xVecErr[0], &yVecErr[0]);
  grxy->SetMarkerStyle(8);

  return grxy;
}

TGraphErrors *PlotMuonTrackXY(lite_interface::SingleMuonTrack *smt)
{
  return PlotMuonTrack(smt, 1);
}

TGraphErrors *PlotMuonTrackZY(lite_interface::SingleMuonTrack *smt)
{
  return PlotMuonTrack(smt, 2);
}

TGraphErrors *PlotMuonTrackXY(std::vector<lite_interface::Point3D *> vecOfPoint3D)
{
  return PlotMuonTrack(vecOfPoint3D, 1);
}

TGraphErrors *PlotMuonTrackZY(std::vector<lite_interface::Point3D *> vecOfPoint3D)
{
  return PlotMuonTrack(vecOfPoint3D, 2);
}

TH1F *PlotEnergySum(std::vector<lite_interface::SingleMuonTrack *> smtVec)
{
  TH1F *energSumHist = new TH1F("EnergySumHist", "Histogram of Energy Sum", 1000, 0, 400);
  std::vector<lite_interface::SingleMuonTrack *>::iterator itr;
  int count = 0;
  for (itr = smtVec.begin(); itr != smtVec.end(); itr++) {
    // std::cout << "@@@@@@@@@@@@@@@ EVENT : " << count++ << " @@@@@@@@@@@@@@@@@@@" << std::endl;
    //(*itr)->Print();
    energSumHist->Fill((*itr)->GetEnergySum());
  }
  // energSumHist->Scale(1/energSumHist->Integral());
  return energSumHist;
}

std::vector<TH1D *> PlotEnergyDistributionWithMultiplicity(std::vector<unsigned int> sizeVector,
                                                           std::vector<double> energySumVector)
{

  std::vector<TH1D *> vecOfHists;
  for (unsigned int i = 0; i < numOfLayers; i++) {
    std::cout << "Inserting histogram for layer : " << i << std::endl;
    std::string title = "layer-" + std::to_string(i);
    vecOfHists.push_back(new TH1D(title.c_str(), title.c_str(), 500, 10, 250));
    vecOfHists[i]->SetLineColor(i + 1);
  }

  std::cout << "Szie of MuonTrcke Vector : " << sizeVector.size() << std::endl;

  for (unsigned int i = 0; i < sizeVector.size(); i++) {
    if (sizeVector[i] > 0 && sizeVector[i] <= numOfLayers) {
      if (energySumVector[i] > 0. && energySumVector[i] < 400.) vecOfHists[sizeVector[i] - 1]->Fill(energySumVector[i]);
    }
  }

  for (unsigned int i = 0; i < vecOfHists.size(); i++) {
    std::cout << "NEntries in hist : " << i << " : " << vecOfHists[i]->GetEntries() << std::endl;
    // if(vecOfHists[i]->GetEntries() > 0)
    // vecOfHists[i]->Scale(1/vecOfHists[i]->Integral());
  }
  return vecOfHists;
}

std::vector<TH1D *> PlotEnergyDistributionWithMultiplicity(std::vector<SingleMuonTrack *> muonTrackVec)
{

  std::vector<TH1D *> vecOfHists;
  for (unsigned int i = 0; i < numOfLayers; i++) {
    std::cout << "Inserting histogram for layer : " << i << std::endl;
    std::string title = "layer-" + std::to_string(i);
    vecOfHists.push_back(new TH1D(title.c_str(), title.c_str(), 500, 10, 250));
    vecOfHists[i]->SetLineColor(i + 1);
  }

  std::cout << "Szie of MuonTrcke Vector : " << muonTrackVec.size() << std::endl;

  for (unsigned int i = 0; i < muonTrackVec.size(); i++) {
    if (muonTrackVec[i]->size() > 0 && muonTrackVec[i]->size() <= numOfLayers) {
      // std::cout << "Size : " << muonTrackVec[i]->size() <<" : EnergySum : " << muonTrackVec[i]->GetEnergySum() <<
      // std::endl;
      double energySum = 0.;
      /*
       * To put the constraint to have single hit in each layer
       * uncomment the line below
       */
      if (muonTrackVec[i]->SingleHitInEachLayer()) {
        energySum = muonTrackVec[i]->GetEnergySum(); /// 1000.;

        if (energySum > 0. && energySum < 400.) vecOfHists[muonTrackVec[i]->size() - 1]->Fill(energySum);
      }
    }
  }

  for (unsigned int i = 0; i < vecOfHists.size(); i++) {
    std::cout << "NEntries in hist : " << i << " : " << vecOfHists[i]->GetEntries() << std::endl;
    /*if(vecOfHists[i]->GetEntries() > 0)
      vecOfHists[i]->Scale(1/vecOfHists[i]->Integral());*/
  }
  return vecOfHists;
}
// TCanvas* PlotEnergyDistributionWithMultiplicity(std::vector<SingleMuonTrack*> muonTrackVec, unsigned int
// multiplicity){
std::vector<TH1D *> PlotEnergyDistributionWithMultiplicity_Old(std::vector<SingleMuonTrack *> muonTrackVec,
                                                               unsigned int multiplicity)
{

  std::vector<short int> multiplicityVec;
  if (multiplicity == 0) {
    for (unsigned int i = 0; i < numOfLayers; i++) {
      multiplicityVec.push_back(i + 1);
    }
  } else {
    multiplicityVec.push_back(multiplicity);
  }

  TCanvas *can = new TCanvas();
  if (multiplicityVec.size() != 1) {
    float t             = sqrt(multiplicityVec.size());
    unsigned short rows = std::floor(t);
    unsigned short cols = std::ceil(t);
    can->Divide(cols, rows);
  }

  if (multiplicityVec.size() == 1) {
    char *title                      = Form("Energy histogram for Multiplicity %d", multiplicity);
    TH1D *histEnergyWithMultiplicity = new TH1D(title, title, 500, 100000, 250000);
    for (unsigned int i = 0; i < muonTrackVec.size(); i++) {
      if (muonTrackVec[i]->size() == multiplicity) {
        histEnergyWithMultiplicity->Fill(muonTrackVec[i]->GetEnergySum());
      }
    }
    histEnergyWithMultiplicity->Draw();
  } else {
    std::vector<TH1D *> vecOfHists;
    for (unsigned int i = 0; i < multiplicityVec.size(); i++) {
      char *str = Form("Multiplicity-%d", i + 1);
      int xlow  = multiplicityVec[i] * 20000. - 60000;
      int xhigh = multiplicityVec[i] * 20000. + 60000;
      xlow      = 0;
      xhigh     = 240000;
      vecOfHists.push_back(new TH1D(str, str, 50, xlow, xhigh));
      vecOfHists[i]->GetXaxis()->SetTitle("Energy sum");
      vecOfHists[i]->GetXaxis()->CenterTitle(true);
      vecOfHists[i]->GetYaxis()->SetTitle("Normalized Counts");
      vecOfHists[i]->GetYaxis()->CenterTitle(true);
    }
    for (unsigned int i = 0; i < muonTrackVec.size(); i++) {
      // std::cout << "BREAK at Muon Track vector of size :  " << (muonTrackVec[i]->fSingleMuonTrack).size() <<
      // std::endl;
      if (muonTrackVec[i]->size() <= numOfLayers && muonTrackVec[i]->size() > 0)
        vecOfHists[muonTrackVec[i]->size() - 1]->Fill(muonTrackVec[i]->GetEnergySum());
    }
    for (unsigned int i = 0; i < multiplicityVec.size(); i++) {
      can->cd(i + 1);
      vecOfHists[i]->Draw();
    }

    TLegend *legend = new TLegend(); // 0.1,0.7,0.48,0.9);
    legend->SetHeader("Multiplicity", "C");
    new TCanvas();

    for (unsigned int i = 0; i < multiplicityVec.size(); i++) {
      vecOfHists[i]->SetLineColor(i + 1);
      vecOfHists[i]->Scale(1 / vecOfHists[i]->Integral());
      legend->AddEntry(vecOfHists[i], Form("Multiplicity_%d", i + 1), "l");
      vecOfHists[i]->GetXaxis()->SetNdivisions(12);
      vecOfHists[i]->Draw("same");
    }
    legend->Draw();

    return vecOfHists;
  }

  // return can;

  // histEnergyWithMultiplicity->Draw();
}

TF1 *GetFittedMuonTrackFormulaXY(std::vector<lite_interface::Point3D *> vecOfPoint3D, bool exact)
{
  return GetFittedMuonTrackFormula(vecOfPoint3D, exact, true);
}

TF1 *GetFittedMuonTrackFormulaZY(std::vector<lite_interface::Point3D *> vecOfPoint3D, bool exact)
{
  return GetFittedMuonTrackFormula(vecOfPoint3D, exact, false);
}

TF1 *GetFittedMuonTrackFormula(std::vector<lite_interface::Point3D *> vecOfPoint3D, bool exact, bool xy)
{
  std::vector<Double_t> xVec, yVec, zVec;
  std::vector<Double_t> xVecErr, yVecErr, zVecErr;
  std::vector<lite_interface::Point3D *>::iterator itr;
  for (itr = vecOfPoint3D.begin(); itr != vecOfPoint3D.end(); itr++) {
    xVec.push_back((*itr)->GetX());
    yVec.push_back((*itr)->GetY());
    zVec.push_back((*itr)->GetZ());
    if (exact) {
      xVecErr.push_back(errorX_Exact);
      yVecErr.push_back(errorY_Exact);
      zVecErr.push_back(errorZ_Exact);
    } else {
      xVecErr.push_back(errorX_Smeared);
      yVecErr.push_back(errorY_Smeared);
      zVecErr.push_back(errorZ_Smeared);
    }
  }
  TGraphErrors *gr;
  if (xy)
    gr = new TGraphErrors(xVec.size(), &xVec[0], &yVec[0], &xVecErr[0], &yVecErr[0]);
  else
    gr = new TGraphErrors(zVec.size(), &zVec[0], &yVec[0], &zVecErr[0], &yVecErr[0]);

  TF1 *formula = new TF1("Formula", LinearFit, -45, 45, 2);
  gr->Fit(formula, "qn");
  // gr->Fit(formula,"r");
  // double c = formula->GetParameter(0);
  // double m = formula->GetParameter(1);
  return formula;
}
std::vector<lite_interface::Point3D *> CreateFittedTrack(std::vector<lite_interface::Point3D *> vecOfPoint3D)
{
  std::vector<Double_t> xVec, yVec, zVec;
  std::vector<Double_t> xVecErr, yVecErr, zVecErr;
  std::vector<lite_interface::Point3D *>::iterator itr;
  for (itr = vecOfPoint3D.begin(); itr != vecOfPoint3D.end(); itr++) {
    // std::cout <<"=================" << std::endl;
    //(*itr)->Print();
    xVec.push_back((*itr)->GetX());
    xVecErr.push_back(errorX);
    yVec.push_back((*itr)->GetY());
    yVecErr.push_back(errorY);
    zVec.push_back((*itr)->GetZ());
    zVecErr.push_back(10.);
  }
  // std::cout <<"================= Trying to create the Fitted Track from Class FittedTracks ==================== : "
  // << __FILE__ << " : " << __LINE__ << std::endl;
  TGraphErrors *grxy = new TGraphErrors(xVec.size(), &xVec[0], &yVec[0], &xVecErr[0], &yVecErr[0]);
  // std::cout << "++++++++++++++ Fit XY ++++++++++++++++" << std::endl;
  std::vector<double> fittedX = GetFittedXorZ(grxy, vecOfPoint3D);
  TGraphErrors *grzy          = new TGraphErrors(zVec.size(), &zVec[0], &yVec[0], &zVecErr[0], &yVecErr[0]);
  // std::cout << "++++++++++++++ Fit ZY ++++++++++++++++" << std::endl;
  std::vector<double> fittedZ = GetFittedXorZ(grzy, vecOfPoint3D);
  std::vector<Point3D *> fittedMuonTrack;
  for (unsigned int i = 0; i < xVec.size(); i++) {
    /*Point3D *pt = new Point3D(fittedX[i],yVec[i],fittedZ[i]);
    std::cout << "From CreateFitted Track : " << __FILE__ <<" : " << __LINE__ ; pt->Print();
    fittedMuonTrack.push_back(pt);*/
    fittedMuonTrack.push_back(new Point3D(fittedX[i], yVec[i], fittedZ[i]));
  }
  return fittedMuonTrack;
}

std::vector<double> GetFittedXorZ(TGraphErrors *gr, std::vector<lite_interface::Point3D *> vecOfPoint3D)
{
  std::vector<Double_t> xVec, yVec, zVec;
  std::vector<Double_t> xVecErr, yVecErr, zVecErr;
  std::vector<lite_interface::Point3D *>::iterator itr;
  for (itr = vecOfPoint3D.begin(); itr != vecOfPoint3D.end(); itr++) {
    // std::cout <<"=================" << std::endl;
    //(*itr)->Print();
    xVec.push_back((*itr)->GetX());
    xVecErr.push_back(5);
    yVec.push_back((*itr)->GetY());
    yVecErr.push_back(5.);
    zVec.push_back((*itr)->GetZ());
    zVecErr.push_back(5);
  }
  TF1 *formula = new TF1("Formula", LinearFit, -45, 45, 2);
  gr->Fit(formula, "qn");
  // gr->Fit(formula,"r");
  double c = formula->GetParameter(0);
  double m = formula->GetParameter(1);
  delete formula;

  std::vector<double> vecOfEstimatedXorZ;
  for (unsigned int i = 0; i < xVec.size(); i++) {
    vecOfEstimatedXorZ.push_back((yVec[i] - c) / m);
  }
  return vecOfEstimatedXorZ;
}

double GetDeviation(std::vector<lite_interface::Point3D *> incoming, std::vector<lite_interface::Point3D *> outgoing)
{
  if (incoming.size() > 0 && outgoing.size() > 0) {
    Point3D *incomingStart = incoming[0];
    Point3D *incomingEnd   = incoming[incoming.size() - 1];
    Point3D *outgoingStart = outgoing[0];
    Point3D *outgoingEnd   = outgoing[outgoing.size() - 1];
    TVector3 incomingTVec3(incomingEnd->GetX() - incomingStart->GetX(), incomingEnd->GetY() - incomingStart->GetY(),
                           incomingEnd->GetZ() - incomingStart->GetZ());

    TVector3 outgoingTVec3(outgoingEnd->GetX() - outgoingStart->GetX(), outgoingEnd->GetY() - outgoingStart->GetY(),
                           outgoingEnd->GetZ() - outgoingStart->GetZ());

    return outgoingTVec3.Angle(incomingTVec3);
  }
}

double GetZenithAngle(std::vector<lite_interface::Point3D *> vecOfPoint3D)
{
  TVector3 ref(0., -1., 0.);
  if (vecOfPoint3D.size() > 0) {
    Point3D *startPoint = vecOfPoint3D[0];
    Point3D *endPoint   = vecOfPoint3D[vecOfPoint3D.size() - 1];
    TVector3 muonDir(TVector3(endPoint->GetX(), endPoint->GetY(), endPoint->GetZ()) -
                     TVector3(startPoint->GetX(), startPoint->GetY(), startPoint->GetZ()));
    return muonDir.Angle(ref);
  }
}

int GetBinNumber(int nbins, double start, double end, double val)
{
  double binwidth = (end - start) / nbins;
  return (int)(val / binwidth);
}

TH1F *Plot_Acc_Corr_ZenithAngle(std::vector<double> zenithAngleVect, int opt)
{
  int numOfBins     = 50;
  double startangle = 0.02;
  double endangle   = 1.5;

  std::string title = "";
  if (opt == 1) title = "ZenithAngleLinear";
  if (opt == 2) title = "ZenithAngleParam";
  if (opt == 3) title = "ZenithAngleMeanHitPoint";
  if (opt == 3) title = "ZenithAngleCRY";
  TH1F *zenithAngleHist = new TH1F(title.c_str(), title.c_str(), numOfBins, startangle, endangle);
  // TH1F *zenithAngleHist = new TH1F(title.c_str(), title.c_str(),numOfBins,0.02,1.5);
  for (unsigned int i = 0; i < zenithAngleVect.size(); i++) {
    // if(zenithAngleVect[i] < 0.96)
    if (zenithAngleVect[i] < endangle) zenithAngleHist->Fill(zenithAngleVect[i]);
  }

  int startAcceptanceBinNum = GetBinNumber(numOfBins, startangle, endangle, M_PI / 4.);
  std::cout << "BIN CENTER : " << zenithAngleHist->GetBinCenter(startAcceptanceBinNum) << std::endl;
  /*long int denoBinContent = 0;
  for(unsigned int i = startAcceptanceBinNum ; i < zenithAngleHist->GetNbinsX(); i++){
    denoBinContent += zenithAngleHist->GetBinContent(i);
  }

  std::cout << "DenoContent : " << denoBinContent << std::endl;
   */
  long int startBinContent = zenithAngleHist->GetBinContent(startAcceptanceBinNum);
  // long int startBinContent = denoBinContent;
  for (unsigned int i = startAcceptanceBinNum; i < zenithAngleHist->GetNbinsX(); i++) {
    long int initialContent = zenithAngleHist->GetBinContent(i);
    double weightFactor     = (1.0 * initialContent / startBinContent);
    std::cout << "WeightFactor : " << weightFactor << std::endl;
    long int contentToSet = (weightFactor * initialContent);
    std::cout << "CONTENT TO SET : " << contentToSet << std::endl;
    zenithAngleHist->SetBinContent(i, contentToSet);
    std::cout << "Initial Bin Content : " << initialContent
              << " : Acc corr Bin Content : " << zenithAngleHist->GetBinContent(i) << std::endl;
  }

  zenithAngleHist->Scale(1 / zenithAngleHist->Integral());
  // TF1 *formula = new TF1("zenForm", "[0]*sin(x)*cos(x)*pow(cos(x),[1])", 0.05,0.96);
  TF1 *formula = new TF1("zenForm", "[0]*sin(x)*cos(x)*pow(cos(x),[1])", startangle, endangle);
  zenithAngleHist->Fit(formula, "r");
  return zenithAngleHist;
}

TH1F *GetSolidAngleCorrectedHist(TH1F *solidAngleHist)
{

  int nbins = 100;
  TH1F *solidAngleCorrectedHist =
      new TH1F(Form("Solid angle corrected %s", solidAngleHist->GetName()),
               Form("Solid angle corrected %s", solidAngleHist->GetName()), nbins, 0.05, M_PI / 2.);
  solidAngleCorrectedHist->GetYaxis()->SetTitle("I_{#theta}  ( cm^{-2}sec^{-1}st^{-1})");
  solidAngleCorrectedHist->GetXaxis()->SetTitle("#theta (radian)");
  for (int i = 0; i < nbins; i++) {
    double binCenter  = solidAngleHist->GetXaxis()->GetBinCenter(i);
    double binContent = solidAngleHist->GetBinContent(i);
    // std::cout << "binContent : " << binContent << " : binCenter : " << binCenter << std::endl;
    // solidAngleCorrectedHist->SetBinContent(i,binContent/(2*M_PI*std::sin(binCenter)*std::cos(binCenter)));
    solidAngleCorrectedHist->SetBinContent(i, binContent / (2 * M_PI * std::sin(binCenter) * std::cos(binCenter)));
  }
  return solidAngleCorrectedHist;
}

TH1F *PlotZenithAngle(std::vector<double> zenithAngleVect, int opt)
{
  int numOfBins     = 100;
  double startangle = 0.05;
  double endangle   = 0.96; // M_PI/2.;
  std::string title = "";
  if (opt == 1) title = "ZenithAngleLinear";
  if (opt == 2) title = "ZenithAngleParam";
  if (opt == 3) title = "ZenithAngleMeanHitPoint";
  if (opt == 4) title = "ZenithAngleCRY";
  if (opt == 5) title = "ZenithAngle_ML";
  // TH1F *zenithAngleHist = new TH1F(title.c_str(), title.c_str(),numOfBins,0.05,M_PI/2.);
  TH1F *zenithAngleHist = new TH1F(title.c_str(), title.c_str(), numOfBins, startangle, endangle);

  // TH1F *zenithAngleHist = new TH1F(title.c_str(), title.c_str(),numOfBins,0.02,1.5);
  for (unsigned int i = 0; i < zenithAngleVect.size(); i++) {
    // if(zenithAngleVect[i] < 0.96)
    // if(zenithAngleVect[i] < 1.5)
    zenithAngleHist->Fill(zenithAngleVect[i]);
  }
  // zenithAngleHist->Scale(1/zenithAngleHist->Integral());
  // TF1 *formula = new TF1("zenForm", "[0]*sin(x)*cos(x)*pow(cos(x),[1])", 0.05,M_PI/2.);
  // TF1 *formula = new TF1("zenForm", "[0]*sin(x)*cos(x)*pow(cos(x),[1])", startangle,endangle);
  TF1 *formula = new TF1("zenForm", "[0]*sin(x)*pow(cos(x),[1])", startangle, endangle);

  // TF1 *formula = new TF1("zenForm", "[0]*sin(x)*cos(x)*pow(cos(x),[1])", 0.05,1.5);
  zenithAngleHist->Fit(formula, "r");

  TLegend *legend = new TLegend();
  legend->AddEntry(zenithAngleHist, "Data");

  double n = formula->GetParameter(2);
  std::cout << "Fit N : " << n << std::endl;
  // std::string fittext = "#splitline{Fitting with mcos^{n}#theta}{and n comes out to be"+std::to_string(n)+"}";
  std::string fittext = "Fitting with mcos^{n}#theta";
  legend->AddEntry(formula, fittext.c_str());
  legend->Draw();
  return zenithAngleHist;
}

TH1F *PlotZenithAngle(std::vector<SingleMuonTrack *> muonTrackVec, int opt)
{
  TVector3 ref(0., -1., 0.);
  int numOfBins     = 50;
  std::string title = "";
  if (opt == 1) title = "ZenithAngleLinear";
  if (opt == 2) title = "ZenithAngleParam";
  if (opt == 3) title = "ZenithAngleMeanHitPoint";
  TH1F *zenithAngleHist = new TH1F(title.c_str(), title.c_str(), numOfBins, 0.02, 1.5);
  std::cout << "Size of Muon Track Vector : " << muonTrackVec.size() << std::endl;
  for (unsigned int trackIndex = 0; trackIndex < muonTrackVec.size(); trackIndex++) {
    if (muonTrackVec[trackIndex]->size() > 5) {
      if (opt == 1) {
        double angleVal = muonTrackVec[trackIndex]->GetZenithAngle_Linear();
        // if(angleVal < 0.96)
        zenithAngleHist->Fill(angleVal);
      }
      if (opt == 2) {
        double angleVal = muonTrackVec[trackIndex]->GetZenithAngle_Param();
        // if(angleVal < 0.96)
        zenithAngleHist->Fill(angleVal);
        // zenithAngleHist->Fill(muonTrackVec[trackIndex]->GetZenithAngle_Param());
      }
#ifdef USE_FOR_SIMULATION
      if (opt == 3) {
        double angleVal = muonTrackVec[trackIndex]->GetZenithAngle_MeanHitPoint();
        // std::cout << "Angle Value : " << angleVal << std::endl;
        if (angleVal < 0.96) zenithAngleHist->Fill(angleVal);
        // zenithAngleHist->Fill(muonTrackVec[trackIndex]->GetZenithAngle_Param());
      }
      if (opt == 4) {
        double angleVal = muonTrackVec[trackIndex]->GetZenithAngle_ExactHitPoint();
        // std::cout << "Angle Value : " << angleVal << std::endl;
        if (angleVal < 0.96) zenithAngleHist->Fill(angleVal);
        // zenithAngleHist->Fill(muonTrackVec[trackIndex]->GetZenithAngle_Param());
      }
#endif
    }
  }
  // TF1 *formula = new TF1("Cos2ThetaFit",Cos2ThetaFit,0.02,0.96,2);
  // zenithAngleHist->Scale(1/zenithAngleHist->Integral());
  TF1 *formula = new TF1("zenForm", "[0]*sin(x)*cos(x)*pow(cos(x),[1])", 0.05, 1.5);
  zenithAngleHist->Fit(formula, "r");
  return zenithAngleHist;
  // gr->Fit(formula,"r");
  //		double c = formula->GetParameter(0);
  //		double m = formula->GetParameter(1);
  //		delete formula;

  /*TH1F *solidAngleCorrectedHist = new TH1F("Solid angle corrected AngularDistribution ","Solid angle corrected
  AngularDistribution",numOfBins,0.2,0.96); solidAngleCorrectedHist->GetYaxis()->SetTitle("I_{#theta}  (
  cm^{-2}sec^{-1}st^{-1})"); solidAngleCorrectedHist->GetXaxis()->SetTitle("#theta (radian)");

  for(int i =0  ; i < numOfBins ; i++){
    double binCenter = zenithAngleHist->GetXaxis()->GetBinCenter(i);
      double binContent = zenithAngleHist->GetBinContent(i);
      //std::cout << "binContent : " << binContent << " : binCenter : " << binCenter << std::endl;
      solidAngleCorrectedHist->SetBinContent(i,binContent/(2*M_PI*std::sin(binCenter)*std::cos(binCenter)));
  }
  //new TCanvas();
  TF1 *cosSqr = new TF1("cosSqr", "[0]*pow(cos(x),[1])", 0.2,0.96);
  solidAngleCorrectedHist->Fit(cosSqr,"r");
  //solidAngleCorrectedHist->Draw();

  //
  return solidAngleCorrectedHist;*/

  // zenithAngleHist->Scale(1/zenithAngleHist->Integral());
    /*std::cout << "@@@@@@@@@@ Fitted Parameter for ZenithAngle Histogram @@@@@@@@@" << std::endl;
		new TCanvas();
		TF1 *zenForm = new TF1("zenForm", "[0]*sin(x)*pow(cos(x),[1])", 0.05,M_PI/2.);
		//TF1 *zenForm = new TF1("zenForm", "[0]*pow(cos(x),[1]+1)", 0.05,M_PI/2.);
		zenithAngleHist->Fit(zenForm,"r");
		zenithAngleHist->Draw();

		std::cout << "@@@@@@@@@@ Fitted Parameter for SolidAngle Corrected Histogram @@@@@@@@@" << std::endl;
		TH1F *solidAngleCorrectedHist = new TH1F("Solid angle corrected AngularDistribution ","Solid angle corrected AngularDistribution",numOfBins,0.,M_PI/2.);
		solidAngleCorrectedHist->GetYaxis()->SetTitle("I_{#theta}  ( cm^{-2}sec^{-1}st^{-1})");
		solidAngleCorrectedHist->GetXaxis()->SetTitle("#theta (radian)");

		for(int i =0  ; i < numOfBins ; i++){
		 	double binCenter = zenithAngleHist->GetXaxis()->GetBinCenter(i);
		   	double binContent = zenithAngleHist->GetBinContent(i);
		   	//std::cout << "binContent : " << binContent << " : binCenter : " << binCenter << std::endl;
		   	solidAngleCorrectedHist->SetBinContent(i,binContent/(2*M_PI*std::sin(binCenter)*std::cos(binCenter)));
		}
		new TCanvas();
		//TF1 *cosSqr = new TF1("cosSqr",Cos2ThetaFit,0,M_PI/2,2);
		TF1 *cosSqr = new TF1("cosSqr", "[0]*pow(cos(x),[1])", 0.2,0.96);
		solidAngleCorrectedHist->Fit(cosSqr,"r");
		solidAngleCorrectedHist->Draw();


		//New Histogram
		TH1F *zenithAngle2D = new TH1F("ZenithAngle in 2D","ZenithAngle in 2D using  #frac{z_{2}-z_{1}}{#sqrt{(x_{2}-x_{1})^{2} + (y_{2}-y_{1})^{2} + (z_{2}-z_{1})^{2}}}",numOfBins,-1*M_PI/2.,M_PI/2.);
		//zenithAngle2D->GetYaxis()->SetTitle("I_{#theta}  ( cm^{-2}sec^{-1}st^{-1})");
		zenithAngle2D->GetXaxis()->SetTitle("#theta (radian)");
		for(unsigned int trackIndex = 0 ; trackIndex < fittedMuonTracks.size() ; trackIndex++){
				std::vector<Point3D*> singleMuonTrack = fittedMuonTracks[trackIndex];
				Point3D *startPoint = singleMuonTrack[0];
				//std::cout <<"Start Point : ";
				//startPoint->Print();
				Point3D *endPoint = singleMuonTrack[singleMuonTrack.size()-1];
				//std::cout << "End Point : " ;
				//endPoint->Print();
				TVector3 muonDir(TVector3(endPoint->x,endPoint->y,endPoint->z)-TVector3(startPoint->x,startPoint->y,startPoint->z));
				double angVal = asin((endPoint->z-startPoint->z)/muonDir.Mag());
				//std::cout << "Angle Value : " << angVal << std::endl;
				//zenithAngle2D->Fill((angVal*M_PI/180.)*1000.);
				//if(fabs(startPoint->z) < 50. && fabs(endPoint->z) < 50.)
				zenithAngle2D->Fill(angVal);
		}
		new TCanvas();
		zenithAngle2D->Draw();

*/	}

    } // namespace lite_interface
