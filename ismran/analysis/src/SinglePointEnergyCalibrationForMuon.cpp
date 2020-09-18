/*
 * SinglePointEnergyCalibrationForMuon.cpp
 *
 *  Created on: 17-Sep-2020
 *      Author: rsehgal
 */

#include "SinglePointEnergyCalibrationForMuon.h"
#include "includes.hh"
#include "ScintillatorBar_V2.h"
#include "Histograms.h"
//#include "Calibration.h"
#include <TH1I.h>
#include "SingleMuonTrack.h"
#include "HardwareNomenclature.h"
#include <TFile.h>
#include "HelperFunctions.h"

SinglePointEnergyCalibrationForMuon::SinglePointEnergyCalibrationForMuon(){

}

SinglePointEnergyCalibrationForMuon::SinglePointEnergyCalibrationForMuon(std::string datafileName) {
	// TODO Auto-generated constructor stub
	fDatafileName = datafileName;
	fout = new TFile("singlePointMuonEnergyCalib.root","RECREATE");
	//fMapOfEnergyCalibFactorForMuon = new TMap();
	LoadDataAndSort();
	CheckPairs();
	ValidatePairs();
	CreateScintillatorVector();
	DoSinglePointEnergyCalibrationForMuon();
}

SinglePointEnergyCalibrationForMuon::~SinglePointEnergyCalibrationForMuon() {
	// TODO Auto-generated destructor stub
	delete fout;
	Reset();
}

SinglePointEnergyCalibrationForMuon::SinglePointEnergyCalibrationForMuon(std::vector<ScintillatorBar_V2*> vecOfScintillatorBar){
	fVecOfScintillatorBar = vecOfScintillatorBar;
	DoSinglePointEnergyCalibrationForMuon();
}

void SinglePointEnergyCalibrationForMuon::LoadDataAndSort() {
	TFile *fp = new TFile(fDatafileName.c_str(), "r");
	UShort_t brch; //! board #  and channel number ( its packed in as follows )	//! board*16 + chno.
	UInt_t qlong;   //! integrated charge in long gate 88 nsec
	ULong64_t tstamp;  //! time stamp in pico sec.
	UInt_t time;    //! real computer time in sec

	TTree *tr = (TTree*) fp->Get("ftree");
	tr->SetBranchAddress("fBrCh", &brch);
	tr->SetBranchAddress("fQlong", &qlong);
	tr->SetBranchAddress("fTstamp", &tstamp);
	tr->SetBranchAddress("fTime", &time);

	TTimeStamp *times = new TTimeStamp();
	Long64_t nEntries = tr->GetEntries();
	std::cout << "Total number of Entries : " << nEntries << std::endl;

	Long64_t nb = 0;

	for (Long64_t iev = 0; iev < nEntries; iev++) {
		nb += tr->GetEntry(iev);
		if (0)
			std::cout << brch << " , " << qlong << " , " << tstamp << " , " << time << std::endl;

		fVecOfTreeEntry.push_back(new TreeEntry(brch, qlong, tstamp, time));

		if (iev % 500000 == 0) {
			times->Set(time, kTRUE, offset, kFALSE);
			std::cout << " Processing event : " << iev << "\t"
					<< times->GetTimeSpec() << std::endl;
		}

	}      //! event loop

	fp->Close();

	return;
}

void SinglePointEnergyCalibrationForMuon::CheckPairs(){
	std::cout << "=============== Checking Pairs =================="<< std::endl;

	for (unsigned int i = 0; i < fVecOfTreeEntry.size();) {

		if (abs(fVecOfTreeEntry[i]->brch - fVecOfTreeEntry[i + 1]->brch) == 1) {
			fVecOfPairedTreeEntry.push_back(fVecOfTreeEntry[i]);
			fVecOfPairedTreeEntry.push_back(fVecOfTreeEntry[i + 1]);
			i = i + 2;
		}else{
			i++;
		}

	}
	std::cout << "Length of Paired Entries Vector : " << fVecOfPairedTreeEntry.size()
			<< std::endl;
	std::cout << "Percentage of Unpaired Entries : "
			<< (1.*(fVecOfTreeEntry.size()-fVecOfPairedTreeEntry.size()) / fVecOfTreeEntry.size()) * 100 << " %"
			<< std::endl;
	fVecOfTreeEntry.clear();
}

void SinglePointEnergyCalibrationForMuon::ValidatePairs(){
	std::cout << "=============== Validating Pairs =================="<< std::endl;
	std::cout << "Size of vector to be validated : " << fVecOfPairedTreeEntry.size() << std::endl;
	unsigned int counter = 0 ;
	unsigned int counterDelT = 0;
	unsigned int counterOddSmall = 0;
	unsigned int notAPairCounter = 0;
	std::vector<TreeEntry*> vecOfValidatedPairedTreeEntry;
	for(unsigned long int i= 0 ; i < fVecOfPairedTreeEntry.size() ; ){

		unsigned int smallChannelNum = (fVecOfPairedTreeEntry[i]->brch < fVecOfPairedTreeEntry[i+1]->brch) ? fVecOfPairedTreeEntry[i]->brch : fVecOfPairedTreeEntry[i+1]->brch;
		if(smallChannelNum%2){
			counterOddSmall++;
			//std::cout << "I : " << fVecOfPairedTreeEntry[i]->brch << " : I+1 : " << fVecOfPairedTreeEntry[i+1]->brch << std::endl;
		}

		int diff = fVecOfPairedTreeEntry[i]->tstamp - fVecOfPairedTreeEntry[i+1]->tstamp;
		if(abs(diff) > 25000){
			//std::cout << "Abs DelT : " << abs(diff) << std::endl;
			counterDelT++;
		}

		if(abs(fVecOfPairedTreeEntry[i]->brch - fVecOfPairedTreeEntry[i+1]->brch) != 1){
			notAPairCounter++;
		}

		if((abs(fVecOfPairedTreeEntry[i]->brch - fVecOfPairedTreeEntry[i+1]->brch) != 1)
					|| (abs(diff) > 25000)
					|| (smallChannelNum%2)){

				counter++;
		}else{
			//Lets push it to new vector which later replace the old one and get itself cleared
			vecOfValidatedPairedTreeEntry.push_back(fVecOfPairedTreeEntry[i]);
			vecOfValidatedPairedTreeEntry.push_back(fVecOfPairedTreeEntry[i+1]);
		}
		i += 2;
	}

	fVecOfPairedTreeEntry.clear();
	for(unsigned long int i = 0 ; i < vecOfValidatedPairedTreeEntry.size() ; i++){
		fVecOfPairedTreeEntry.push_back(vecOfValidatedPairedTreeEntry[i]);
	}
	vecOfValidatedPairedTreeEntry.clear();

	std::cout << "=======================================================" << std::endl;
	std::cout << "@@@@@@@@@@@@@ Overall Validation Summary @@@@@@@@@@@@@@" << std::endl;
	std::cout << "Invalid ChannelNum counter : " << counterOddSmall << std::endl;
	std::cout << "Invalid DelT counter : " << counterDelT << std::endl;
	std::cout << "Not A Pair counter : " << notAPairCounter << std::endl;
	std::cout << "Globally there are " << counter << " invalid pairs" << std::endl;
	//sleep(10);
}

void SinglePointEnergyCalibrationForMuon::CreateScintillatorVector(){


	for(unsigned long int i = 0 ; i < fVecOfPairedTreeEntry.size(); ){
		Double_t qmean = sqrt(fVecOfPairedTreeEntry[i]->qlong * fVecOfPairedTreeEntry[i+1]->qlong);

		if((fVecOfPairedTreeEntry[i]->brch)/2 < (numOfLayers*numOfBarsInEachLayer)){
			/*
			 * Putting MUON ENERGY CUT
			 */
			if((qmean > qstart) && (qmean < qend)){

				ScintillatorBar_V2 *scint;
				if(fVecOfPairedTreeEntry[i]->brch < fVecOfPairedTreeEntry[i+1]->brch){
					scint = new ScintillatorBar_V2(fVecOfPairedTreeEntry[i]->brch,fVecOfPairedTreeEntry[i+1]->brch,
												   fVecOfPairedTreeEntry[i]->tstamp,fVecOfPairedTreeEntry[i+1]->tstamp,
												   fVecOfPairedTreeEntry[i]->qlong,fVecOfPairedTreeEntry[i+1]->qlong);
					//fVecOfScintillatorBar.push_back(scint);
				}else{
					scint = new ScintillatorBar_V2(fVecOfPairedTreeEntry[i+1]->brch,fVecOfPairedTreeEntry[i]->brch,
												   fVecOfPairedTreeEntry[i+1]->tstamp,fVecOfPairedTreeEntry[i]->tstamp,
												   fVecOfPairedTreeEntry[i+1]->qlong,fVecOfPairedTreeEntry[i]->qlong);
					//fVecOfScintillatorBar.push_back(scint);
				}
				/*
				 * Doing DelT correction if within the energy range
				 */

//				scint->deltaTstampCorrected = scint->deltaTstamp - fCalib->GetCalibrationDataOf(scint->barIndex)->fDeltaTCorr*1000;
//				EstimateHitPosition(scint);
				fVecOfScintillatorBar.push_back(scint);
			}
		}
		i+=2;
	}

	/*
	 * Now, since the Scintillator vector is ready, we DON'T need vector of Paired Entries,
	 * so better idea would be to free the space occupied by it, hence clearing it
	 */
	fVecOfPairedTreeEntry.clear();
	std::cout << "Size of Scintillator Vector : " << fVecOfScintillatorBar.size() << std::endl;
}

void SinglePointEnergyCalibrationForMuon::DoSinglePointEnergyCalibrationForMuon(){
	InitializeHistograms();
	FillHistograms();
	for(unsigned int i = 0 ; i < fhistogramsVec.size() ; i++){
		fhistogramsVec[i]->DoSinglePointEnergyCalibrationForMuon();
		fout->cd();
		fhistogramsVec[i]->fhistQMean->Write();
		fVecOfEnergyCalibFactorForMuon.push_back(fhistogramsVec[i]->fEnergyCalibrationFactor);
		//fhistogramsVec[i]->FillCorrectedQMean();
	}
	//FillCorrectedQMeanHistogram();

}

void SinglePointEnergyCalibrationForMuon::FillEnergyCalibrationMap(){
	for(unsigned int i = 0 ; i < fVecOfEnergyCalibFactorForMuon.size() ; i++){
		fMapOfEnergyCalibFactorForMuon.insert(std::pair<std::string,unsigned int>(vecOfBarsNamess[i],fVecOfEnergyCalibFactorForMuon[i]));
		//fMapOfEnergyCalibFactorForMuon->Add(vecOfBarsNamess[i].c_str(),fVecOfEnergyCalibFactorForMuon[i]);
	}
}

/*void SinglePointEnergyCalibrationForMuon::FillCorrectedQMeanHistogram(){
	for(unsigned long int i = 0 ; i < fVecOfScintillatorBar.size() ; i++){
		double offset = fhistogramsVec[fVecOfScintillatorBar[i]->barIndex]->fEnergyCalibrationFactor;
		fVecOfScintillatorBar[i]->qlongMeanCorrected = fVecOfScintillatorBar[i]->qlongMean + offset;
		//fhistogramsVec[fVecOfScintillatorBar[i]->barIndex]->fhistQMeanCorrected->Fill(fVecOfScintillatorBar[i]->qlongMean + offset);
		fhistogramsVec[fVecOfScintillatorBar[i]->barIndex]->fhistQMeanCorrected->Fill(fVecOfScintillatorBar[i]->qlongMeanCorrected);
	}
}*/

void SinglePointEnergyCalibrationForMuon::InitializeHistograms(){
	unsigned int numOfBars = numOfLayers*numOfBarsInEachLayer;
	for(unsigned short int barIndex = 0 ; barIndex < numOfBars ; barIndex++ ){
		fhistogramsVec.push_back( new Histograms(vecOfBarsNamess[barIndex],barIndex ));
	}
	std::cout << "@@@@@@@@@@@ Size of HIST-VECTOR : " << fhistogramsVec.size() <<" @@@@@@@@@@" << std::endl;
}

void SinglePointEnergyCalibrationForMuon::FillHistograms(){
	for(unsigned long int i = 0 ; i < fVecOfScintillatorBar.size() ; i++){
		fhistogramsVec[fVecOfScintillatorBar[i]->barIndex]->FillHistogram(fVecOfScintillatorBar[i]);
	}
}

void SinglePointEnergyCalibrationForMuon::Reset(){
	ResetVector<TreeEntry>(fVecOfTreeEntry);
	ResetVector<TreeEntry>(fVecOfPairedTreeEntry);
	//ResetVector<ScintillatorBar_V2>(fVecOfScintillatorBar);
	ResetVector<Histograms>(fhistogramsVec);
}
