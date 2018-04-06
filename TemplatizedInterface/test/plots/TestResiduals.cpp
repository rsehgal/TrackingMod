/*
 * TestResiduals.cpp
 *
 *  Created on: Apr 5, 2018
 *      Author: rsehgal
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
#include "Fit2DLinear.h"

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

	Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);
	int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();

	/* We need EventProcess, because this guy will
	 * will give us the hit point vector for every
	 * event.
	 */
	Tomography::EventProcessor eventProcessor;
	Tomography::SetupManager *setup = Tomography::SetupManager::instance();

	Tomography::evolution::Voxelator::Create(50*cm,50*cm,45*cm,10*cm,10*cm,9*cm);

	int detectedEventCount = 0;

	for (int evNo = 0; evNo < numOfEvents; evNo++) {
		//if(evNo==10)
			//break;
		//std::cout << "=============== Event No : " << evNo << " =========================== " << std::endl;
		eventProcessor.ProcessEvent(evNo);
		if(setup->EventDetected()){
			std::cout << "==============  Event Detected..... : EvNo : " << evNo << "  ==================" <<  std::endl;
			detectedEventCount++;
//			if(detectedEventCount == 10)
//				break;

			std::vector<Tracking::Vector3D<double>> hitPointVector = eventProcessor.GetHitPointVector();
			std::cout<<"Hit Point Vector Size : " << hitPointVector.size() << std::endl;

			Tracking::Vector3D<double> dir = hitPointVector[1]-hitPointVector[0];
			Tracking::Vector3D<double> unitDir = dir.Unit();
			double dist = (zcoordinateVector[2]-zcoordinateVector[1])/unitDir.z();
			Tracking::Vector3D<double> extraPolatedHitPoint = hitPointVector[1] + ( unitDir * dist );
			std::cout << "HitPointVector[0] : "; hitPointVector[0].Print();
			std::cout << "HitPointVector[1] : "; hitPointVector[1].Print();
			std::cout << "HitPointVector[2] : "; hitPointVector[2].Print();
			std::cout << "ExtraPolatedHit Point : " ; extraPolatedHitPoint.Print();


/*
			Tomography::Track incoming, outgoing;
			int numOfDetectors = 4;
			if(numOfDetectors==3){
				incoming.Set(hitPointVector[0],hitPointVector[1]);
				outgoing.Set(hitPointVector[1],hitPointVector[2]);
				//Tomography::EventHelper u(incoming,outgoing);
			}else{
				incoming.Set(hitPointVector[0],hitPointVector[hitPointVector.size()/2-1]);
				outgoing.Set(hitPointVector[hitPointVector.size()/2],hitPointVector[hitPointVector.size()-1]);
			}

			std::cout << "Deviation  : " << CommonFunc::Functions::instance()->GetAngleInRadian(incoming,outgoing) << std::endl;
			Tomography::EventHelper u(incoming,outgoing);
*/

		}
		if(detectedEventCount == 10)
						break;
	}


//	std::cout << "Total Number of Detected Events : " << detectedEventCount << std::endl;
//	Tomography::RunHelper *runHelper = new Tomography::RunHelper();

}


