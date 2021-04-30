/*
 * MuonTrackAnalysis_RefinedTrackTest.cpp
 *
 *  Created on: 06-Apr-2021
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
#include <TH2F.h>

int main(int argc, char *argv[]){

	TFile *fpRefined = new TFile("refinedTracks.root","RECREATE");
	GenerateScintMatrixXYCenters();
	for(unsigned int i = 0 ; i < vecOfScintXYCenter.size() ; i++){
			if(!(i%9))
				std::cout <<"=================================" << std::endl;
			vecOfScintXYCenter[i].Print();
		}

	//return 0;


	TApplication *fApp = new TApplication("Test", NULL, NULL);
	lite_interface::SingleMuonTrack *smt = new lite_interface::SingleMuonTrack;

	lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
	std::string filename = argv[1];

	unsigned int numOfTracksToRead = 100000;
	std::vector<lite_interface::SingleMuonTrack*> smtVec = GetMuonTracksVector(filename,numOfTracksToRead);
	std::cout << "Size of SMTVec : " << smtVec.size() << std::endl;

	TH2F *histScatPlaneIncoming_UsingAntiCoincidence = new TH2F("HitPointOnScatPlaneUsing_AntiCoincidence_With_ConfiningBars","HitPointOnScatPlaneUsing_AntiCoincidence_With_ConfiningBars",200,-50.,50.,200,-50.,50.);
	TH2F *histScatPlaneIncoming_UsingCoincidence = new TH2F("HitPointOnScatPlaneUsing_Coincidence_With_ConfiningBars","HitPointOnScatPlaneUsing_Coincidence_With_ConfiningBars",200,-50.,50.,200,-50.,50.);

	for(unsigned int i = 0 ; i < smtVec.size() ; i++){

		if(smtVec[i]->HitInRequiredLayers() && smtVec[i]->NoHitInScintillators(vecOfAntiCoincideneScint)){
		bool validIncoming = true;
		bool validOutgoing = true;

		std::vector<lite_interface::Point3D*> incoming = GetTrackFromLayers(smtVec[i],incomingLayerIndices);
		std::vector<lite_interface::Point3D*> outgoing = GetTrackFromLayers(smtVec[i],outgoingLayerIndices);
		//std::cout << "JUST BEFORE Calculating validIncoming : " ;
		for(unsigned int j = 0 ; j < incoming.size() ; j++){
			lite_interface::Point3D *temp = incoming[j];
			//temp->Print();
			validIncoming &= (temp->GetX() > -50. && temp->GetX() < 50. &&
				    //temp->GetY() > -50. && temp->GetY() < 50. &&
				    temp->GetZ() > -50. && temp->GetZ() < 50.);
					//(temp->GetX() < 9000 && temp->GetY() < 9000 && temp->GetZ() < 9000);
		}
		validIncoming &= (incoming.size() > 0);

		for(unsigned int j = 0 ; j < outgoing.size() ; j++){
			lite_interface::Point3D *temp = outgoing[j];
			validOutgoing &= (temp->GetX() > -50 && temp->GetX() < 50 &&
				    //temp->GetY() > -50 && temp->GetY() < 50 &&
				    temp->GetZ() > -50 && temp->GetZ() < 50);
					//(temp->GetX() < 9000 && temp->GetY() < 9000 && temp->GetZ() < 9000);
		}
		validOutgoing &= (outgoing.size() > 0);
		if(validOutgoing && validIncoming){

			unsigned int scattererLayerIndex = vecOfLayersYPos.size()-1; 
			lite_interface::Point3D *hitPtOnScatPlaneUsingIncomingTrack = ExtrapolatePointOnLayer(incoming[1],incoming[2],scattererLayerIndex); 
			//lite_interface::Point3D *hitPtOnScatPlaneUsingIncomingTrack = ExtrapolatePointOnLayer(incoming[0],incoming[incoming.size()-1],incoming[1],scattererLayerIndex); 
			histScatPlaneIncoming_UsingAntiCoincidence->Fill(hitPtOnScatPlaneUsingIncomingTrack->GetX(),hitPtOnScatPlaneUsingIncomingTrack->GetZ());
		}
		
	}
	if(smtVec[i]->HitInRequiredLayers() && smtVec[i]->HitInAnyScintillators(vecOfAntiCoincideneScint)){
		bool validIncoming = true;
		bool validOutgoing = true;

		std::vector<lite_interface::Point3D*> incoming = GetTrackFromLayers(smtVec[i],incomingLayerIndices);
		std::vector<lite_interface::Point3D*> outgoing = GetTrackFromLayers(smtVec[i],outgoingLayerIndices);
		//std::cout << "JUST BEFORE Calculating validIncoming : " ;
		for(unsigned int j = 0 ; j < incoming.size() ; j++){
			lite_interface::Point3D *temp = incoming[j];
			//temp->Print();
			validIncoming &= (temp->GetX() > -50. && temp->GetX() < 50. &&
				    //temp->GetY() > -50. && temp->GetY() < 50. &&
				    temp->GetZ() > -50. && temp->GetZ() < 50.);
					//(temp->GetX() < 9000 && temp->GetY() < 9000 && temp->GetZ() < 9000);
		}
		validIncoming &= (incoming.size() > 0);

		for(unsigned int j = 0 ; j < outgoing.size() ; j++){
			lite_interface::Point3D *temp = outgoing[j];
			validOutgoing &= (temp->GetX() > -50 && temp->GetX() < 50 &&
				    //temp->GetY() > -50 && temp->GetY() < 50 &&
				    temp->GetZ() > -50 && temp->GetZ() < 50);
					//(temp->GetX() < 9000 && temp->GetY() < 9000 && temp->GetZ() < 9000);
		}
		validOutgoing &= (outgoing.size() > 0);
		if(validOutgoing && validIncoming){
			std::cout<< "=============== INCOMING ================" << std::endl;
			for(unsigned int ii = 0 ; ii < incoming.size() ; ii++){
				incoming[ii]->Print();
			}
			std::cout<< "=============== Outgoing ================" << std::endl;
			for(unsigned int ii = 0 ; ii < outgoing.size() ; ii++){
				outgoing[ii]->Print();
			}
			unsigned int scattererLayerIndex = vecOfLayersYPos.size()-1; 
			lite_interface::Point3D *hitPtOnScatPlaneUsingIncomingTrack = ExtrapolatePointOnLayer(incoming[1],incoming[2],scattererLayerIndex); 
			//lite_interface::Point3D *hitPtOnScatPlaneUsingIncomingTrack = ExtrapolatePointOnLayer(incoming[0],incoming[incoming.size()-1],incoming[1],scattererLayerIndex); 
			histScatPlaneIncoming_UsingCoincidence->Fill(hitPtOnScatPlaneUsingIncomingTrack->GetX(),hitPtOnScatPlaneUsingIncomingTrack->GetZ());
		}
		
	}

	}

	new TCanvas("Can_Hit_point_On_Scat_Plane_Using_AntiCoindence","Can_Hit_point_On_Scat_Plane_Using_AntiCoincidence");
	histScatPlaneIncoming_UsingAntiCoincidence->Draw("colz");

	new TCanvas("Can_Hit_point_On_Scat_Plane_Using_Coindence","Can_Hit_point_On_Scat_Plane_Using_Coincidence");
	histScatPlaneIncoming_UsingCoincidence->Draw("colz");


	fApp->Run();


	return 0;
}
