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
#include "VoxelV2.h"
#include <TStyle.h>

int main(int argc, char *argv[]){

	//
	std::string filename = std::string(argv[1]);
	int numOfParts = std::atoi(argv[2]);
	double voxelSizeXY = (double)100*cm/numOfParts;
	double voxelSizeZ = (double)100*cm/numOfParts;
	Tomography::evolution::Voxelator *voxelator = Tomography::evolution::Voxelator::Create(50*cm,50*cm,44*cm,voxelSizeXY,voxelSizeXY,voxelSizeZ);

	Tracking::ImageReconstruction fIm;
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	//gStyle->SetOptStat(0);
	TCanvas *c=new TCanvas();
	c->Divide(2,3);
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

	TH1F *sdOfEachVoxel = new TH1F("SDofEachVoxel","SDofEachVoxel",totalNumOfVoxels,0,totalNumOfVoxels);

	std::ifstream infile(filename+".txt");

	//int numOfLayers=4;

	//Vector to store vector of PoCA Point
	std::vector<Tracking::Vector3D<double>> pocaPtVector;
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
			Tomography::Track ref(Vector3D<double>(0.,0.,0.),Vector3D<double>(0.,0.,-1.));
			double angleIncoming = CommonFunc::Functions::instance()->GetAngleInRadian(incoming,ref);
			double angleOutgoing = CommonFunc::Functions::instance()->GetAngleInRadian(outgoing,ref);
			scat = angleOutgoing-angleIncoming;

			//if(std::fabs(scat) > 0.0001){
			//if (std::fabs(scat) > Tomography::unscatteringThreshold)
			{
				std::cout <<"FABS of scattering Angle : " << std::fabs(scat) << std::endl;
				Tracking::Vector3D<double> poCA = fIm.POCA(incoming,outgoing);
				poCA.SetColor(scat);

				if(voxelator->IsGenuine(poCA)){
					pocaPtVector.push_back(poCA);
					histScat->Fill(scat);
					int voxelNum = voxelator->GetVoxelNumber(poCA);

					//std::cout << "Voxel Num : " << voxelNum << std::endl;
					voxelHist->Fill(voxelNum);
					/*if((voxelNum < numOfLayers*numOfParts*numOfParts) ||
							( voxelNum > ((numOfParts-numOfLayers)*numOfParts*numOfParts) && voxelNum < totalNumOfVoxels)){
					//if( ( voxelNum > ((numOfParts-numOfLayers)*numOfParts*numOfParts) && voxelNum < totalNumOfVoxels))
						voxelHistNoisy->Fill(voxelNum);
					}*/
					if(voxelator->IfVoxelExistInNoisyEnvelop(voxelNum))
						voxelHistNoisy->Fill(voxelNum);

					//-----------------------------
					//Inserting PoCA in the Voxels
					int voxNum = Tomography::evolution::Voxelator::instance()->IfVoxelExist(voxelNum);
					if(voxNum < 0.){

						Tomography::evolution::Voxelator::instance()->Insert(poCA,voxelNum);
					}else{
						Tomography::evolution::Voxelator::instance()->GetVoxelVector()[voxNum]->Insert(poCA);
					}

					//------------------------------
			}
			}
			//std::cout << "Scattering : " << scat << std::endl;



	}

	voxelator->PredictThreshold();
	unsigned int thresholdVal=voxelator->GetThresholdVal();
	std::cout << "Calculated Threshold : " << thresholdVal << std::endl;


	//Trying to get filtered PocaPoint directly
	std::vector<Tracking::Vector3D<double>> filteredPocaPtVector;
	for(unsigned int i = 0 ; i < voxelator->GetVoxelVector().size() ; i++){
		int voxelNum = voxelator->GetVoxelVector()[i]->GetVoxelNum();
		if(!(voxelator->IfVoxelExistInNoisyEnvelop(voxelNum))){
		if(voxelator->GetVoxelVector()[i]->GetPointCount() > thresholdVal){
			for(unsigned int j = 0 ; j < voxelator->GetVoxelVector()[i]->GetPocaPointsVector().size() ; j++){
				filteredPocaPtVector.push_back(voxelator->GetVoxelVector()[i]->GetPocaPointsVector()[j]);
			}
		}
	}
	}





	CommonFunc::Functions::instance()->WriteToFile("NewPoca.txt",pocaPtVector);
	CommonFunc::Functions::instance()->WriteToFile("NewFilteredPoca.txt",filteredPocaPtVector);

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





	//Tomography::evolution::Voxelator::instance()->CalcSDOfEachVoxel();
	std::cout << "Size of Voxel Vector : " << voxelator->GetVoxelVector().size() << std::endl;
	std::vector<Tomography::Voxel_V2*> filteredVoxelVector = voxelator->GetFilteredVoxelVectorBasedOnWeightedCount();
	std::cout << "Size of Filtered Voxel Vector : " << filteredVoxelVector.size() << std::endl;

	CommonFunc::Functions::instance()->WriteToFile("filtered-"+filename+"-Weighted.txt",
												   Tomography::evolution::Voxelator::instance()->GetFilteredPocaPtVector(
												   filteredVoxelVector));

	std::cout <<"=======================================================" << std::endl;
	for(unsigned int i = 0 ; i < voxelator->GetVoxelVector().size() ; i++){
		//sdOfEachVoxel->Fill(voxelator->GetVoxelVector()[i]->GetVoxelNum(),voxelator->GetVoxelVector()[i]->GetStandardDeviation());

		/*std::cout <<"Voxel Num : " << voxelator->GetVoxelVector()[i]->GetVoxelNum()
				  <<" : SD : " << voxelator->GetVoxelVector()[i]->GetStandardDeviation()
				  <<" : PointCount in Voxel : " << voxelator->GetVoxelVector()[i]->GetPointCount() << std::endl;
		*/


		if(voxelator->GetVoxelVector()[i]->GetPointCount() == voxelator->GetVoxelVector()[i]->GetPocaPointsVector().size()){
		for(unsigned int j = 0 ; j < voxelator->GetVoxelVector()[i]->GetPointCount() ; j++){
			std::cout << voxelator->GetVoxelVector()[i]->GetPocaPointsVector()[j].GetColor() << " , ";
		}
		std::cout << std::endl;
		std::cout << "Lets Calculate SD Now ..." << std::endl;
		voxelator->GetVoxelVector()[i]->CalcSD();
		double sd=voxelator->GetVoxelVector()[i]->GetStandardDeviation() ;
		std::cout <<"Calculated SD : " << sd << std::endl;
		sdOfEachVoxel->Fill(voxelator->GetVoxelVector()[i]->GetVoxelNum(),sd);
		}else{
			std::cout << "INVALID>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
		}
	}


	std::cout <<"=======================================================" << std::endl;
	c->cd(5);
	sdOfEachVoxel->Draw();

	fApp->Run();
	return 0;
}


