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
#include <TH1F.h>
#include <TApplication.h>
#include <TStyle.h>



using Tracking::Vector3D;

int main(int argc, char *argv[]){

	TApplication *fApp = new TApplication("Test", NULL, NULL);
	std::string type = std::string(argv[1]);
	int numOfParts = std::atoi(argv[2]);
	double voxelSizeXY = (double)100*cm/numOfParts;
	double zDimSize=(double)45*cm;
	double voxelSizeZ = 2*zDimSize/numOfParts;
	Tomography::evolution::Voxelator *voxelator = Tomography::evolution::Voxelator::Create(50*cm,50*cm,zDimSize,voxelSizeXY,voxelSizeXY,voxelSizeZ);

	std::string fileToRead = type;
	std::string fileToWrite = "ClassWithVoxels.txt";
	std::string voxelFile = "Voxels.txt";

	std::ifstream infile(fileToRead);
	std::ofstream outfile(fileToWrite);
	std::ofstream outVoxelFile(voxelFile);

	while(!infile.eof()){
		double x=0.,y=0.,z=0.,matClass=0.,exPar=0.;
		infile >> x >> y >> z >> matClass >> exPar;
		unsigned int voxelNum = voxelator->GetVoxelNumber(x,y,z);

		outfile << x << " " <<  y << " " << z << " " <<  matClass << " " << voxelNum << std::endl;
		
	}	

	outfile.close();
	infile.close();

	Vector3D<double> voxDim = voxelator->GetEachVoxelDim();
	int col=1;
	outVoxelFile  <<  voxDim.x() << " " <<  voxDim.y() << " " <<  voxDim.z() << " " <<  col << std::endl;


	Tomography::EventHelper evHelper(fileToWrite);
	//Tomography::evolution::Voxelator *voxelator=Tomography::evolution::Voxelator::instance();
	std::vector<Tomography::Voxel_V2*> voxelVector = voxelator->GetVoxelVector();
	std::cout << "===================================================" << std::endl;
	std::cout<<"Size of Voxel Vector : " << voxelVector.size() << std::endl;
	for(unsigned int i = 0 ; i < voxelVector.size() ; i++){
		Vector3D<double> voxCenter = voxelVector[i]->GetVoxelCenter();
		voxelVector[i]->FillClassHist();
		TH1F *h=voxelVector[i]->GetClassHist();
		int binmax = h->GetMaximumBin();
		//double x = h->GetXaxis()->GetBinCenter(binmax);
		std::cout << "Num of PoCA points for index : "<< i << " : Voxel Number : " << voxelVector[i]->GetVoxelNum()
				  << " : " << voxelVector[i]->GetPocaPointsVector().size()
				  << " : Majority Class : " << binmax << std::endl;
		outVoxelFile << voxCenter.x() << " " << voxCenter.y() << " " << voxCenter.z()  
					 << " " << binmax << " " << binmax << std::endl;

	}
	//voxelVector[0]->FillClassHist();
	//voxelVector[0]->GetClassHist()->Draw();

	outVoxelFile.close();
	fApp->Run();

	return 0;
}
