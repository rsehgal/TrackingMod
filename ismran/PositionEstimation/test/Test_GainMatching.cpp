/*
**	Filename : Test_GainMatching.cpp
**	2021-07-23
**	username : rsehgal
*/
// DOCBEGIN
/*
** Test Program to generate the gain matching plot using source data
*/
// DOCEND
#include <iostream>
#include <string>
#include "CharacterizationFileReader.h"
#include <fstream>
#include <TH1F.h>
#include <TApplication.h>
#include <vector>

int main(int argc, char *argv[])
{
  TApplication *fApp   = new TApplication("Test", NULL, NULL);
  std::string filename = argv[1];
  CharacterizationFileReader cfReader(filename);
  FillAttenCoeffVec();
  std::vector<Event *> eventsVec = cfReader.GetAllEvents(std::atoi(argv[2]));
  TH1F *hist                     = new TH1F("HistLogQNearByQFar", "HistLogQNearByQFar", 100, -5., 5.);
  TH1F *histQNear                = new TH1F("Hist_QNear", "HistQNear", 15000, 0., 15000);
  TH1F *histQFar                 = new TH1F("Hist_QFar", "HistQFar", 15000, 0., 15000);
  TH1F *histQGM                  = new TH1F("Hist_QGM", "Hist_QGM", 15000, 0., 15000);
  TH1F *histDelT                 = new TH1F("Hist_DelT", "Hist_DelT", 100, -20., 20.);
  TH1F *histEnergy                 = new TH1F("Hist_Energy", "Hist_Energy", 1000, 0., 50.);

  // Energy Calibration
  std::string barName = cfReader.GetBarName();
  TF1 *energyCalibFormula = cfReader.GetEnergyCalibFormula();

      for (unsigned int i = 0; i < eventsVec.size(); i++)
  {
    // std::cout << eventsVec[i]->GetLogQNearByQFar() << std::endl;
    hist->Fill(eventsVec[i]->GetLogQNearByQFar());
    histQNear->Fill(eventsVec[i]->GetQNear());
    histQFar->Fill(eventsVec[i]->GetQFar());
    histQGM->Fill(eventsVec[i]->GetQMean());
    histDelT->Fill(eventsVec[i]->GetDelT() / 1000.);
    histEnergy->Fill(energyCalibFormula->Eval(eventsVec[i]->GetQMean()));
   
  }

  histQNear->SetLineColor(4);
  histQFar->SetLineColor(6);

  new TCanvas("Q", "Q");
  hist->Draw();
  new TCanvas("GainMatched", "GainMatched");
  histQNear->Draw();
  histQFar->Draw("same");
  new TCanvas("QGeomMean", "QGeomMean");
  histQGM->Draw();
  new TCanvas("DelT", "DelT");
  histDelT->Draw();

  new TCanvas("CalibratedEnergy","CalibratedEnergy");
  histEnergy->Draw();

  {
    std::string dir                          = "/home/rsehgal/DevShare/CalibrationFiles/PS01_S2AB1017";
    std::vector<std::string> filenamesVector = {"Cs137_PS01_S2AB1017_Couples_N1164V_Th10_F1160V_Th12_-45cm.root",
                                                "Cs137_PS01_S2AB1017_Couples_N1164V_Th10_F1160V_Th12_-40cm.root",
                                                "Cs137_PS01_S2AB1017_Couples_N1164V_Th10_F1160V_Th12_-30cm.root",
                                                "Cs137_PS01_S2AB1017_Couples_N1164V_Th10_F1160V_Th12_-20cm.root",
                                                "Cs137_PS01_S2AB1017_Couples_N1164V_Th10_F1160V_Th12_-10cm.root",
                                                "Cs137_PS01_S2AB1017_Couples_N1164V_Th10_F1160V_Th12_+0cm.root",
                                                //"Cs137_PS01_S2AB1017_Couples_N1164V_Th10_F1160V_Th12_+0cm.root",
                                                "Cs137_PS01_S2AB1017_Couples_N1164V_Th10_F1160V_Th12_+10cm.root",
                                                "Cs137_PS01_S2AB1017_Couples_N1164V_Th10_F1160V_Th12_+20cm.root",
                                                "Cs137_PS01_S2AB1017_Couples_N1164V_Th10_F1160V_Th12_+30cm.root",
                                                "Cs137_PS01_S2AB1017_Couples_N1164V_Th10_F1160V_Th12_+40cm.root",
                                                "Cs137_PS01_S2AB1017_Couples_N1164V_Th10_F1160V_Th12_+45cm.root"};

    std::vector<double> vecOfPixelPos;

    std::vector<TH1F *> vecOfHist;
    std::vector<TH1F *> vecOfHistQ;

    std::vector<double> vecOfZError;

    std::vector<double> vecOfDelTMean;
    std::vector<double> vecOfDelTError;

    std::vector<double> vecOfMeanQ;
    std::vector<double> vecOfQError;

    for (unsigned int i = 0; i < filenamesVector.size(); i++) {
      std::string name  = "HistDelT" + filenamesVector[i];
      std::string nameQ = "HistQ" + filenamesVector[i];
      vecOfHist.push_back(new TH1F(name.c_str(), name.c_str(), 100, -20., 20.));
      vecOfHistQ.push_back(new TH1F(nameQ.c_str(), nameQ.c_str(), 1000, -5., 5.));
      vecOfZError.push_back(5.);
    }

    vecOfHistQ[0]->SetMarkerStyle(53);
    vecOfHistQ[0]->SetMarkerSize(0.8);
    vecOfHistQ[1]->SetMarkerStyle(8);
    vecOfHistQ[1]->SetMarkerSize(0.8);
    vecOfHistQ[2]->SetMarkerStyle(28);
    vecOfHistQ[2]->SetMarkerSize(1.0);
    vecOfHistQ[3]->SetMarkerStyle(26);
    vecOfHistQ[3]->SetMarkerSize(0.9);
    /*vecOfHistQ[4]->SetMarkerStyle(29);
    vecOfHistQ[4]->SetMarkerSize(1.1);
    vecOfHistQ[5]->SetMarkerStyle(56);
    vecOfHistQ[5]->SetMarkerSize(1.2);*/

    vecOfHist[0]->SetMarkerStyle(53);
    vecOfHist[0]->SetMarkerSize(0.8);
    vecOfHist[1]->SetMarkerStyle(8);
    vecOfHist[1]->SetMarkerSize(0.8);
    vecOfHist[2]->SetMarkerStyle(28);
    vecOfHist[2]->SetMarkerSize(1.0);
    vecOfHist[3]->SetMarkerStyle(26);
    vecOfHist[3]->SetMarkerSize(0.9);
    /*vecOfHist[4]->SetMarkerStyle(29);
    vecOfHist[4]->SetMarkerSize(1.1);
    vecOfHist[5]->SetMarkerStyle(56);
    vecOfHist[5]->SetMarkerSize(1.2);*/

    for (unsigned int j = 0; j < filenamesVector.size(); j++) {
      cfReader.ResetWith(dir + "/" + filenamesVector[j]);
      FillAttenCoeffVec();
      std::vector<Event *> eventsVec = cfReader.GetAllEvents(std::atoi(argv[2]));
      for (unsigned int i = 0; i < eventsVec.size(); i++) {
        vecOfHist[j]->Fill(eventsVec[i]->GetDelT() / 1000.);
        vecOfHistQ[j]->Fill(eventsVec[i]->GetLogQNearByQFar());
      }
      vecOfHist[j]->Scale(1 / vecOfHist[j]->Integral());
      vecOfHistQ[j]->Scale(1 / vecOfHistQ[j]->Integral());

      TF1 *formulaQ = new TF1("FormulaQ", "gaus", -5., 5.);
      formulaQ->SetParameters(vecOfHistQ[j]->GetMaximum(), vecOfHistQ[j]->GetMean(), vecOfHistQ[j]->GetStdDev());
      vecOfHistQ[j]->Fit(formulaQ, "qn");

      /*vecOfQError.push_back(formulaQ->GetParameter(2));
      vecOfDelTError.push_back(cfReader.GetSigma());*/

      vecOfMeanQ.push_back(formulaQ->GetParameter(1));
      vecOfQError.push_back(0.);

      vecOfDelTError.push_back(0.);
      vecOfDelTMean.push_back(cfReader.GetMean());

      vecOfPixelPos.push_back(cfReader.GetActualPosition());
    }

    new TCanvas("DelT_At_Diff_Pos", "DelT_At_Diff_Pos");
    for (unsigned int j = 0; j < filenamesVector.size(); j++) {
      vecOfHist[j]->Draw("same");
    }

    new TCanvas("Q_At_Diff_Pos", "Q_At_Diff_Pos");
    for (unsigned int j = 0; j < filenamesVector.size(); j++) {
      vecOfHistQ[j]->Draw("same");
    }

    auto grDelT =
        new TGraphErrors(vecOfHist.size(), &vecOfDelTMean[0], &vecOfPixelPos[0], &vecOfDelTError[0], &vecOfZError[0]);
    auto grQ = new TGraphErrors(vecOfHistQ.size(), &vecOfMeanQ[0], &vecOfPixelPos[0], &vecOfQError[0], &vecOfZError[0]);

    TLegend *legendTZ = new TLegend(0.1, 0.7, 0.24, 0.5);
    new TCanvas("Graph of DelT vs Z", "Graph of DelT vs Z");
    grDelT->Draw("ap");
    grDelT->SetMarkerStyle(53);
    TF1 *formulaTZ = new TF1("FormulaTZ", Pol3, -10, 10, 4);
    formulaTZ->SetLineColor(2);
    formulaTZ->SetLineWidth(3);
    formulaTZ->Draw("same");
    grDelT->Fit(formulaTZ, "qn");

    legendTZ->AddEntry("grDelT", "Data Point", "p");
    legendTZ->AddEntry("formulaTZ", "Fit", "l");
    legendTZ->Draw("same");

    TLegend *legendQZ = new TLegend(0.1, 0.7, 0.24, 0.5);
    new TCanvas("Graph of Q vs Z", "Graph of Q vs Z");
    grQ->Draw("ap");
    TF1 *formulaQZ = new TF1("FormulaQZ", Pol3, -3, 3, 4);
    grQ->Fit(formulaQZ, "qn");
    formulaQZ->SetLineWidth(3);
    grQ->SetMarkerStyle(53);
    formulaQZ->Draw("same");

    legendQZ->AddEntry("grQ", "Data Point", "p");
    legendQZ->AddEntry("formulaQZ", "Fit", "l");
    legendQZ->Draw("same");

    /*gr->SetTitle("TGraphErrors Example");
    gr->SetMarkerColor(4);
    gr->SetMarkerStyle(21);
    gr->Draw("ALP");*/
  }

  fApp->Run();
}
