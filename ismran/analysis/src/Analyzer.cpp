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
#include "Plotter.h"



Analyzer::Analyzer() {
	// TODO Auto-generated constructor stub

}

Analyzer::~Analyzer() {
	// TODO Auto-generated destructor stub
}

void Analyzer::SystemHealthCheckup(TreeEntryVector treeEntryVec){
	unsigned int poscounter = 0;
	unsigned int negcounter = 0;
	for(unsigned int index = 0  ; index < treeEntryVec.size() ;){
		unsigned int near = treeEntryVec[index].brch;
		unsigned int far = treeEntryVec[index+1].brch;
		unsigned int barIndex = near/2;
		histVec[5*barIndex]->Fill(treeEntryVec[index].qlong);
		histVec[5*barIndex+1]->Fill(treeEntryVec[index+1].qlong);
		histVec[5*barIndex+2]->Fill(sqrt(treeEntryVec[index].qlong*treeEntryVec[index+1].qlong));
		int diff = treeEntryVec[index].tstamp - treeEntryVec[index+1].tstamp;
		if(diff < 0.)
			negcounter++;
		else
			poscounter++;
		histVec[5*barIndex+3]->Fill(diff);
//		histVec[5*barIndex+4]->Fill();

		index += 2;
	}
	std::cout <<"Positive Counter : " << poscounter <<" : Negative Counter : " << negcounter << std::endl;
}


void Analyzer::DisplaySystemHealth(){
	TCanvas *systemHealthCanvas = new TCanvas("TestCanvas","TestCanvas",800,600);
	unsigned int totalNumOfBars = numOfLayers*numOfBarsInEachLayer;
	systemHealthCanvas->Divide(3,19);
	/*systemHealthCanvas->Divide(2,2);
	systemHealthCanvas->cd(1);
	histVec[0]->SetLineColor(1);
	histVec[0]->Draw();
	histVec[1]->SetLineColor(2);
	histVec[1]->Draw("same");*/
	for(unsigned int index = 0 ; index < totalNumOfBars ; index++){
		std::cout << "Changing to subplot : " << (4*index) << std::endl;
		//int plotIndex = 4*index;
		int plotIndex = index;
		systemHealthCanvas->cd(plotIndex+1);
		//new TCanvas();
		histVec[5*index]->SetLineColor(1);
		histVec[5*index]->Draw();
		histVec[5*index+1]->SetLineColor(2);
		histVec[5*index+1]->Draw("same");

		/*systemHealthCanvas->cd(4*index+1);
		histVec[5*index+2]->Draw();

		systemHealthCanvas->cd(4*index+2);
		histVec[5*index+3]->Draw();*/



	}
//	systemHealthCanvas->Update();
	/*histVec[0]->SetLineColor(1);
	histVec[0]->Draw();
	histVec[1]->SetLineColor(2);
	histVec[1]->Draw("same");*/



}

void Analyzer::InitializeHistograms(){
	unsigned int numOfChannels = numOfLayers*numOfBarsInEachLayer*2;
	for(unsigned short int i = 0 ; i < numOfChannels ; ){
			unsigned int barIndex=i/2;
			std::string barId="Bar-"+std::to_string(barIndex);
			std::string histNearName = barId+"-Near";
			std::string histFarName = barId+"-Far";
			std::string histMeanName = barId+"-Mean";
			std::string histDelTName = barId+"-DelT";
			std::string histDelTCorrName = barId+"-DelTCorrected";
			//const char *histMeanName=barId.c_str();
			int qstart=0;
			int qend=25000;
			int nbins=500;
			histVec.push_back(new TH1D(histNearName.c_str(),histNearName.c_str(),nbins,qstart,qend));
			histVec.push_back(new TH1D(histFarName.c_str(),histFarName.c_str(),nbins,qstart,qend));
			histVec.push_back(new TH1D(histMeanName.c_str(),histMeanName.c_str(),nbins,qstart,qend));
			histVec.push_back(new TH1D(histDelTName.c_str(),histDelTName.c_str(),nbins,-25000,25000));
			histVec.push_back(new TH1D(histDelTCorrName.c_str(),histDelTCorrName.c_str(),nbins,-25000,25000));
			i=i+2;
		}

	std::cout << "@@@@@@@@@@@ Size of HIST-VECTOR : " << histVec.size() <<" @@@@@@@@@@" << std::endl;
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

/*
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
	PlotNMuonTrack(muonTrackVec,20);


*/
	std::cout << "========== Trying to Check System Health ============" << std::endl;
	InitializeHistograms();
	SystemHealthCheckup(pairedEntryVec);
	DisplaySystemHealth();

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
 * This function should be called only after **EstimateZPositionForAnEventOnBar**
 */
void Analyzer::EstimateFittedHitPosition(std::vector<ScintillatorBar*> singleMuonTrack){

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
			//singleMuonTrack[index]->EstimateHitPositionAlongY();//Corresponds to Y, Will change the name later to Y
			//(singleMuonTrack[index]->hitPosition).x=0;
			(singleMuonTrack[index]->hitPosition).y=0;
			(singleMuonTrack[index]->hitPosition).z=estZ;

		}


	}

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
