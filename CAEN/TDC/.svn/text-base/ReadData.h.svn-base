//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Oct  5 13:43:31 2010 by ROOT version 5.27/02
// from TTree BSC_DATA_TREE/BSC RAW DATA
// found on file: data.root
//////////////////////////////////////////////////////////

#ifndef ReadData_h
#define ReadData_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TCanvas.h>
#include <vector>
#include <iostream>
#include <sstream>

using std::vector;
using std::cout;
using std::endl;
using std::stringstream;

class ReadData {
private:
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   TH1I * h;
   TCanvas * c;
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
   TBranch        *b_fSamplesPerCh1;   //!
   TBranch        *b_fHeader11;   //!
   TBranch        *b_fHeader12;   //!
   TBranch        *b_fHeader13;   //!
   TBranch        *b_fHeader14;   //!
   TBranch        *b_FADC0_CH0;   //!
   TBranch        *b_FADC0_CH1;   //!
   TBranch        *b_FADC0_CH2;   //!
   TBranch        *b_FADC0_CH3;   //!
   TBranch        *b_FADC0_CH4;   //!
   TBranch        *b_FADC0_CH5;   //!
   TBranch        *b_FADC0_CH6;   //!
   TBranch        *b_FADC0_CH7;   //!
  /*
  TBranch        *b_fSamplesPerCh2;   //!
   TBranch        *b_fHeader21;   //!
   TBranch        *b_fHeader22;   //!
   TBranch        *b_fHeader23;   //!
   TBranch        *b_fHeader24;   //!
   TBranch        *b_FADC1_CH0;   //!
   TBranch        *b_FADC1_CH1;   //!
   TBranch        *b_FADC1_CH2;   //!
   TBranch        *b_FADC1_CH3;   //!
   TBranch        *b_FADC1_CH4;   //!
   TBranch        *b_FADC1_CH5;   //!
   TBranch        *b_FADC1_CH6;   //!
   TBranch        *b_FADC1_CH7;   //!
   TBranch        *b_fSamplesPerCh3;   //!
   TBranch        *b_fHeader31;   //!
   TBranch        *b_fHeader32;   //!
   TBranch        *b_fHeader33;   //!
   TBranch        *b_fHeader34;   //!
   TBranch        *b_FADC2_CH0;   //!
   TBranch        *b_FADC2_CH1;   //!
   TBranch        *b_FADC2_CH2;   //!
   TBranch        *b_FADC2_CH3;   //!
   TBranch        *b_FADC2_CH4;   //!
   TBranch        *b_FADC2_CH5;   //!
   TBranch        *b_FADC2_CH6;   //!
   TBranch        *b_FADC2_CH7;   //!
   */
   vector< TH1F * > fHistos;
public:
   ReadData( std::string aFileName = "data.root", TTree *tree=0);
   virtual ~ReadData();
  virtual void FillHistos();
  virtual void CreateHistos();
  virtual void CreateHistos1();
  virtual void DrawHistos();
  void DrawChannel( unsigned ch=0, unsigned event = 0 );
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef ReadData_cxx
ReadData::ReadData( std::string aFileName, TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject( aFileName.c_str() );
      if (!f) {
         f = new TFile( aFileName.c_str() );
      }
      tree = (TTree*)gDirectory->Get( "BSC_DATA_TREE" );

   }
   Init( tree );
}

ReadData::~ReadData()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t ReadData::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry( entry );
}

Long64_t ReadData::LoadTree(Long64_t entry)
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

void ReadData::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).
	c = new TCanvas( "c", "Canvas", 1200, 600 );
	h = 0;
	c->Divide( 4, 2 );
	
fHistos.resize( 24, 0 );
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
  
   fChain->SetBranchAddress("FADC0_SAMPLES", &FADC0_SAMPLES, &b_fSamplesPerCh1);
   fChain->SetBranchAddress("FADC0_HEADER1", &FADC0_HEADER1, &b_fHeader11);
   fChain->SetBranchAddress("FADC0_HEADER2", &FADC0_HEADER2, &b_fHeader12);
   fChain->SetBranchAddress("FADC0_HEADER3", &FADC0_HEADER3, &b_fHeader13);
   fChain->SetBranchAddress("FADC0_HEADER4", &FADC0_HEADER4, &b_fHeader14);

   fChain->SetBranchAddress( "FADC0_CH0", &FADC0_CH0, &b_FADC0_CH0 );
   fChain->SetBranchAddress( "FADC0_CH1", &FADC0_CH1, &b_FADC0_CH1 );
   fChain->SetBranchAddress( "FADC0_CH2", &FADC0_CH2, &b_FADC0_CH2 );
   fChain->SetBranchAddress( "FADC0_CH3", &FADC0_CH3, &b_FADC0_CH3 );
   fChain->SetBranchAddress( "FADC0_CH4", &FADC0_CH4, &b_FADC0_CH4 );
   fChain->SetBranchAddress( "FADC0_CH5", &FADC0_CH5, &b_FADC0_CH5 );
   fChain->SetBranchAddress( "FADC0_CH6", &FADC0_CH6, &b_FADC0_CH6 );
   fChain->SetBranchAddress( "FADC0_CH7", &FADC0_CH7, &b_FADC0_CH7 );
/*

   fChain->SetBranchAddress("FADC1_SAMPLES", &FADC1_SAMPLES, &b_fSamplesPerCh2);
   fChain->SetBranchAddress("FADC1_HEADER1", &FADC1_HEADER1, &b_fHeader21);
   fChain->SetBranchAddress("FADC1_HEADER2", &FADC1_HEADER2, &b_fHeader22);
   fChain->SetBranchAddress("FADC1_HEADER3", &FADC1_HEADER3, &b_fHeader23);
   fChain->SetBranchAddress("FADC1_HEADER4", &FADC1_HEADER4, &b_fHeader24);

   fChain->SetBranchAddress("FADC1_CH0", &FADC1_CH0, &b_FADC1_CH0);
   fChain->SetBranchAddress("FADC1_CH1", &FADC1_CH1, &b_FADC1_CH1);
   fChain->SetBranchAddress("FADC1_CH2", &FADC1_CH2, &b_FADC1_CH2);
   fChain->SetBranchAddress("FADC1_CH3", &FADC1_CH3, &b_FADC1_CH3);
   fChain->SetBranchAddress("FADC1_CH4", &FADC1_CH4, &b_FADC1_CH4);
   fChain->SetBranchAddress("FADC1_CH5", &FADC1_CH5, &b_FADC1_CH5);
   fChain->SetBranchAddress("FADC1_CH6", &FADC1_CH6, &b_FADC1_CH6);
   fChain->SetBranchAddress("FADC1_CH7", &FADC1_CH7, &b_FADC1_CH7);


   fChain->SetBranchAddress("FADC2_SAMPLES", &FADC2_SAMPLES, &b_fSamplesPerCh3);
   fChain->SetBranchAddress("FADC2_HEADER1", &FADC2_HEADER1, &b_fHeader31);
   fChain->SetBranchAddress("FADC2_HEADER2", &FADC2_HEADER2, &b_fHeader32);
   fChain->SetBranchAddress("FADC2_HEADER3", &FADC2_HEADER3, &b_fHeader33);
   fChain->SetBranchAddress("FADC2_HEADER4", &FADC2_HEADER4, &b_fHeader34);

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

Bool_t ReadData::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


void ReadData::DrawChannel( unsigned ch, unsigned event ){
	c->Divide( 1, 1 );
	c->Draw();
	if (!fChain) return;
	Long64_t nentries = fChain->GetEntriesFast(); 
	if( ( event < 0 ) && ( event > nentries ) ) return;
	fChain->GetEntry( event );   
   
   //if( h ) delete h;
	//cout << "TTT = " << FADC0_HEADER4 << ", " << FADC1_HEADER4 << ", " << FADC2_HEADER4 << endl; 
	CreateHistos();	
	//DrawHistos();
	for( unsigned i = 0; i < 44464; ++i ){
	  fHistos[ ch ]->Fill( i, FADC0_CH0->at( i ) );
	} 
	fHistos[ ch ]->Draw();
}

void ReadData::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
	if (!fChain) return;
	Long64_t nentries = fChain->GetEntriesFast(); 
	if( ( entry < 0 ) && ( entry > nentries ) ) return;
	fChain->GetEntry( entry );   
   
   //if( h ) delete h;
	//cout << "TTT = " << FADC0_HEADER4 << ", " << FADC1_HEADER4 << ", " << FADC2_HEADER4 << endl; 
	CreateHistos();	
	//DrawHistos();
	for( unsigned i = 0; i < 44464; ++i ){
		//cout << i << " " << FADC0_CH0->at( i ) << endl;
		
		fHistos[ 0 ]->Fill( i, FADC0_CH0->at( i ) );	
		fHistos[ 1 ]->Fill( i, FADC0_CH1->at( i ) );
		fHistos[ 2 ]->Fill( i, FADC0_CH2->at( i ) );
		fHistos[ 3 ]->Fill( i, FADC0_CH3->at( i ) );
		fHistos[ 4 ]->Fill( i, FADC0_CH4->at( i ) );	
		fHistos[ 5 ]->Fill( i, FADC0_CH5->at( i ) );
		fHistos[ 6 ]->Fill( i, FADC0_CH6->at( i ) );
		fHistos[ 7 ]->Fill( i, FADC0_CH7->at( i ) );
	/*	
		fHistos[ 8 ]->Fill( i, FADC1_CH0->at( i ) );	
		fHistos[ 9 ]->Fill( i, FADC1_CH1->at( i ) );
		fHistos[ 10 ]->Fill( i, FADC1_CH2->at( i ) );
		fHistos[ 11 ]->Fill( i, FADC1_CH3->at( i ) );
		fHistos[ 12 ]->Fill( i, FADC1_CH4->at( i ) );	
		fHistos[ 13 ]->Fill( i, FADC1_CH5->at( i ) );
		fHistos[ 14 ]->Fill( i, FADC1_CH6->at( i ) );
		fHistos[ 15 ]->Fill( i, FADC1_CH7->at( i ) );
		
		fHistos[ 16 ]->Fill( i, FADC2_CH0->at( i ) );	
		fHistos[ 17 ]->Fill( i, FADC2_CH1->at( i ) );
		fHistos[ 18 ]->Fill( i, FADC2_CH2->at( i ) );
		fHistos[ 19 ]->Fill( i, FADC2_CH3->at( i ) );
		fHistos[ 20 ]->Fill( i, FADC2_CH4->at( i ) );	
		fHistos[ 21 ]->Fill( i, FADC2_CH5->at( i ) );
		fHistos[ 22 ]->Fill( i, FADC2_CH6->at( i ) );
		fHistos[ 23 ]->Fill( i, FADC2_CH7->at( i ) );
*/
	  
	}  	
	DrawHistos();
	//	c->Draw();
//   fChain->Show(entry);
	
	
}

Int_t ReadData::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return entry - entry + 1;
}

void ReadData::FillHistos(){

}

void ReadData::CreateHistos(){
	fHistos.clear();
	fHistos.resize( 3 * 8, 0 );
	
	const unsigned long SamplesCount = 44464; //TODO: Read from the tree
	
	for( unsigned i = 0; i < 8; ++i ){
		stringstream ss, ss1;
		ss << "H" << i;	
		ss1 << "fADC " << i/8 << " channel " << i % 8 << " Spectrum;Samples;Level";
		if( fHistos[ i ] != 0  ){
			cout << "Deleting histo " << i << endl;		
			delete fHistos[ i ];
		}
		
		std::cout << "Creating histogram "<< ss.str() << " ( "<< ss1.str() << " )" << std::endl;
		TH1F * 	hist = new TH1F( ss.str().c_str(), ss1.str().c_str(), SamplesCount, 0, SamplesCount );
		//cout << h;		
		fHistos[ i ] = hist;	
		//cout << " " << fHistos[ i ] << endl;
	}
}


void ReadData::CreateHistos1(){
	for( unsigned i = 0; i < fHistos.size(); ++i ){
		stringstream ss, ss1;
		ss << "H" << i;	
		ss1 << "fADC " << i/8 << " channel " << i % 8 << " Integral;Integral;Level";
		if( fHistos[ i ] != 0  ){
			cout << "Deleting histo " << i << endl;		
			delete fHistos[ i ];
		}
		TH1F * 	hist = new TH1F( ss.str().c_str(), ss1.str().c_str(), 255, 0, 255 );
		cout << hist;		
		fHistos[ i ] = hist;	
		cout << " " << fHistos[ i ] << endl;
	}
}

void ReadData::DrawHistos(){
	c->Draw();
	for( unsigned i = 0; i < 8; ++i ){
		c->cd( i + 1 );
		cout << "Drawing histogram " << fHistos[ i ]->GetName() << "..." <<endl;
		fHistos[ i ]->Draw();
		c->Modified();
	}
	c->Update();
}


#endif // #ifdef ReadData_cxx
