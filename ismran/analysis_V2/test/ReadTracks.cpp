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
#include "PsBar.h"
int main(int argc,char *argv[]){
	/*
	 * Set lite_interface::IsSimulation = true, if need to process simulated data file
	 * else for experimental data set it to false
	 */
	lite_interface::IsSimulation = false;

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

		{
			//std::cout << "==============================" << std::endl;
			//if((smt->GetMuonTrack()).size() > 20 )
			//	smt->Print();

//			std::vector<lite_interface::Point3D*> fittedLinearTrack = CreateFittedTrack(smt->Get3DHitPointVector());
//			std::vector<lite_interface::Point3D*> fittedParamTrack = CreateFittedTrack(smt->Get3DHitPointVector_Param());
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

	std::vector<float> vecOfMeanDelT;
	std::vector<float> vecOfMeanZ;
	std::vector<float> vecOfMeanZFitted;
	std::vector<float> vecOfMeanZ_Err;
	std::vector<float> vecOfMeanDelT_Err;
	std::vector<float> vecOfMeanZFitted_Err;

	 int tIndex = 5;
	 int bins = 11;
	 TF1 *formPoly3 = new TF1("Pol3","[0]+[1]*x+[2]*x*x+[3]*x*x*x",-8.,8.);
	for( tIndex = 0 ; tIndex < bins ; tIndex++)
	{

	//float lowerDelT = -5000;// -10000+2000*tIndex;
	//float upperDelT = -3000;//-10000+2000*(tIndex+1);
	float extremeVal = 8000;
	float jump = (2*extremeVal)/bins;
	//float lowerDelT = -8000+1000*tIndex;
	//float upperDelT = -8000+1000*(tIndex+1);
	//float lowerDelT = -1.*extremeVal+1000*tIndex;
	//float upperDelT = -1*extremeVal+1000*(tIndex+1);
	float lowerDelT = -1.*extremeVal+jump*tIndex;
	float upperDelT = -1*extremeVal+jump*(tIndex+1);
	float meanDelT = (lowerDelT+upperDelT)/2.;
	std::cout <<"=====================================" << std::endl;
	std::cout << "MeanDelT : " << meanDelT << std::endl;
	std::cout <<"=====================================" << std::endl;
	vecOfMeanDelT.push_back(meanDelT/1000.);
	vecOfMeanDelT_Err.push_back(0.);


	TH1F *histZ = new TH1F("ZHist","ZHist",100,-100,100);
	TH1F *confineHistZ = new TH1F("confineZHist","confineZHist",100,-100,100);
	TH1F *confineHistZFitted = new TH1F("confineZHistFitted","confineZHistFitted",100,-100,100);
	TH1F *histDelTCorr = new TH1F("histDelTCorr","histDelTCorr",100,-25000,25000);

	TH1F *histDelT = new TH1F("histDelT","histDelT",100,-25000,25000);

		unsigned int countNew = 0 ;
		//std::vector<unsigned int> vecOfScintId = {50,59,68,77};
		//std::vector<unsigned int> vecOfScintId = {49,58,67,76};
		//std::vector<unsigned int> vecOfScintId = {50,59,68,77};
		std::vector<unsigned int> vecOfScintId = {49,58,67,76};
		unsigned int countNewCrossChecked = 0;
		std::vector<float> delTVec;
		std::vector<float> zVec;
		barIndex = 58;
		unsigned int layerIndex = (barIndex/numOfBarsInEachLayer);
		unsigned int countConfinedMuons = 0;
		for(unsigned int i = 0 ; i < smtVec.size() ; i++){

			if(smtVec[i]->CheckTrackForRequiredScintillators(vecOfScintId)){
				double startDelT = 0.;
									double endDelT = 0.;
									double zval = 0.;
				for(unsigned int j = 0 ; j < smtVec[i]->size() ; j++){
					/*
					 * Logic to confine the muon with delT
					 */
					/*double startDelT = 0.;
					double endDelT = 0.;
					double zval = 0.;*/

					if((smtVec[i]->GetMuonTrack())[j]->GetBarIndex() == 49){
						startDelT = (smtVec[i]->GetMuonTrack())[j]->GetDelTCorrected();
					}

					if((smtVec[i]->GetMuonTrack())[j]->GetBarIndex() == 76){
						endDelT = (smtVec[i]->GetMuonTrack())[j]->GetDelTCorrected();
					}

					if((smtVec[i]->GetMuonTrack())[j]->GetBarIndex() == barIndex){
						zval = (smtVec[i]->GetMuonTrack())[j]->EstimateHitPosition_Param()->GetZ();
					}



					if((smtVec[i]->GetMuonTrack())[j]->GetBarIndex() == barIndex){
						delTVec.push_back((smtVec[i]->GetMuonTrack())[j]->GetDelTCorrected());

						countNewCrossChecked++;
						lite_interface::Point3D *hitPt = (smtVec[i]->GetMuonTrack())[j]->EstimateHitPosition_Param();
						zVec.push_back(hitPt->GetZ());
						//hitPt->Print();
						histZ->Fill(hitPt->GetZ());
						histDelTCorr->Fill((smtVec[i]->GetMuonTrack())[j]->GetDelTCorrected());
						histDelT->Fill((smtVec[i]->GetMuonTrack())[j]->GetDelT());
						//std::cout << "delT : " << (smtVec[i]->GetMuonTrack())[j]->GetDelT() << " : Corr : " << (smtVec[i]->GetMuonTrack())[j]->GetDelTCorrected() << std::endl;
					}
				}

				//if(std::fabs(startDelT) < 1000 && std::fabs(endDelT) < 1000 ){

				//std::cout << "@@@@@ Mean Del T : " << meanDelT << std::endl;


				if(startDelT < upperDelT && startDelT > lowerDelT && endDelT < upperDelT && endDelT > lowerDelT ){
					countConfinedMuons++;
					confineHistZ->Fill(zval);
				//}


					std::vector<lite_interface::Point3D*> fittedParamTrack = CreateFittedTrack(smtVec[i]->Get3DHitPointVector_Param());
					for(unsigned int m = 0 ; m < fittedParamTrack.size() ; m++){
						if(fittedParamTrack[m]->GetLayer() == layerIndex){
							zval = fittedParamTrack[m]->GetZ();
							//std::cout << "ZVal : " << zval << std::endl;
							//std::cout << "@@@@@ Processing Muon track : " << i << " @@@@@" << std::endl;
							//fittedParamTrack[m]->Print();
							confineHistZFitted->Fill(zval);
						}
					}


				}

				countNew++;
			}
		}

#if(0)
	TGraph *delTvsZ = lite_interface::PlotDelTvsZ(barIndex,delTVec,zVec);
	std::cout << "Total number of Muons with required scint pattern : " << countNew << std::endl;
	std::cout << "CRoss checked : " << countNewCrossChecked << std::endl;
	new TCanvas();
	histZ->Draw();

	new TCanvas();
	histDelT->SetLineColor(2);
	histDelT->Draw();
	histDelTCorr->Draw("same");

	new TCanvas();
	delTvsZ->Draw("ap");



	TF1 *param = calib->GetCalibrationDataOf(barIndex)->fParameterization_F;
	new TCanvas();
	param->Draw();


	std::cout << "Number of Confined muon tracks : " << countConfinedMuons << std::endl;
	new TCanvas();
	//confineHistZ->Scale(1/confineHistZ->Integral());
	confineHistZ->Draw();

	new TCanvas();
	confineHistZFitted->SetLineColor(6);
	//confineHistZFitted->Scale(1/confineHistZFitted->Integral());
	confineHistZFitted->Draw();
#endif

	float meanOfHistZ = confineHistZ->GetMean();
	float meanOfHistZFitted = confineHistZFitted->GetMean();

	TF1 *formulaHistZ = new TF1("gaus","gaus",meanOfHistZ-20,meanOfHistZ+20);
	TF1 *formulaHistZFitted = new TF1("gaus","gaus",meanOfHistZFitted-20,meanOfHistZFitted+20);

	confineHistZ->Fit(formulaHistZ,"r");
	confineHistZFitted->Fit(formulaHistZFitted,"r");
	float meanZ = formulaHistZ->GetParameter(1);
	float sigmaZ = formulaHistZ->GetParameter(2);
	float meanZFitted = formulaHistZFitted->GetParameter(1);
	float sigmaZFitted = formulaHistZFitted->GetParameter(2);
	vecOfMeanZ.push_back(meanZ);
	vecOfMeanZ_Err.push_back(sigmaZ/2.);
	vecOfMeanZFitted.push_back(meanZFitted);
	vecOfMeanZFitted_Err.push_back(sigmaZFitted/2.);



	std::cout << "@@ MeanDelT : " << meanDelT << " :  MeanZ : " << meanZ <<" : MeanFittedZ : " << meanZFitted << std::endl;

/*	delete formulaHistZ;
	delete formulaHistZFitted;
	delete histZ;
	delete confineHistZ;
	delete confineHistZFitted;*/
	//delete histDelTCorr;

}

	TGraph *meanZvsDelT = new TGraph(vecOfMeanDelT.size(),&vecOfMeanDelT[0],&vecOfMeanZ[0]);
	meanZvsDelT->SetMarkerStyle(8);
	meanZvsDelT->SetMarkerSize(0.7);
	TGraphErrors *meanZvsDelT_Err = new TGraphErrors(vecOfMeanDelT.size(),&vecOfMeanDelT[0],&vecOfMeanZ[0],&vecOfMeanDelT_Err[0],&vecOfMeanZ_Err[0]);
	meanZvsDelT_Err->SetMarkerStyle(8);
	meanZvsDelT_Err->SetMarkerSize(0.7);
	TGraph *meanZvsDelTFitted = new TGraph(vecOfMeanDelT.size(),&vecOfMeanDelT[0],&vecOfMeanZFitted[0]);
	meanZvsDelTFitted->SetMarkerStyle(8);
	meanZvsDelTFitted->SetMarkerSize(0.7);
	TGraphErrors *meanZvsDelTFitted_Err = new TGraphErrors(vecOfMeanDelT.size(),&vecOfMeanDelT[0],&vecOfMeanZFitted[0],&vecOfMeanDelT_Err[0],&vecOfMeanZFitted_Err[0]);
	meanZvsDelTFitted_Err->SetMarkerStyle(8);
	meanZvsDelTFitted_Err->SetMarkerSize(0.7);

	new TCanvas();
	meanZvsDelT->Draw("ap");

	new TCanvas();
	meanZvsDelT_Err->Draw("ap");

	new TCanvas();
	meanZvsDelTFitted->Draw("ap");

	new TCanvas();
	meanZvsDelTFitted_Err->Draw("ap");

	meanZvsDelTFitted_Err->Fit(formPoly3,"r");


	TH1F *histZ_param = new TH1F("ZHist_Param","ZHist_Param",100,-100,100);
	TH1F *histZ_param_new = new TH1F("ZHist_Param_New","ZHist_Param_New",100,-100,100);
	//std::vector<float> zvec;
	//std::vector<float> deltVec;
	//TF1 *delT_58=(TF1*)_file0->Get("fdelt_shift_Cs137_PS59_S1AA6640_0cm")
	TF1 *delt_F = calib->GetCalibrationDataOf(barIndex)->fDelTCorr_F;
	TF1 *param = calib->GetCalibrationDataOf(barIndex)->fParameterization_F;
	float mean = delt_F->GetParameter(1);
	std::cout <<"@@@@ Sampling form the delT distribution @@@@" << std::endl;
	unsigned int numOfSamples = 10000;
	for(unsigned int i = 0 ; i < numOfSamples; i++){
		//deltVec.push_back(param->GetRandom()-mean);
		float delT_corr = delt_F->GetRandom()-mean;
		//zvec.push_back(formPoly3->Eval(delT_corr));
		float z_param_new = formPoly3->Eval(delT_corr);
		histZ_param_new->Fill(z_param_new);

		float z_param = param->Eval(delT_corr);
		histZ_param->Fill(z_param);
	}

	//new TCanvas();
	//param->Draw();

	new TCanvas();
	histZ_param->Draw();

	new TCanvas();
	histZ_param_new->Draw();


	new TCanvas();
	param->Draw();
	formPoly3->SetLineColor(2);
	formPoly3->Draw("same");


	fApp->Run();

	return 0;
}



