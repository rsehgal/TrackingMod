/*
 * Test2DLinearFit.cpp
 *
 *  Created on: Mar 5, 2018
 *      Author: rsehgal
 */

#include "Fit2DLinear.h"
#include <iostream>
#include <fstream>
#include "base/Vector3D.h"
int main(){
	std::ifstream Infile;
	Infile.open("regressionData2.txt");
	std::vector<double> xVect, yVect;
	int count=15;
	while(count)
	{
		double x = 0., y=0.;
		Infile>> x >> y;
		xVect.push_back(x);
		yVect.push_back(y);
		std::cout<< x <<"  "<< y << std::endl << std::endl;
		count--;
	}
	Infile.close();

	Tomography::Fit2DLinear f;
	f.Fit(xVect,yVect);
	f.PrintFitModel();

	std::vector<Tracking::Vector3D<double>> hitpointVect;
	f.CalculateResiduals(hitpointVect);
	f.PrintResiduals();

}



