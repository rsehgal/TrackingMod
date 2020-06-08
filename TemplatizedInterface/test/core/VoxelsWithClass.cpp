/*
 * OfflineRunEventHelper.cpp
 *
 *  Created on: June 07, 2020
 *      Author: raman
 */
#include "RunHelper.h"
#include "EventHelper.h"
#include "Voxelator_Evolution.h"
#include "G4SystemOfUnits.hh"
#include "MLEM.h"
#include "VoxelV2.h"
#include "CommonFunc.h"
#include <fstream>
#include "Stats.h"
#include "base/Vector3D.h"

using Tracking::Vector3D;

int main(int argc, char *argv[]){

	std::string type = std::string(argv[1]);
	int numOfParts = std::atoi(argv[2]);
	double voxelSizeXY = (double)100*cm/numOfParts;
	double zDimSize=(double)45*cm;
	double voxelSizeZ = 2*zDimSize/numOfParts;
	Tomography::evolution::Voxelator *voxelator = Tomography::evolution::Voxelator::Create(50*cm,50*cm,zDimSize,voxelSizeXY,voxelSizeXY,voxelSizeZ);

	std::string fileToRead = type;
	std::string fileToWrite = "ClassWithVoxels.txt";

	std::ifstream infile(fileToRead);
	std::ofstream outfile(fileToWrite);

	while(!infile.eof()){
		double x=0.,y=0.,z=0.,matClass=0.,exPar=0.;
		infile >> x >> y >> z >> matClass >> exPar;
		unsigned int voxelNum = voxelator->GetVoxelNumber(x,y,z);
		outfile << x << " " <<  y << " " << z << " " <<  matClass << " " << voxelNum << std::endl;
	}	

	outfile.close();
	infile.close();

}
