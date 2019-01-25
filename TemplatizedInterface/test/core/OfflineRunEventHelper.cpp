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
#include "MLEM.h"
#include "VoxelV2.h"
#include "CommonFunc.h"
#include <fstream>
#include "Stats.h"

void PrintCleanVoxelsNumberAndCount(){
	std::cout << "================ Entered PrintCleanVoxelsNumberAndCount ================ " << std::endl;
	Tomography::evolution::Voxelator *voxelator = Tomography::evolution::Voxelator::instance();
	std::vector<Tomography::Voxel_V2*> voxelVector = voxelator->GetVoxelVector();
	//std::vector<Tracking::Vector3D<double>> filteredPocaPtVector = voxelator->GetFilteredPocaPtVectorUsingCleanedVoxel();
	std::vector<Tomography::Voxel_V2*> filteredVoxelVectorUsingCleanedVoxel = voxelator->GetVoxelVector(); //voxelator->GetFilteredVoxelVectorUsingCleanVoxel();
	//std::cout << "FilteredVoxelVectorUsingCleanedVoxel Size : " << filteredVoxelVectorUsingCleanedVoxel.size() << std::endl;

	int pocaCounter=0;

	std::ofstream outfile("filtereddd.txt");

	for(int i = 0 ; i < filteredVoxelVectorUsingCleanedVoxel.size() ; ){


		//pocaCounter += (filteredVoxelVectorUsingCleanedVoxel[i]->GetPocaPointsVector()).size();
		std::cout << "FilteredVoxel Num : " << i << std::endl;
		//filteredVoxelVectorUsingCleanedVoxel[i]->Print();



		Tomography::Voxel_V2 *voxel = filteredVoxelVectorUsingCleanedVoxel[i];
		int voxelNum = voxel->GetVoxelNum();
		if(voxelNum < 0 || voxelNum > Tomography::evolution::Voxelator::instance()->GetTotalNumberOfVoxels())
			continue;
		else{
			std::vector<Tracking::Vector3D<double>> pocaPtVector =
					voxel->GetPocaPointsVector();
			for (int j = 0; j < pocaPtVector.size(); j++) {
				outfile << pocaPtVector[j].x() << " " << pocaPtVector[j].y() << " "
						<< pocaPtVector[j].z() << " 0.0" << std::endl;
			}
		}
		i++;

		//delete voxel;

	} //loop ends

	outfile.close();

	//std::cout << "Total Num of filtered Poca Points : "  << pocaCounter << std::endl;

//	std::cout << "Filtered PocaPt VectorSize: "
	/*for(int i = 0 ; i < voxelVector.size() ; i++){
		if(voxelVector[i]->GetCleanVoxelCount() < 50){
			std::cout << "Voxel Num : " << voxelVector[i]->GetVoxelNum() << " :: CleanCount : " << voxelVector[i]->GetCleanVoxelCount()
					  <<"     ::::::::   " <<  __FILE__ << " : " << __LINE__ <<  std::endl;
		}
	}*/
}

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
	//Tomography::evolution::Voxelator::Create(50*cm,50*cm,75*cm,5*cm,5*cm,7.5*cm);
	Tomography::evolution::Voxelator::Create(50*cm,50*cm,45*cm,5*cm,5*cm,4.5*cm);
	{
		std::string fileToRead = "Track"+type+".txt";
		std::string fileToWrite = "EventHelperTrack"+type+".txt";
		Tomography::EventHelper u(fileToRead, fileToWrite);
		//Tomography::RunHelper r(type);
		Tomography::RunHelper* r = Tomography::RunHelper::instance(type);
		Tomography::ScatteringDensity s(125);
		s.Print();

		//PrintCleanVoxelsNumberAndCount();
		//Tomography::evolution::Voxelator::instance()->TestWrite();


		std::vector<Tracking::Vector3D<double>> pocaPtVector = r->GetPocaPtVector();

		std::cout << "Length of PocaVector : " << pocaPtVector.size() << std::endl;

//		Tomography::Stats st;
//		st.CreateScatteringHistogram();

		r->DetectorTrueAndFalsePositive();
		Tomography::Stats st;
		st.CreateScatteringHistogram();
		//Storing True positive in file
		CommonFunc::Functions::instance()->WriteToFile("TruePositivePoca.txt",r->GetTruePositivePocaPtVector());
		//Storing False positive in file
		CommonFunc::Functions::instance()->WriteToFile("FalsePositivePoca.txt",r->GetFalsePositivePocaPtVector());
		CommonFunc::Functions::instance()->WriteToFile("ThetaCutFilteredPocaPt.txt",r->ThetaCutFiltering());


		//std::vector<Tracking::Vector3D<double>> pocaPtVector;
		//Tomography::evolution::Voxelator::instance()->GetFilteredPocaPtVectorUsingCleanedVoxel(pocaPtVector);
		//std::cout << "Length of filtered PocaVector : " << pocaPtVector.size() << std::endl;
		//CommonFunc::Functions::instance()->WriteToFile("filteredPocaPtRunHelperUsingCleanVoxels-"+type+".txt",pocaPtVector);
	}
	return 0;
}



