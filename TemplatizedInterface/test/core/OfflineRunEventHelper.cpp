/*
 * OfflineRunEventHelper.cpp
 *
 *  Created on: Aug 8, 2018
 *      Author: raman
 */
#include "RunHelper.h"
#include "EventHelper.h"
#include "Voxelator_Evolution.h"
#include "G4SystemOfUnits.hh"


int main(int argc, char *argv[]){
	std::string type = std::string(argv[1]);
	Tomography::evolution::Voxelator::Create(50*cm,50*cm,45*cm,5*cm,5*cm,4.5*cm);
	{
		std::string fileToRead = "Track"+type+".txt";
		std::string fileToWrite = "EventHelperTrack"+type+".txt";
		Tomography::EventHelper u(fileToRead, fileToWrite);
		Tomography::RunHelper r(type);
	}
	return 0;
}



