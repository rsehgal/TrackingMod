/*
 * dissection.cpp
 *
 *  Created on: 02-Mar-2021
 *      Author: rsehgal
 */

#include <iostream>
#include <vector>
#include "PairFinder.h"
#include "Analyzer.h"
#include "Calibration.h"
#include <TApplication.h>
#include <TCanvas.h>
#include "Plotter.h"
#include "includes.hh"
#include "HardwareNomenclature.h"
#include <TSpectrum.h>
#include  "Histograms.h"

int main(int argc, char *argv[]){
		TApplication *fApp = new TApplication("Test", NULL, NULL);
		lite_interface::SingleMuonTrack *smt = new lite_interface::SingleMuonTrack;

		lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
		std::string filename = argv[1];

		TFile *trackFile = new TFile(filename.c_str(),"READ");
		TTree *trackTree = (TTree*)trackFile->Get("TracksTree");
		trackTree->SetBranchAddress("MuonTracks",&smt);

		Long64_t nentries = trackTree->GetEntries();
		std::cout << "Number of Entries in specified file : " << nentries << std::endl;
		Long64_t nbytes = 0;

		std::vector<lite_interface::SingleMuonTrack*> smtVec;
		TH1F *multiplicityHist = new TH1F("TrackMultiplicity","TrackMultiplicity",20,0,20);
		TH1F *trackDeltDist = new TH1F("TrackDeltDist","TrackDeltDist",200,0,20000);

		int counter = 0;
		for (Long64_t i=0; i<nentries;i++) {

				//std::cout << "Fetching Entry : " << i << std::endl;
				nbytes += trackTree->GetEntry(i);
				//if(!(i % 100000) && i!=0)
					//std::cout << "Processed : " << i << " Tracks ........" << std::endl;

				//if(smt->SingleHitInEachLayer())

				if(smtVec.size() > 1){
					std::cout << "TSmallTStamp of current track : " << (smt->GetMuonTrack())[0]->fTSmallTimeStamp << std::endl;
					std::cout << "TSmallTStamp of previous track : " << (smtVec[smtVec.size()-1]->GetMuonTrack())[0]->fTSmallTimeStamp << std::endl;
				}
				if(smtVec.size() > 1 &&  (smt->GetMuonTrack())[0]->fTSmallTimeStamp < (smtVec[smtVec.size()-1]->GetMuonTrack())[0]->fTSmallTimeStamp)
					std::cout << "ROLLOVER DETECTED................. : TrackId : " << i << std::endl;


				smtVec.push_back(new lite_interface::SingleMuonTrack(*smt));
				multiplicityHist->Fill(smt->size());
				std::vector<lite_interface::ScintillatorBar_V2*> vecOfScint = smt->GetMuonTrack();
				trackDeltDist->Fill(vecOfScint[vecOfScint.size()-1]->fTSmallTimeStamp - vecOfScint[0]->fTSmallTimeStamp );


				if(smt->HitInRequiredLayers(vecOfRequiredLayers))
				{
					counter++;
					std::cout <<"===================== Printing Track : " << i << " ===================" << std::endl;
					smt->Print();
				}
			}

		std::cout <<"Number of Tracks with Hit in required layers : " << counter << std::endl;

/*
		lite_interface::Analyzer analyzerObj(filename);
		std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec = analyzerObj.GetVectorOfScintillators();

		std::vector<TreeEntry*> vecOfPairedTreeEntries = analyzerObj.GetPairFinder()->GetVectorOfPairedTreeEntries();
		for(unsigned int i = 0 ; i < vecOfPairedTreeEntries.size() ; i++){
			vecOfPairedTreeEntries[i]->Print();
		}

		TH1F *singleBarEnergyDepoHist = new TH1F("singleBarEnergyDepoHist","singleBarEnergyDepoHist",40,0,40);
		for(unsigned int i = 0 ; i  < scintBarVec.size() ; i++){
			if(scintBarVec[i]->GetQMeanCorrected() > 15)
				singleBarEnergyDepoHist->Fill(scintBarVec[i]->GetQMeanCorrected());
		}

		new TCanvas();
		singleBarEnergyDepoHist->Draw();
*/

		new TCanvas("Can_TrackMultiplicty","Can_TrackMultiplicty");
		multiplicityHist->Draw();
		new TCanvas("TrackDelTHist","TrackDelTHist");
		trackDeltDist->Draw();
		fApp->Run();

		return 0;

}
