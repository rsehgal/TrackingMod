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
#include "Plotter.h"
#include "TCanvas.h"
#include "HardwareNomenclature.h"

namespace lite_interface {

bool IsSimulation = false;

bool Analyzer::CompareTimestampScintillator(ScintillatorBar_V2 *i1, ScintillatorBar_V2 *i2){
	return (i1->fTSmallTimeStamp < i2->fTSmallTimeStamp);
}

Analyzer::Analyzer() {
	// TODO Auto-generated constructor stub

}

//Analyzer::Analyzer(std::string datafileName,std::string outputfileName, bool simulation) : fDatafileName(datafileName), fOutputfileName(outputfileName){
Analyzer::Analyzer(std::string datafileName,std::string outputfileName ): fDatafileName(datafileName), fOutputfileName(outputfileName){


#ifdef USE_FOR_SIMULATION
		IsSimulation = true;
		std::cout << "Going to create vector of Scintillator Bar from Simulated Data..."  << std::endl;
		CreateScintillatorVector_FromSimulation();
#else
		fPairFinder =  new PairFinder(fDatafileName);
		CreateScintillatorVector();
#endif
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

#ifdef USE_FOR_SIMULATION
void Analyzer::CreateScintillatorVector_FromSimulation(){
	std::cout << "Inside CreateScintillatorVector_FromSimulation............" << std::endl;
	TFile *f = new TFile(fDatafileName.c_str(),"READ");
	TTree *ftree = (TTree*) f->Get("ftree");

	//Declaration of leaves types
	   UInt_t          qlongNear;
	   //UInt_t          qlongFar;
	   Double_t        qlongMean;
	   //Double_t        qlongMeanCorrected;
	   //ULong64_t       tstampNear;
	   //ULong64_t       tstampFar;
	   ULong64_t       tsmallTimeStamp;
	   Long64_t        deltaTstamp;
	   //Long64_t        deltaTstampCorrected;
	   UShort_t        barIndex;

	   Point3D *hitPoint;
	   //UShort_t        layerIndex;
	   Double_t        hitX;
	   Double_t        hitY;
	   Double_t        hitZ;

	   // Set branch addresses.
	   ftree->SetBranchAddress("qlongNear",&qlongNear);
	   //ftree->SetBranchAddress("qlongFar",&qlongFar);
	   ftree->SetBranchAddress("qlongMean",&qlongMean);
	   //ftree->SetBranchAddress("qlongMeanCorrected",&qlongMeanCorrected);
	   //ftree->SetBranchAddress("tstampNear",&tstampNear);
	   //ftree->SetBranchAddress("tstampFar",&tstampFar);
	   ftree->SetBranchAddress("tsmallTimeStamp",&tsmallTimeStamp);
	   ftree->SetBranchAddress("deltaTstamp",&deltaTstamp);
	   //ftree->SetBranchAddress("deltaTstampCorrected",&deltaTstampCorrected);
	   ftree->SetBranchAddress("barIndex",&barIndex);

	   //ftree->SetBranchAddress("hitPoint",&hitPoint);
	   //ftree->SetBranchAddress("layerIndex",&layerIndex);
	   ftree->SetBranchAddress("hitX",&hitX);
	   ftree->SetBranchAddress("hitY",&hitY);
	   ftree->SetBranchAddress("hitZ",&hitZ);

	   Long64_t nentries = ftree->GetEntries();

      Long64_t nbytes = 0;
      for (Long64_t i=0; i<nentries;i++) {
    	  nbytes += ftree->GetEntry(i);
    	  if(!(i%100000))
    		  std::cout << "Processed : " << i << " events....." << std::endl;
    	  ScintillatorBar_V2 *scint = new ScintillatorBar_V2(barIndex,qlongNear,qlongMean,tsmallTimeStamp,deltaTstamp, hitX, hitY, hitZ);
    	  fVecOfScintillatorBar.push_back(scint);
    	  //fVecOfScintillatorBar[i]->Print();

	  }


#if(0)
	std::vector<TreeEntry*> vectorOfPairedTreeEntries = fPairFinder->GetVectorOfPairedTreeEntries();
	std::cout << "TRYING TO CREATE VECTOR OF SCINTILLATORS>>......................... : Size : " << vectorOfPairedTreeEntries.size() << std::endl;

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
#endif
	std::cout << "Size of Scintillator Vector : " << fVecOfScintillatorBar.size() << std::endl;

}
#endif

void Analyzer::ReconstructMuonTrack(){
	//TTree::SetMaxTreeSize(100000000);
	std::sort(fVecOfScintillatorBar.begin(), fVecOfScintillatorBar.end(),CompareTimestampScintillator);

	unsigned int scintVecSize = fVecOfScintillatorBar.size();
	std::cout << "ScintVectSize : " << scintVecSize << std::endl;

	lite_interface::SingleMuonTrack *singleMuonTrack = new lite_interface::SingleMuonTrack();
	std::vector<lite_interface::Point3D*> *hitPointVec = new std::vector<lite_interface::Point3D*>;
	std::vector<lite_interface::Point3D*> *hitPointVec_Param = new std::vector<lite_interface::Point3D*>;
	std::vector<lite_interface::SingleMuonTrack*> smtVec;
	double energySum = 0;
	std::vector<double> *energyVec = new std::vector<double>;

	unsigned int hitInAllLayersCounter = 0;
	//TFile *tracksFile = new TFile("tracks.root","RECREATE");
	TFile *tracksFile = new TFile(fOutputfileName.c_str(),"RECREATE");
	TTree *tracksTree = new TTree("TracksTree","TracksTree");
	TTree *hitPointVecTree = new TTree("HitPointVecTree","HitPointVecTree");
	tracksTree->Branch("MuonTracks","lite_interface::SingleMuonTrack", &singleMuonTrack);
	hitPointVecTree->Branch("HitPointVec","std::vector<lite_interface::Point3D*>", &hitPointVec);
	hitPointVecTree->Branch("HitPointVecParam","std::vector<lite_interface::Point3D*>", &hitPointVec_Param);

#ifdef USE_FOR_SIMULATION
	std::vector<lite_interface::Point3D*> *meanHitPointVec = new std::vector<lite_interface::Point3D*>;
	hitPointVecTree->Branch("MeanHitPointVec","std::vector<lite_interface::Point3D*>", &meanHitPointVec);
#endif
	hitPointVecTree->Branch("EnergySum", &energySum, "energySum/D");
	hitPointVecTree->Branch("EnergyVector","std::vector<double>", &energyVec);

	singleMuonTrack->push_back(fVecOfScintillatorBar[0]);
	unsigned int count=0;
	for (unsigned int i = 1; i < scintVecSize; i++) {
		//if(!(i%100000))
			//std::cout << "Processed : " << i << " : bars " << std::endl;

		if ((fVecOfScintillatorBar[i]->fTSmallTimeStamp - fVecOfScintillatorBar[i - 1]->fTSmallTimeStamp) < 20000) {
			//Within 20ns window
			singleMuonTrack->push_back(fVecOfScintillatorBar[i]);
			//std::cout << __FILE__ << " : " << __LINE__ << " : ";		fVecOfScintillatorBar[i]->Print();
		} else {
			//Outside 20ns window, implied track ends, hence either store it in the vector of write it to the ROOT file
			singleMuonTrack->Sort();
			if(singleMuonTrack->size() > 8)
			{

				if(singleMuonTrack->IsClearTrack())
				{
					std::vector<double> enVec = singleMuonTrack->GetDepositedEnergyVector();
					energyVec = &enVec;
					std::vector<std::string> barNamesVector = singleMuonTrack->GetBarNamesVector();
					std::vector<lite_interface::Point3D*> vec = singleMuonTrack->Get3DHitPointVector();
					if(count < 4){
					std::cout <<"--------- Using Muon -------------" << std::endl;
					for(unsigned short k =0 ; k < vec.size() ; k++){
						std::cout << "BarName : " << barNamesVector[k] << " : Corrected DelT : " << (singleMuonTrack->GetMuonTrack())[k]->GetDelTCorrected() <<" : " ;
						vec[k]->Print();
					}
					}

					energySum = singleMuonTrack->GetEnergySum();
					hitPointVec = &vec;


					std::vector<lite_interface::Point3D*> vec_Param = singleMuonTrack->Get3DHitPointVector_Param();
					if(count < 4){
					std::cout <<"--------- Using Param -------------" << std::endl;
					for(unsigned short k =0 ; k < vec_Param.size() ; k++){
						std::cout << "BarName : " << barNamesVector[k] << " : ";
						vec_Param[k]->Print();
					}
					}
					hitPointVec_Param = &vec_Param;

#ifdef USE_FOR_SIMULATION
					std::vector<lite_interface::Point3D*> vec_meanHitPoint = singleMuonTrack->GetMean3DHitPointVector();
					if(count < 4){
							std::cout <<"--------- Using MeanHit Point -------------" << std::endl;
							for(unsigned short k =0 ; k < vec_meanHitPoint.size() ; k++){
								std::cout << "BarName : " << barNamesVector[k] << " : ";
								vec_meanHitPoint[k]->Print();
							}
					}
					meanHitPointVec = &vec_meanHitPoint;
#endif

					count++;

					//singleMuonTrack->GetFittedTrack(0);
					//singleMuonTrack->GetFittedTrack(1);

#ifdef USE_FOR_SIMULATION

					//singleMuonTrack->GetFittedTrack(2);
#endif
					if(count <= 4){
						std::cout << "======== Single Muon Track Count : " << count << " =========" << std::endl;
						singleMuonTrack->Print();
					}



					tracksTree->Fill();
					hitPointVecTree->Fill();
					//hitPointVecTree_Param->Fill();
				}
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
			//singleMuonTrack = new SingleMuonTrack();
			singleMuonTrack->push_back(fVecOfScintillatorBar[i]);
		}
	}

	//TCanvas *can=lite_interface::PlotEnergyDistributionWithMultiplicity(smtVec);
	//can->Write();
	tracksTree->Write();
	hitPointVecTree->Write();
	//hitPointVecTree_Param->Write();
	tracksFile->Close();

	std::cout
			<< "Number of Muon Tracks where all the layers detected the muon : "
			<< hitInAllLayersCounter << std::endl;

	//return muonTrackVec;
}

} /* namespace lite_interface */