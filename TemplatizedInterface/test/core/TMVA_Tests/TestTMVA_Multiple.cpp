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
#include "base/Global.h"
#include <map>

/*
 * Test Code to convert the Data from TrackExact.txt to ROOT tree format
 * that can be used directly in machine learning algorithm using
 * TMVA / sklearn / Keras.
 *
 * The advantage of this over TestTMVAData.cpp is that it can be used for
 * simulation setup having different objects at different places.
 *
 * Process : One the simulation is over, then run
 * 			 ./TestTMVA_Multiple Exact
 *
 * Output : This will create a ROOT file with two TTrees, one for Signal
 * 	        and other for Background
 *
 */

int main(int argc, char *argv[]){
	//Tomography::MLEM mlem;
	//mlem.Debugger();

	//Tomography::FillMaterial();
	std::map<std::string,unsigned int> material;
	material.insert(std::pair<std::string, unsigned int>("BG", 0));
	material.insert(std::pair<std::string, unsigned int>("Al", 1));
	material.insert(std::pair<std::string, unsigned int>("Fe", 2));
	material.insert(std::pair<std::string, unsigned int>("Pb", 3));
	material.insert(std::pair<std::string, unsigned int>("U", 4));


	//std::string mat = argv[2];

	if(argc == 1){
			std::cerr << "=======================================\n"
					  << "Please provided the required argument \n\n"
					  << "Following are possible values : Exact OR Fitted \n\n"
					  << "./TestTMVA_Multiple <Exact/Fitted>\n"
					  << "./TestTMVA_Multiple Exact\n"
					  << "=======================================\n";
			return 1;
		}


	//TTree branches
	int mater;
	double pr;
	double deltaTheta;
	double deltaThetaX;
	double deltaThetaY;
	double deltaX;
	double deltaY;
	double pocaPtX;
	double pocaPtY;
	double pocaPtZ;
	double absDeltaTheta;
	double incomingThetaX;
	double incomingThetaY;
	double outgoingThetaX;
	double outgoingThetaY;

	//Creating Tree for TMVA
	//Tracking::TreeSingleTon::Create("TMVA_"+mat+".root");
	Tracking::TreeSingleTon::Create("TMVA.root");
	Tracking::TreeSingleTon* singleTonObject = Tracking::TreeSingleTon::instance();
	{
	//Creating Signal Tree
	Tracking::ROOTTree *tr = new Tracking::ROOTTree("Signal");
	/*int material;
	double pr;
	double deltaTheta;
	double deltaThetaX;
	double deltaThetaY;
	double deltaX;
	double deltaY;
	double pocaPtX;
	double pocaPtY;
	double pocaPtZ;
	double absDeltaTheta;
	double incomingThetaX;
	double incomingThetaY;
	double outgoingThetaX;
	double outgoingThetaY;*/


	tr->CreateBranch("deltaTheta", deltaTheta);
	tr->CreateBranch("deltaThetaX", deltaThetaX);
	tr->CreateBranch("deltaThetaY", deltaThetaY);
	tr->CreateBranch("deltaX", deltaX);
	tr->CreateBranch("deltaY", deltaY);
	tr->CreateBranch("pr", pr);
	tr->CreateBranch("pocaPtX", pocaPtX);
	tr->CreateBranch("pocaPtY", pocaPtY);
	tr->CreateBranch("pocaPtZ", pocaPtZ);
	tr->CreateBranch("material", mater);
	tr->CreateBranch("absDeltaTheta", absDeltaTheta);
	tr->CreateBranch("incomingThetaX", incomingThetaX);
	tr->CreateBranch("incomingThetaY", incomingThetaY);
	tr->CreateBranch("outgoingThetaX", outgoingThetaX);
	tr->CreateBranch("outgoingThetaY", outgoingThetaY);
	tr->InitializeTree();
	singleTonObject->InsertTree(tr);
    }
	{
	//Creating Background Tree
	Tracking::ROOTTree *tr = new Tracking::ROOTTree("Background");
	/*int material;
	double pr;
	double deltaTheta;
	double deltaThetaX;
	double deltaThetaY;
	double deltaX;
	double deltaY;
	double pocaPtX;
	double pocaPtY;
	double pocaPtZ;
	double absDeltaTheta;
	double incomingThetaX;
	double incomingThetaY;
	double outgoingThetaX;
	double outgoingThetaY;*/


	tr->CreateBranch("deltaTheta", deltaTheta);
	tr->CreateBranch("deltaThetaX", deltaThetaX);
	tr->CreateBranch("deltaThetaY", deltaThetaY);
	tr->CreateBranch("deltaX", deltaX);
	tr->CreateBranch("deltaY", deltaY);
	tr->CreateBranch("pr", pr);
	tr->CreateBranch("pocaPtX", pocaPtX);
	tr->CreateBranch("pocaPtY", pocaPtY);
	tr->CreateBranch("pocaPtZ", pocaPtZ);
	tr->CreateBranch("material", mater);
	tr->CreateBranch("absDeltaTheta", absDeltaTheta);
	tr->CreateBranch("incomingThetaX", incomingThetaX);
	tr->CreateBranch("incomingThetaY", incomingThetaY);
	tr->CreateBranch("outgoingThetaX", outgoingThetaX);
	tr->CreateBranch("outgoingThetaY", outgoingThetaY);

	tr->InitializeTree();
	singleTonObject->InsertTree(tr);
	}



	Tomography::MLEM mlem("abc");
	std::vector<Tomography::Track> incomingTrackVector;
	std::vector<Tomography::Track> outgoingTrackVector;
	std::vector<double> momentumVector;
//	std::string type = std::string(argv[1]);
	//std::string fileToRead = "Track"+type+mat+".txt";
//	std::string fileToRead = "Track"+type+".txt";
	std::string fileToRead = std::string(argv[1])+".txt";
	CommonFunc::GetTracksVector(fileToRead,incomingTrackVector,outgoingTrackVector, momentumVector);
	std::cout << "Ayush : Total Number of Tracks : " << incomingTrackVector.size() << std::endl;

	//Getting Signal Tree
	Tracking::ROOTTree *signal = singleTonObject->GetROOTTree("Signal");
	Tracking::ROOTTree *background = singleTonObject->GetROOTTree("Background");


	Tracking::ImageReconstruction im;
	int sigCounter=0;
	int backCounter=0;


	for (int i = 0; i < incomingTrackVector.size(); i++) {
		std::cout << "Shachi : Processing Event no : " << i << std::endl;
		Tomography::Track incoming = incomingTrackVector[i];
		Tomography::Track outgoing = outgoingTrackVector[i];
		double momentum = momentumVector[i];
		Tomography::ScatteringData s = mlem.SetScatteringData(incoming,outgoing,momentum);
		Tracking::Vector3D<double> pocaPt = im.POCA(incoming,outgoing);
		bool falsePos = CommonFunc::Functions::IsFalsePositivePoca<true>(pocaPt);

		//s.Print();
		std::cout << s.sPr << " , " << s.sDeltaThetaX << " , " << s.sDeltaThetaY << " , " << s.sDeltaX << " , " << s.sDeltaY << std::endl;

		if(falsePos){
			backCounter++;
			//background->FillBranch("pr",s.sPr);
			background->FillBranch("deltaTheta",pocaPt.GetColor());
			background->FillBranch("deltaThetaX",s.sDeltaThetaX);
			background->FillBranch("deltaThetaY",s.sDeltaThetaY);
			background->FillBranch("deltaX",s.sDeltaX);
			background->FillBranch("deltaY",s.sDeltaY);
			background->FillBranch("pr",s.sPr);
			background->FillBranch("pocaPtX",pocaPt.x());
			background->FillBranch("pocaPtY",pocaPt.y());
			background->FillBranch("pocaPtZ",pocaPt.z());
			background->FillBranch("material",material["BG"]);
			background->FillBranch("absDeltaTheta",std::fabs(pocaPt.GetColor()));
			background->FillBranch("incomingThetaX", s.sIncomingThetaX);
			background->FillBranch("incomingThetaY", s.sIncomingThetaY);
			background->FillBranch("outgoingThetaX", s.sOutgoingThetaX);
			background->FillBranch("outgoingThetaY", s.sOutgoingThetaY);

			background->Fill();

		}else{
			sigCounter++;
			//signal->FillBranch("pr",s.sPr);
			signal->FillBranch("deltaTheta",pocaPt.GetColor());
			signal->FillBranch("deltaThetaX",s.sDeltaThetaX);
			signal->FillBranch("deltaThetaY",s.sDeltaThetaY);
			signal->FillBranch("deltaX",s.sDeltaX);
			signal->FillBranch("deltaY",s.sDeltaY);
			signal->FillBranch("pr",s.sPr);
			signal->FillBranch("pocaPtX",pocaPt.x());
			signal->FillBranch("pocaPtY",pocaPt.y());
			signal->FillBranch("pocaPtZ",pocaPt.z());
			std::string mat = CommonFunc::Functions::material;
			signal->FillBranch("material",material[mat]);
			signal->FillBranch("absDeltaTheta",std::fabs(pocaPt.GetColor()));
			signal->FillBranch("incomingThetaX", s.sIncomingThetaX);
			signal->FillBranch("incomingThetaY", s.sIncomingThetaY);
			signal->FillBranch("outgoingThetaX", s.sOutgoingThetaX);
			signal->FillBranch("outgoingThetaY", s.sOutgoingThetaY);
			signal->Fill();
		}





	}

	std::cout<<"SigCounter : " << sigCounter << " : BackCounter : " << backCounter << std::endl;

	singleTonObject->WriteToFile();

	return 0;
}

