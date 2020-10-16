#include "DataTree.h"
#include "HardwareNomenclature.h"
#include "ScintillatorBar_V2.h"
#include "Analyzer_V2.h"
#include <TApplication.h>
#include "SingleMuonTrack.h"
#include "Histograms.h"
#include "HelperFunctions.h"
#include "Calibration.h"
#include "colors.h"
#include <TFile.h>
#include <TLegend.h>
int main(){

	TFile *f = new TFile("AFile.root","RECREATE");
	TLegend *legend = new TLegend();//0.1,0.7,0.48,0.9);
	legend->SetHeader("ZenithAngle Dist.","C");

	bool verbose = false;
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

	double hitX;
	double hitY;
	double hitZ;



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
	fTree->SetBranchAddress("hitX",&hitX);
	fTree->SetBranchAddress("hitY",&hitY);
	fTree->SetBranchAddress("hitZ",&hitZ);

	unsigned int fNumOfEntries = fTree->GetEntries();

	Calibration *cb = new Calibration("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib.root");
	Analyzer_V2 av;

	GenerateScintMatrixXYCenters();
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
		scintBarVec.push_back(new ScintillatorBar_V2(tstampNear,tstampFar,qlongMean,barIndex,hitX/10.,hitY/10.,hitZ/10.));
		scintBarVec[i]->EstimateHitPosition_V2(cb);
	}


	if(verbose){
	//JUST FOR DEBUGGING
		std::cout << "============== DEBUGGING PRINT STATEMENTS ==============" << std::endl;
		fNumOfEntries = 10;
		for(unsigned int i = 0 ; i < fNumOfEntries ; i++){
			scintBarVec[i]->Print();
		}
	}


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
	
	av.PlotEnergyDistributionWithMultiplicity(muonTrackVec,6);
	
	//av.fittedMuonTracks = av.PlotTracks_V2(muonTrackVec,0,false);
	//av.fittedMuonTracks = av.GetFittedMeanHitPointTrack(muonTrackVec);

	av.fittedMuonTracks = av.GetFittedTrackVector(muonTrackVec,2);
	std::cout <<"Size : MeanHitPointVec : " << av.fittedMuonTracks.size() << std::endl;

	if(verbose){
		std::cout << "=============== TRYING TO PRINT FITTED MUON TRACKS ===============" << std::endl;
		for(unsigned int i = 0 ; i < av.fittedMuonTracks.size() ; i++){
			std::cout <<"============================================================" << std::endl;
			for(unsigned int j = 0 ; j < av.fittedMuonTracks[i].size() ; j++){
				av.fittedMuonTracks[i][j]->Print();
			}

		}
	}
	av.InitializeHistograms();
	av.FillHistograms();
	av.DisplayHistograms();
	std::cout << "Size of Fitted Muon Track vector : " << av.fittedMuonTracks.size() << std::endl;
	TH1 *hist = PlotZenithAngle(av.fittedMuonTracks,"Zenith Angle Hist using Exact Mean Hit Points");
	hist->Scale(1/hist->Integral());
	hist->SetLineColor(3);
	new TCanvas();
//	hist->Draw();

	//vecOfHists[i]->Scale(1/vecOfHists[i]->Integral());


	//delete hist;
	//av.fittedMuonTracks = av.PlotTracks_V2(muonTrackVec,0,false);
	av.fittedMuonTracks = av.GetFittedTrackVector(muonTrackVec,0);
	std::cout <<"Size : Estimated Parameterized HitPointVec : " << av.fittedMuonTracks.size() << std::endl;
	TH1 *hist2 = PlotZenithAngle(av.fittedMuonTracks,"Zenith Angle Hist using Estimated Hit Points from Parameterization");
	hist2->Scale(1/hist2->Integral());
	hist2->SetLineColor(2);


	av.fittedMuonTracks = av.GetFittedTrackVector(muonTrackVec,1);
	std::cout <<"Size : Estimated SOL HitPointVec : " << av.fittedMuonTracks.size() << std::endl;
	TH1 *hist3 = PlotZenithAngle(av.fittedMuonTracks,"Zenith Angle Hist using Estimated Hit Points from SOL");
	hist3->Scale(1/hist3->Integral());
	hist3->SetLineColor(4);

	legend->AddEntry(hist,"Mean Hit Point","l");
	legend->AddEntry(hist2,"Est. Hit Point Param","l");
	legend->AddEntry(hist3,"Est. Hit Point SOL","l");



	hist->Draw();
	hist2->Draw("same");
	hist3->Draw("same");
	legend->Draw();

	f->cd();
	hist->Write();
	hist2->Write();
	hist3->Write();

	av.PlotZenithAngle_XY();
	av.PlotDiffZHistogram();

	std::cout << std::endl << "========================================" << std::endl;
	std::cout << "LAST INSTRUCTION EXECUTED.........." << std::endl;

	fApp->Run();
	return 0;
}
