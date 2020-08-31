/*
 * Analyzer.cpp
 *
 *  Created on: 21-Aug-2020
 *      Author: rsehgal
 */

#include "Analyzer.h"
#include "TTree.h"
#include "includes.hh"
#include "PsBar.h"
#include "TreeEntry.h"



Analyzer::Analyzer() {
	// TODO Auto-generated constructor stub

}

Analyzer::~Analyzer() {
	// TODO Auto-generated destructor stub
}

Analyzer::Analyzer(std::string calibFileName, std::string dataFileName) {
	fCalib = new Calibration(calibFileName);
	fDataFileName = dataFileName;
	TFile *fp = new TFile(fDataFileName.c_str(), "r");
	UShort_t brch; //! board #  and channel number ( its packed in as follows )	//! board*16 + chno.
	UInt_t qlong;   //! integrated charge in long gate 88 nsec
	ULong64_t tstamp;  //! time stamp in pico sec.
	UInt_t time;    //! real computer time in sec
	Int_t dt;      //! delt between near and far channels

	TTree *tr = (TTree*) fp->Get("ftree");
	tr->SetBranchAddress("fBrCh", &brch);
	tr->SetBranchAddress("fQlong", &qlong);
	tr->SetBranchAddress("fTstamp", &tstamp);
	tr->SetBranchAddress("fTime", &time);
	tr->SetBranchAddress("fDelt", &dt);

	UInt_t CurrTime = 0;
	UInt_t StartTime = 0;
	UInt_t StopTime = 0;

	float minCh = 0;
	float maxCh = 32000;
	int nbins = (int) (maxCh - minCh) / 1;

	Long64_t nb = 0;


	TTimeStamp *times = new TTimeStamp();
	Long64_t nEntries = tr->GetEntries();
	for (Long64_t iev = 0; iev < nEntries; iev++) {
		nb += tr->GetEntry(iev);
		if (0)
			std::cout << brch << " , " << qlong << " , " << tstamp << " , " << time << ", " << dt << std::endl;
		fVecOfTreeEntry.push_back(TreeEntry(brch, qlong, tstamp, time, dt));
		if (iev % 500000 == 0) {
			times->Set(time, kTRUE, offset, kFALSE);
			std::cout << " Processing event : " << iev << "\t" << times->GetTimeSpec() << std::endl;
		}

		if (iev == 0) {
			StartTime = time;
			CurrTime = time;
		} else
			StopTime = time;

		UShort_t qch_n = (UShort_t)(((UInt_t) qlong) >> 16);
		UShort_t qch_f = (UShort_t)((UInt_t) qlong);
		float gmean = sqrt(qch_n * qch_f);

		float delt = 1.0 * dt / 1000.;
	}      //! event loop

	TreeEntryVector pairedEntryVec = CheckPairs(fVecOfTreeEntry);
	scintBarVec = DetectMuonHits(pairedEntryVec);
	std::cout <<"========== Trying to Sort ScintillatorBar Vector ==============" << std::endl;
	std::sort(scintBarVec.begin(),scintBarVec.end(),CompareTimestampScintillator);
	PrintScintillatorVector(scintBarVec);
	std::vector<std::vector<ScintillatorBar*>> muonTrackVec = DetectMuonTracks(scintBarVec);
	PrintMuonTrackVectorAllLayers(muonTrackVec);
	std::cout<<"=========================================== Trying to estimate hit position ===========================================" << std::endl;
	for(unsigned int index = 0 ; index < muonTrackVec.size() ; index++){
		EstimateZPositionForAnEventOnBar(muonTrackVec[index]);
	}
	PrintMuonTrackVectorAllLayers(muonTrackVec);

}

void Analyzer::PerformDelTCorrection(){
	for(unsigned int index = 0 ; index < scintBarVec.size(); index++){
		scintBarVec[index]->corrected = true;
		std::cout << "delT of barIndex : " << scintBarVec[index]->barIndex << " : " << scintBarVec[index]->deltaTstamp 
		<< " : CorrectionOffset : " 
				  << fCalib->GetCalibrationDataOf(scintBarVec[index]->barIndex)->fDeltaTCorr
				  << std::endl;
		scintBarVec[index]->deltaTstampCorrected = 
				scintBarVec[index]->deltaTstamp - 
				fCalib->GetCalibrationDataOf(scintBarVec[index]->barIndex)->fDeltaTCorr*1000;
	}
	std::cout<< "DelTCorrection done..." <<std::endl;
}

void Analyzer::EstimateZPositionOn(unsigned int barIndex){
	TF1 *param = fCalib->GetCalibrationDataOf(barIndex)->fParameterization_F;
	TH1F *hitZPos = new TH1F("Hit Position along Z","Hit Position along Z",100,-70.,70.0);
	TH2F *hitZPos2D = new TH2F("Hit Position along Z 2D","Hit Position along Z 2D",100,-70.,70.0,100,0.,100.);
	for(unsigned int index = 0 ; index < scintBarVec.size(); index++){
		if(scintBarVec[index]->barIndex == barIndex){
			long double correctedDelT = scintBarVec[index]->deltaTstampCorrected/1000.;
			float estZ = param->Eval(correctedDelT);
			std::cout << "Corrected DelT : " << correctedDelT << " : Hit Position along Z : " << estZ << std::endl;
			if(estZ > -50. && estZ < 50.){
			hitZPos->Fill(estZ);
			hitZPos2D->Fill(estZ,49.5);
			}
		}
	}
	new TCanvas();
	hitZPos->Draw();
	new TCanvas();
	hitZPos2D->Draw();

}

/*
 * Function to estimate the Hit position for a complete event on Bar
 * It does delT correction and then calculates the estimated Z position.
 *
 * For X and Y, for the time being we are taking 5cm as X, and top plane
 * of scintillator bar as Y
 *
 * @input : Single Muon Track for an event
 * @output : Returns nothing, instead it just sets the **hitPosition** for
 *           Scintillator Bars that forms the track
 */
void Analyzer::EstimateZPositionForAnEventOnBar(std::vector<ScintillatorBar*> singleMuonTrack){
	unsigned short startIndex = singleMuonTrack[0]->barIndex;
	unsigned short endIndex = singleMuonTrack[singleMuonTrack.size()-1]->barIndex;

	TF1 *paramStart = fCalib->GetCalibrationDataOf(startIndex)->fParameterization_F;
	TF1 *paramEnd = fCalib->GetCalibrationDataOf(endIndex)->fParameterization_F;

	/*
	 * Doing delT correction for an event
	 */
	for(unsigned int index = 0 ; index < singleMuonTrack.size() ; index++){
		singleMuonTrack[index]->corrected = true;
		singleMuonTrack[index]->deltaTstampCorrected =
							singleMuonTrack[index]->deltaTstamp -
							fCalib->GetCalibrationDataOf(singleMuonTrack[index]->barIndex)->fDeltaTCorr*1000;

		TF1 *param = fCalib->GetCalibrationDataOf(index)->fParameterization_F;
		long double correctedDelT = scintBarVec[index]->deltaTstampCorrected/1000.;
		float estZ = param->Eval(correctedDelT);
		if(estZ > -50. && estZ < 50.){
			singleMuonTrack[index]->EstimateHitPositionAlongX();
			singleMuonTrack[index]->EstimateHitPositionAlongZ();//Corresponds to Y, Will change the name later to Y
			(singleMuonTrack[index]->hitPosition).y=estZ;

		}


	}

	/*singleMuonTrack[0]->deltaTstampCorrected =
					singleMuonTrack[0]->deltaTstamp -
					fCalib->GetCalibrationDataOf(startIndex)->fDeltaTCorr*1000;

	singleMuonTrack[singleMuonTrack.size()-1]->deltaTstampCorrected =
			singleMuonTrack[singleMuonTrack.size()-1]->deltaTstamp -
					fCalib->GetCalibrationDataOf(endIndex)->fDeltaTCorr*1000;

	for(unsigned int index = 0 ; index < scintBarVec.size(); index++){
		if(scintBarVec[index]->barIndex == barIndex){
			long double correctedDelT = scintBarVec[index]->deltaTstampCorrected/1000.;
			float estZ = param->Eval(correctedDelT);
			std::cout << "Corrected DelT : " << correctedDelT << " : Hit Position along Z : " << estZ << std::endl;
			if(estZ > -50. && estZ < 50.){
			hitZPos->Fill(estZ);
			hitZPos2D->Fill(estZ,49.5);
			}
		}
	}
*/
}


void Analyzer::PlotHistOf(unsigned int barIndex){
		unsigned int nbins=100, tstart = -25000, tend = 25000;
		TH1D *histDelT=new TH1D("DeltaT / DeltaT Corrected","TimeDiffHist",nbins,tstart,tend);
		histDelT->SetLineColor(1);
		TH1D *histDelTCorr=new TH1D("DeltaT Corr","DeltaT Corr",nbins,tstart,tend);
		histDelTCorr->SetLineColor(2);
		for(unsigned int index = 0 ; index < scintBarVec.size(); index++){
			if(scintBarVec[index]->barIndex == barIndex){
				histDelT->Fill(scintBarVec[index]->deltaTstamp);
				histDelTCorr->Fill(scintBarVec[index]->deltaTstampCorrected);
			}
		}
		auto legend = new TLegend();//0.1,0.7,0.48,0.9);
	    legend->SetHeader("DeltaT histograms","C"); // option "C" allows to center the header
	    legend->AddEntry(histDelT,"DeltaT Histogram","f");
	    legend->AddEntry(histDelTCorr,"DeltaT Corrected Histogram","l");
	    //legend->AddEntry("f1","Function abs(#frac{sin(x)}{x})","l");
	    //legend->AddEntry("gr","Graph with error bars","lep");
	    legend->Draw();
		histDelT->Draw();
		histDelTCorr->Draw("same");
		
}
