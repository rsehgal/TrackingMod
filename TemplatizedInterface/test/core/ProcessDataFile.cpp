/*
 * ProcessDataFile.cpp
 *
 *  Created on: Feb 27, 2018
 *      Author: rsehgal
 *
 *  This code will take RunNumber as input, and process the
 *  corresponding data file, and will create following
 *
 *  1) Angular distribution histogram.
 *
 */

#include <iostream>
#include <fstream>
#include <vector>
#include "EventProcessor.h"
#include "EventHelper.h"
#include "RunHelper.h"
#include "Tree.h"
#include "CommonFunc.h"
#include "Properties.h"
#include "GlassRpc.h"
#include "DetectorMapping.h"
#include "SetupManager.h"
#include <TH1F.h>
#include <G4SystemOfUnits.hh>

typedef Tomography::Properties Detector;

int main(int arc, char *argv[]){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	int nxbins = 200;
	TH1F *hist = new TH1F("Data", "Data", nxbins, -0.5, 1.5);

	std::string temp_str = std::to_string(atoi(argv[1]));
	temp_str += ".root";
	int clusterSize = 1;
	std::ofstream fs("MuonAngles.txt");

	Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::create("testMapping.txt");
	std::vector<std::string> detectorNamesVector = detectorMap->GetDetectorNamesVector();
	std::vector<int> startChannelVector = detectorMap->GetStartingChannelVector();
	std::vector<int> moduleVector = detectorMap->GetModuleVector();
	std::vector<double> zcoordinateVector = detectorMap->GetZCoordinateVector();

	for(int i = 0 ; i < detectorNamesVector.size()-1; i++){
	    std::cout << "Name of Detector : " << (i+1) << " : " << detectorNamesVector[i] << std::endl;
	    Detector *rpc = new Tomography::GlassRpc(moduleVector[i], detectorNamesVector[i], zcoordinateVector[i], startChannelVector[i]);
	    rpc->SetClusterSize(1);
	}

	fs << 3000 << " " << "G4_Fe" << " " << 100 << " ";

	Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);
	int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
	Tomography::EventProcessor eventProcessor;
	Tomography::SetupManager *setup = Tomography::SetupManager::instance();
	//numOfEvents = 2;
#define RECONSTRUCT
#ifdef RECONSTRUCT
	Tomography::evolution::Voxelator::Create(50*cm,50*cm,45*cm,10*cm,10*cm,9*cm);
#endif
	for (int evNo = 0; evNo < numOfEvents; evNo++) {
		std::cout << "=============== Event No : " << evNo << " =========================== " << std::endl;
		eventProcessor.ProcessEvent(evNo);
		if(setup->EventDetected()){
			std::cout << "Event Detected....." << std::endl;

		/*Tomography::Track incoming = eventProcessor.GetIncomingTrack();
		//Tomography::Track outgoing = eventProcessor.GetOutgoingTrack();
		Tomography::Track ref(G4ThreeVector(0.,0.,0.),G4ThreeVector(0.,0.,-1.));
		//incoming.Print();
		double angleIncoming = CommonFunc::Functions::instance()->GetAngleInRadian(incoming,ref);
		hist->Fill(angleIncoming);
		fs << angleIncoming << " ";
		*/

		std::vector<Tracking::Vector3D<double>> hitPointVector = eventProcessor.GetHitPointVector();
		Tomography::Track incoming(hitPointVector[0],hitPointVector[2]);
		Tomography::Track outgoing(hitPointVector[3],hitPointVector[5]);
		Tomography::EventHelper u(incoming,outgoing);
		//break;
		}else{
			std::cout << "Event NOT Detected....." << std::endl;
		}
	}

#ifdef RECONSTRUCT
   Tomography::RunHelper *runHelper = new Tomography::RunHelper();
  //Now trying to calculate Radiation for the whole run
  std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
  std::cout<<"\033[1;31m                              SD : " << CommonFunc::Functions::instance()->StandardDeviation(runHelper->GetScatteringAngleVector()) << "  radians for 2 sigma  \033[0m\n" << std::endl;
  //std::cout<<"                              SD : " << CommonFunc::Functions::instance()->StandardDeviation(b1Run->GetScatteringAngleVector()) << std::endl;
  std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
  std::cout<<"\033[1;31m                              RL : " << CommonFunc::Functions::instance()->RadiationLength(runHelper->GetScatteringAngleVector(),10) << "  cms  \033[0m\n" <<  std::endl;
  //std::cout<<"                              RL : " << CommonFunc::Functions::instance()->RadiationLength(b1Run->GetScatteringAngleVector(),10) << std::endl;
  std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
#endif

	hist->Draw();
	fs.close();


	fApp->Run();


}



