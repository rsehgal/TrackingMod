/*
 * OfflineThresholdTest.cpp
 *
 *  Created on: 08-Jan-2020
 *      Author: rsehgal
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
#include "TApplication.h"
#include "TCanvas.h"
#include "TStyle.h"

int main(int argc, char *argv[]){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	gStyle->SetOptStat(0);
	TCanvas *c=new TCanvas();
	c->Divide(2,2);
	if(argc == 1){
		std::cerr << "=======================================\n"
				  << "Please provided the required argument \n\n"
				  << "Following are possible values : Exact OR Fitted \n\n"
				  << "./OfflineRunEventHelper <Exact/Fitted>\n"
				  << "=======================================\n";
		return 1;
	}
	std::string type = std::string(argv[1]);
	int numOfParts = std::atoi(argv[2]);
	double voxelSizeXY = (double)100*cm/numOfParts;
	double voxelSizeZ = (double)88*cm/numOfParts;
	double voxelVolume = voxelSizeXY*voxelSizeXY*voxelSizeZ;
	Tomography::evolution::Voxelator *voxelator = Tomography::evolution::Voxelator::Create(50*cm,50*cm,44*cm,voxelSizeXY,voxelSizeXY,voxelSizeZ);
	int totalNumOfVoxels = voxelator->GetTotalNumberOfVoxels();

	std::string fileToRead = type+".txt";

	std::ifstream infile(fileToRead);
	//12 variable to read xyz for two tracks
	double incomingTrackP1X = 0., incomingTrackP1Y = 0., incomingTrackP1Z = 0.;
	double incomingTrackP2X = 0., incomingTrackP2Y = 0., incomingTrackP2Z = 0.;
	double outgoingTrackP1X = 0., outgoingTrackP1Y = 0., outgoingTrackP1Z = 0.;
	double outgoingTrackP2X = 0., outgoingTrackP2Y = 0., outgoingTrackP2Z = 0.;
	double scattererHitted = 0.;
	double momentum = 0.;

	unsigned int genuineCounter=0;

	std::ofstream outfile("RawPoCAGenuineThresholding.txt");
	//TH1F *voxelPointCountHist = new TH1F("voxelPointCountHist","voxelPointCountHist",50,0,50);
	//TH1F *voxelHistNoisy = new TH1F("VoxelHistogram","VoxelHistogram",totalNumOfVoxels,0,totalNumOfVoxels);
	TH1F *voxelHist = new TH1F("VoxelHistogram","VoxelHistogram",totalNumOfVoxels,0,totalNumOfVoxels);

	unsigned int evNo=0;
	//unsigned int counter=249944;
	while (!infile.eof()) {
	//while (counter) {
		    //std::cout << "@@@@ Processing EvNo : " << evNo << std::endl;
		    //counter--;
		    evNo++;
			infile >> incomingTrackP1X >> incomingTrackP1Y >> incomingTrackP1Z
					>> incomingTrackP2X >> incomingTrackP2Y >> incomingTrackP2Z
					>> outgoingTrackP1X >> outgoingTrackP1Y >> outgoingTrackP1Z
					>> outgoingTrackP2X >> outgoingTrackP2Y >> outgoingTrackP2Z
					>> scattererHitted >> momentum;

			Tomography::Track incoming(
								Tracking::Vector3D<double>(incomingTrackP1X,incomingTrackP1Y, incomingTrackP1Z),
								Tracking::Vector3D<double>(incomingTrackP2X,incomingTrackP2Y, incomingTrackP2Z));

			Tomography::Track outgoing(
								Tracking::Vector3D<double>(outgoingTrackP1X,outgoingTrackP1Y, outgoingTrackP1Z),
								Tracking::Vector3D<double>(outgoingTrackP2X,outgoingTrackP2Y, outgoingTrackP2Z));

			ImageReconstruction Im;
			Tracking::Vector3D<double> poca = Im.POCA(incoming,outgoing);
			if(poca.GetColor() > Tomography::unscatteringThreshold)
			{
			//if(CommonFunc::Functions::instance()->GetAngleInRadian(incoming,outgoing) > Tomography::unscatteringThreshold){
				bool isGenuine = Tomography::evolution::Voxelator::instance()->IsGenuine(poca);
				if(isGenuine){
					genuineCounter++;
					outfile << poca.x() <<" "<< poca.y() <<" "<< poca.z() <<" "<< poca.GetColor() <<" "<< poca.GetDoCA() << std::endl;
					int voxelNum = voxelator->GetVoxelNumber(poca);
					voxelHist->Fill(voxelNum);
					/*if(voxelator->IfVoxelExistInNoisyEnvelop(voxelNum))
						voxelHistNoisy->Fill(voxelNum);*/

					int voxNum = Tomography::evolution::Voxelator::instance()->IfVoxelExist(voxelNum);
					if (voxNum < 0.) {

						Tomography::evolution::Voxelator::instance()->Insert(poca,	voxelNum);
					} else {
						Tomography::evolution::Voxelator::instance()->GetVoxelVector()[voxNum]->Insert(poca);
					}
				}
			}
	}

	//Reading the voxelHist histogram and creating another histogram for voxelPointCount
	//int numOfBins = voxelHistNoisy->GetSize();

	voxelator->PredictThreshold();
	double threshold = voxelator->GetThresholdVal();
	std::cout <<"Chosen threhold : "<< threshold << std::endl;

	std::vector<Tracking::Vector3D<double>> pocaPtVector;
		for(unsigned int i = 0 ; i < voxelator->GetVoxelVector().size() ; i++){
			Tomography::Voxel_V2 *voxel= voxelator->GetVoxelVector()[i];
			if(voxel->GetPointCount() > threshold){
				for(unsigned int j = 0 ; j < voxel->GetPocaPointsVector().size() ; j++){
					pocaPtVector.push_back(voxel->GetPocaPointsVector()[j]);
				}
			}
		}

	CommonFunc::Functions::instance()->WriteToFile("filtered-ThresholdingBased-"+type+".txt",pocaPtVector);


	TH1F *histNoisyPointCount = voxelator->GetPocaPtCountHistOfNoisyVoxels();

	/*voxelator->CalcSDOfEachVoxel();
	int totalNumOfVoxels = voxelator->GetTotalNumberOfVoxels();
	TH1F *sdOfEachVoxel = new TH1F("SDofEachVoxel","SDofEachVoxel",totalNumOfVoxels,0,totalNumOfVoxels);
	for(unsigned int i = 0 ; i < voxelator->GetVoxelVector().size() ; i++){
		sdOfEachVoxel->SetBinContent(voxelator->GetVoxelVector()[i]->GetVoxelNum(),voxelator->GetVoxelVector()[i]->GetStandardDeviation());
	}

	std::vector<Tracking::Vector3D<double>> pocaPtVector;
	for(unsigned int i = 0 ; i < voxelator->GetVoxelVector().size() ; i++){
		Tomography::Voxel_V2 *voxel= voxelator->GetVoxelVector()[i];
		if((int)(voxel->GetStandardDeviation()*voxel->GetPointCount()) > 0){
			for(unsigned int j = 0 ; j < voxel->GetPocaPointsVector().size() ; j++){
				pocaPtVector.push_back(voxel->GetPocaPointsVector()[j]);
			}
		}
	}

	CommonFunc::Functions::instance()->WriteToFile("filtered-"+type+".txt",pocaPtVector);

	std::cout << "Total Number of Events processed : " << evNo << std::endl;
	std::cout << "Genuine Debugging Counter : " << genuineCounter << std::endl;
	outfile.close();

	sdOfEachVoxel->Draw();
	fApp->Run();
*/
	c->cd(1);
	voxelHist->Draw();
	c->cd(2);
	histNoisyPointCount->Draw();

	new TCanvas();
	voxelHist->Draw();

	fApp->Run();

}



