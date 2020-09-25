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
#include "SingleMuonTrack.h"
#include "HardwareNomenclature.h"
#include <TFile.h>
#include "SinglePointEnergyCalibrationForMuon.h"
#include "HelperFunctions.h"

unsigned long int Analyzer_V2::fMuonTrackNum = 0;

Analyzer_V2::Analyzer_V2() {
	// TODO Auto-generated constructor stub

}

Analyzer_V2::~Analyzer_V2() {
	// TODO Auto-generated destructor stub
	delete fout;
}

bool CheckRange(std::vector<Point3D*> singleMuonTrack){
	for(unsigned int i = 0 ; i < singleMuonTrack.size() ; i++){
		if(std::isinf(singleMuonTrack[i]->x) || std::isinf(singleMuonTrack[i]->y) || std::isinf(singleMuonTrack[i]->z)
			// || (abs((singleMuonTrack[i]->x) <= 45.) ) || (abs((singleMuonTrack[i]->y) <= 45.) ) || (abs((singleMuonTrack[i]->z) <= 50.) )
		){
			return true;
		}
	}
	return false;
}

void ClearMuonTrackVec(std::vector< SingleMuonTrack* > muonTrackVec){
	for(unsigned int i = 0 ; i < muonTrackVec.size() ; i++){
		delete muonTrackVec[i];
	}
}
Analyzer_V2::Analyzer_V2(std::string datafileName, Calibration *calib,unsigned long int bunchSize){
	fBunchSize = bunchSize;
	fout = new TFile("calib.root","RECREATE");
	GenerateScintMatrixXYCenters();
	for(unsigned int i = 0 ; i < vecOfScintXYCenter.size() ; i++){
		vecOfScintXYCenter[i].Print();
	}

	//sleep(20);
	fCalib = calib;
	fDatafileName = datafileName;
	//LoadDataAndSort();
	LoadDataAndSort();
	CheckPairs();
	ValidatePairs();
	CreateScintillatorVector();
	std::vector< SingleMuonTrack* > muonTrackVec = ReconstrutTrack_V2();
	PlotHistOfNumOfMuonHitsInMuonTracks_V2(muonTrackVec);
	PlotHistOfDelTBetweenMuonTracks_V2(muonTrackVec);
	CalculateTotalEnergyDepositionForMuonTracks(muonTrackVec);
	std::vector< SingleMuonTrack* > filteredMuonTrackVec = PlotEnergyLossDistributionOfMuonTracks(muonTrackVec);
	HistInitializer();
	FillCoincidenceHist_V2(muonTrackVec);
	PlotCoincidenceCountGraph();
	PlotEnergyDistributionWithMultiplicity(muonTrackVec,0);
	std::vector<SingleMuonTrack*> filtered = FiltrationBasedOnCosmicMuonRate(muonTrackVec);
	//fittedMuonTracks = PlotTracks_V2(filteredMuonTrackVec,50);
	fittedMuonTracks = PlotTracks_V2(filtered,10);

/*
	CheckPairs();
	ValidatePairs();

	CreateScintillatorVector(); //Create Vector of Scintillator with delT correction and single point energy correction from Calibration files
	std::vector< SingleMuonTrack* > muonTrackVec = ReconstrutTrack_V2(); //Create vector of muon tracks
	//fVecOfScintillatorBar.clear();

	PlotHistOfNumOfMuonHitsInMuonTracks_V2(muonTrackVec);
	PlotHistOfDelTBetweenMuonTracks_V2(muonTrackVec);
	//DisplayHistogramsOf(75);
	//PrintMuonTrackVector_V2(muonTrackVec);
	CalculateTotalEnergyDepositionForMuonTracks(muonTrackVec);
	std::vector< SingleMuonTrack* > filteredMuonTrackVec = PlotEnergyLossDistributionOfMuonTracks(muonTrackVec);
	muonTrackVec.clear();
	fittedMuonTracks = PlotTracks_V2(filteredMuonTrackVec,50);
*/

	//Uncomment below line to get the histograms
	InitializeHistograms();
	FillHistograms();
	DisplayHistograms();


}

void Analyzer_V2::ProcessBunch(){
	CheckPairs();
	ValidatePairs();

	CreateScintillatorVector(); //Create Vector of Scintillator with delT correction and single point energy correction from Calibration files
	std::vector< SingleMuonTrack* > muonTrackVec = ReconstrutTrack_V2(); //Create vector of muon tracks
	//fVecOfScintillatorBar.clear();

	PlotHistOfNumOfMuonHitsInMuonTracks_V2(muonTrackVec);
	PlotHistOfDelTBetweenMuonTracks_V2(muonTrackVec);
	//DisplayHistogramsOf(75);
	//PrintMuonTrackVector_V2(muonTrackVec);


	CalculateTotalEnergyDepositionForMuonTracks(muonTrackVec);
	std::vector< SingleMuonTrack* > filteredMuonTrackVec = PlotEnergyLossDistributionOfMuonTracks(muonTrackVec);
	std::cout << "Going to Fill Skimmed Muon Track Vector........" << std::endl;
	for(unsigned int i = 0 ; i < filteredMuonTrackVec.size() ; i++){
		filteredMuonTrackVec[i]->FillSkimmedMuonTracksVector();
	}
	std::cout << "Skimmed Muon Track Vector filled ........" << std::endl;
	//muonTrackVec.clear();
	//fittedMuonTracks = PlotTracks_V2(filteredMuonTrackVec,50);

	//Freeing the memory
	ResetVector<SingleMuonTrack>(muonTrackVec);
	filteredMuonTrackVec.clear();
	//ResetBunchMemory();
}

void Analyzer_V2::ResetBunchMemory(){
	std::cout << "Resetting Bunch memory......." << std::endl;
	ResetVector<TreeEntry>(fVecOfTreeEntry);
	ResetVector<TreeEntry>(fVecOfPairedTreeEntry);
	std::cout << "Bunch memory Freed......." << std::endl;
	//ResetVector<ScintillatorBar_V2>(fVecOfScintillatorBar);

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

/*
	if(fBunchSize == 0){
		fBunchSize = nEntries;
	}
	unsigned long int bunchSize = fBunchSize;
	unsigned long int intSlot = nEntries / bunchSize;
	unsigned long int tailEntries = nEntries % bunchSize;
	unsigned long int intEntries = intSlot * bunchSize;

	std::cout <<"Number of bunches : " << intSlot << std::endl;
	std::cout <<"Number of entries in each bunch  : " << intEntries << std::endl;
	std::cout <<"Number of entries in tail : " << tailEntries << std::endl;

	int bunchId = 0 ;

	for (Long64_t iev = 0; iev < nEntries; iev++) {
	//for (Long64_t iev = 0; iev < intEntries; iev++) {

		nb += tr->GetEntry(iev);
		if (0)
			std::cout <<"Entry : " << iev << " : "<< brch << " , " << qlong << " , " << tstamp << " , " << time << std::endl;

		fVecOfTreeEntry.push_back(new TreeEntry(brch, qlong, tstamp, time));

		if ((iev+1) % bunchSize == 0) {
			std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ PROCESSING BUNCHID from HEAD: " << bunchId << " @@@@@@@@@@@@@@@@@@@@@@@@@@ " << std::endl;
			times->Set(time, kTRUE, offset, kFALSE);
			std::cout << " Processing event : " << iev << "\t"
					<< times->GetTimeSpec() << std::endl;
			ProcessBunch();
			bunchId++;
			//return;
		}

	}      //! event loop

	std::cout << "\n\n\nMemory Occupied by fVecOfTreeEntry : " << fVecOfTreeEntry.size()*sizeof(TreeEntry) << std::endl;
	//sleep(10);
	//Tail loop
	std::cout << "\n\n\n\n @@@@@@@@@@@@@@@@@@ GOING TO PROCESS TAIL @@@@@@@@@@@@@@@@ \n\n\n\n ";
	//sleep(10);
	std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ PROCESSING BUNCHID : " << bunchId << " @@@@@@@@@@@@@@@@@@@@@@@@@@ " << std::endl;
	for (Long64_t iev = intEntries; iev < nEntries; iev++) {
			nb += tr->GetEntry(iev);
			if (0)
				std::cout <<"Entry : " << iev << " : "<< brch << " , " << qlong << " , " << tstamp << " , " << time << std::endl;

			fVecOfTreeEntry.push_back(new TreeEntry(brch, qlong, tstamp, time));



			if (iev % bunchSize == 0 && iev!=0) {
				times->Set(time, kTRUE, offset, kFALSE);
				std::cout << " Processing event : " << iev << "\t"
						<< times->GetTimeSpec() << std::endl;
				ProcessBunch();
				//return;
			}

		}      //! event loop
	if(fVecOfTreeEntry.size() > 10){
		ProcessBunch();
	}
*/


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

	for (unsigned int i = 0; i < fVecOfTreeEntry.size()-1;) {

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
				scint->qlongMeanCorrected = scint->qlongMean + fCalib->GetCalibrationDataOf(scint->barIndex)->fEnergyCalibrationFactor;
				EstimateHitPosition(scint);
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

std::vector < SingleMuonTrack* > Analyzer_V2::ReconstrutTrack_V2(){

	std::sort(fVecOfScintillatorBar.begin(),fVecOfScintillatorBar.end(),CompareTimestampScintillator);

	unsigned int hitLength = fVecOfScintillatorBar.size();
	std::cout << "HitLength from DetectMuonTracks : " << hitLength << std::endl;
	std::vector < SingleMuonTrack* > muonTrackVec;
	SingleMuonTrack *singleMuonTrack = new SingleMuonTrack();
	singleMuonTrack->push_back(fVecOfScintillatorBar[0]);
	unsigned int hitInAllLayersCounter = 0;
	for (unsigned int i = 1; i < hitLength; i++) {

		if ((fVecOfScintillatorBar[i]->tsmallTimeStamp
				- fVecOfScintillatorBar[i - 1]->tsmallTimeStamp) < 20000) {
			//Within 20ns window
			singleMuonTrack->push_back(fVecOfScintillatorBar[i]);
		} else {
			//Outside 20ns window
			//Sorting the single muon track by BarIndex (in DESCENDING order), to form a logical track
			//std::sort(singleMuonTrack.begin(), singleMuonTrack.end(),CompareBarIndexInScintillator);
			singleMuonTrack->Sort();

			//singleMuonTrack.CalculateTotalEnergyDeposited();

			muonTrackVec.push_back(singleMuonTrack);
			//Just counting the number of muon where all the layer detected the muon
			//std::cout << "Track Finshed : Length : "  << singleMuonTrack.size() << std::endl;
			if (singleMuonTrack->size() == numOfLayers)
				hitInAllLayersCounter++;
			//singleMuonTrack.clear();
			singleMuonTrack = new SingleMuonTrack();
			singleMuonTrack->push_back(fVecOfScintillatorBar[i]);
		}
	}

	std::cout
			<< "Number of Muon Tracks where all the layers detected the muon : "
			<< hitInAllLayersCounter << std::endl;

	return muonTrackVec;
}

void Analyzer_V2::CalculateTotalEnergyDepositionForMuonTracks(std::vector< SingleMuonTrack *> muonTrackVec){
	for(unsigned long int i = 0 ; i < muonTrackVec.size() ; i++){
		muonTrackVec[i]->CalculateTotalEnergyDeposited();
	}
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

void Analyzer_V2::PrintMuonTrackVector_V2(std::vector< SingleMuonTrack* > muonTrackVec){
	//unsigned int muonTrackVecLength = muonTrackVec.size();
	unsigned int muonTrackVecLength = 20;
	for(unsigned int i = 0 ; i < muonTrackVecLength ; i++){
		std::cout << "====== Print Muon Track : " << i << " : Num of Layers Hitted : " << muonTrackVec[i]->size()  << " ======" << std::endl;
		muonTrackVec[i]->Print();
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

void Analyzer_V2::PlotHistOfNumOfMuonHitsInMuonTracks_V2(std::vector< SingleMuonTrack* > muonTrackVec){
	TH1I *hist = new TH1I("Hist of Muon Hits In Muon Tracks","Hist of Muon Hits In Muon Tracks",20,0,20);
	unsigned int muonTrackVecLength = muonTrackVec.size();

	for(unsigned int i = 0 ; i < muonTrackVecLength ; i++){
		unsigned int muonHitLength = (muonTrackVec[i]->fSingleMuonTrack).size();
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

void Analyzer_V2::PlotHistOfDelTBetweenMuonTracks_V2(std::vector< SingleMuonTrack* > muonTrackVec){
	TH1F *hist = new TH1F("Hist of DelT(ms) between Muon Tracks","Hist of DelT(ms) between Muon Tracks",50,0,10);
	unsigned int muonTrackVecLength = muonTrackVec.size();
	ULong64_t previous = GetMeanTValueOfATrack(muonTrackVec[0]->fSingleMuonTrack);
	for(unsigned int i = 1 ; i < muonTrackVecLength ; i++){
		ULong64_t next = GetMeanTValueOfATrack(muonTrackVec[i]->fSingleMuonTrack);
		ULong64_t delT = next-previous;
		Double_t delTms = 1.*delT/1e+3;
		//std::cout << "DelT in Millisecond : " << delT <<std::endl;
		previous = next;
		if((muonTrackVec[i]->fSingleMuonTrack).size() > 2)
			hist->Fill(delTms);
	}
	new TCanvas();
	hist->Draw();
}

void Analyzer_V2::FillCoincidenceHist_V2(std::vector< SingleMuonTrack* > muonTrackVec){
	TH2F *coincidenceHist2D = new TH2F("CoincidenceHist2D","CoincidenceHist2D",numOfBarsInEachLayer,0,numOfBarsInEachLayer,numOfBarsInEachLayer,0,numOfBarsInEachLayer);
	unsigned int muonTrackVecLength = muonTrackVec.size();
	for(unsigned int i = 0 ; i < muonTrackVecLength ; i++){
			std::vector<ScintillatorBar_V2*> singleMuonTrack=muonTrackVec[i]->fSingleMuonTrack;
			//std::cout << singleMuonTrack[0]->layerIndex <<" : " << singleMuonTrack[singleMuonTrack.size()-1]->layerIndex << std::endl;
			if( (singleMuonTrack[0]->layerIndex == 8) && (singleMuonTrack[singleMuonTrack.size()-1]->layerIndex == 0)){
				unsigned int x = singleMuonTrack[0]->barIndex % numOfBarsInEachLayer;
				unsigned int y = singleMuonTrack[singleMuonTrack.size()-1]->barIndex % numOfBarsInEachLayer;
				coincidenceHist2D->Fill(x,y);
				Fill2DHist(x,y);
			}

	}

	//TH1D * projh2X = coincidenceHist2D->ProjectionX();
	//TH1D * projh2Y = coincidenceHist2D->ProjectionY();

	std::vector<int> x={0,1,2,3,4,5,6,7,8};
	std::vector<double> y;


	for(unsigned int i = 0 ; i < x.size() ; i++){
		y.push_back(coincidenceHist2D->GetBinContent(0,x[i]));
		std::cout << "Bin Content : (0," << x[i] <<") : " << coincidenceHist2D->GetBinContent(0,x[i]) << std::endl;
		std::cout << "Bin Content MyHist2D : (0," << x[i] <<") : " << myhist2D[0][x[i]] << std::endl;
	}

	new TCanvas();
	coincidenceHist2D->Draw("LEGO2");

	//new TCanvas();

	//TGraph *gr = new TGraph(x.size(),&x[0],&y[0]);
	//gr->Draw("p");

	/*TCanvas *can = new TCanvas();
	can->Divide(2,2);

	can->cd(1);
	coincidenceHist2D->Draw("LEGO2");

	can->cd(3);
	projh2X->Draw();

	can->cd(4);
	projh2Y->Draw();
*/
	return;
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

void Analyzer_V2::DisplayHistograms(bool reverse){
	TCanvas *can = new TCanvas("Histograms","Histograms",800,600);
	can->SetLogx();
	//TCanvas *can = new TCanvas("c1");
	unsigned int totalNumOfBars = 8;// numOfLayers*numOfBarsInEachLayer;
	can->Divide(5,totalNumOfBars,0,0);
	//std::cout<< "Number of Pads : " << (((TList*) can->GetListOfPrimitives())->GetSize()) << std::endl;

	/*fhistogramsVec[0]->fhistQNear->Draw();
	fhistogramsVec[0]->fhistQFar->Draw("same");*/
	unsigned int totBars = numOfLayers*numOfBarsInEachLayer;
	if(reverse){
		for(unsigned int barIndex = (totBars-1) ; barIndex >= (totBars - totalNumOfBars) ; barIndex--){
					int padIndex = 5*barIndex + 1;
					//std::cout << "Changing to subplot : " << padIndex << std::endl;
					can->cd(padIndex);
					gPad->SetLogy();
					//fhistogramsVec[barIndex]->fhistQNearFarPad->cd();
					fhistogramsVec[barIndex]->fhistQNear->Draw();
					fhistogramsVec[barIndex]->fhistQFar->Draw("same");

					can->cd(5*barIndex+2);
					gPad->SetLogy();
					fhistogramsVec[barIndex]->fhistQMean->Draw();

					can->cd(5*barIndex+3);
					gPad->SetLogy();
					fhistogramsVec[barIndex]->fhistQMeanCorrected->Draw();

					can->cd(5*barIndex+4);
			//		/gPad->SetLogy();
					fhistogramsVec[barIndex]->fhistDelT->Draw();

					can->cd(5*barIndex+5);
					fhistogramsVec[barIndex]->fhistDelTCorrected->Draw();
				}
	}else{
		for(unsigned int barIndex = 0 ; barIndex < totalNumOfBars ; barIndex++){
			int padIndex = 5*barIndex + 1;
			//std::cout << "Changing to subplot : " << padIndex << std::endl;
			can->cd(padIndex);
			gPad->SetLogy();
			//fhistogramsVec[barIndex]->fhistQNearFarPad->cd();
			fhistogramsVec[barIndex]->fhistQNear->Draw();
			fhistogramsVec[barIndex]->fhistQFar->Draw("same");

			can->cd(5*barIndex+2);
			gPad->SetLogy();
			fhistogramsVec[barIndex]->fhistQMean->Draw();

			can->cd(5*barIndex+3);
			gPad->SetLogy();
			fhistogramsVec[barIndex]->fhistQMeanCorrected->Draw();

			can->cd(5*barIndex+4);
	//		/gPad->SetLogy();
			fhistogramsVec[barIndex]->fhistDelT->Draw();

			can->cd(5*barIndex+5);
			fhistogramsVec[barIndex]->fhistDelTCorrected->Draw();
		}
	}
}

void Analyzer_V2::DisplayHistogramsOf(unsigned int barIndex){
	new TCanvas();
	fhistogramsVec[barIndex]->fhistQNear->Draw();
	fhistogramsVec[barIndex]->fhistQFar->Draw("same");

	new TCanvas();
	fhistogramsVec[barIndex]->fhistQMean->Draw();
	fhistogramsVec[barIndex]->fhistQMeanCorrected->SetLineColor(2);
	fhistogramsVec[barIndex]->fhistQMeanCorrected->Draw("same");

	new TCanvas();
	fhistogramsVec[barIndex]->fhistDelT->Draw();
	fhistogramsVec[barIndex]->fhistDelTCorrected->SetLineColor(2);
	fhistogramsVec[barIndex]->fhistDelTCorrected->Draw("same");

}

void Analyzer_V2::EstimateHitPosition(ScintillatorBar_V2 *scint){
	TF1 *param = fCalib->GetCalibrationDataOf(scint->barIndex)->fParameterization_F;
	long double correctedDelT = scint->deltaTstampCorrected / 1000.;
	float estZ = param->Eval(correctedDelT);
	//std::cout << "Corrected DelT : " << correctedDelT << " : Hit Position along Z : " << estZ << std::endl;
	/*(scint->hitPosition).z=estZ;
	scint->EstimateHitPositionAlongY();
	scint->EstimateHitPositionAlongX();
	 */
	/*
	 * Put a check on the estimated Z of the hit point
	 */
	if (estZ > -50. && estZ < 50.) {
		(scint->hitPosition).z=estZ;
		scint->EstimateHitPositionAlongY();
		scint->EstimateHitPositionAlongX();
	}
}

//void Analyzer_V2::PlotTracks_V2(std::vector< SingleMuonTrack* > muonTrackVec,unsigned int numOfTracks){
std::vector< std::vector<Point3D*> >  Analyzer_V2::PlotTracks_V2(std::vector< SingleMuonTrack* > muonTrackVec,unsigned int numOfTracks){

	std::vector< std::vector<Point3D*> > fittedMuonTracks;
	if(numOfTracks==0){
		numOfTracks = muonTrackVec.size();
	}
	unsigned int ntracks = numOfTracks;
	unsigned int counter = 0;
	while(ntracks){
		if(muonTrackVec[counter]->fIsValid){
			if((muonTrackVec[counter]->fSingleMuonTrack).size() > 6){
				//PlotOneTrack(muonTrackVec[counter]->fSingleMuonTrack);


				/*
				 * Putting some more VERY STRINGENT cuts to select really good tracks for visualization purpose
				 */
				if( ((muonTrackVec[counter]->fSingleMuonTrack).size() == numOfLayers)
					&& ((muonTrackVec[counter]->fSingleMuonTrack[0]->hitPosition).y > 35)
					&& ((muonTrackVec[counter]->fSingleMuonTrack[numOfLayers-1]->hitPosition).y < -35) )

				{
					std::vector<Point3D*> fittedSingleMuonTrack = muonTrackVec[counter]->PlotTrack();
					if(!CheckRange(fittedSingleMuonTrack)){
						fittedMuonTracks.push_back(fittedSingleMuonTrack);
						muonTrackVec[counter]->Print();
						ntracks--;
					}
				}
			}
		}
		counter++;
	}
	return fittedMuonTracks;
}

std::vector< SingleMuonTrack* > Analyzer_V2::PlotEnergyLossDistributionOfMuonTracks(std::vector< SingleMuonTrack* > muonTrackVec){
	(new TCanvas())->SetLogy();
	gStyle->SetOptStat(1);
	TH1F *energyLossMuonsHist = new TH1F("Histogram of Energy loss of Muon Tracks","Histogram of Energy loss of Muon Tracks",1000,10000,400000);
	std::vector< SingleMuonTrack* > filteredMuonTrackVec;
	for(unsigned long int i = 0 ; i < muonTrackVec.size(); i++){
		energyLossMuonsHist->Fill(muonTrackVec[i]->fTotalEnergyDeposited);
		if(muonTrackVec[i]->fTotalEnergyDeposited > 120000){
			filteredMuonTrackVec.push_back(muonTrackVec[i]);

		}
	}
	int binmax = energyLossMuonsHist->GetMaximumBin();
	double bin = energyLossMuonsHist->GetXaxis()->GetBinCenter(binmax);
	std::cout << "Peak of the Histogram of Energy Loss of Muon Tracks is at : " << bin << std::endl;

	std::cout << "Going to Draw EnergyLossHistogram : " << __FILE__ << " : " << __LINE__ << std::endl;
	//sleep(5);
	energyLossMuonsHist->Draw();
	fout->cd();
	//energyLossMuonsHist->Write();
	return filteredMuonTrackVec;
}

void Analyzer_V2::PlotCoincidenceCountGraph(){
	std::vector<int> xvec;
	std::vector<std::vector<int>> yvec;
	std::vector<TGraph*> vecOfGraphs;
	/*for(unsigned int i = 0 ; i < numOfLayers ; i++){
		xvec.push_back(i);
	}*/
	for(unsigned int i = 0 ; i < numOfLayers ; i++){
		xvec.push_back(i);
		std::vector<int> ysubVec;
		for(unsigned int j = 0 ; j < numOfBarsInEachLayer ; j++){
			ysubVec.push_back(myhist2D[i][j]);
		}
		yvec.push_back(ysubVec);
	}


	for(unsigned int i = 0 ; i < numOfBarsInEachLayer ; i++){
		vecOfGraphs.push_back(new TGraph(xvec.size(),&xvec[0],&yvec[i][0]));
		vecOfGraphs[i]->SetTitle(Form("Coincidence of bar num %d in top layer with bottom layer",i+1));
		vecOfGraphs[i]->SetLineColor(4);
		vecOfGraphs[i]->SetMarkerStyle(8);
		vecOfGraphs[i]->SetMarkerColor(6);
		vecOfGraphs[i]->SetMarkerSize(0.4);
		vecOfGraphs[i]->GetXaxis()->SetNdivisions(9);
		vecOfGraphs[i]->GetXaxis()->SetTitle("Bar Number in Bottom layer");
		//vecOfGraphs[i]->GetXaxis()->SetLabelSize(15);
		vecOfGraphs[i]->GetXaxis()->CenterTitle(true);
		vecOfGraphs[i]->GetYaxis()->SetTitle("Counts");
		//vecOfGraphs[i]->GetYaxis()->SetTitleSize(1.5);
		vecOfGraphs[i]->GetYaxis()->CenterTitle(true);
		vecOfGraphs[i]->Draw("ap");
	}

	TCanvas *can = new TCanvas();
	can->Divide(3,3);

	for(unsigned int i = 0 ; i < numOfBarsInEachLayer ; i++){
		can->cd(i+1);
		std::string str = std::string(Form("Coincidence of %d  Bar in top layer with bottom layer",i+1));
		DrawGrid(str, 9, 9);
		vecOfGraphs[i]->Draw("ap");
	}

}

void Analyzer_V2::PlotEnergyDistributionWithMultiplicity(std::vector<SingleMuonTrack*> muonTrackVec, unsigned int multiplicity){
	std::vector<short int> multiplicityVec;
	if(multiplicity == 0){
		for(unsigned int i = 0 ; i < numOfLayers ; i++){
			multiplicityVec.push_back(i+1);
		}
	}else{
		multiplicityVec.push_back(multiplicity);
	}

	TCanvas *can = new TCanvas();
	if(multiplicityVec.size()!=1){
		float t = sqrt(multiplicityVec.size());
		unsigned short rows = std::floor(t);
		unsigned short cols = std::ceil(t);
		can->Divide(cols,rows);

	}

	if(multiplicityVec.size()==1){
		char *title = Form("Energy histogram for Multiplicity %d",multiplicity);
		TH1D *histEnergyWithMultiplicity = new TH1D(title,title,500,100000,250000);
		for(unsigned int i = 0 ; i < muonTrackVec.size() ; i ++){
			if((muonTrackVec[i]->fSingleMuonTrack).size() == multiplicity){
				histEnergyWithMultiplicity->Fill(muonTrackVec[i]->fTotalEnergyDeposited);
			}
		}
		histEnergyWithMultiplicity->Draw();
	}else{
		std::vector<TH1D*> vecOfHists;
		for(unsigned int i = 0 ; i < multiplicityVec.size() ; i++){
			char *str = Form("Multiplicity-%d",i+1);
			int xlow = multiplicityVec[i]*20000. - 60000;
			int xhigh = multiplicityVec[i]*20000. + 60000;
			xlow = 0;
			xhigh = 240000;
			vecOfHists.push_back(new TH1D(str,str,500,xlow,xhigh));
			vecOfHists[i]->GetXaxis()->SetTitle("Energy sum");
			vecOfHists[i]->GetXaxis()->CenterTitle(true);
			vecOfHists[i]->GetYaxis()->SetTitle("Counts");
			vecOfHists[i]->GetYaxis()->CenterTitle(true);
		}
		for(unsigned int i = 0 ; i < muonTrackVec.size() ; i ++){
			//std::cout << "BREAK at Muon Track vector of size :  " << (muonTrackVec[i]->fSingleMuonTrack).size() << std::endl;
			if((muonTrackVec[i]->fSingleMuonTrack).size() <= numOfLayers && (muonTrackVec[i]->fSingleMuonTrack).size() > 0)
				vecOfHists[(muonTrackVec[i]->fSingleMuonTrack).size()-1]->Fill(muonTrackVec[i]->fTotalEnergyDeposited);
		}
		for(unsigned int i = 0 ; i < multiplicityVec.size() ; i++){
			can->cd(i+1);
			vecOfHists[i]->Draw();
		}

		TLegend *legend = new TLegend();//0.1,0.7,0.48,0.9);
	    legend->SetHeader("Multiplicity","C");
		new TCanvas();

		for(unsigned int i = 0 ; i < multiplicityVec.size() ; i++){
			vecOfHists[i]->SetLineColor(i+1);
			vecOfHists[i]->Scale(1/vecOfHists[i]->Integral());
			legend->AddEntry(vecOfHists[i],Form("Multiplicity_%d",i+1),"l");
			vecOfHists[i]->GetXaxis()->SetNdivisions(12);
			vecOfHists[i]->Draw("same");
		}
		legend->Draw();

	}


	//histEnergyWithMultiplicity->Draw();
}

std::vector<SingleMuonTrack*>  Analyzer_V2::FiltrationBasedOnCosmicMuonRate(std::vector<SingleMuonTrack*> muonTrackVec){
		std::vector<SingleMuonTrack*> filteredMuonTrackVec;
		unsigned int muonTrackVecLength = muonTrackVec.size();
		ULong64_t previous = GetMeanTValueOfATrack(muonTrackVec[0]->fSingleMuonTrack);
		for(unsigned int i = 1 ; i < muonTrackVecLength ; i++){
			ULong64_t next = GetMeanTValueOfATrack(muonTrackVec[i]->fSingleMuonTrack);
			ULong64_t delT = next-previous;
			Double_t delTms = 1.*delT/1e+6;
			//std::cout << "DelT in Millisecond : " << delT <<std::endl;
			previous = next;
			if(delTms > 2){
				filteredMuonTrackVec.push_back(muonTrackVec[i]);
			}
		}
		return filteredMuonTrackVec;
		//new TCanvas();
		//hist->Draw();
}
