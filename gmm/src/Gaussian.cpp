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
/*
	AllocateMemory();
	*fMean = 0.;
	*(fMean+1) = 0.;
	*(fCovars+0) = 0.;
	*(fCovars+1) = 0.;
	*(fCovars+2) = 0.;
	*(fCovars+3) = 0.;
*/
}

Gaussian::Gaussian(double *mean, double **covars){
	fMean = mean;
	fCovars = covars;
}

void Gaussian::AllocateMemory(){
	fDim = 2;
	fMean = new double[2];
	fCovars = new double*[2];
	for(int i = 0; i < 2; ++i) {
	    fCovars[i] = new double[2];
	}
}

void Gaussian::AllocateMemory3(){
	fDim = 3;
	fMean = new double[3];
	fCovars = new double*[3];
	for(int i = 0; i < 3; ++i) {
	    fCovars[i] = new double[3];
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

Gaussian::Gaussian(double mean[3], double covars[3][3]){
	AllocateMemory3();
	*fMean = mean[0];
	*(fMean+1) = mean[1];
	*(fMean+2) = mean[2];
	*(fCovars+0) = covars[0];
	*(fCovars+1) = covars[1];
	*(fCovars+2) = covars[2];
	*(fCovars+3) = covars[3];
	*(fCovars+4) = covars[4];
	*(fCovars+5) = covars[5];
	*(fCovars+6) = covars[6];
	*(fCovars+7) = covars[7];
	*(fCovars+8) = covars[8];


}

void Gaussian::PrintMean() const{

	for(int i = 0 ; i < fDim ; i++)
		std::cout << fMean[i] << " , " ;
	std::cout << std::endl;
}

void Gaussian::PrintCovars() const{
	for(int i = 0 ; i < fDim ; i++){
		for(int j = 0 ; j < fDim ; j++){
			std::cout << fCovars[i][j] << " , " ;
		}
		std::cout << std::endl;
	}

}

Gaussian::~Gaussian() {
	// TODO Auto-generated destructor stub
	delete [] fMean;
	delete [] fCovars;
}

} /* namespace Tomography */
