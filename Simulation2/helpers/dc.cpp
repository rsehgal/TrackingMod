#include <iostream>
#include <TFile.h>
#include<TH1D.h>
#include<TCanvas.h>
#include <TApplication.h>
#include <TH1F.h>
#include<TRandom.h>
#include<TPad.h>
# include <fstream>
#include "Riostream.h"
#include <TNtuple.h>
using namespace std;
void dc();

int main(int argc,char **argv){
TApplication *fApp  = new TApplication("TEst",NULL,NULL);
dc();
fApp->Run();
return 0;
}


void dc()
{

TCanvas *c1 = new TCanvas("c1","Dipayan_apr17",200,10,600,400);
c1->SetGrid();
//   TString dir = gSystem->UnixPathName(gInterpreter->GetCurrentMacroName());
//   dir.ReplaceAll("dc.cpp","");
 //  dir.ReplaceAll("/./","/");
   ifstream in;
   in.open("energy.txt");

   Float_t x;
   Int_t nlines = 0;
   TFile *f = new TFile("dc.root","RECREATE");
   TH1F *h1 = new TH1F("h1","x distribution",100,0.,30.);
   TNtuple *ntuple = new TNtuple("ntuple","data from ascii file","x:y");

   while (1) {
      in >> x ;
      if (!in.good()) break;
      if (nlines < 1001) printf("x=%8f",x);
      h1->Fill(x);
      ntuple->Fill(x);
      nlines++;
   }
   printf(" found %d points\n",nlines);

   in.close();

   f->Write();
}


 
