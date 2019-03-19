/*
 * TestTMVAData.cpp
 *
 *  Created on: 19-Mar-2019
 *      Author: rsehgal
 */


#include "MLEM.h"
#include <iostream>
#include "RunHelper.h"
#include "EventHelper.h"
#include "Voxelator_Evolution.h"
#include "G4SystemOfUnits.hh"
#include "CommonFunc.h"
#include "Track.h"
#include <vector>
#include "ScatteringDensityV2.h"
#include "ROOTTree.h"
#include "TreeSingleTon.h"
#include "base/Vector3D.h"
#include "Imaging.h"


int main(int argc, char *argv[]){
	//Tomography::MLEM mlem;
	//mlem.Debugger();

	if(argc == 1){
			std::cerr << "=======================================\n"
					  << "Please provided the required argument \n\n"
					  << "Following are possible values : Exact OR Fitted \n\n"
					  << "./TestMLEM <Exact/Fitted>\n"
					  << "=======================================\n";
			return 1;
		}

	//Creating Tree for TMVA
	Tracking::TreeSingleTon::Create("TMVA.root");
	Tracking::TreeSingleTon* singleTonObject = Tracking::TreeSingleTon::instance();
	{
	//Creating Signal Tree
	Tracking::ROOTTree *tr = new Tracking::ROOTTree("Signal");
	double pr;
	double deltaThetaX;
	double deltaThetaY;
	double deltaX;
	double deltaY;
	//tr->CreateBranch("pr", pr);
	tr->CreateBranch("deltaThetaX", deltaThetaX);
	tr->CreateBranch("deltaThetaY", deltaThetaY);
	tr->CreateBranch("deltaX", deltaX);
	tr->CreateBranch("deltaY", deltaY);
	tr->InitializeTree();
	singleTonObject->InsertTree(tr);
    }
	{
	//Creating Background Tree
	Tracking::ROOTTree *tr = new Tracking::ROOTTree("Background");
	double pr;
	double deltaThetaX;
	double deltaThetaY;
	double deltaX;
	double deltaY;
	//tr->CreateBranch("pr", pr);
	tr->CreateBranch("deltaThetaX", deltaThetaX);
	tr->CreateBranch("deltaThetaY", deltaThetaY);
	tr->CreateBranch("deltaX", deltaX);
	tr->CreateBranch("deltaY", deltaY);
	tr->InitializeTree();
	singleTonObject->InsertTree(tr);
	}



	Tomography::MLEM mlem("abc");
	std::vector<Tomography::Track> incomingTrackVector;
	std::vector<Tomography::Track> outgoingTrackVector;
	std::vector<double> momentumVector;
	std::string type = std::string(argv[1]);
	std::string fileToRead = "Track"+type+".txt";
	CommonFunc::GetTracksVector(fileToRead,incomingTrackVector,outgoingTrackVector, momentumVector);
	std::cout << "Ayush : Total Number of Tracks : " << incomingTrackVector.size() << std::endl;

	//Getting Signal Tree
	Tracking::ROOTTree *signal = singleTonObject->GetROOTTree("Signal");
	Tracking::ROOTTree *background = singleTonObject->GetROOTTree("Background");


	Tracking::ImageReconstruction im;
	for (int i = 0; i < incomingTrackVector.size(); i++) {
		std::cout << "Shachi : Processing Event no : " << i << std::endl;
		Tomography::Track incoming = incomingTrackVector[i];
		Tomography::Track outgoing = outgoingTrackVector[i];
		double momentum = momentumVector[i];
		Tomography::ScatteringData s = mlem.SetScatteringData(incoming,outgoing);
		Tracking::Vector3D<double> pocaPt = im.POCA(incoming,outgoing);
		bool falsePos = CommonFunc::Functions::IsFalsePositivePoca<true>(pocaPt);

		//s.Print();
		std::cout << s.sPr << " , " << s.sDeltaThetaX << " , " << s.sDeltaThetaY << " , " << s.sDeltaX << " , " << s.sDeltaY << std::endl;

		if(falsePos){
			//background->FillBranch("pr",s.sPr);
			background->FillBranch("deltaThetaX",s.sDeltaThetaX);
			background->FillBranch("deltaThetaY",s.sDeltaThetaY);
			background->FillBranch("deltaX",s.sDeltaX);
			background->FillBranch("deltaY",s.sDeltaY);
		}else{
			//signal->FillBranch("pr",s.sPr);
			signal->FillBranch("deltaThetaX",s.sDeltaThetaX);
			signal->FillBranch("deltaThetaY",s.sDeltaThetaY);
			signal->FillBranch("deltaX",s.sDeltaX);
			signal->FillBranch("deltaY",s.sDeltaY);
		}

		signal->Fill();
		background->Fill();


	}

	singleTonObject->WriteToFile();

	return 0;
}

