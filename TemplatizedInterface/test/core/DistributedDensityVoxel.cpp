/*
 * DistributedDensityVoxel.cpp
 *
 *  Created on: Feb 1, 2018
 *      Author: rsehgal
 */

/*
 * This code will consume fileterePocaPoints and then try to distribute scattering
 * density among nearby Voxel.
 */

#include "CommonFunc.h"
#include <fstream>
#include <iostream>
#include "Voxelator_Evolution.h"
#include "G4SystemOfUnits.hh"

int main(int argc, char **argv){

	Tomography::evolution::Voxelator::Create(50*cm,50*cm,45*cm,5*cm,5*cm,4.5*cm);
    Vector3D<double> pocaPt;
    std::ifstream ft;
    ft.open("filteredPocaPt.txt");
    double x=0.,y=0.,z=0;
    double color=0;

    //Input number of events at command line
    int count = atoi(argv[1]);
    std::vector<Tomography::Voxel*> enclosingVoxelsVector;

    int insertCounter = 0 ;

    while(count){
    	count--;
        	ft >> x >> y >> z >> color;
        	pocaPt.Set(x,y,z, std::fabs(color));
        	std::vector<Tomography::Voxel*> enclosingVoxelsVectorForAPt = CommonFunc::Functions::instance()->GetEnclosingVoxelWithScatteringIntensity(pocaPt);
        	for(int i =0 ; i < enclosingVoxelsVectorForAPt.size(); i++){

        		if(enclosingVoxelsVectorForAPt[i]->GetScatteringDensity() > 0.01){
        			insertCounter++;
        			std::cout << "RAMAN ScatteringDensity : " << enclosingVoxelsVectorForAPt[i]->GetScatteringDensity() << std::endl;
        			if(enclosingVoxelsVectorForAPt[i]->GetScatteringDensity() < 0.001)
        				std::cout<<"WHAT....................... Not POSSIBLE........... " << std::endl;
        			else{
        				enclosingVoxelsVector.push_back(enclosingVoxelsVectorForAPt[i]);
        				if(enclosingVoxelsVector[enclosingVoxelsVector.size()-1]->GetScatteringDensity() < 0.01)
        					std::cout<<"HERE.... IT ... COMES............. : "<<  std::endl;
        				else
        					std::cout<<"COMING CORRECTLY ........... : "<< enclosingVoxelsVector[enclosingVoxelsVector.size()-1]->GetScatteringDensity() << std::endl;
        			}
        		}
        	}
        	//enclosingVoxelsVectorForAPt.clear();
    }

    CommonFunc::Functions::instance()->WriteToFile("enclosingIntensityVoxels.txt",enclosingVoxelsVector);
    std::cout<<"Total Insertions must be : " << insertCounter;



}



