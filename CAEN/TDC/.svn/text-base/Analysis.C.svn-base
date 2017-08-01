#define Analysis_cxx
#include "Analysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
using std::cout;
using std::endl;

void Analysis::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L Analysis.C
//      Root > Analysis t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for ( Long64_t jentry = 0; jentry < nentries; ++jentry ) {
     Long64_t ientry = LoadTree( jentry ); 
     if (ientry < 0) break;
     cout << "Event " << jentry <<   ":" << endl;  
     
     nb = fChain->GetEntry(jentry);   
     nbytes += nb;
     if ( Cut( ientry ) < 0 ) continue;
     int min0( 1000 ), max0( 0 );
     int min1( 1000 ), max1( 0 );
     int min2( 1000 ), max2( 0 );
     int min0b( 100000 ), max0b( 0 );
     int min1b( 100000 ), max1b( 0 );
     int min2b( 100000 ), max2b( 0 );
     double Integral0 = 0.0; 
     double Integral1 = 0.0;
     double Integral2 = 0.0;

     for( int sample = 0; sample < FADC0_SAMPLES; ++sample ){
       int raw = FADC0_CH0->at( sample );
       if( raw > max0 ){ max0 = raw; max0b = sample; }
       if( min0 > raw ){ min0 = raw; min0b = sample; }
       Integral0 += raw;
       /*
       raw = FADC1_CH0->at( sample );
       if( raw > max1 ){ max1 = raw; max1b = sample; }
       if( min1 > raw ){ min1 = raw; min1b = sample; }
       Integral1 += raw;
       
       raw = FADC2_CH0->at( sample );
       if( raw > max2 ){ max2 = raw; max2b = sample; }
       if( min2 > raw ){ min2 = raw; min2b = sample; }
       Integral2 += raw;
       */
       //cout << raw << "\t";
     }
     Integral0 /= FADC0_SAMPLES;
     cout << "FADC 0:  Samples :" << FADC0_SAMPLES << ", Integral = " << Integral0 << "( min = " << min0 << " in sample " << min0b << " / max = " << max0 << " in sample " << max0b << " )" << endl;
     /*
     Integral1 /= FADC1_SAMPLES;
     cout << "FADC 1:  Samples :" << FADC1_SAMPLES << ", Integral = " << Integral1 << "( min = " << min1 << " in sample " << min1b << " / max = " << max1 << " in sample " << max1b << " )" << endl;
     
     Integral2 /= FADC2_SAMPLES;
     cout << "FADC 2:  Samples :" << FADC0_SAMPLES << ", Integral = " << Integral0 << "( min = " << min2 << " in sample " << min2b << " / max = " << max2 << " in sample " << max2b << " )" << endl;
     */
     cout << endl << endl;
   }
}
