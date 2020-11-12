/*
 * Analyzer.cpp
 *
 *  Created on: 09-Nov-2020
 *      Author: rsehgal
 */

#include "Analyzer.h"
#include "PairFinder.h"
#include "TreeEntry.h"
#include "ScintillatorBar_V2.h"
#include "SingleMuonTrack.h"

namespace lite_interface {

bool Analyzer::CompareTimestampScintillator(ScintillatorBar_V2 *i1, ScintillatorBar_V2 *i2){
	return (i1->fTSmallTimeStamp < i2->fTSmallTimeStamp);
}

Analyzer::Analyzer() {
	// TODO Auto-generated constructor stub

}

Analyzer::Analyzer(std::string datafileName) : fDatafileName(datafileName){
	fPairFinder =  new PairFinder(fDatafileName);
	CreateScintillatorVector();
	ReconstructMuonTrack();
}

Analyzer::~Analyzer() {
	// TODO Auto-generated destructor stub
}

PairFinder* Analyzer::GetPairFinder() const{
	return fPairFinder;
}

std::vector<ScintillatorBar_V2*> Analyzer::GetVectorOfScintillators()const{
	return fVecOfScintillatorBar;
}

void Analyzer::CreateScintillatorVector(){
	std::vector<TreeEntry*> vectorOfPairedTreeEntries = fPairFinder->GetVectorOfPairedTreeEntries();
	std::cout << "TRYING TO CREATE VECTOR OF SCINTILLATORS>>......................... : Size : " << vectorOfPairedTreeEntries.size() << std::endl;
#if(1)
	for(unsigned long int i = 0 ; i < vectorOfPairedTreeEntries.size(); ){
		Double_t qmean = sqrt(vectorOfPairedTreeEntries[i]->qlong * vectorOfPairedTreeEntries[i+1]->qlong);
		ULong64_t tstampSmall = (vectorOfPairedTreeEntries[i]->tstamp < vectorOfPairedTreeEntries[i+1]->tstamp) ?
								 vectorOfPairedTreeEntries[i]->tstamp : vectorOfPairedTreeEntries[i+1]->tstamp;

		if((vectorOfPairedTreeEntries[i]->brch)/2 < (numOfLayers*numOfBarsInEachLayer)){
			/*
			 * Putting MUON ENERGY CUT
			 */
			//if((qmean > qstart) && (qmean < qend))
			{

				ScintillatorBar_V2 *scint;
				if(vectorOfPairedTreeEntries[i]->brch < vectorOfPairedTreeEntries[i+1]->brch){

					scint = new ScintillatorBar_V2(vectorOfPairedTreeEntries[i]->brch/2,vectorOfPairedTreeEntries[i]->qlong,
												   qmean, tstampSmall,(vectorOfPairedTreeEntries[i]->tstamp-vectorOfPairedTreeEntries[i+1]->tstamp));

				}else{
					scint = new ScintillatorBar_V2(vectorOfPairedTreeEntries[i+1]->brch/2,vectorOfPairedTreeEntries[i+1]->qlong,
												   qmean, tstampSmall,(vectorOfPairedTreeEntries[i+1]->tstamp-vectorOfPairedTreeEntries[i]->tstamp));
				}
				/*
				 * Doing DelT correction if within the energy range
				 */
				/*
				scint->deltaTstampCorrected = scint->deltaTstamp - fCalib->GetCalibrationDataOf(scint->barIndex)->fDeltaTCorr*1000;
				scint->qlongMeanCorrected = scint->qlongMean + fCalib->GetCalibrationDataOf(scint->barIndex)->fEnergyCalibrationFactor;
				//EstimateHitPosition(scint);
				scint->EstimateHitPosition_V2(fCalib);*/
				//scint->EstimateHitPosition(fCalib);

				fVecOfScintillatorBar.push_back(scint);
			}
		}
		i+=2;
	}

	/*
	 * Now, since the Scintillator vector is ready, we DON'T need vector of Paired Entries,
	 * so better idea would be to free the space occupied by it, hence clearing it
	 */
	vectorOfPairedTreeEntries.clear();
	std::cout << "Size of Scintillator Vector : " << fVecOfScintillatorBar.size() << std::endl;
#endif
}

void Analyzer::ReconstructMuonTrack(){
	//TTree::SetMaxTreeSize(100000000);
	std::sort(fVecOfScintillatorBar.begin(), fVecOfScintillatorBar.end(),CompareTimestampScintillator);

	unsigned int scintVecSize = fVecOfScintillatorBar.size();
	std::cout << "ScintVectSize : " << scintVecSize << std::endl;
	//std::vector<SingleMuonTrack*> muonTrackVec;
	lite_interface::SingleMuonTrack *singleMuonTrack = new lite_interface::SingleMuonTrack();

	//std::vector<ScintillatorBar_V2*> smt = singleMuonTrack->GetMuonTrack();
	//SingleMuonTrack *smt=new SingleMuonTrack;
	//std::vector<ScintillatorBar_V2*> smt = singleMuonTrack->GetMuonTrack();

	unsigned int hitInAllLayersCounter = 0;
	TFile *tracksFile = new TFile("tracks.root","RECREATE");
	TTree *tracksTree = new TTree("TracksTree","TracksTree");
	//tracksTree->Branch("MuonTracks","MuonTracks", &smt);
	tracksTree->Branch("MuonTracks","lite_interface::SingleMuonTrack", &singleMuonTrack);
	//tracksTree->Branch("MuonTracks","lite_interface::SingleMuonTrack", &smt);

	singleMuonTrack->push_back(fVecOfScintillatorBar[0]);
	unsigned int count=0;
	for (unsigned int i = 1; i < scintVecSize; i++) {
		//if(!(i%100000))
			//std::cout << "Processed : " << i << " : bars " << std::endl;

		if ((fVecOfScintillatorBar[i]->fTSmallTimeStamp - fVecOfScintillatorBar[i - 1]->fTSmallTimeStamp) < 20000) {
			//Within 20ns window
			singleMuonTrack->push_back(fVecOfScintillatorBar[i]);
		} else {

			//Outside 20ns window, implied track ends, hence either store it in the vector of write it to the ROOT file
			singleMuonTrack->Sort();
			//smt = new SingleMuonTrack(*singleMuonTrack);

			//std::cout << smt <<" : " << singleMuonTrack << std::endl;
			//muonTrackVec.push_back(singleMuonTrack);
			//smt = singleMuonTrack->GetMuonTrack();
			//tracksTree->Fill();
			if(singleMuonTrack->size() > 8){
				count++;
				if(count <= 4){
					std::cout << "======== Single Muon Track Count : " << count << " =========" << std::endl;
					singleMuonTrack->Print();
				}
				tracksTree->Fill();
			//std::cout << "=====================" << std::endl;
			//singleMuonTrack->Print();
			}



			//Just counting the number of muon where all the layer detected the muon
			//std::cout << "Track Finshed : Length : "  << singleMuonTrack.size() << std::endl;
			if (singleMuonTrack->size() == numOfLayers)
				hitInAllLayersCounter++;

			//singleMuonTrack = new SingleMuonTrack();

			/*if(smt->size() > 4){
				if(count <= 4){
					std::cout << "======== SMT Count : " << count << " =========" << std::endl;
					smt->Print();
				}
			}*/

			singleMuonTrack->clear();
			singleMuonTrack->push_back(fVecOfScintillatorBar[i]);
		}
	}

	tracksTree->Write();
	tracksFile->Close();

	std::cout
			<< "Number of Muon Tracks where all the layers detected the muon : "
			<< hitInAllLayersCounter << std::endl;

	//return muonTrackVec;
}

} /* namespace lite_interface */
