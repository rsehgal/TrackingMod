//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Sep 16 19:49:54 2011 by ROOT version 5.27/04
// from TTree BSC_DATA_TREE/BSC RAW DATA
// found on file: data-20110916193507.root
//////////////////////////////////////////////////////////

#ifndef A1_h
#define A1_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <TCanvas.h>
#include <TH1.h>

using std::cout;
using std::endl;
using std::vector;
using std::stringstream;

typedef std::vector< vector< unsigned int> * > ModuleVector;
typedef std::vector< TBranch* > ModuleBranches;
const unsigned ChannelsCount = 128;

class A1 {
 private:
  unsigned _ModulesCount;
  std::vector < ModuleVector > _Data; 
  std::vector < ModuleBranches > _Branches;
 public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain
    
  // Declaration of leaf types
  vector< Int_t > NUMBER_OF_CHANNELS;
  ModuleVector HitsPerChannel;//vector<unsigned int> *Module2_HITS_PER_CHANNEL;
   // List of branches
   TBranch        *b__nChannels;   //!
   TBranch        *b_Module2_HITS_PER_CHANNEL;   //!
     
 //Canvases
   TCanvas * c1;
   TCanvas * c2;
   //Histograms
   TH1F * hTOF1;
   TH1F * hTOF2;
   TH1F *hTrigger;
   TH1F *hTOFDiff;
   A1(char * fName, unsigned ModulesCnt = 2);
   virtual ~A1();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   virtual void     PlotChannel( unsigned aModule, unsigned aChannel );
};

#endif

#ifdef A1_cxx
A1::A1( char * fName, unsigned ModulesCnt )
{
  TTree *tree = 0;
  _ModulesCount = ModulesCnt;
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fName);
      if (!f) {
         f = new TFile(fName);
      }
      tree = (TTree*)gDirectory->Get("BSC_DATA_TREE");

   }
   Init(tree);
   c1 = NULL;
}

A1::~A1()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t A1::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t A1::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void A1::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).
  
   // Set object pointer
  _Data.resize( _ModulesCount );
  _Branches.resize( _ModulesCount );  
  HitsPerChannel.resize( _ModulesCount );
  NUMBER_OF_CHANNELS.resize( _ModulesCount );
  for( unsigned i = 0; i < _ModulesCount; ++i ){
    _Data[ i ].resize( ChannelsCount );
    _Branches[ i ].resize( ChannelsCount );
  }
  //HITS_PER_CHANNEL = 0;
   
  // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass( 1 );
   //std::cout << "Data.size() = " << _Data.size() << std::endl;
   for( unsigned j = 0; j < _Data.size(); ++j ){
     //std::cout << "Data["<< j<<".size() = " << _Data[ j ].size() << std::endl;
     stringstream name;
     for( unsigned k = 0; k < _Data[ j ].size(); ++k ){
       name.str("");
       name << "Module"<< j + 2 << "_LE_CH" << k;
       TBranch * br = new TBranch();
       TBranch * branch_info;
       if( branch_info = fChain->GetBranch( name.str().c_str() ) ){
         std::vector< unsigned > * ch = new std::vector< unsigned >;
         fChain->SetBranchAddress( name.str().c_str(), &ch, &br );
         _Data[ j ][ k ] = ch;
         _Branches[ j ][ k ] = br;
       }
     }
     name.str("");
     name << "Module"<< j + 2 << "_NUMBER_OF_CHANNELS";
     TBranch * branch;
     fChain->SetBranchAddress( name.str().c_str(), &NUMBER_OF_CHANNELS[ j ], &branch);
     name.str("");
     name << "Module"<< j + 2 << "_HITS_PER_CHANNEL";
     fChain->SetBranchAddress( name.str().c_str(), &HitsPerChannel[ j ], &branch );
   
   }
   Notify();
}

Bool_t A1::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void A1::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t A1::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef A1_cxx
