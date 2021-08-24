/*
**	Filename : Test_LanGausSampling.cpp
**	2021-08-18
**	username : rsehgal
*/
#include <iostream>
#include "langaus.h"
#include "includes.h"
#include <TFile.h>
#include <TTree.h>
int main(int argc, char *argv[])
{
  double param[4] = {2.014, 19.40, 19535., 2.67};
  // Declaration of leaves types
  UInt_t qlongNear;
  Double_t qlongMean;
  ULong64_t tsmallTimeStamp;
  Long64_t deltaTstamp;
  UShort_t barIndex;
  Double32_t dX;
  Double32_t dY;
  Double32_t dZ;
  Double32_t deX;
  Double32_t deY;
  Double32_t deZ;
  Double32_t dLog;

  TFile *fd =  new TFile("simData.root","r");

  TTree *ftree = (TTree*)fd->Get("ftree");
  // Set branch addresses.
  ftree->SetBranchAddress("qlongNear", &qlongNear);
  ftree->SetBranchAddress("qlongMean", &qlongMean);
  ftree->SetBranchAddress("tsmallTimeStamp", &tsmallTimeStamp);
  ftree->SetBranchAddress("deltaTstamp", &deltaTstamp);
  ftree->SetBranchAddress("barIndex", &barIndex);
  ftree->SetBranchAddress("hitX", &dX);
  ftree->SetBranchAddress("hitY", &dY);
  ftree->SetBranchAddress("hitZ", &dZ);
  ftree->SetBranchAddress("exactHitX", &deX);
  ftree->SetBranchAddress("exactHitY", &deY);
  ftree->SetBranchAddress("exactHitZ", &deZ);
  ftree->SetBranchAddress("logQFarByQNear", &dLog);

  //     This is the loop skeleton
  //       To read only selected branches, Insert statements like:
  // ftree->SetBranchStatus("*",0);  // disable all branches
  // TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

  TFile *ff = new TFile("landau.root", "RECREATE");
  ff->cd();
  Long64_t nentries    = ftree->GetEntries();
  TH1F *enerHist       = new TH1F("EnerHist", "EnerHist", 1000, 10, 50.);
  TH1F *enerHistLandau = new TH1F("EnerHistLandau", "EnerHistLandau", 100, 10, 60.);
  std::cout << "Total Num of entries : " << nentries << std::endl;
  Long64_t nbytes = 0;
  for (Long64_t i = 0; i < nentries; i++) {
    nbytes += ftree->GetEntry(i);
    
    enerHist->Fill(qlongMean);
    //enerHistLandau->Fill(gRandom->Landau(19.36, 2.647));
    if(qlongMean >18.)	
    enerHistLandau->Fill(langaufun(&qlongMean,param));
  }

  enerHistLandau->Write();
  ff->Close();
}
