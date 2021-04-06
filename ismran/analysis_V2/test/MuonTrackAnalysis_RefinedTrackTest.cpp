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

	//lite_interface::Analyzer analyzerObj(filename);
	//analyzerObj->ReconstructMuonTrack();

	TFile *trackFile = new TFile(filename.c_str(),"READ");
	TTree *trackTree = (TTree*)trackFile->Get("TracksTree");
	trackTree->SetBranchAddress("MuonTracks",&smt);

	Long64_t nentries = trackTree->GetEntries();

	Long64_t nbytes = 0;

	std::vector<lite_interface::SingleMuonTrack*> smtVec;

	int counter = 0;
	for (Long64_t i=0; i<nentries;i++) {

		//std::cout << "Fetching Entry : " << i << std::endl;
		nbytes += trackTree->GetEntry(i);
		if(!(i % 100000) && i!=0)
			std::cout << "Processed : " << i << " Tracks ........" << std::endl;

		//if(smt->SingleHitInEachLayer())
		smtVec.push_back(new lite_interface::SingleMuonTrack(*smt));
	}

	std::cout << "Size of SMTVec : " << smtVec.size() << std::endl;

//	TH2F *hist2D_Layer0 = new TH2F("HitPointOnLayer_0","HitPointOnLayer_0",200,-50,50,200,-50,50);
//	TH2F *hist2D_Layer1 = new TH2F("HitPointOnLayer_1","HitPointOnLayer_1",200,-50,50,200,-50,50);
//	TH2F *hist2D_Layer2 = new TH2F("HitPointOnLayer_2","HitPointOnLayer_2",200,-50,50,200,-50,50);
//	TH2F *hist2D_Layer3 = new TH2F("HitPointOnLayer_3","HitPointOnLayer_3",200,-50,50,200,-50,50);
//	TH2F *hist2D_Layer4 = new TH2F("HitPointOnLayer_4","HitPointOnLayer_4",200,-50,50,200,-50,50);
//	TH2F *hist2D_Layer5 = new TH2F("HitPointOnLayer_5","HitPointOnLayer_5",200,-50,50,200,-50,50);
//	TH2F *hist2D_Layer8 = new TH2F("HitPointOnLayer_8","HitPointOnLayer_8",200,-50,50,200,-50,50);


	unsigned int extrpolatedLayerIndex = 5;
	std::string name = "Layer_"+std::to_string(extrpolatedLayerIndex)+"xdiff_zdifff";
	std::string histname="Hist_"+name;
	TH2F *hist2D_xdiff_zdiff = new TH2F(histname.c_str(),histname.c_str(),400,-100,100,400,-100,100);
	TH1F *histAngle = new TH1F("DeviationAngleHist","DeviationAngleHist",100,-1.5,1.5);


	for(unsigned int i = 0 ; i < smtVec.size() ; i++){

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

		if(1){
		if(validIncoming && validOutgoing ){
			std::vector<lite_interface::Point3D*> fittedIncomingTrack = lite_interface::CreateFittedTrack(incoming);
			std::vector<lite_interface::Point3D*> fittedOutgoingTrack = lite_interface::CreateFittedTrack(outgoing);

			bool validFittedIncoming = true;
			bool validFittedOutgoing = true;

			for(unsigned int j = 0 ; j < fittedIncomingTrack.size() ; j++){
				lite_interface::Point3D *temp = fittedIncomingTrack[j];
				validFittedIncoming &= (temp->GetX() > -50 && temp->GetX() < 50 &&
									    //temp->GetY() > -50 && temp->GetY() < 50 &&
									    temp->GetZ() > -50 && temp->GetZ() < 50);
			}

			for(unsigned int j = 0 ; j < fittedOutgoingTrack.size() ; j++){
				lite_interface::Point3D *temp = fittedOutgoingTrack[j];
				validFittedOutgoing &= (temp->GetX() > -50 && temp->GetX() < 50 &&
									    //temp->GetY() > -50 && temp->GetY() < 50 &&
									    temp->GetZ() > -50 && temp->GetZ() < 50);
			}

			if(validFittedIncoming && validFittedOutgoing){
				double incomingZenithAngle = GetZenithAngle(fittedIncomingTrack);
				double outgoingZenithAngle = GetZenithAngle(fittedOutgoingTrack);
				double angleDiff = outgoingZenithAngle-incomingZenithAngle;
				histAngle->Fill(angleDiff);
			}
		}}

		if(0){
		if(validIncoming && validOutgoing ){
			std::cout <<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
			//Incoming
			std::cout << "============ Printing Valid Incoming Track ==========" << std::endl;
			for(unsigned int j = 0 ; j < incoming.size() ; j++){
				lite_interface::Point3D *temp = incoming[j];
				temp->Print();

			}
			{
			/*std::vector<lite_interface::Point3D*> fittedTrack = lite_interface::CreateFittedTrack(incoming);
			std::cout << "*********** Printing Fitted Incoming Track *********" << std::endl;
			for(unsigned int j = 0 ; j < fittedTrack.size() ; j++){
				lite_interface::Point3D *temp = fittedTrack[j];
				temp->Print();

			}*/
			}

			//Outgoing
			std::cout << "============ Printing Valid Outgoing Track ==========" << std::endl;
			for(unsigned int j = 0 ; j < outgoing.size() ; j++){
				lite_interface::Point3D *temp = outgoing[j];
				temp->Print();

			}
			{
			/*std::vector<lite_interface::Point3D*> fittedTrack = lite_interface::CreateFittedTrack(outgoing);
			std::cout << "*********** Printing Fitted Outgoing Track *********" << std::endl;
			for(unsigned int j = 0 ; j < fittedTrack.size() ; j++){
				lite_interface::Point3D *temp = fittedTrack[j];
				temp->Print();

			}*/
			}
		}
	 }
	}



/*
	std::string canName="Can_"+name;
	TCanvas *can_xdiff_zdifff = new TCanvas(canName.c_str(),canName.c_str());
	hist2D_xdiff_zdiff->Draw("colz");
*/
/*
	TCanvas *canLayer3 = new TCanvas("Layer3","Layer3");
	hist2D_Layer3->Draw("colz");

	new TCanvas("Layer5","Layer5");
	hist2D_Layer5->Draw("colz");

	TCanvas *canLayer8 = new TCanvas("Layer8","Layer8");
	hist2D_Layer8->Draw("colz");
*/

/*
	std::string matWithExt = filename.substr(13);
	TFile *fp = new TFile(("HitPattern"+matWithExt).c_str(),"RECREATE");
	fp->cd();
	hist2D_xdiff_zdiff->Write();
*/

/*
	hist2D_Layer3->Write();
	hist2D_Layer5->Write();

	hist2D_Layer8->Write();
*/



//	fp->Close();

	new TCanvas("Hist of Deviation","Hist of Deviation");
	histAngle->Draw();

	fApp->Run();


	return 0;
}
