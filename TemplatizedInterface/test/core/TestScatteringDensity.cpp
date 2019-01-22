/*
 * TestScatteringDensity.cpp
 *
 *  Created on: 03-Jan-2019
 *      Author: rsehgal
 */

#include <iostream>
#include "RunHelper.h"
#include "EventHelper.h"
#include "Voxelator_Evolution.h"
#include "G4SystemOfUnits.hh"
#include "ScatteringDensityV2.h"



int main(int argc, char *argv[]){

	if(argc == 1){
			std::cerr << "=======================================\n"
					  << "Please provided the required argument \n\n"
					  << "Following are possible values : Exact OR Fitted \n\n"
					  << "./TestScatteringDensity <Exact/Fitted>\n"
					  << "=======================================\n";
			return 1;
	}


	std::string type = std::string(argv[1]);
	Tomography::evolution::Voxelator::Create(50*cm,50*cm,75*cm,20*cm,20*cm,30*cm);
		{
			std::string fileToRead = "Track"+type+".txt";
			std::string fileToWrite = "EventHelperTrack"+type+".txt";
			Tomography::EventHelper u(fileToRead, fileToWrite);
			Tomography::RunHelper r(type);
			Tomography::ScatteringDensityV2 *scatteringDensity = Tomography::ScatteringDensityV2::instance();
			scatteringDensity->Initialize();
			scatteringDensity->Print();
		}

	return 0;
}



