#include "DataTree.h"
#include "HardwareNomenclature.h"
#include "ScintillatorBar_V2.h"
#include "Analyzer_V2.h"
#include <TApplication.h>
#include "SingleMuonTrack.h"
#include "Histograms.h"
#include "HelperFunctions.h"
int main(){
	/*DataTree t("simulatedData.root");
	unsigned int numOfEntries = t.GetEntries();
	numOfEntries = 10;
	TTree *tr= t.GetTree();
	for(unsigned int i = 0 ; i < numOfEntries ; i++){
		//t.GetEntry(i);
		//t.Print();
		tr->GetEntry(i);
		t.Print();
	}
*/
	//const char*  scintName; 
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	std::string scintName;
	UInt_t    qlongNear;   //! integrated charge in long gate 88 nsec
	UInt_t    qlongFar;   //! integrated charge in long gate 88 nsec
	//UInt_t    qlongMean;   //! integrated charge in long gate 88 nsec
	Double_t    qlongMean;   //! integrated charge in long gate 88 nsec
	Double_t    qlongMeanCorrected;   //! integrated charge in long gate 88 nsec


	ULong64_t tstampNear;  //! time stamp in pico sec.
	ULong64_t tstampFar;  //! time stamp in pico sec.
	ULong64_t tsmallTimeStamp;  //! time stamp in pico sec.
	Long64_t deltaTstamp;  //! time stamp in pico sec.
	Long64_t deltaTstampCorrected;

	unsigned short barIndex;
	unsigned short layerIndex;



	TFile *fp = new TFile("simulatedData.root");
	TTree* fTree = (TTree*) fp->Get("ftree");
	//float mass, charge, ...;
	//fTree->SetBranchAddress("scintName", &scintName);
	fTree->SetBranchAddress("qlongNear", &qlongNear);
	fTree->SetBranchAddress("qlongFar", &qlongFar);
	fTree->SetBranchAddress("qlongMean", &qlongMean);
	fTree->SetBranchAddress("qlongMeanCorrected", &qlongMeanCorrected);
	fTree->SetBranchAddress("tstampNear", &tstampNear);
	fTree->SetBranchAddress("tstampFar", &tstampFar);
	fTree->SetBranchAddress("tsmallTimeStamp", &tsmallTimeStamp);
	fTree->SetBranchAddress("deltaTstamp", &deltaTstamp);
	fTree->SetBranchAddress("deltaTstampCorrected", &deltaTstampCorrected);
	fTree->SetBranchAddress("barIndex", &barIndex);
	fTree->SetBranchAddress("layerIndex", &layerIndex);

	unsigned int fNumOfEntries = fTree->GetEntries();
	std::vector<ScintillatorBar_V2*> scintBarVec;
	//fNumOfEntries = 10;
	for(unsigned int i = 0 ; i < fNumOfEntries ; i++){
		fTree->GetEntry(i);
		scintName = vecOfBarsNamess[barIndex];
		//std::cout <<"======================================================================" << std::endl;
		/*std::cout << scintName <<" , " << qlongNear <<" , " << qlongFar << " , " <<qlongMean
		          << " , " << qlongMeanCorrected << " , " << tstampNear <<" , " << tstampFar
		          << " , " << tsmallTimeStamp << " , " << deltaTstamp << " , " << deltaTstampCorrected
		          << " , " << barIndex << " , " << layerIndex << std::endl;*/
		scintBarVec.push_back(new ScintillatorBar_V2(tstampNear,tstampFar,qlongMean,barIndex));
	}


	/*for(unsigned int i = 0 ; i < fNumOfEntries ; i++){
		scintBarVec[i]->Print();
	}*/
   	
	Analyzer_V2 av;
	av.fVecOfScintillatorBar = scintBarVec;
	std::vector< SingleMuonTrack* > muonTrackVec = av.ReconstrutTrack_V2();
	av.PlotHistOfNumOfMuonHitsInMuonTracks_V2(muonTrackVec);
	av.CalculateTotalEnergyDepositionForMuonTracks(muonTrackVec);


	/*for(unsigned int i = 0 ;i < muonTrackVec.size() ; i++){
		std::cout << "Energy deposited by Muon : " << i << " : "  << muonTrackVec[i]->fTotalEnergyDeposited << std::endl;
	}*/
	HistInitializer();
	av.FillCoincidenceHist_V2(muonTrackVec);
	av.PlotCoincidenceCountGraph();
	
	av.PlotEnergyDistributionWithMultiplicity(muonTrackVec,0);
	
	av.fittedMuonTracks = av.PlotTracks_V2(muonTrackVec,0,false);
	std::cout << "Size of Fitted Muon Track vector : " << av.fittedMuonTracks.size() << std::endl;
	av.PlotZenithAngle();
	fApp->Run();
	return 0;
}