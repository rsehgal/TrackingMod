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
#include "Analyzer.h"
#include <TF1.h>
int main(int argc,char *argv[]){
	/*
	 * Set lite_interface::IsSimulation = true, if need to process simulated data file
	 * else for experimental data set it to false
	 */
	lite_interface::IsSimulation = true;

	//TFile *f=new TFile(argv[2],"RECREATE");

	TApplication *fApp = new TApplication("Test", NULL, NULL);
	GenerateScintMatrixXYCenters();
	lite_interface::SingleMuonTrack *smt = new lite_interface::SingleMuonTrack;
	//std::vector<lite_interface::ScintillatorBar_V2*> *smt = new std::vector<lite_interface::ScintillatorBar_V2*>;
	//lite_interface::Calibration *calib = lite_interface::Calibration::instance("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib.root");
	lite_interface::Calibration *calib = lite_interface::Calibration::instance("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib2.root");
	std::string outputFileName=argv[1];
	TFile *trackFile = new TFile(outputFileName.c_str(),"READ");
	TTree *trackTree = (TTree*)trackFile->Get("TracksTree");
	trackTree->SetBranchAddress("MuonTracks",&smt);

	Long64_t nentries = trackTree->GetEntries();

	Long64_t nbytes = 0;
	//if(nentries > 5)
		//nentries = 5;

	unsigned int barIndex = 5;
	std::vector<lite_interface::ScintillatorBar_V2*> vectOfScintBars;
	//std::vector<lite_interface::ScintillatorBar_V2*> vectOfScintBars2;
	//std::vector<lite_interface::ScintillatorBar_V2*> vectOfScintBarsProfile;
	//vectOfScintBars.reserve(nentries);

	  std::vector<lite_interface::ScintillatorBar_V2*>::iterator itr;

	  std::vector<lite_interface::SingleMuonTrack*> smtVec;

	  std::vector<lite_interface::ScintillatorBar_V2*> scintBarVecForNewTrack;

	unsigned int count = 0;
	unsigned int count2 = 0;
	std::cout << "Total number of Entries : " << nentries << std::endl;

	unsigned int countVisTracks = 0;

	//nentries = 100;

	for (Long64_t i=0; i<nentries;i++) {
		//std::cout << "Fetching Entry : " << i << std::endl;
		nbytes += trackTree->GetEntry(i);
		if(!(i % 10000) && i!=0)
			std::cout << "Processed : " << i << " entries........" << std::endl;


		/*
		 * Trying to plot few tracks
		 */
		/*if(i<1){
			new TCanvas();
			TGraphErrors *grxy = lite_interface::PlotMuonTrack(smt,2);
			grxy->SetMarkerStyle(8);
			grxy->Draw("p");
			//DrawGrid("Muon Track in XY plane; X axis ; Y axis", 9, 9);
			f->cd();
			grxy->Write();
			f->Close();
		}*/

		//if(i < 5 )
		{
			//std::cout << "==============================" << std::endl;
			//if((smt->GetMuonTrack()).size() > 20 )
			//	smt->Print();

			std::vector<lite_interface::Point3D*> fittedLinearTrack = CreateFittedTrack(smt->Get3DHitPointVector());
			std::vector<lite_interface::Point3D*> fittedParamTrack = CreateFittedTrack(smt->Get3DHitPointVector_Param());
#ifdef USE_FOR_SIMULATION
			std::vector<lite_interface::Point3D*> fittedMeanTrack = CreateFittedTrack(smt->GetMean3DHitPointVector());

			double zenithAngleMean = smt->GetZenithAngle(3);
			double zenithAngleExact = smt->GetZenithAngle(4);

			if(smt->size() > 6 && countVisTracks < 20){
				std::string title="Track : "+std::to_string(countVisTracks)+" : ZenithAngleMean : "+std::to_string(zenithAngleMean)+" : ZenithAngleExact : "+std::to_string(zenithAngleExact);
				TCanvas *can = new TCanvas(title.c_str(),title.c_str());
				can->Divide(2,3);
				can->cd(1);
				DrawGrid("Muon Track in XY plane; X axis ; Y axis", 9, 9);
				//PlotMuonTrackXY(CreateFittedTrack(smt->GetExact3DHitPointVector()))->Draw("p");
				PlotMuonTrackXY(smt->GetExact3DHitPointVector())->Draw("p");
				can->cd(2);
				DrawGrid("Muon Track in ZY plane; Z axis ; Y axis", 9, 9);
				//PlotMuonTrackZY(CreateFittedTrack(smt->GetExact3DHitPointVector()))->Draw("p");
				PlotMuonTrackZY(smt->GetExact3DHitPointVector())->Draw("p");


				can->cd(3);
				DrawGrid("Mean Muon Track in XY plane; X axis ; Y axis", 9, 9);
				//PlotMuonTrackXY(CreateFittedTrack(smt->GetExact3DHitPointVector()))->Draw("p");
				PlotMuonTrackXY(smt->GetMean3DHitPointVector())->Draw("p");
				can->cd(4);
				DrawGrid("Mean Muon Track in ZY plane; Z axis ; Y axis", 9, 9);
				//PlotMuonTrackZY(CreateFittedTrack(smt->GetExact3DHitPointVector()))->Draw("p");
				PlotMuonTrackZY(smt->GetMean3DHitPointVector())->Draw("p");

				can->cd(5);
				DrawGrid("Linear Muon Track in XY plane; X axis ; Y axis", 9, 9);
				//PlotMuonTrackXY(CreateFittedTrack(smt->GetExact3DHitPointVector()))->Draw("p");
				PlotMuonTrackXY(smt->Get3DHitPointVector())->Draw("p");
				can->cd(6);
				DrawGrid("Linear Muon Track in ZY plane; Z axis ; Y axis", 9, 9);
				//PlotMuonTrackZY(CreateFittedTrack(smt->GetExact3DHitPointVector()))->Draw("p");
				PlotMuonTrackZY(smt->Get3DHitPointVector())->Draw("p");

				countVisTracks++;
			}
#endif



			std::vector<lite_interface::ScintillatorBar_V2*> scintBarVecOfATrack = smt->GetMuonTrack();
			//std::cout << "============ Printing from REad Tracks =============" << std::endl;
			for(unsigned int j = 0 ; j < scintBarVecOfATrack.size() ; j++){
#ifdef USE_FOR_SIMULATION
				//scintBarVecOfATrack[j]->fittedMean = fittedMeanTrack[j];
#endif
				//scintBarVecOfATrack[j]->fittedLinear = fittedLinearTrack[j];
				//scintBarVecOfATrack[j]->fittedParam = fittedParamTrack[j];

				//scintBarVecOfATrack[j]->Print();
			}

			scintBarVecForNewTrack.clear();

			for(unsigned int j = 0 ; j < scintBarVecOfATrack.size() ; j++){

				//(smt->GetMuonTrack())[j]->Print();
				if(scintBarVecOfATrack[j]->GetBarIndex() > 81){
					count++;

				}

				//vectOfScintBarsProfile.push_back(new lite_interface::ScintillatorBar_V2(*scintBarVecOfATrack[j]));
				vectOfScintBars.push_back(new lite_interface::ScintillatorBar_V2(*scintBarVecOfATrack[j]));
				scintBarVecForNewTrack.push_back(new lite_interface::ScintillatorBar_V2(*scintBarVecOfATrack[j]));
			}
			smtVec.push_back(new lite_interface::SingleMuonTrack(scintBarVecForNewTrack));
			//std::cout << "============== Printing Muon Tracks after reading from testSim.root ================" << std::endl;
			//smtVec[i]->Print();
		}

	}

	std::cout << "Number of Scintillators with wrong BarIndex : " << count << std::endl;

	std::cout << "++++++++++++++++++++ VecOfScintBars Size : " << vectOfScintBars.size() << " ++++++++++++++++++++++++++++" << std::endl;
	for(unsigned int i = 0 ; i< vectOfScintBars.size() ; i++){
		if(i < 5){
		//	vectOfScintBars[i]->Print();
			//std::cout << vectOfScintBars[i]->GetBarIndex() << std::endl;
		}
	}

	//f->cd();

	TH1F *histQmeanCorrected = PlotQMeanCorrected(vectOfScintBars,5);
	histQmeanCorrected->SetLineColor(kGreen);
	TH1F *histQmeanCorrected2 = PlotQMeanCorrected(vectOfScintBars,15);
	histQmeanCorrected2->SetLineColor(kMagenta);
	new TCanvas();
	histQmeanCorrected->Draw();
	histQmeanCorrected2->Draw("same");

	new TCanvas();
	TH1F *histDelT = PlotDelT(vectOfScintBars,barIndex);
	TH1F *histDelTCorrected = PlotDelTCorrected(vectOfScintBars,barIndex);
	histDelTCorrected->SetLineColor(kMagenta);
	histDelT->Draw();
	histDelTCorrected->Draw("same");

	new TCanvas();
	TH1F *histBarProfile = PlotBarsProfile(vectOfScintBars);
	histBarProfile->Draw();

	new TCanvas();
	TH2F* hitPointsOnBar = PlotHitPointsOnBar(vectOfScintBars,5);
	hitPointsOnBar->Draw();

	new TCanvas();
	TH1F *energySumHist = PlotEnergySum(smtVec);
	energySumHist->Draw();

	new TCanvas("ZenithAngleWithLinearEstimation","Zenith Angle With Linear Estimation");
	TH1F *zenithAngleHist = PlotZenithAngle(smtVec,1);
	zenithAngleHist->Draw();
	//zenithAngleHist->Write();


	new TCanvas("ZenithAngleWithParamEstimation","Zenith Angle With Param Estimation");
	TH1F *zenithAngleHist_Param = PlotZenithAngle(smtVec,2);
	zenithAngleHist_Param->Draw();
	//zenithAngleHist_Param->Write();

#ifdef USE_FOR_SIMULATION
	new TCanvas("ZenithAngleWithMeanHitPoint","Zenith Angle With Mean Hit Point");
	TH1F *zenithAngleHist_MeanHitPoint = PlotZenithAngle(smtVec,3);
	zenithAngleHist_MeanHitPoint->Draw();
	//zenithAngleHist_MeanHitPoint->Write();


	new TCanvas("ZenithAngleWithExactHitPoint","Zenith Angle With Exact Hit Point");
	TH1F *zenithAngleHist_ExactHitPoint = PlotZenithAngle(smtVec,4);
	zenithAngleHist_ExactHitPoint->Draw();
#endif

	new TCanvas("DelT Vs Z","DelT Vs Z");
	TGraph *delTvsZ = PlotDelTvsZ(vectOfScintBars,5);
	delTvsZ->Draw("ap");
	TF1 *formula = calib->GetCalibrationDataOf(5)->fParameterization_F;
	formula->SetLineColor(2);
	formula->Draw("same");
	TGraph *delTvsZ_Linear = PlotDelTvsZ(vectOfScintBars,5,true);
	delTvsZ_Linear->SetLineColor(4);
	delTvsZ_Linear->Draw("same");

	std::vector<TH1D*> vecHist = lite_interface::PlotEnergyDistributionWithMultiplicity(smtVec);
	new TCanvas("Energy with multiplicity..","Energy with multiplicity..");
	for(unsigned int i = 0; i < vecHist.size();  i++){
		if(vecHist[i]->GetEntries() > 10){
			//vecHist[i]->Scale(1/vecHist[i]->Integral());
			new TCanvas();
			vecHist[i]->Draw();
		}
	}

	std::cout << "@@@@@@@@ Wrong track Counter : " << lite_interface::SingleMuonTrack::wrongTrackCounter << std::endl;
	//f->Close();
	TH1F *histZ = new TH1F("ZHist","ZHist",100,-50,50);
	unsigned int countNew = 0 ;
	std::vector<unsigned int> vecOfScintId = {50,59,68,77};
	for(unsigned int i = 0 ; i < smtVec.size() ; i++){
		if(smtVec[i]->CheckTrackForRequiredScintillators(vecOfScintId))
			for(unsigned int j = 0 ; j < smtVec[i]->size() ; j++){
				if((smtVec[i]->GetMuonTrack())[j]->GetBarIndex() == 68)
					histZ->Fill((smtVec[i]->GetMuonTrack())[j]->EstimateHitPosition_Param()->GetZ());
			}

			countNew++;
	}
	std::cout << "Total number of Muons with required scint pattern : " << countNew << std::endl;
	histZ->Draw();
	fApp->Run();

	return 0;
}



