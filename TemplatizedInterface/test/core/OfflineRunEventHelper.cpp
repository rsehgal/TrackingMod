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
	if(argc == 1){
		std::cerr << "=======================================\n"
				  << "Please provided the required argument \n\n"
				  << "Following are possible values : Exact OR Fitted \n\n"
				  << "./OfflineRunEventHelper <Exact/Fitted>\n"
				  << "=======================================\n";
		return 1;
	}
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



