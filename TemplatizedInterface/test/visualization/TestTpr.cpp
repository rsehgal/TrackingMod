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

//using Tomography::VisualizationHelper;
using Tracking::Vector3D;

int main(int argc, char *argv[]){
	Tomography::DetectorMapping::create("testMapping.txt");
	const char* filename = argv[1];
	int truePositive = 0 ;
	int falsePositive = 0 ;
	std::ifstream ft(filename);
	double x=0.,y=0.,z=0, color=0;
    while(!ft.eof()){
    	ft >> x >> y >> z >> color;
    	std::cout << "X : " << x <<" : Y : " << y << " : Z : " << z << std::endl;
    	bool falsePos = CommonFunc::Functions::IsFalsePositivePoca<true>(Vector3D<double>(x,y,z));
    	if(falsePos)
    		falsePositive++;
    	else
    		truePositive++;
    }

    double tpr = (double)truePositive / (truePositive+falsePositive);
    double fpr = (double)falsePositive / (truePositive+falsePositive);

    std::cout << "TruePositive : " << truePositive << " : FalsePositive : " << falsePositive << std::endl;
    std::cout << "TPR : " << tpr <<" : FPR : " << fpr << std::endl;

    return 0;

}



