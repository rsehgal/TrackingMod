//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Sep 25 13:59:29 2010 by ROOT version 5.27/04
// from TTree BSC_DATA_TREE/BSC RAW DATA
// found on file: data.root
//////////////////////////////////////////////////////////

#ifndef Analysis_h
#define Analysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class Analysis {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           FADC0_SAMPLES;
   Int_t           FADC0_HEADER1;
   Int_t           FADC0_HEADER2;
   Int_t           FADC0_HEADER3;
   Int_t           FADC0_HEADER4;
   vector<unsigned int> *FADC0_CH0;
   vector<unsigned int> *FADC0_CH1;
   vector<unsigned int> *FADC0_CH2;
   vector<unsigned int> *FADC0_CH3;
   vector<unsigned int> *FADC0_CH4;
   vector<unsigned int> *FADC0_CH5;
   vector<unsigned int> *FADC0_CH6;
   vector<unsigned int> *FADC0_CH7;
   /*
   Int_t           FADC1_SAMPLES;
   Int_t           FADC1_HEADER1;
   Int_t           FADC1_HEADER2;
   Int_t           FADC1_HEADER3;
   Int_t           FADC1_HEADER4;
   vector<unsigned int> *FADC1_CH0;
   vector<unsigned int> *FADC1_CH1;
   vector<unsigned int> *FADC1_CH2;
   vector<unsigned int> *FADC1_CH3;
   vector<unsigned int> *FADC1_CH4;
   vector<unsigned int> *FADC1_CH5;
   vector<unsigned int> *FADC1_CH6;
   vector<unsigned int> *FADC1_CH7;
   Int_t           FADC2_SAMPLES;
   Int_t           FADC2_HEADER1;
   Int_t           FADC2_HEADER2;
   Int_t           FADC2_HEADER3;
   Int_t           FADC2_HEADER4;
   vector<unsigned int> *FADC2_CH0;
   vector<unsigned int> *FADC2_CH1;
   vector<unsigned int> *FADC2_CH2;
   vector<unsigned int> *FADC2_CH3;
   vector<unsigned int> *FADC2_CH4;
   vector<unsigned int> *FADC2_CH5;
   vector<unsigned int> *FADC2_CH6;
   vector<unsigned int> *FADC2_CH7;
   */
   // List of branches
   TBranch        *b_fSamplesPerCh0;   //!
   TBranch        *b_fHeader10;   //!
   TBranch        *b_fHeader20;   //!
   TBranch        *b_fHeader30;   //!
   TBranch        *b_fHeader40;   //!
   TBranch        *b_FADC0_CH0;   //!
   TBranch        *b_FADC0_CH1;   //!
   TBranch        *b_FADC0_CH2;   //!
   TBranch        *b_FADC0_CH3;   //!
   TBranch        *b_FADC0_CH4;   //!
   TBranch        *b_FADC0_CH5;   //!
   TBranch        *b_FADC0_CH6;   //!
   TBranch        *b_FADC0_CH7;   //!
   /*
   TBranch        *b_fSamplesPerCh1;   //!
   TBranch        *b_fHeader11;   //!
   TBranch        *b_fHeader21;   //!
   TBranch        *b_fHeader31;   //!
   TBranch        *b_fHeader41;   //!
   TBranch        *b_FADC1_CH0;   //!
   TBranch        *b_FADC1_CH1;   //!
   TBranch        *b_FADC1_CH2;   //!
   TBranch        *b_FADC1_CH3;   //!
   TBranch        *b_FADC1_CH4;   //!
   TBranch        *b_FADC1_CH5;   //!
   TBranch        *b_FADC1_CH6;   //!
   TBranch        *b_FADC1_CH7;   //!

   TBranch        *b_fSamplesPerCh2;   //!
   TBranch        *b_fHeader12;   //!
   TBranch        *b_fHeader22;   //!
   TBranch        *b_fHeader32;   //!
   TBranch        *b_fHeader42;   //!
   TBranch        *b_FADC2_CH0;   //!
   TBranch        *b_FADC2_CH1;   //!
   TBranch        *b_FADC2_CH2;   //!
   TBranch        *b_FADC2_CH3;   //!
   TBranch        *b_FADC2_CH4;   //!
   TBranch        *b_FADC2_CH5;   //!
   TBranch        *b_FADC2_CH6;   //!
   TBranch        *b_FADC2_CH7;   //!
   */
   Analysis(TTree *tree=0);
   virtual ~Analysis();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Analysis_cxx
Analysis::Analysis(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("data.root");
      if (!f) {
         f = new TFile("data.root");
      }
      tree = (TTree*)gDirectory->Get("BSC_DATA_TREE");

   }
   Init(tree);
}

Analysis::~Analysis()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Analysis::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Analysis::LoadTree(Long64_t entry)
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

void Analysis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   FADC0_CH0 = 0;
   FADC0_CH1 = 0;
   FADC0_CH2 = 0;
   FADC0_CH3 = 0;
   FADC0_CH4 = 0;
   FADC0_CH5 = 0;
   FADC0_CH6 = 0;
   FADC0_CH7 = 0;
   /*
   FADC1_CH0 = 0;
   FADC1_CH1 = 0;
   FADC1_CH2 = 0;
   FADC1_CH3 = 0;
   FADC1_CH4 = 0;
   FADC1_CH5 = 0;
   FADC1_CH6 = 0;
   FADC1_CH7 = 0;
   FADC2_CH0 = 0;
   FADC2_CH1 = 0;
   FADC2_CH2 = 0;
   FADC2_CH3 = 0;
   FADC2_CH4 = 0;
   FADC2_CH5 = 0;
   FADC2_CH6 = 0;
   FADC2_CH7 = 0;
   */  
 // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("FADC0_SAMPLES", &FADC0_SAMPLES, &b_fSamplesPerCh0 );
   fChain->SetBranchAddress("FADC0_HEADER1", &FADC0_HEADER1, &b_fHeader10 );
   fChain->SetBranchAddress("FADC0_HEADER2", &FADC0_HEADER2, &b_fHeader20 );
   fChain->SetBranchAddress("FADC0_HEADER3", &FADC0_HEADER3, &b_fHeader30 );
   fChain->SetBranchAddress("FADC0_HEADER4", &FADC0_HEADER4, &b_fHeader40 );
   fChain->SetBranchAddress("FADC0_CH0", &FADC0_CH0, &b_FADC0_CH0);
   fChain->SetBranchAddress("FADC0_CH1", &FADC0_CH1, &b_FADC0_CH1);
   fChain->SetBranchAddress("FADC0_CH2", &FADC0_CH2, &b_FADC0_CH2);
   fChain->SetBranchAddress("FADC0_CH3", &FADC0_CH3, &b_FADC0_CH3);
   fChain->SetBranchAddress("FADC0_CH4", &FADC0_CH4, &b_FADC0_CH4);
   fChain->SetBranchAddress("FADC0_CH5", &FADC0_CH5, &b_FADC0_CH5);
   fChain->SetBranchAddress("FADC0_CH6", &FADC0_CH6, &b_FADC0_CH6);
   fChain->SetBranchAddress("FADC0_CH7", &FADC0_CH7, &b_FADC0_CH7);
   /*
   fChain->SetBranchAddress("FADC1_SAMPLES", &FADC1_SAMPLES, &b_fSamplesPerCh1 );
   fChain->SetBranchAddress("FADC1_HEADER1", &FADC1_HEADER1, &b_fHeader11 );
   fChain->SetBranchAddress("FADC1_HEADER2", &FADC1_HEADER2, &b_fHeader21 );
   fChain->SetBranchAddress("FADC1_HEADER3", &FADC1_HEADER3, &b_fHeader31 );
   fChain->SetBranchAddress("FADC1_HEADER4", &FADC1_HEADER4, &b_fHeader41 );
   fChain->SetBranchAddress("FADC1_CH0", &FADC1_CH0, &b_FADC1_CH0);
   fChain->SetBranchAddress("FADC1_CH1", &FADC1_CH1, &b_FADC1_CH1);
   fChain->SetBranchAddress("FADC1_CH2", &FADC1_CH2, &b_FADC1_CH2);
   fChain->SetBranchAddress("FADC1_CH3", &FADC1_CH3, &b_FADC1_CH3);
   fChain->SetBranchAddress("FADC1_CH4", &FADC1_CH4, &b_FADC1_CH4);
   fChain->SetBranchAddress("FADC1_CH5", &FADC1_CH5, &b_FADC1_CH5);
   fChain->SetBranchAddress("FADC1_CH6", &FADC1_CH6, &b_FADC1_CH6);
   fChain->SetBranchAddress("FADC1_CH7", &FADC1_CH7, &b_FADC1_CH7);
   fChain->SetBranchAddress("FADC2_SAMPLES", &FADC2_SAMPLES, &b_fSamplesPerCh2 );
   fChain->SetBranchAddress("FADC2_HEADER1", &FADC2_HEADER1, &b_fHeader12 );
   fChain->SetBranchAddress("FADC2_HEADER2", &FADC2_HEADER2, &b_fHeader22 );
   fChain->SetBranchAddress("FADC2_HEADER3", &FADC2_HEADER3, &b_fHeader32 );
   fChain->SetBranchAddress("FADC2_HEADER4", &FADC2_HEADER4, &b_fHeader42 );
   fChain->SetBranchAddress("FADC2_CH0", &FADC2_CH0, &b_FADC2_CH0);
   fChain->SetBranchAddress("FADC2_CH1", &FADC2_CH1, &b_FADC2_CH1);
   fChain->SetBranchAddress("FADC2_CH2", &FADC2_CH2, &b_FADC2_CH2);
   fChain->SetBranchAddress("FADC2_CH3", &FADC2_CH3, &b_FADC2_CH3);
   fChain->SetBranchAddress("FADC2_CH4", &FADC2_CH4, &b_FADC2_CH4);
   fChain->SetBranchAddress("FADC2_CH5", &FADC2_CH5, &b_FADC2_CH5);
   fChain->SetBranchAddress("FADC2_CH6", &FADC2_CH6, &b_FADC2_CH6);
   fChain->SetBranchAddress("FADC2_CH7", &FADC2_CH7, &b_FADC2_CH7);
   */
   Notify();
}

Bool_t Analysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Analysis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if ( !fChain ) return;
   fChain->Show( entry );
}
Int_t Analysis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
  return 1;
  return entry;
}
#endif // #ifdef Analysis_cxx
