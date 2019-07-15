/*
 * TestAcceptance.cpp
 *
 *  Created on: July 08, 2019
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
#include <TH2F.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TStyle.h>

int main(int argc, char *argv[]){
	 TApplication *fApp = new TApplication("Test", NULL, NULL);
	 double contours[3];
	    contours[0] = 500;
	    contours[1] = 700;
	    contours[2] = 900;
	    //

	    //Int_t colors[10] = {kRed, kYellow, kGreen, kBlue, kMagenta,kOrange, kCyan,kPink,kAzure,kGray};
	    gStyle->SetPalette(kRainBow);

	 TCanvas c;
	 c.Divide(2,2);
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
	double voxelSizeXY =  (double)100*cm/numOfParts; // 5*cm;//
	double voxelSizeZ =  (double)100*cm/numOfParts; //5*cm;//
	Tomography::evolution::Voxelator *voxelator = Tomography::evolution::Voxelator::Create(50*cm,50*cm,105.*cm,voxelSizeXY,voxelSizeXY,voxelSizeZ);
	{
		std::string fileToRead = "Track"+type+".txt";
		std::string fileToWrite = "EventHelperTrack"+type+".txt";
		Tomography::EventHelper u(fileToRead, fileToWrite);
		std::cout << "===========================================================" << std::endl;
		//std::cout << "********* Contributory Track Counter : " << u.fContributoryTrackCounter << std::endl;
		std::cout << "TotalCount in Voxels for Acceptance : " << Tomography::evolution::Voxelator::instance()->GetTotalVoxelCountForAcceptance() << std::endl;
		std::cout << "===========================================================" << std::endl;

//		Tomography::RunHelper* r = Tomography::RunHelper::instance(type);
//		std::vector<Tracking::Vector3D<double>> pocaPtVector = r->GetPocaPtVector();
//		std::cout << "Length of PocaVector : " << pocaPtVector.size() << std::endl;
//		r->DetectorTrueAndFalsePositive();
//		std::cout << "TruePositiveCount : " << r->GetTruePositivePocaPtCount() << std::endl;

		Tomography::evolution::Voxelator *voxelator = Tomography::evolution::Voxelator::instance();
		int nbinsx = voxelator->GetVoxelatorDim().x();
		int nbinsy = voxelator->GetVoxelatorDim().y();
		int nbinsz = voxelator->GetVoxelatorDim().z();
		int zCenterSlice = nbinsz/2;
		Tracking::Vector3D<double> voxelizedVolumeDim = voxelator->GetVoxelizedVolumeDim();
		int startx = -1*voxelizedVolumeDim.x()/2;
		int endx = voxelizedVolumeDim.x()/2;
		int starty = -1*voxelizedVolumeDim.y()/2;
		int endy = voxelizedVolumeDim.y()/2;

		std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		TH2F *acceptanceHist1 = new TH2F("AcceptanceHistogram1","AcceptanceHistogram",nbinsx,startx,endx,nbinsy,starty,endy);
		TH2F *acceptanceHist2 = new TH2F("AcceptanceHistogram2","AcceptanceHistogram",nbinsx,startx,endx,nbinsy,starty,endy);
		TH2F *acceptanceHist3 = new TH2F("AcceptanceHistogram3","AcceptanceHistogram",nbinsx,startx,endx,nbinsy,starty,endy);
		double min = 0.;
		double max = 1.;
		acceptanceHist1->GetZaxis()->SetRangeUser(min, max);
		acceptanceHist2->GetZaxis()->SetRangeUser(min, max);
		acceptanceHist3->GetZaxis()->SetRangeUser(min, max);
		std::vector<unsigned int> eachVoxelCountForAcceptance = voxelator->GetEachVoxelCountForAcceptance();
		auto minmax = std::minmax_element(eachVoxelCountForAcceptance.begin(), eachVoxelCountForAcceptance.end());
		int max_element = *minmax.second;

		/*for(auto& n : eachVoxelCountForAcceptance){
			bool valid=true;
			Tracking::Vector3D<double> voxelCenter = voxelator->GetVoxelCenter(n,valid);
			std::cout <<"VoxelCenter : " ; voxelCenter.Print();
			if(valid)
				acceptanceHist->Fill(voxelCenter.x(),voxelCenter.y());
		}*/

		//Acceptance Histogram for top layer
		//for (unsigned int i = 0 ; i < eachVoxelCountForAcceptance.size() ; i++) {
		int numOfPixels = nbinsx*nbinsy;
		for (unsigned int i = (nbinsz-1)*numOfPixels ; i < nbinsz*numOfPixels ; i++) {
			bool valid = true;
			std::cout <<"eachVoxelCountForAcceptance[i] : " << eachVoxelCountForAcceptance[i] << std::endl;
			if(eachVoxelCountForAcceptance[i] > 0){
				Tracking::Vector3D<double> voxelCenter = voxelator->GetVoxelCenter(i, valid);
				std::cout << "VoxelCenter : ";
				voxelCenter.Print();
				if (valid)
					acceptanceHist1->Fill(voxelCenter.x(), voxelCenter.y(),(double)eachVoxelCountForAcceptance[i]/max_element);
			}
		}

		//Acceptance Histogram for middle layer
		for (unsigned int i = (nbinsz/2)*numOfPixels ; i < (nbinsz/2 + 1)*numOfPixels ; i++) {
					bool valid = true;
					std::cout <<"eachVoxelCountForAcceptance[i] : " << eachVoxelCountForAcceptance[i] << std::endl;
					if(eachVoxelCountForAcceptance[i] > 0){
						Tracking::Vector3D<double> voxelCenter = voxelator->GetVoxelCenter(i, valid);
						std::cout << "VoxelCenter : ";
						voxelCenter.Print();
						if (valid)
							acceptanceHist2->Fill(voxelCenter.x(), voxelCenter.y(),(double)eachVoxelCountForAcceptance[i]/max_element);
					}
		}

		//Acceptance Histogram for middle layer
				for (unsigned int i = 0 ; i < numOfPixels ; i++) {
							bool valid = true;
							std::cout <<"eachVoxelCountForAcceptance[i] : " << eachVoxelCountForAcceptance[i] << std::endl;
							if(eachVoxelCountForAcceptance[i] > 0){
								Tracking::Vector3D<double> voxelCenter = voxelator->GetVoxelCenter(i, valid);
								std::cout << "VoxelCenter : ";
								voxelCenter.Print();
								if (valid)
									acceptanceHist3->Fill(voxelCenter.x(), voxelCenter.y(),(double)eachVoxelCountForAcceptance[i]/max_element);
							}
				}

		c.cd(1);
//		c_1->SetLogX();
//		c_1->SetLogY();
//		c_1->SetLogZ();
		//acceptanceHist->SetContour(3,contours);
		acceptanceHist1->Draw("colz");
		acceptanceHist1->Draw("cont2 same");

		c.cd(2);
		acceptanceHist2->Draw("colz");
		acceptanceHist2->Draw("cont2 same");

		c.cd(3);
		acceptanceHist3->Draw("colz");
		acceptanceHist3->Draw("cont2 same");


		c.cd(4);
		int totalNumOfVoxels = voxelator->GetTotalNumberOfVoxels();
		TH1F *voxelCountHist = new TH1F("VoxelCountHistogram","VoxelCountHistogram",totalNumOfVoxels,0,totalNumOfVoxels);
		for(unsigned int i = 0 ; i < eachVoxelCountForAcceptance.size() ; i++){
			voxelCountHist->Fill(i,eachVoxelCountForAcceptance[i]);
		}
		voxelCountHist->Draw();

		fApp->Run();


	}
	return 0;
}



