/*
 * TestPathLengthInVoxels.cpp
 *
 *  Created on: 10-Jan-2019
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
#include "VoxelNavigator.h"


int main(int argc, char *argv[]){
	//Tomography::MLEM mlem;
	//mlem.Debugger();
#if 0
	if(argc == 1){
			std::cerr << "=======================================\n"
					  << "Please provided the required argument \n\n"
					  << "Following are possible values : Exact OR Fitted \n\n"
					  << "./TestPathLengthInVoxels <Exact/Fitted>\n"
					  << "=======================================\n";
			return 1;
	}

	std::vector<Tomography::Track> incomingTrackVector;
	std::vector<Tomography::Track> outgoingTrackVector;
	std::vector<double> momentumVector;

	std::string type = std::string(argv[1]);
	Tomography::evolution::Voxelator::Create(50 * cm, 50 * cm, 45 * cm, 20 * cm, 20 * cm, 18 * cm);

	std::cout << "===================================================================" << std::endl;
	std::cout << "CenterAyush Voxel Number : " << Tomography::evolution::Voxelator::instance()->GetVoxelNumber(0.,0.,0.) << std::endl;
	std::cout << "===================================================================" << std::endl;
	std::string fileToRead = "Track" + type + ".txt";
	std::string fileToWrite = "EventHelperTrack" + type + ".txt";
	Tomography::EventHelper u(fileToRead, fileToWrite);
	Tomography::RunHelper r(type);

	Tomography::MLEM mlem;

	CommonFunc::GetTracksVector(fileToRead, incomingTrackVector,outgoingTrackVector, momentumVector);
	std::cout << "Ayush : Total Number of Tracks : " << incomingTrackVector.size() << std::endl;
	for (int i = 0; i < incomingTrackVector.size(); i++) {
		//for(int i = 0 ; i < 10 ; i++){
		std::cout << "Shachi : Processing Event no : " << i << std::endl;
		Tomography::Track incoming = incomingTrackVector[i];
		Tomography::Track outgoing = outgoingTrackVector[i];
		double momentum = momentumVector[i];
		mlem.VoxelFinder(incoming, outgoing, momentum);

	}

	std::vector<std::vector<Tomography::LTOfEachVoxel>> vectorOfLTVector = mlem.GetVectorOfLTVector();
	Tomography::ScatteringDensityV2 *scatteringDensity = Tomography::ScatteringDensityV2::instance();
	scatteringDensity->InitializeZero();

	int totalNumOfVoxels = Tomography::evolution::Voxelator::instance()->GetTotalNumberOfVoxels();
	for(int i = 0 ; i < vectorOfLTVector.size() ; i ++){
		for(int j = 0 ; j < vectorOfLTVector[i].size() ; j++ ){
			scatteringDensity->AddPathLengthInVoxel(vectorOfLTVector[i][j].sVoxelNum,vectorOfLTVector[i][j].sL);
		}

	}

	double maxVal =  0.;
	int maxIndex = 0;
	for(int i = 0 ; i < totalNumOfVoxels ; i++){
		double val = scatteringDensity->GetTotalPathLengthVector()[i];
		std::cout << val << " ";
		if(!(i%25))
			std::cout << std::endl;
		if(maxVal < val){
			maxVal = val;
			maxIndex = i ;
		}
	}
	std::cout << "\n===============================================================" << std::endl;
	std::cout << "Max Index : " << maxIndex << " :: Max Value : " << maxVal << std::endl;


	int voxNum = 46;
	Tracking::Vector3D<double> voxCenter = Tomography::evolution::Voxelator::instance()->GetVoxelCenter(voxNum);
	std::cout << "Voxel Center of Voxel Number :  " << voxNum << " : " ; voxCenter.Print();
#endif
	Tomography::evolution::Voxelator::Create(50*cm,50*cm,45*cm,5*cm,5*cm,4.5*cm);
	std::cout <<"==================== EDGE TEST ====================" << std::endl;
	Tomography::VoxelNavigator voxelNavigator;
	Tracking::Vector3D<double> testPoint(415.919,-500,-224.789);
	Tracking::Vector3D<double> voxelCenter(425,-475,-202.5);
	Tomography::Track tr = Tomography::Track(Tracking::Vector3D<double>(383.201,-338.4,45.8332),Tracking::Vector3D<double>(443.146,-634.483,-450));
	std::cout << "TrackLength : " << tr.GetTrackLength() << std::endl;
	Tracking::Vector3D<double> dir = tr.GetDirCosine();
	double step = voxelNavigator.ComputeStep(testPoint,dir,voxelCenter);
	std::cout << "Step Length : " << step << std::endl;



	return 0;
}


