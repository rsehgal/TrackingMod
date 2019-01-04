/*
 * TestMLEM.cpp
 *
 *  Created on: 21-Dec-2018
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

	std::vector<Tomography::Track> incomingTrackVector;
	std::vector<Tomography::Track> outgoingTrackVector;

	std::string type = std::string(argv[1]);
	//Tomography::evolution::Voxelator::Create(50*cm,50*cm,75*cm,20*cm,20*cm,30*cm);
	Tomography::evolution::Voxelator::Create(50*cm,50*cm,45*cm,20*cm,20*cm,18*cm);
		//{
			std::string fileToRead = "Track"+type+".txt";
			std::string fileToWrite = "EventHelperTrack"+type+".txt";
			Tomography::EventHelper u(fileToRead, fileToWrite);
			Tomography::RunHelper r(type);
			//Tomography::ScatteringDensity s(125);
			//s.Print();
		//}

		//Satisfied by these
		Tomography::MLEM mlem;
		//mlem.Debugger();

		//Doing Real test now


		CommonFunc::GetTracksVector(fileToRead,incomingTrackVector,outgoingTrackVector);
		std::cout << "Ayush : Total Number of Tracks : " << incomingTrackVector.size() << std::endl;
		for(int i = 0 ; i < incomingTrackVector.size() ; i++){
		//for(int i = 0 ; i < 10 ; i++){
			std::cout << "Shachi : Processing Event no : " << i << std::endl;
			Tomography::Track incoming = incomingTrackVector[i];
			Tomography::Track outgoing = outgoingTrackVector[i];
			mlem.VoxelFinder(incoming,outgoing);

		}

		mlem.EMUpdate();

		std::ofstream outfile("mlem.txt");
		Tracking::Vector3D<double> voxelSize = Tomography::evolution::Voxelator::instance()->GetEachVoxelDim();
		outfile << voxelSize.x() << " " << voxelSize.y() << " " << voxelSize.z() <<  " " << "0" << std::endl;
		Tomography::ScatteringDensityV2 *scatteringDensity = Tomography::ScatteringDensityV2::instance();
		for(int i = 0 ; i < scatteringDensity->GetLambda().size() ; i++ ){
		//for(int i = 0 ; i < 5 ; i++ ){
			Tracking::Vector3D<double> voxelCenter = Tomography::evolution::Voxelator::instance()->GetVoxelCenter(i);
			outfile << voxelCenter.x() << " " << voxelCenter.y() << " " << voxelCenter.z() << " " << "0" << " " << scatteringDensity->GetLambda()[i] << std::endl;
		}
		outfile.close();

		std::cout << "Scattering density of origin Voxel : " << scatteringDensity->GetLambda()[62] << std::endl;





	return 0;
}


