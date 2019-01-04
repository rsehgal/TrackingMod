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


int main(int argc, char *argv[]){
	//Tomography::MLEM mlem;
	//mlem.Debugger();

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
		//for(int i = 0 ; i < 100 ; i++){
			std::cout << "Shachi : Processing Event no : " << i << std::endl;
			Tomography::Track incoming = incomingTrackVector[i];
			Tomography::Track outgoing = outgoingTrackVector[i];
			mlem.VoxelFinder(incoming,outgoing);

		}

		mlem.EMUpdate();





	return 0;
}


