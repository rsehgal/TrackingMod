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
	int dimension = 3;
	Tomography::GMM *gmm = new Tomography::GMM(numOfGaussians,dimension);

	std::cout << "No. of Gaussian : " << gmm->GetNumOfGaussians() << std::endl;
	for(int i = 0 ; i < numOfGaussians ; i++){
		std::cout << "Mean of Gaussian : " << i << " : " ; gmm->GetGaussianVector()[i]->PrintMean();
		std::cout << "Covars of Gaussian : " << i << " : " << std::endl; gmm->GetGaussianVector()[i]->PrintCovars();
	}

	std::cout << "Prob : "<< gmm->GetGaussianVector()[0]->CalculateProbability(1.,2.,3.) << std::endl;

	return 0;
}


