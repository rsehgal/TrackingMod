#define ReadData_cxx
#include "ReadData.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void ReadData::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L ReadData.C
//      Root > ReadData t
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
   CreateHistos1();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry < nentries; jentry++ ) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      cout << "SIZE = " << FADC0_CH1->size() << endl;

      if( ( jentry % 100 == 0 ) && ( jentry > 0 ) ) cout << jentry << " entries analysed... " << endl;
      
      cout << "TTT = " << FADC0_HEADER4 
      //<< ", " << FADC1_HEADER4 << ", " << FADC2_HEADER4 
      << endl; 
		//cout << "Size = " << FADC0_CH0->size() << endl;
		vector< double > Integrals( 24, 0 );	
		for( unsigned i = 0; i < FADC0_CH1->size(); ++i ){
			//cout << i << " " << FADC0_CH0->at( i ) << endl;
		
			Integrals[ 0 ] += FADC0_CH0->at( i );	
			Integrals[ 1 ] += FADC0_CH1->at( i );
			Integrals[ 2 ] += FADC0_CH2->at( i );
			Integrals[ 3 ] += FADC0_CH3->at( i );
			Integrals[ 4 ] += FADC0_CH4->at( i );	
			Integrals[ 5 ] += FADC0_CH5->at( i );
			Integrals[ 6 ] += FADC0_CH6->at( i );
			Integrals[ 7 ] += FADC0_CH7->at( i );
/*		
			Integrals[ 8 ] += FADC1_CH0->at( i );	
			Integrals[ 9 ] += FADC1_CH1->at( i );
			Integrals[ 10 ] += FADC1_CH2->at( i );
			Integrals[ 11 ] += FADC1_CH3->at( i );
			Integrals[ 12 ] += FADC1_CH4->at( i );	
			Integrals[ 13 ] += FADC1_CH5->at( i );
			Integrals[ 14 ] += FADC1_CH6->at( i );
			Integrals[ 15 ] += FADC1_CH7->at( i );
		
			Integrals[ 16 ] += FADC2_CH0->at( i );	
			Integrals[ 17 ] += FADC2_CH1->at( i );
			Integrals[ 18 ] += FADC2_CH2->at( i );
			Integrals[ 19 ] += FADC2_CH3->at( i );
			Integrals[ 20 ] += FADC2_CH4->at( i );	
			Integrals[ 21 ] += FADC2_CH5->at( i );
			Integrals[ 22 ] += FADC2_CH6->at( i );
			Integrals[ 23 ] += FADC2_CH7->at( i );	
*/		}
/*		fHistos[ 0 ]->Fill( Integrals[ 0 ]/FADC0_CH0->size() );	
		fHistos[ 1 ]->Fill( Integrals[ 1 ]/FADC0_CH0->size() );
		fHistos[ 2 ]->Fill( Integrals[ 2 ]/FADC0_CH0->size() );
		fHistos[ 3 ]->Fill( Integrals[ 3 ]/FADC0_CH0->size() );
		fHistos[ 4 ]->Fill( Integrals[ 4 ]/FADC0_CH0->size() );	
		fHistos[ 5 ]->Fill( Integrals[ 5 ]/FADC0_CH0->size() );
		fHistos[ 6 ]->Fill( Integrals[ 6 ]/FADC0_CH0->size() );
		fHistos[ 7 ]->Fill( Integrals[ 7 ]/FADC0_CH0->size() );
		
		fHistos[ 8 ]->Fill( Integrals[ 8 ]/FADC0_CH0->size() );	
		fHistos[ 9 ]->Fill( Integrals[ 9 ]/FADC0_CH0->size() );
		fHistos[ 10 ]->Fill( Integrals[ 10 ]/FADC0_CH0->size() );
		fHistos[ 11 ]->Fill( Integrals[ 11 ]/FADC0_CH0->size() );
		fHistos[ 12 ]->Fill( Integrals[ 12 ]/FADC0_CH0->size() );	
		fHistos[ 13 ]->Fill( Integrals[ 13 ]/FADC0_CH0->size() );
		fHistos[ 14 ]->Fill( Integrals[ 14 ]/FADC0_CH0->size() );
		fHistos[ 15 ]->Fill( Integrals[ 15 ]/FADC0_CH0->size() );
			
		fHistos[ 16 ]->Fill( Integrals[ 16 ]/FADC0_CH0->size() );	
		fHistos[ 17 ]->Fill( Integrals[ 17 ]/FADC0_CH0->size() );
		fHistos[ 18 ]->Fill( Integrals[ 18 ]/FADC0_CH0->size() );
		fHistos[ 19 ]->Fill( Integrals[ 19 ]/FADC0_CH0->size() );
		fHistos[ 20 ]->Fill( Integrals[ 20 ]/FADC0_CH0->size() );	
		fHistos[ 21 ]->Fill( Integrals[ 21 ]/FADC0_CH0->size() );
		fHistos[ 22 ]->Fill( Integrals[ 22 ]/FADC0_CH0->size() );
		fHistos[ 23 ]->Fill( Integrals[ 23 ]/FADC0_CH0->size() );  
	*/
	DrawHistos();
	}
}
