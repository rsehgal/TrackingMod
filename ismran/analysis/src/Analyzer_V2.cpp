/*
 * Analyzer_V2.cpp
 *
 *  Created on: 03-Sep-2020
 *      Author: rsehgal
 */

#include "Analyzer_V2.h"
#include "includes.hh"
#include "TreeEntry.h"
#include "ScintillatorBar_V2.h"
#include "Histograms.h"
#include "Calibration.h"
#include <TH1I.h>
Analyzer_V2::Analyzer_V2() {
	// TODO Auto-generated constructor stub

}

Analyzer_V2::~Analyzer_V2() {
	// TODO Auto-generated destructor stub
}

Analyzer_V2::Analyzer_V2(std::string datafileName, Calibration *calib){
	fCalib = calib;
	fDatafileName = datafileName;
	LoadDataAndSort();
	CheckPairs();
	ValidatePairs();
	//ValidatePairs();
	CreateScintillatorVector();
	std::vector< std::vector<ScintillatorBar_V2*> > muonTrackVec = ReconstrutTrack();
	PrintMuonTrackVector(muonTrackVec);
	PlotHistOfNumOfMuonHitsInMuonTracks(muonTrackVec);
	PlotHistOfDelTBetweenMuonTracks(muonTrackVec);
	InitializeHistograms();
	FillHistograms();
	DisplayHistograms();

}

void Analyzer_V2::LoadDataAndSort() {
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

	//Sorting the TreeEntryVec with *tstamp* in ascending order
	std::cout
			<< "=========== Trying to do inplace sorting in TreeEntryVector =========== "
			<< std::endl;
	std::cout << "Size of TreeEntry vector to sort : " << fVecOfTreeEntry.size()
			<< std::endl;
	//std::sort(fVecOfTreeEntry.begin(), fVecOfTreeEntry.end(), CompareTimestamp);
	std::cout << "TreeEntryVector Sorting done..... " << std::endl;
	//sleep(60);
	std::cout << "Returning  ......." << std::endl;
	return;
}

void Analyzer_V2::CheckPairs(){
	std::cout << "=============== Checking Pairs =================="<< std::endl;

	for (unsigned int i = 0; i < fVecOfTreeEntry.size();) {

		/*if (abs(fVecOfTreeEntry[i]->brch - fVecOfTreeEntry[i + 1]->brch) > 1) {
			i++;
		} else {

			fVecOfPairedTreeEntry.push_back(fVecOfTreeEntry[i]);
			fVecOfPairedTreeEntry.push_back(fVecOfTreeEntry[i + 1]);
			i = i + 2;
		}*/

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

void Analyzer_V2::ValidatePairs(){
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
				//fVecOfPairedTreeEntry[i]->Print();
				//fVecOfPairedTreeEntry[i+1]->Print();
				/*std::cout << "Invalid pair found : " << fVecOfPairedTreeEntry[i]->brch <<" : "
						  << fVecOfPairedTreeEntry[i+1]->brch
						  << " : DelT : " << (fVecOfPairedTreeEntry[i]->tstamp-fVecOfPairedTreeEntry[i+1]->tstamp) << std::endl;*/
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

void Analyzer_V2::CreateScintillatorVector(){


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

				scint->deltaTstampCorrected = scint->deltaTstamp - fCalib->GetCalibrationDataOf(scint->barIndex)->fDeltaTCorr*1000;
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

std::vector< std::vector<ScintillatorBar_V2*> > Analyzer_V2::ReconstrutTrack(){

	std::sort(fVecOfScintillatorBar.begin(),fVecOfScintillatorBar.end(),CompareTimestampScintillator);

	unsigned int hitLength = fVecOfScintillatorBar.size();
	std::cout << "HitLength from DetectMuonTracks : " << hitLength << std::endl;
	std::vector < std::vector<ScintillatorBar_V2*> > muonTrackVec;
	std::vector<ScintillatorBar_V2*> singleMuonTrack;
	singleMuonTrack.push_back(fVecOfScintillatorBar[0]);
	unsigned int hitInAllLayersCounter = 0;
	for (unsigned int i = 1; i < hitLength; i++) {

		if ((fVecOfScintillatorBar[i]->tsmallTimeStamp
				- fVecOfScintillatorBar[i - 1]->tsmallTimeStamp) < 20000) {
			//Within 20ns window
			singleMuonTrack.push_back(fVecOfScintillatorBar[i]);
		} else {
			//Outside 20ns window
			//Sorting the single muon track by BarIndex (in DESCENDING order), to form a logical track
			std::sort(singleMuonTrack.begin(), singleMuonTrack.end(),CompareBarIndexInScintillator);
			muonTrackVec.push_back(singleMuonTrack);
			//Just counting the number of muon where all the layer detected the muon
			//std::cout << "Track Finshed : Length : "  << singleMuonTrack.size() << std::endl;
			if (singleMuonTrack.size() == numOfLayers)
				hitInAllLayersCounter++;
			singleMuonTrack.clear();
			singleMuonTrack.push_back(fVecOfScintillatorBar[i]);
		}
	}

	std::cout
			<< "Number of Muon Tracks where all the layers detected the muon : "
			<< hitInAllLayersCounter << std::endl;

	return muonTrackVec;
}

bool Analyzer_V2::CompareTimestampScintillator(ScintillatorBar_V2 *i1, ScintillatorBar_V2 *i2)
{
	return (i1->tsmallTimeStamp < i2->tsmallTimeStamp);

}

bool Analyzer_V2::CompareBarIndexInScintillator(ScintillatorBar_V2 *i1, ScintillatorBar_V2 *i2)
{
	return (i1->barIndex > i2->barIndex);
}

void Analyzer_V2::PrintMuonTrackVector(std::vector< std::vector<ScintillatorBar_V2*> > muonTrackVec){
	//unsigned int muonTrackVecLength = muonTrackVec.size();
	unsigned int muonTrackVecLength = 20;
	for(unsigned int i = 0 ; i < muonTrackVecLength ; i++){
		std::cout << "====== Print Muon Track : " << i << " : Num of Layers Hitted : " << muonTrackVec[i].size()  << " ======" << std::endl;
		unsigned int muonHitLength = muonTrackVec[i].size();
		for(unsigned int j = 0 ; j < muonHitLength ; j++){
			muonTrackVec[i][j]->Print();
		}
	}
}

void Analyzer_V2::PlotHistOfNumOfMuonHitsInMuonTracks(std::vector< std::vector<ScintillatorBar_V2*> > muonTrackVec){
	TH1I *hist = new TH1I("Hist of Muon Hits In Muon Tracks","Hist of Muon Hits In Muon Tracks",20,0,20);
	unsigned int muonTrackVecLength = muonTrackVec.size();

	for(unsigned int i = 0 ; i < muonTrackVecLength ; i++){
		unsigned int muonHitLength = muonTrackVec[i].size();
		hist->Fill(muonHitLength);
	}
	new TCanvas();
	hist->Draw();
}

ULong64_t Analyzer_V2::GetMeanTValueOfATrack(std::vector<ScintillatorBar_V2*> singleMuonTrack){
	ULong64_t sum = 0;
	for(unsigned short int i =0 ; i < singleMuonTrack.size() ; i++){
		sum += (singleMuonTrack[i]->tsmallTimeStamp/1000.); //Addition done in nanosecond, NOT in picosecond
	}

	return (sum/singleMuonTrack.size());
}

void Analyzer_V2::PlotHistOfDelTBetweenMuonTracks(std::vector< std::vector<ScintillatorBar_V2*> > muonTrackVec){
	TH1F *hist = new TH1F("Hist of DelT(ms) between Muon Tracks","Hist of DelT(ms) between Muon Tracks",100,0,40);
	unsigned int muonTrackVecLength = muonTrackVec.size();
	ULong64_t previous = GetMeanTValueOfATrack(muonTrackVec[0]);
	for(unsigned int i = 1 ; i < muonTrackVecLength ; i++){
		ULong64_t next = GetMeanTValueOfATrack(muonTrackVec[i]);
		ULong64_t delT = next-previous;
		Double_t delTms = 1.*delT/1e+6;
		//std::cout << "DelT in Millisecond : " << delT <<std::endl;
		previous = next;
		if(muonTrackVec[i].size() > 2)
			hist->Fill(delTms);
	}
	new TCanvas();
	hist->Draw();
}

void Analyzer_V2::InitializeHistograms(){
	unsigned int numOfBars = numOfLayers*numOfBarsInEachLayer;
	for(unsigned short int barIndex = 0 ; barIndex < numOfBars ; barIndex++ ){
		fhistogramsVec.push_back( new Histograms("Bar-"+std::to_string(barIndex),barIndex ));
	}
	std::cout << "@@@@@@@@@@@ Size of HIST-VECTOR : " << fhistogramsVec.size() <<" @@@@@@@@@@" << std::endl;
}

void Analyzer_V2::FillHistograms(){
	for(unsigned long int i = 0 ; i < fVecOfScintillatorBar.size() ; i++){
		fhistogramsVec[fVecOfScintillatorBar[i]->barIndex]->FillHistogram(fVecOfScintillatorBar[i]);
	}
}

void Analyzer_V2::DisplayHistograms(){
	TCanvas *can = new TCanvas("Histograms","Histograms",800,600);
	can->SetLogx();
	//TCanvas *can = new TCanvas("c1");
	unsigned int totalNumOfBars = 10;// numOfLayers*numOfBarsInEachLayer;
	can->Divide(4,totalNumOfBars,0,0);
	//std::cout<< "Number of Pads : " << (((TList*) can->GetListOfPrimitives())->GetSize()) << std::endl;

	/*fhistogramsVec[0]->fhistQNear->Draw();
	fhistogramsVec[0]->fhistQFar->Draw("same");*/

	for(unsigned int barIndex = 0 ; barIndex < totalNumOfBars ; barIndex++){
		int padIndex = 4*barIndex + 1;
		//std::cout << "Changing to subplot : " << padIndex << std::endl;
		can->cd(padIndex);
		gPad->SetLogy();
		//fhistogramsVec[barIndex]->fhistQNearFarPad->cd();
		fhistogramsVec[barIndex]->fhistQNear->Draw();
		fhistogramsVec[barIndex]->fhistQFar->Draw("same");

		can->cd(4*barIndex+2);
		gPad->SetLogy();
		fhistogramsVec[barIndex]->fhistQMean->Draw();

		can->cd(4*barIndex+3);
//		/gPad->SetLogy();
		fhistogramsVec[barIndex]->fhistDelT->Draw();

		can->cd(4*barIndex+4);
		fhistogramsVec[barIndex]->fhistDelTCorrected->Draw();
	}
}
