/*
 * TestGaussian.cpp
 *
 *  Created on: Feb 13, 2018
 *      Author: rsehgal
 */

#include "Gaussian.h"
#include <iostream>

using Tomography::Gaussian;

int main(){
	std::cout <<"--------------- Trying to Create 2D Gaussian-----------" << std::endl;
	double mean[2]={0.5,0.5};
	double covars[2][2] = {{1.,2.},{3.,4.}};
	Gaussian *g = new Gaussian(mean,covars);
	g->PrintMean();
	g->PrintCovars();

	std::cout <<"--------------- Trying to Create 3D Gaussian-----------" << std::endl;
	double mean3[3]={0.5,0.5,0.7};
	double covars3[3][3] = {{1.,2.,3.},{3.,4.,5.},{3.,4.,6.}};
	Gaussian *g3 = new Gaussian(mean3,covars3);
	g3->PrintMean();
	g3->PrintCovars();
	std::cout << "Determinant : " << std::endl << g3->GetDeterminant() << std::endl;
	std::cout << "Transpose : " << std::endl << g3->CalculateCovarsTranspose() << std::endl;
	std::cout << "Inverse : " << std::endl << g3->CalculateCovarsInverse() << std::endl;
	std::cout << "Probability : " << g3->CalculateProbability(1.,2.,3.) << std::endl;

	return 0;
}



