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


	std::string mat = argv[2];

	if(argc == 1){
			std::cerr << "=======================================\n"
					  << "Please provided the required argument \n\n"
					  << "Following are possible values : Exact OR Fitted \n\n"
					  << "./TestMLEM <Exact/Fitted>\n"
					  << "=======================================\n";
			return 1;
		}

	//Creating Tree for TMVA
	Tracking::TreeSingleTon::Create("TMVA_"+mat+".root");
	Tracking::TreeSingleTon* singleTonObject = Tracking::TreeSingleTon::instance();
	{
	//Creating Signal Tree
	Tracking::ROOTTree *tr = new Tracking::ROOTTree("Signal");
	int material;
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

	//tr->CreateBranch("pr", pr);
	tr->CreateBranch("deltaTheta", deltaTheta);
	tr->CreateBranch("deltaThetaX", deltaThetaX);
	tr->CreateBranch("deltaThetaY", deltaThetaY);
	tr->CreateBranch("deltaX", deltaX);
	tr->CreateBranch("deltaY", deltaY);
	tr->CreateBranch("pocaPtX", pocaPtX);
	tr->CreateBranch("pocaPtY", pocaPtY);
	tr->CreateBranch("pocaPtZ", pocaPtZ);
	tr->CreateBranch("material", material);
	tr->CreateBranch("absDeltaTheta", absDeltaTheta);
	tr->InitializeTree();
	singleTonObject->InsertTree(tr);
    }
	{
	//Creating Background Tree
	Tracking::ROOTTree *tr = new Tracking::ROOTTree("Background");
	int material;
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

	//tr->CreateBranch("pr", pr);
	tr->CreateBranch("deltaTheta", deltaTheta);
	tr->CreateBranch("deltaThetaX", deltaThetaX);
	tr->CreateBranch("deltaThetaY", deltaThetaY);
	tr->CreateBranch("deltaX", deltaX);
	tr->CreateBranch("deltaY", deltaY);
	tr->CreateBranch("pocaPtX", pocaPtX);
	tr->CreateBranch("pocaPtY", pocaPtY);
	tr->CreateBranch("pocaPtZ", pocaPtZ);
	tr->CreateBranch("material", material);
	tr->CreateBranch("absDeltaTheta", absDeltaTheta);

	tr->InitializeTree();
	singleTonObject->InsertTree(tr);
	}



	Tomography::MLEM mlem("abc");
	std::vector<Tomography::Track> incomingTrackVector;
	std::vector<Tomography::Track> outgoingTrackVector;
	std::vector<double> momentumVector;
	std::string type = std::string(argv[1]);
	std::string fileToRead = "Track"+type+mat+".txt";
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
		Tomography::ScatteringData s = mlem.SetScatteringData(incoming,outgoing);
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
			background->FillBranch("pocaPtX",pocaPt.x());
			background->FillBranch("pocaPtY",pocaPt.y());
			background->FillBranch("pocaPtZ",pocaPt.z());
			background->FillBranch("material",material["BG"]);
			background->FillBranch("absDeltaTheta",std::fabs(pocaPt.GetColor()));
			background->Fill();

		}else{
			sigCounter++;
			//signal->FillBranch("pr",s.sPr);
			signal->FillBranch("deltaTheta",pocaPt.GetColor());
			signal->FillBranch("deltaThetaX",s.sDeltaThetaX);
			signal->FillBranch("deltaThetaY",s.sDeltaThetaY);
			signal->FillBranch("deltaX",s.sDeltaX);
			signal->FillBranch("deltaY",s.sDeltaY);
			signal->FillBranch("pocaPtX",pocaPt.x());
			signal->FillBranch("pocaPtY",pocaPt.y());
			signal->FillBranch("pocaPtZ",pocaPt.z());
			signal->FillBranch("material",material[mat]);
			signal->FillBranch("absDeltaTheta",std::fabs(pocaPt.GetColor()));
			signal->Fill();
		}





	}

	std::cout<<"SigCounter : " << sigCounter << " : BackCounter : " << backCounter << std::endl;

	singleTonObject->WriteToFile();

	return 0;
}

