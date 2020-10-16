/*
 * TestDataExp.cpp
 *
 *  Created on: 12-Oct-2020
 *      Author: rsehgal
 */

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
#include "FittedTracks.h"
int main(){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	bool verbose = false;
	Calibration *cb = new Calibration("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib.root");
	//Analyzer_V2 av("/home/rsehgal/BackBoneSoftwares/ismranData/ISMRAN_81bars_Th10All_CosmicRun_15hrs42mins_26Aug2020_2.root",cb,0,false);
	Analyzer_V2 av("/home/rsehgal/BackBoneSoftwares/ismranData/ISMRAN_81bars_Th10All_CosmicRun_09hrs34mins_02Sep2020_0.root",cb,0,false);
	std::vector< SingleMuonTrack* > muonTrackVec = av.ReconstrutTrack_V2();

	av.PlotHistOfNumOfMuonHitsInMuonTracks_V2(muonTrackVec);
	av.CalculateTotalEnergyDepositionForMuonTracks(muonTrackVec);

	std::cout << std::endl << "CalculateTotalEnergyDepositionForMuonTracks... DONE...." << std::endl;

	HistInitializer();
	av.FillCoincidenceHist_V2(muonTrackVec);
	av.PlotCoincidenceCountGraph();
	av.PlotEnergyDistributionWithMultiplicity(muonTrackVec,0);

	std::cout << std::endl << "PlotEnergyDistributionWithMultiplicity... DONE...." << std::endl;


	/* Dummy test
	 * Actually this will go to a function in Analyzer
	 *
	 * Uncomment the below block to plot some fitted tracks.
	 */

	/*unsigned int numOfTracksToPlot = 10;
	unsigned int counter = 0 ;
	for(unsigned int i = 0 ; i < muonTrackVec.size() ;i++){

		if(muonTrackVec[i]->size() == numOfLayers){
			counter++;
			muonTrackVec[i]->CreateFittedMuonTrack(1);
			muonTrackVec[i]->fFittedTrack->PlotTrack();
			if(counter==numOfTracksToPlot)
				break;
		}
	}*/

	//std::vector<SingleMuonTrack*> filtered = av.FiltrationBasedOnCosmicMuonRate(muonTrackVec);

	std::cout << std::endl << "Number of Muon Tracks to be processed : " << muonTrackVec.size() << std::endl;
	av.fittedMuonTracks = av.GetFittedTrackVector(muonTrackVec,1);
	std::cout <<"Size : Estimated SOL HitPointVec : " << av.fittedMuonTracks.size() << std::endl;
	TH1 *hist3 = PlotZenithAngle(av.fittedMuonTracks,"Zenith Angle Hist using Estimated Hit Points from SOL");

	TH1 *hist32 = PlotZenithAngle(av.fittedMuonTracks,"(Non-Normalized)Zenith Angle Hist using Estimated Hit Points from SOL");
	hist32->SetLineColor(4);

	hist3->Scale(1/hist3->Integral());
	hist3->SetLineColor(4);
	new TCanvas();
	hist3->Draw();

	av.fittedMuonTracks = av.GetFittedTrackVector(muonTrackVec,0);
	std::cout <<"Size : Estimated Param HitPointVec : " << av.fittedMuonTracks.size() << std::endl;
	TH1 *hist5 = PlotZenithAngle(av.fittedMuonTracks,"Zenith Angle Hist using Estimated Hit Points from Parameterization");

	TH1 *hist52 = PlotZenithAngle(av.fittedMuonTracks,"(Non-Normalized)Zenith Angle Hist using Estimated Hit Points from Parameterization");;
	hist52->SetLineColor(2);

	hist5->Scale(1/hist5->Integral());
	hist5->SetLineColor(2);
	hist5->Draw("same");

	TH1 *hist4 = PlotZenithAngle_XY(av.fittedMuonTracks,"Zenith Angle Hist using X, Y of Estimated Hit Points from SOL ");
	hist4->Scale(1/hist4->Integral());
	hist4->SetLineColor(4);
	new TCanvas();
	hist4->Draw();


	new TCanvas();
	hist32->Draw();
	hist52->Draw("same");

	TH1 *hist53 = PlotZenithAngle(av.fittedMuonTracks,"Normalized Zenith Angle Hist using Estimated Hit Points from Parameterization");
	hist53->Scale(1/hist53->Integral());
	new TCanvas();
	hist53->Draw();

/*
	unsigned int counter = 0;
	for(unsigned int i = 0 ; i < muonTrackVec.size() ;i++){
		counter++;
		if(!(counter%100000))
			std::cout << "Processed : " << counter << " events.........." << std::endl;

		muonTrackVec[i]->CreateFittedMuonTrack(1);
		std::vector<Point3D*> fitedtrack = muonTrackVec[i]->fFittedTrack->GetFittedTrack();
		if(!CheckRange(fitedtrack))
					av.fittedMuonTracks.push_back(fitedtrack);

		if(muonTrackVec[i]->size() >= numOfLayers){
			std::cout << "=============================== @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ =======================" << std::endl;
			//muonTrackVec[i]->Print();
			muonTrackVec[i]->CreateFittedMuonTrack(1);
			av.fittedMuonTracks.push_back(muonTrackVec[i]->fFittedTrack->GetFittedTrack());
			PrintPoint3DVector(muonTrackVec[i]->fFittedTrack->GetFittedTrack());
		}
	}

	std::cout << std::endl << "Fitted Track vector read... DONE...." << std::endl;

	//av.fittedMuonTracks = av.PlotTracks_V2(filtered,0,false);

	av.PlotZenithAngle();
	std::cout << std::endl << "Plotting Zenith Angle in 3D Done........." << std::endl;

	av.PlotZenithAngle_XY();
	std::cout << std::endl << "Plotting Zenith Angle in XY Done........." << std::endl;

	std::cout << std::endl << "Zenith Angle distribution ... DONE...." << std::endl;

	std::cout << std::endl << "EXECUTED THE FINAL INSTRUCTION..............." << std::endl;
*/

	fApp->Run();
}


