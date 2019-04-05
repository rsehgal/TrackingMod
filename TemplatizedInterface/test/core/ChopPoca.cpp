/*
 * ChopPoca.cpp
 *
 *  Created on: 03-Apr-2019
 *      Author: rsehgal
 *
 *  This program is useful for remove the PocaPt written to file.
 *  It remove the PocaPt which lie outside the VoxelizedVolume region
 *  and also those whose angular deviation is less than unscatteringThreshold
 */

#include <iostream>
#include <fstream>
#include "base/Vector3D.h"
#include "Voxelator_Evolution.h"
#include <G4SystemOfUnits.hh>

using Tracking::Vector3D;


int main(int argc, char *argv[]){
	std::ifstream infile(argv[1]);
	std::ofstream outfile("genuinePocaPt.txt");
	Tomography::evolution::Voxelator::Create(50*cm,50*cm,45*cm,2.5*cm,2.5*cm,2.25*cm);
	double x,y,z,scattering;
	while(!infile.eof()){
		infile >> x >> y >> z >> scattering;
		Vector3D<double> pocaPt(x,y,z,scattering);
		bool isGenuine = Tomography::evolution::Voxelator::instance()->IsGenuine(pocaPt);
		if (isGenuine) {
			if (std::fabs(scattering) > Tomography::unscatteringThreshold) {
				outfile << x << " " << y << " " << z << " " << scattering << std::endl;
			}
		}
	}
}




