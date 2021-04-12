#include "PairFinder.h"
#include "includes.hh"
#include "TreeEntry.h"

namespace lite_interface{

unsigned int PairFinder::numOfShots = 0;
unsigned int PairFinder::shotNo = 0;

PairFinder::PairFinder(){

}

PairFinder::PairFinder(std::string datafilename) : fDatafileName(datafilename){
	LoadDataAndSort();
	CheckPairs();
	ValidatePairs();
}

PairFinder::~PairFinder(){

}

void PairFinder::LoadDataAndSort() {
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

	if(numOfShots==0){
		numOfShots=1;
		shotNo = 1;
	}
	unsigned long int numOfEventsInOneShot = nEntries/numOfShots;
	if(numOfShots==1)
	numOfEventsInOneShot = 10000;

	//for (Long64_t iev = 0; iev < nEntries; iev++) {
	for (Long64_t iev = (shotNo-1)*numOfEventsInOneShot; iev < shotNo*numOfEventsInOneShot; iev++) {
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

	std::cout << "Size of TreeEntry vector : " << fVecOfTreeEntry.size()
			<< std::endl;
	//std::sort(fVecOfTreeEntry.begin(), fVecOfTreeEntry.end(), CompareTimestamp);
	//std::cout << "TreeEntryVector Sorting done..... " << std::endl;
	return;
}

void PairFinder::CheckPairs(){
	std::cout << "=============== Checking Pairs =================="<< std::endl;

	for (unsigned int i = 0; i < fVecOfTreeEntry.size()-1;) {

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

void PairFinder::ValidatePairs(){
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

std::vector<TreeEntry*> PairFinder::GetVectorOfPairedTreeEntries() const {
	return fVecOfPairedTreeEntry;
}

} /* End of lite_interface namespace */
