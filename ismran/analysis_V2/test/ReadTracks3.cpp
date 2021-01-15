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
struct ScintData{
	ULong64_t sTAvg;
	float sZ;
	Long_t sDelT;

	ScintData(ULong64_t tavg,float z,Long_t delt){
		sTAvg = tavg;
		sZ = z;
		sDelT = delt;
	}

	ScintData(float z,Long_t delt){
		sTAvg = 0.;
		sZ = z;
		sDelT = delt;
	}
};

bool IsCorrelatedEvent(std::vector<ScintData*> vecOfScintData,double startDelTInPs, double endDelTInPs, double zExtInCm){
	bool delTInRange = true;
	for(unsigned int i = 0 ; i < vecOfScintData.size() ; i++){
		delTInRange &= ( (vecOfScintData[i]->sDelT > startDelTInPs) && (vecOfScintData[i]->sDelT < endDelTInPs) );
	}

	bool zInReqExt = true;
	for(unsigned int i = 0 ; i < vecOfScintData.size()-1 ; i++){
		double zdiff = (vecOfScintData[i]->sZ - vecOfScintData[i+1]->sZ);
		//std::cout << "Zdiff : " << zdiff << std::endl;
		zInReqExt &= (std::fabs(zdiff) < zExtInCm);
	}

	zInReqExt &= (std::fabs(vecOfScintData[0]->sZ - vecOfScintData[vecOfScintData.size()-1]->sZ) < zExtInCm);

	return (delTInRange && zInReqExt);

}

struct Hist{
	TH1F *sDelTHist;
	TH1F *sZHist;
	TH1F *sTAvHist;

	Hist(std::string name){
		sDelTHist = new TH1F((name+"_DelT").c_str(),(name+"_DelT").c_str(),100,-25000,25000);
		sZHist = new TH1F((name+"_Z").c_str(),(name+"_Z").c_str(),100,-100,100);
	}
};


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
	lite_interface::Calibration *calib = lite_interface::Calibration::instance("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib2.root");
	std::string outputFileName=argv[1];
	TFile *trackFile = new TFile(outputFileName.c_str(),"READ");
	TTree *trackTree = (TTree*)trackFile->Get("TracksTree");
	trackTree->SetBranchAddress("MuonTracks",&smt);


	Long64_t nentries = trackTree->GetEntries();

	Long64_t nbytes = 0;
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
			std::vector<lite_interface::ScintillatorBar_V2*> scintBarVecOfATrack = smt->GetMuonTrack();
			scintBarVecForNewTrack.clear();

			for(unsigned int j = 0 ; j < scintBarVecOfATrack.size() ; j++){

				//(smt->GetMuonTrack())[j]->Print();
				if(scintBarVecOfATrack[j]->GetBarIndex() > 81){
					count++;

				}
				vectOfScintBars.push_back(new lite_interface::ScintillatorBar_V2(*scintBarVecOfATrack[j]));
				scintBarVecForNewTrack.push_back(new lite_interface::ScintillatorBar_V2(*scintBarVecOfATrack[j]));
			}
			smtVec.push_back(new lite_interface::SingleMuonTrack(scintBarVecForNewTrack));
		}

	}
//----------------------------------------


	TH1F *histZ = new TH1F("ZHist","ZHist",100,-100,100);
	TH1F *confineHistZ = new TH1F("confineZHist","confineZHist",100,-100,100);
//	TH1F *confineHistZFitted = new TH1F("confineZHistFitted","confineZHistFitted",100,-100,100);
//	TH1F *histDelTCorr = new TH1F("histDelTCorr","histDelTCorr",100,-25000,25000);

	TH1F *histDelT = new TH1F("histDelT","histDelT",100,-25000,25000);
	TH1F *histDelT_Check = new TH1F("histDelT_Check","histDelT_Check",100,-25000,25000);

		unsigned int countNew = 0 ;
		//std::vector<unsigned int> vecOfScintId = {50,59,68,77};
		//std::vector<unsigned int> vecOfScintId = {49,58,67,76};
		//std::vector<unsigned int> vecOfScintId = {50,59,68,77};
		std::vector<unsigned int> vecOfScintId = {49,58,67};//,76};
		unsigned int countNewCrossChecked = 0;
		std::vector<float> delTVec;
		std::vector<float> zVec;
		barIndex = 58;
		unsigned int layerIndex = (barIndex/numOfBarsInEachLayer);
		unsigned int countConfinedMuons = 0;
		unsigned int validFirstCounter = 0;
		unsigned int validThirdCounter = 0;
		unsigned int validObjectCounter = 0;
		unsigned int countCorrTSeq = 0;
		unsigned int countValid = 0 ;

		int bins = 11;
		float extremeVal = 8000;
		float jump = (2*extremeVal)/bins;


		std::vector<Hist*> vecOfHist;
		//Hist *histObj = new Hist();
		//for (location slots loop)

		TH1F *delT_01 = new TH1F("DelT_01","DelT_12",100,-10000.,10000.);
		TH1F *delT_12 = new TH1F("DelT_12","DelT_23",100,-10000.,10000.);
		TH1F *delT_20 = new TH1F("DelT_20","DelT_31",100,-10000.,10000.);

		std::vector<TH1F*> interBarHistVec;
		for( unsigned int tIndex = 0 ; tIndex < bins ; tIndex++)
		{
			TH1F *histInterBarDelT = new TH1F(("TIndex_"+std::to_string(tIndex)).c_str(),("TIndex_"+std::to_string(tIndex)).c_str(),100,-10000.,10000.);
			std::cout <<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
			float lowerDelT = -1.*extremeVal+jump*tIndex;
			float upperDelT = -1*extremeVal+jump*(tIndex+1);

			vecOfHist.clear();
			for(unsigned int i = 0 ; i < vecOfScintId.size() ; i++){
				vecOfHist.push_back(new Hist(vecOfBarsNamess[vecOfScintId[i]]));
			}

			std::vector<ScintData*> vecOfScintData;

			std::vector<ULong64_t> vecOfTAv;
			for(unsigned int i = 0 ; i < smtVec.size() ; i++){
				vecOfScintData.clear();
				if(smtVec[i]->CheckTrackForRequiredScintillators(vecOfScintId)){

					for(unsigned int j = 0 ; j < smtVec[i]->size() ; j++){
						bool exist = false;
						for(unsigned int m = 0 ; m < vecOfScintId.size() ; m++){
							exist = ((smtVec[i]->GetMuonTrack())[j]->GetBarIndex() == vecOfScintId[m]);
							if(exist){
								vecOfScintData.push_back(new ScintData((smtVec[i]->GetMuonTrack())[j]->GetTAverage(),
																	   (smtVec[i]->GetMuonTrack())[j]->EstimateHitPosition_Param()->GetZ(),
																	   (smtVec[i]->GetMuonTrack())[j]->GetDelTCorrected() ) );
								break;
							}
						}

					}

					//if(IsCorrelatedEvent(vecOfScintData,6000.,8000.,10.)){
					if(IsCorrelatedEvent(vecOfScintData,lowerDelT,upperDelT,2.)){
						//std::cout << vecOfScintData.size() << " : " <<  vecOfHist.size() << std::endl;
						if(vecOfScintData.size() == vecOfHist.size()){
							for(unsigned int k = 0 ; k < vecOfScintData.size() ; k++){
								/*
								 * Storing TAvg of first Bar
								 */
								/*{
									if(k==0)
										vecOfTAv.push_back(vecOfScintData[k]->sTAvg);
								}*/

								vecOfHist[k]->sDelTHist->Fill(vecOfScintData[k]->sDelT);
								vecOfHist[k]->sZHist->Fill(vecOfScintData[k]->sZ);
							}
						}

						Long_t diff = (vecOfScintData[0]->sTAvg - vecOfScintData[1]->sTAvg);
						histInterBarDelT->Fill(diff);

						if(tIndex == (bins/2)){

							Long_t diff = (vecOfScintData[0]->sTAvg - vecOfScintData[1]->sTAvg);
							//std::cout << "DiffVal : " << diff  << std::endl;
							//delT_01->Fill(vecOfScintData[0]->sTAvg - vecOfScintData[1]->sTAvg);
							delT_01->Fill(diff);

							diff = (vecOfScintData[1]->sTAvg - vecOfScintData[2]->sTAvg);
							//delT_12->Fill(vecOfScintData[1]->sTAvg - vecOfScintData[2]->sTAvg);
							delT_12->Fill(diff);

							diff = (vecOfScintData[2]->sTAvg - vecOfScintData[0]->sTAvg);
							//delT_20->Fill(vecOfScintData[2]->sTAvg - vecOfScintData[0]->sTAvg);
							delT_20->Fill(diff);
						}
					}


				}


			}

			interBarHistVec.push_back(histInterBarDelT);

			/*ULong64_t minVal = *min_element(vecOfTAv.begin(), vecOfTAv.end());
			ULong64_t maxVal = *max_element(vecOfTAv.begin(), vecOfTAv.end());
			std::cout << "@@@@ MIN_ELEMENT : " << minVal <<" : MAX_ELEMENT : " << maxVal <<  " : Diff : " << (maxVal - minVal) << " @@@@" << std::endl;
			 */
			for(unsigned int i = 0 ; i < vecOfHist.size() ; i++){
				std::cout <<"====================================================" << std::endl;
				std::cout << "For DelT Hist of : " << vecOfBarsNamess[vecOfScintId[i]] << " : No. Of Entries : " << vecOfHist[i]->sDelTHist->GetEntries()
						  << " : Mean : " << vecOfHist[i]->sDelTHist->GetMean()
						  << " : SD : " << vecOfHist[i]->sDelTHist->GetStdDev() << std::endl;
				std::cout << "For Z Hist of : " << vecOfBarsNamess[vecOfScintId[i]] << " : No. Of Entries : " << vecOfHist[i]->sZHist->GetEntries()
						  << " : Mean : " << vecOfHist[i]->sZHist->GetMean()
						  << "  :SD : " << vecOfHist[i]->sZHist->GetStdDev() << std::endl;

				if(i==0){
					delTVec.push_back(vecOfHist[i]->sDelTHist->GetMean()/1000.);
					zVec.push_back(vecOfHist[i]->sZHist->GetMean());
				}
			}
		}

		std::cout << "SIZE OF INTERBAR HIST  " << interBarHistVec.size() << std::endl;
		new TCanvas("InterBar","InterBar");
		for(unsigned int i = 0 ; i < interBarHistVec.size() ; i++){
			interBarHistVec[i]->SetLineColor(i+1);
			interBarHistVec[i]->Draw("same");
		}

		new TCanvas("TAvg Diff","TAvg Diff");
		delT_01->SetLineColor(1);
		delT_01->Draw();
		delT_12->SetLineColor(2);
		delT_12->Draw("same");
		delT_20->SetLineColor(4);
		delT_20->Draw("same");

		new TCanvas("Param","Param");
		TF1 *param = calib->GetCalibrationDataOf(49)->fParameterization_F;
		TGraph *gr = new TGraph(delTVec.size(),&delTVec[0],&zVec[0]);
		gr->SetMarkerStyle(8);
		gr->SetMarkerSize(0.8);
		gr->Draw("ap");
		param->Draw("same");
#if(0)
		for(unsigned int i = 0 ; i < smtVec.size() ; i++){

			if(smtVec[i]->CheckTrackForRequiredScintillators(vecOfScintId)){
				ULong64_t startDelT = 0.;
				ULong64_t endDelT = 0.;
				Long_t delTInterBar = 0;
				double zval = 0.;
				bool valid = false;
				bool validFirstBar = false;
				bool validThirdBar = false;
				bool validObjectBar = false;
				std::cout <<"===========================================" << std::endl;
				for(unsigned int j = 0 ; j < smtVec[i]->size() ; j++){

					bool yes=true;
					if((smtVec[i]->GetMuonTrack())[j]->GetBarIndex() == 49){
						endDelT = (smtVec[i]->GetMuonTrack())[j]->GetTAverage();//GetTSmallTimestamp();//GetDelTCorrected();
						validFirstBar =  true;
						if(validFirstBar)
							validFirstCounter++;
						std::cout << "Encountered Bar : " << vecOfBarsNamess[(smtVec[i]->GetMuonTrack())[j]->GetBarIndex()] << ": EndDelT : " << endDelT << std::endl;
					}

					if((smtVec[i]->GetMuonTrack())[j]->GetBarIndex() == 67){
						startDelT = (smtVec[i]->GetMuonTrack())[j]->GetTAverage();//GetTSmallTimestamp();//GetDelTCorrected();
						validThirdBar = true;
						if(validThirdBar)
							validThirdCounter++;
						std::cout << "Encountered Bar : " << vecOfBarsNamess[(smtVec[i]->GetMuonTrack())[j]->GetBarIndex()] << ": StartDelT : " << startDelT << std::endl;
					}

					if((smtVec[i]->GetMuonTrack())[j]->GetBarIndex() == barIndex){
						zval = (smtVec[i]->GetMuonTrack())[j]->EstimateHitPosition_Param()->GetZ();
						validObjectBar = true;
						double delt = (smtVec[i]->GetMuonTrack())[j]->GetDelTCorrected();
						histDelT_Check->Fill(delt);
						//std::cout << delt << std::endl;
						validObjectBar &= (delt > -1000 && delt < 1000);
						if(validObjectBar)
							validObjectCounter++;
					}
				}
				if(startDelT > endDelT)
					countCorrTSeq++;

				valid = (validFirstBar && validThirdBar && validObjectBar);

				if(valid){
					//delTInterBar = (double)std::fabs(endDelT - startDelT);
					delTInterBar = endDelT - startDelT;
					std::cout <<"delTInterBar : " << delTInterBar << std::endl;
					histDelT->Fill(delTInterBar);
					countValid++;
				}

				//std::cout << "EndDelT : " << endDelT <<" :: StartDelT : " << startDelT << std::endl;

			}
		}

		std::cout <<"===========================================" << std::endl;
		std::cout << "CorrectDelT-Seq : " << countCorrTSeq << std::endl;
		std::cout << "ValidFirst : " << validFirstCounter <<" : ValidThird : " << validThirdCounter <<" : ValidObject : " << validObjectCounter << " : CountValid : " << countValid << std::endl;
		std::cout <<"===========================================" << std::endl;

//--------------------------------------------------

		new TCanvas();
		histDelT_Check->Draw();

		new TCanvas();
		histDelT->Draw();
#endif
	fApp->Run();

	return 0;
}



