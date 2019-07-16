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
	 gStyle->SetPalette(kRainBow);
	 gStyle->SetCanvasDefH(500); gStyle->SetCanvasDefW(500);
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

	double voxelizedVolumeHalfX = 50.*cm;
	double voxelizedVolumeHalfY = 50.*cm;
	double voxelizedVolumeHalfZ = 44.*cm;

	double voxelSizeXY =  2*voxelizedVolumeHalfX/numOfParts; // 5*cm;//
	double voxelSizeZ =  2*voxelizedVolumeHalfZ/numOfParts; //5*cm;//
	Tomography::evolution::Voxelator *voxelator = Tomography::evolution::Voxelator::Create(voxelizedVolumeHalfX,voxelizedVolumeHalfY,
																						   voxelizedVolumeHalfZ,voxelSizeXY,voxelSizeXY,voxelSizeZ);
	{
		std::string fileToRead = "Track"+type+".txt";
		std::string fileToWrite = "EventHelperTrack"+type+".txt";
		Tomography::EventHelper u(fileToRead, fileToWrite);
		std::cout << "===========================================================" << std::endl;
		//std::cout << "********* Contributory Track Counter : " << u.fContributoryTrackCounter << std::endl;
		std::cout << "TotalCount in Voxels for Acceptance : " << Tomography::evolution::Voxelator::instance()->GetTotalVoxelCountForAcceptance() << std::endl;
		std::cout << "===========================================================" << std::endl;

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
		TH2F *acceptanceHist1 = new TH2F("AcceptanceHistogram1","",nbinsx,startx,endx,nbinsy,starty,endy);
		acceptanceHist1->SetStats(0);
		TH2F *acceptanceHist2 = new TH2F("AcceptanceHistogram2","",nbinsx,startx,endx,nbinsy,starty,endy);
		acceptanceHist2->SetStats(0);
		TH2F *acceptanceHist3 = new TH2F("AcceptanceHistogram3","",nbinsx,startx,endx,nbinsy,starty,endy);
		acceptanceHist3->SetStats(0);
		double min = 0.;
		double max = 1.;
		acceptanceHist1->GetZaxis()->SetRangeUser(min, max);
		acceptanceHist2->GetZaxis()->SetRangeUser(min, max);
		acceptanceHist3->GetZaxis()->SetRangeUser(min, max);
		std::vector<unsigned int> eachVoxelCountForAcceptance = voxelator->GetEachVoxelCountForAcceptance();
		auto minmax = std::minmax_element(eachVoxelCountForAcceptance.begin(), eachVoxelCountForAcceptance.end());
		int max_element = *minmax.second;

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

		//Acceptance Histogram for Bottom layer
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

		//c.cd(1);
		{
		TCanvas *c=new TCanvas();
//		c_1->SetLogX();
//		c_1->SetLogY();
//		c_1->SetLogZ();
		//acceptanceHist->SetContour(3,contours);
		acceptanceHist1->Draw("colz");
		acceptanceHist1->Draw("cont2 same");
		c->SaveAs("Slice740.eps");
		}


		//c.cd(2);
		{
		TCanvas *c=new TCanvas();
		acceptanceHist2->Draw("colz");
		acceptanceHist2->Draw("cont2 same");
		c->SaveAs("Slice-10.eps");
		}

		//c.cd(3);
		{
		TCanvas *c=new TCanvas();
		acceptanceHist3->Draw("colz");
		acceptanceHist3->Draw("cont2 same");
		c->SaveAs("Slice-740.eps");
		}


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



