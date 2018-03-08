/*
 * TestGmm.cpp
 *
 *  Created on: Mar 8, 2018
 *      Author: rsehgal
 */

#include "GMM.h"
#include "Gaussian.h"
#include <iostream>

int main(){

	int numOfGaussians = 2;
	int dimension = 2;
	Tomography::GMM *gmm = new Tomography::GMM(numOfGaussians,dimension);

	std::cout << "No. of Gaussian : " << gmm->GetNumOfGaussians() << std::endl;
	for(int i =0 ; i < numOfGaussians ; i++){
		gmm->GetGaussianVector()[i]->PrintMean();
		gmm->GetGaussianVector()[i]->PrintCovars();
	}


	return 0;
}


