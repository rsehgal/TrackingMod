/*
 * TestScatteringAndPoCALocation.cpp
 *
 *  Created on: 04-Dec-2019
 *      Author: rsehgal
 */

#include "EventHelper.h"
#include "CommonFunc.h"
#include "Voxelator_Evolution.h"
#include "Files.h"
#include "DetectorMapping.h"
#include "VoxelNavigator.h"
#include "Delta.h"
#include <G4SystemOfUnits.hh>
#include "base/Global.h"
#include <fstream>
#include "Track.h"
#include <iostream>
#include <TApplication.h>
#include <TH1F.h>
#include "Imaging.h"
#include <TCanvas.h>

int main(int argc, char *argv[]){

	//
	std::string filename = std::string(argv[1]);
	int numOfParts = std::atoi(argv[2]);
	double voxelSizeXY = (double)100*cm/numOfParts;
	double voxelSizeZ = (double)100*cm/numOfParts;
	Tomography::evolution::Voxelator *voxelator = Tomography::evolution::Voxelator::Create(50*cm,50*cm,44*cm,voxelSizeXY,voxelSizeXY,voxelSizeZ);

	Tracking::ImageReconstruction fIm;
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	TCanvas *c=new TCanvas();
	c->Divide(2,2);
	double incomingTrackP1X = 0., incomingTrackP1Y = 0., incomingTrackP1Z = 0.;
	double incomingTrackP2X = 0., incomingTrackP2Y = 0., incomingTrackP2Z = 0.;
	double outgoingTrackP1X = 0., outgoingTrackP1Y = 0., outgoingTrackP1Z = 0.;
	double outgoingTrackP2X = 0., outgoingTrackP2Y = 0., outgoingTrackP2Z = 0.;
	double scattererHitted = 0.;
	double momentum = 0.;
	TH1F *histScat = new TH1F("ScatteringHist","ScatteringHist",100,-0.2,0.2);

	int totalNumOfVoxels = voxelator->GetTotalNumberOfVoxels();
	TH1F *voxelHist = new TH1F("VoxelHistogram","VoxelHistogram",totalNumOfVoxels,0,totalNumOfVoxels);
	TH1F *voxelHistNoisy = new TH1F("VoxelHistogram","VoxelHistogram",totalNumOfVoxels,0,totalNumOfVoxels);


	TH1F *voxelPointCountHist = new TH1F("voxelPointCountHist","voxelPointCountHist",50,0,50);



	std::ifstream infile(filename);

	int numOfLayers=4;

	while (!infile.eof()) {
			//fTrackId++;
			infile >> incomingTrackP1X >> incomingTrackP1Y >> incomingTrackP1Z
					>> incomingTrackP2X >> incomingTrackP2Y >> incomingTrackP2Z
					>> outgoingTrackP1X >> outgoingTrackP1Y >> outgoingTrackP1Z
					>> outgoingTrackP2X >> outgoingTrackP2Y >> outgoingTrackP2Z
					>> scattererHitted >> momentum;

			Tomography::Track incoming(Tracking::Vector3D<double>(incomingTrackP1X, incomingTrackP1Y,incomingTrackP1Z),
									   Tracking::Vector3D<double>(incomingTrackP2X, incomingTrackP2Y,incomingTrackP2Z));

			Tomography::Track outgoing(Tracking::Vector3D<double>(outgoingTrackP1X, outgoingTrackP1Y,outgoingTrackP1Z),
									   Tracking::Vector3D<double>(outgoingTrackP2X, outgoingTrackP2Y,outgoingTrackP2Z));

			double scat = CommonFunc::Functions::instance()->GetAngleInRadian(incoming,outgoing);
			if(std::fabs(scat) > 0.00001){
				Tracking::Vector3D<double> poCA = fIm.POCA(incoming,outgoing);
				if(voxelator->IsGenuine(poCA))
					histScat->Fill(scat);
					int voxelNum = voxelator->GetVoxelNumber(poCA);
					std::cout << "Voxel Num : " << voxelNum << std::endl;
					voxelHist->Fill(voxelNum);
					if((voxelNum < numOfLayers*numOfParts*numOfParts) || ( voxelNum > ((numOfParts-numOfLayers)*numOfParts*numOfParts) && voxelNum < totalNumOfVoxels))
					//if( ( voxelNum > ((numOfParts-numOfLayers)*numOfParts*numOfParts) && voxelNum < totalNumOfVoxels))
						voxelHistNoisy->Fill(voxelNum);
			}
			std::cout << "Scattering : " << scat << std::endl;



	}


	//Reading the voxelHist histogram and creating another histogram for voxelPointCount
	int numOfBins = voxelHistNoisy->GetSize();

	c->cd(1);
	histScat->Draw();

	c->cd(2);
	voxelHist->Draw();

	c->cd(3);
	voxelHistNoisy->Draw();

	std::cout <<"NumOfBins from Histogram : " << numOfBins << std::endl;

	for(unsigned int i = 1 ; i < numOfBins-1 ; i++){

			double binContent = voxelHistNoisy->GetBinContent(i);
			//std::cout << "BinContent : " << binContent << std::endl;
			if(binContent >= 1)
			voxelPointCountHist->Fill(binContent);

		}

	c->cd(4);

	voxelPointCountHist->Draw();


	fApp->Run();
	return 0;
}


