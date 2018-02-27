/*
 * Gaussian.cpp
 *
 *  Created on: Feb 13, 2018
 *      Author: rsehgal
 */

#include "Gaussian.h"
#include <iostream>

namespace Tomography {

Gaussian::Gaussian() {
	// TODO Auto-generated constructor stub
	AllocateMemory();
	*fMean = 0.;
	*(fMean+1) = 0.;
	*(fCovars+0) = 0.;
	*(fCovars+1) = 0.;
	*(fCovars+2) = 0.;
	*(fCovars+3) = 0.;
}

Gaussian::Gaussian(double *mean, double **covars){
	fMean = mean;
	fCovars = covars;
}

void Gaussian::AllocateMemory(){
	fMean = new double[2];
	fCovars = new double*[2];
	for(int i = 0; i < 2; ++i) {
	    fCovars[i] = new double[2];
	}
}

Gaussian::Gaussian(double mean[2], double covars[2][2]){
	AllocateMemory();
	*fMean = mean[0];
	*(fMean+1) = mean[1];
	*(fCovars+0) = covars[0];
	*(fCovars+1) = covars[1];
	*(fCovars+2) = covars[2];
	*(fCovars+3) = covars[3];

}

void Gaussian::PrintMean() const{
	std::cout << fMean[0] << " , " << fMean[1] << std::endl;
}

void Gaussian::PrintCovars() const{
	std::cout << fCovars[0][0] << " , " << fCovars[0][1] << std::endl;
	std::cout << fCovars[1][0] << " , " << fCovars[1][1] << std::endl;
}

Gaussian::~Gaussian() {
	// TODO Auto-generated destructor stub
	delete [] fMean;
	delete [] fCovars;
}

} /* namespace Tomography */
