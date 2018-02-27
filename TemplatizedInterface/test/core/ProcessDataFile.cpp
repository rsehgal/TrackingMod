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
	for (int evNo = 0; evNo < numOfEvents; evNo++) {
		eventProcessor.ProcessEvent(evNo);
		if(setup->EventDetected()){

		Tomography::Track incoming = eventProcessor.GetIncomingTrack();
		//Tomography::Track outgoing = eventProcessor.GetOutgoingTrack();
		Tomography::Track ref(G4ThreeVector(0.,0.,0.),G4ThreeVector(0.,0.,-1.));
		//incoming.Print();
		double angleIncoming = CommonFunc::Functions::instance()->GetAngleInRadian(incoming,ref);
		hist->Fill(angleIncoming);
		fs << angleIncoming << " ";
		}
	}

	hist->Draw();
	fs.close();


	fApp->Run();


}



