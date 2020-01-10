/*
 * TestROC.cpp
 *
 *  Created on: 25-Oct-2018
 *      Author: rsehgal
 */

//#include <TApplication.h>
//#include "VisualizationHelper.h"
#include "base/Vector3D.h"
#include <iostream>
#include <fstream>
#include "CommonFunc.h"

#define cm 10

//using Tomography::VisualizationHelper;
using Tracking::Vector3D;

int main(int argc, char *argv[]){
#ifndef TPR_ANALYSIS
	std::cerr << "TPR_ANALYSIS NOT Switched ON, kindly switch on -DTPR_ANALYSIS=ON while giving cmake command .... " << std::endl << "Exiting....." << std::endl;
	exit(1);
#endif
	Tomography::DetectorMapping::create("testMapping.txt");

	const char* filename = argv[1];
	int val = std::atoi(argv[2]);
	int numOfParts = std::atoi(argv[3]);
	int truePositive = 0 ;
	int falsePositive = 0 ;
	std::ifstream ft(filename);
	double doca=0.;
	double x=0.,y=0.,z=0, color=0;
	std::ofstream tpPoca("TpPoca.txt");


	double voxelSizeXY = (double)100/numOfParts;
	double voxelSizeZ = (double)88/numOfParts;
	double voxelVolume = voxelSizeXY*voxelSizeXY*voxelSizeZ;

    while(!ft.eof()){
    	ft >> x >> y >> z >> color >> doca;
    	std::cout << "X : " << x <<" : Y : " << y << " : Z : " << z << std::endl;
    	bool falsePos = CommonFunc::Functions::IsFalsePositivePoca<true>(Vector3D<double>(x,y,z));
    	if(falsePos)
    		falsePositive++;
    	else{
    		truePositive++;
    		tpPoca << x << " " << y << " " << z << " " << color << " " << doca << std::endl;
    	}
    }
    tpPoca.close();

    double tpr = (double)truePositive / (truePositive+falsePositive);
    double fpr = (double)falsePositive / (truePositive+falsePositive);

    double error = tpr*std::sqrt((1./double(truePositive))+(1./double(truePositive+falsePositive)));

    std::ofstream tprHandle;
    std::ofstream fprHandle;
    std::ofstream dataHandle("data.csv",std::ios::app);


	if (val == 0) {
		tprHandle.open("Tpr.txt", std::ios::app);
		fprHandle.open("Fpr.txt", std::ios::app);
	} else {
		tprHandle.open("filteredTpr.txt", std::ios::app);
		fprHandle.open("filteredFpr.txt", std::ios::app);
	}
    std::cout << "TruePositive : " << truePositive << " : FalsePositive : " << falsePositive << std::endl;
    std::cout << "TPR : " << tpr <<" : FPR : " << fpr << std::endl;
    tprHandle << tpr << " " << error << " " ;
    fprHandle << fpr << " ";
    dataHandle << voxelVolume << "," << tpr << "," << fpr << "," << truePositive << "," << falsePositive << std::endl;
    dataHandle.close();

    tprHandle.close();
    fprHandle.close();

    return 0;

}



