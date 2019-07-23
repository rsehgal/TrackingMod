/*
 * TestTrackPredictorWithSimulatedData.cpp
 *
 *  Created on: 27-Jun-2019
 *      Author: rsehgal
 */
#include <iostream>
#include "TrackPredictorV2.h"
#include "Coordinates.h"
#include "Track.h"
#include <iostream>
#include <fstream>
#include "DetectorMapping.h"

using Tomography::TrackPredictorV2;
using Vec_t = Tracking::Vector3D<double>;
using Tomography::Coordinates;

int main(){
	TrackPredictorV2 t;
	Coordinates c;

	std::ifstream infile("ActualHitAndPixelCenter.txt");
	double x1=0.,y1=0.,z1=0.,x2=0.,y2=0.,z2=0.;

	Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::create("testMapping.txt");
	int numOfRpcs = detectorMap->GetNumOfDetectors("GLASS");
	std::vector<std::string> detNamesVector = detectorMap->GetDetectorNamesVector("GLASS");

	int evId =0;
	std::vector< std::vector< Vector3D<double> > > superVect;

	unsigned int timeForNEvents = 0;
	unsigned int totalTime = 0;
	unsigned int N = 10000;
	while(!infile.eof()){
		std::string evString="";
		std::vector< Vector3D<double> > vectOfVector3D;

		for(int i=0; i<numOfRpcs;i++){
			std::string str = "" ;
			infile >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
			Vec_t hitPt(x1,y1,z1);
			vectOfVector3D.push_back(hitPt);
			if(i==0){
				str = detNamesVector[i]+"_"+c.GetStripNumXYZString(hitPt);
			}else{
				str = "_"+detNamesVector[i]+"_"+c.GetStripNumXYZString(hitPt);
			}
			evString += str;

		}
		timeForNEvents += t.Process(evString,vectOfVector3D);
		superVect.push_back(vectOfVector3D);
		//std::cout << "Event string of event id : " << evId <<" : " << evString << std::endl;
		if(!(evId%N)){
			std::cout <<"=======================================" << std::endl;
			std::cout << "No of Events Processed : " << evId << std::endl;
			std::cout << "Event string of event id : " << evId <<" : " << evString << std::endl;
			std::cout << "Time taken to Process : " << N << " events : " << timeForNEvents << std::endl;
			totalTime += timeForNEvents;
			timeForNEvents = 0;
			std::cout << "Setting time to Zero : " <<  timeForNEvents << std::endl;
		}

		evId++;

	}

	std::cout <<"***************************************************************"<<std::endl;
	unsigned int totalComb = t.GetTotalNumOfCombinations();
	std::cout <<"Total Num of Combination : " << totalComb << std::endl;
	unsigned int totalTracks = t.GetTotalNumOfRegisteredTracks();
	std::cout <<"Total Num of Registered Tracks : " << totalTracks << std::endl;
	unsigned int diff = totalTracks - totalComb;
	std::cout << "Diff : " << diff << std::endl;
	std::cout <<"***************************************************************"<<std::endl;
	std::cout <<"Total Time Taken for Search function : " << totalTime << std::endl;

	/*for(unsigned int i = 0 ; i < t.GetTotalNumOfCombinations() ; i++){
		std::cout << "Combination ID : " << i <<" : Combination Name : " << t.GetCombinationName(i)
	                                     << "  : Num of Tracks in Combination : " << t.GetNumOfTracks(i) <<  std::endl;
	}*/

	return 0;
}



