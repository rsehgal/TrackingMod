/*
 * ReadTracks.cpp
 *
 *  Created on: 10-Nov-2020
 *      Author: rsehgal
 */

#include "SingleMuonTrack.h"
#include <iostream>
#include <TTree.h>
#include <TFile.h>
#include "ScintillatorBar_V2.h"
#include "Plotter.h"
#include <vector>
#include<iterator>
#include <TH1F.h>
#include <TApplication.h>
#include "Calibration.h"
#include "HardwareNomenclature.h"
#include <TGraphErrors.h>
#include "SingleMuonTrack.h"
#include "Analyzer.h"
int main(int argc,char *argv[]){
	/*
	 * Set lite_interface::IsSimulation = true, if need to process simulated data file
	 * else for experimental data set it to false
	 */
	lite_interface::IsSimulation = true;
	lite_interface::Calibration *calib = lite_interface::Calibration::instance("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib.root");

	TApplication *fApp = new TApplication("Test", NULL, NULL);
	std::string outputFileName=argv[1];
	TFile *hitPointFile = new TFile(outputFileName.c_str(),"READ");
	TTree *hitPointVecTree = (TTree*)hitPointFile->Get("HitPointVecTree");
	//TTree *hitPointVecTree_Param = (TTree*)hitPointFile->Get("HitPointVecTreeParam");
	std::vector<lite_interface::Point3D*> *vecOfPoint3D = 0;
	double energySum = 0;
	hitPointVecTree->SetBranchAddress("HitPointVec",&vecOfPoint3D);
	hitPointVecTree->SetBranchAddress("EnergySum",&energySum);

	std::vector<lite_interface::Point3D*> *vecOfPoint3D_Param = 0;
	hitPointVecTree->SetBranchAddress("HitPointVecParam",&vecOfPoint3D_Param);

	std::vector<double> *energyVec = new std::vector<double>;
	hitPointVecTree->SetBranchAddress("EnergyVector",&energyVec);
	//hitPointVecTree_Param->SetBranchAddress("HitPointVec",&vecOfPoint3D_Param);

	std::vector<lite_interface::Point3D*> *vecOfPoint3D_MeanHit = 0;
	hitPointVecTree->SetBranchAddress("MeanHitPointVec",&vecOfPoint3D_MeanHit);

	Long64_t nentries = hitPointVecTree->GetEntries();

	Long64_t nbytes = 0;

	lite_interface::SingleMuonTrack *smt = 0;
	TTree *trackTree = (TTree*)hitPointFile->Get("TracksTree");
	trackTree->SetBranchAddress("MuonTracks",&smt);


	unsigned int count = 0;
	TH1F *histDiff = new TH1F("EstimateDiffHist","EstimateDiffHist",100,-30.,30.);
	TGraphErrors *gr;

	std::vector<double> vecOfZenithAngle_Linear;
	std::vector<double> vecOfZenithAngle_Param;
#ifdef USE_FOR_SIMULATION
		std::vector<double> vecOfZenithAngle_MeanHit;
#endif

	for (Long64_t i=0; i<nentries;i++) {
		energyVec->clear();
		nbytes += hitPointVecTree->GetEntry(i);
		/*for(unsigned int j=0 ; j < vecOfPoint3D->size() ; j++){
			vecOfPoint3D->at(j)->Print();
		}
		return 0;*/
		nbytes += trackTree->GetEntry(i);
		//nbytes += hitPointVecTree_Param->GetEntry(i);
		histDiff->Fill((vecOfPoint3D->at(0)->GetZ() - vecOfPoint3D_Param->at(0)->GetZ()));
		if(energySum > 400. || energySum < -400.)
			continue;


		//return GetZenithAngle(CreateFittedTrack(Get3DHitPointVector()));
		vecOfZenithAngle_Linear.push_back(GetZenithAngle(CreateFittedTrack(*vecOfPoint3D)));
		vecOfZenithAngle_Param.push_back(GetZenithAngle(CreateFittedTrack(*vecOfPoint3D_Param)));
#ifdef USE_FOR_SIMULATION
		vecOfZenithAngle_MeanHit.push_back(GetZenithAngle(CreateFittedTrack(*vecOfPoint3D_MeanHit)));
#endif

		if(i >90 && i < 130){

			std::cout << "------------ Event Num : " << i << " --------------------" << std::endl;
			std::cout << "Energy sum : "  << energySum << std::endl;
			smt->Print();
			for(unsigned int k = 0 ; k < vecOfPoint3D_MeanHit->size() ; k++){
				vecOfPoint3D_MeanHit->at(k)->Print();
			}

			std::string title = "Event Num : "+std::to_string(i)+" : EnergySum : "+std::to_string(energySum);
			TCanvas *can = new TCanvas(title.c_str(),title.c_str());
			//TCanvas *can = new TCanvas();
			can->Divide(4,2);
			can->cd(1);
			DrawGrid("Muon Track in XY plane; X axis ; Y axis", 9, 9);
			PlotMuonTrackXY(*vecOfPoint3D)->Draw("p");

			can->cd(2);
			DrawGrid("Muon Track in ZY plane; Z axis ; Y axis", 9, 9);
			PlotMuonTrackZY(*vecOfPoint3D)->Draw("p");
			if(i==129){
				//new TCanvas();
				gr = PlotMuonTrackZY(*vecOfPoint3D);
				std::cout << "===== Printing energy Information of Event : " << i << " =======" << std::endl;
				double totalEner = 0;
				for(unsigned short j = 0 ; j < energyVec->size() ; j++){
					double ener = energyVec->at(j) ;
					std::cout << ener << std::endl;
					totalEner += ener;
				}
				std::cout << "EnergySum of the event : " << totalEner << std::endl;
			}

			/*
			 * Trying to get Fitted Tracks
			 */

			can->cd(3);
			DrawGrid("Fitted Muon Track in XY plane; X axis ; Y axis", 9, 9);
			PlotMuonTrackXY(CreateFittedTrack(*vecOfPoint3D))->Draw("p");

			can->cd(4);
			DrawGrid("Fitted Muon Track in ZY plane; Z axis ; Y axis", 9, 9);
			PlotMuonTrackZY(CreateFittedTrack(*vecOfPoint3D))->Draw("p");


			//---------------- Parameterized stuff ------------------

			can->cd(5);
			DrawGrid("Param Muon Track in XY plane; X axis ; Y axis", 9, 9);
			PlotMuonTrackXY(*vecOfPoint3D_Param)->Draw("p");

			can->cd(6);
			DrawGrid("Param Muon Track in ZY plane; Z axis ; Y axis", 9, 9);
			PlotMuonTrackZY(*vecOfPoint3D_Param)->Draw("p");

			/*
			 * Trying to get Fitted Tracks
			 */

			can->cd(7);
			DrawGrid("Param Fitted Muon Track in XY plane; X axis ; Y axis", 9, 9);
			PlotMuonTrackXY(CreateFittedTrack(*vecOfPoint3D_Param))->Draw("p");

			can->cd(8);
			DrawGrid("Param Fitted Muon Track in ZY plane; Z axis ; Y axis", 9, 9);
			PlotMuonTrackZY(CreateFittedTrack(*vecOfPoint3D_Param))->Draw("p");
		}
	}

	new TCanvas();
	histDiff->Draw();

	new TCanvas();
	gr->Draw("p");

	TFile *fw = new TFile("test.root","RECREATE");
	fw->cd();
	gr->Write();
	fw->Close();

	new TCanvas();
	TH1F *histLinear=lite_interface::PlotZenithAngle(vecOfZenithAngle_Linear,1);
	histLinear->Draw();

	new TCanvas();
	TH1F *histParam=lite_interface::PlotZenithAngle(vecOfZenithAngle_Param,2);
	histParam->Draw();

	new TCanvas();
	TH1F *histMeanHit=lite_interface::PlotZenithAngle(vecOfZenithAngle_MeanHit,3);
	histMeanHit->Draw();

	fApp->Run();
	return 0;
}



