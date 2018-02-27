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
	double mean[2]={0.5,0.5};
	double covars[2][2] = {{1.,2.},{3.,4.}};
	Gaussian *g = new Gaussian(mean,covars);

	g->PrintMean();
	g->PrintCovars();

	return 0;
}



