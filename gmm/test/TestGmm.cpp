/*
 * TestGmm.cpp
 *
 *  Created on: Mar 8, 2018
 *      Author: rsehgal
 */

#include "GMM.h"
#include "GMM1D.h"
#include "Gaussian.h"
#include <iostream>

int main(){

	int numOfGaussians = 5;
	int dimension = 3;
	Tomography::GMM *gmm = new Tomography::GMM(numOfGaussians,dimension);

	std::cout << "No. of Gaussian : " << gmm->GetNumOfGaussians() << std::endl;
	for(int i = 0 ; i < numOfGaussians ; i++){
		std::cout<<"-----------------------------------------------------"<<std::endl;
		std::cout << "Mean of Gaussian : " << i << " : " ; gmm->GetGaussianVector()[i]->PrintMean();
		std::cout << "Covars of Gaussian : " << i << " : " << std::endl; gmm->GetGaussianVector()[i]->PrintCovars();
		std::cout << "Weight of Gaussian : " << i << " : " << gmm->GetGaussianVector()[i]->GetWeight() << std::endl;
	}
	std::cout<<"-----------------------------------------------------"<<std::endl;
	std::cout << "Prob : "<< gmm->GetGaussianVector()[0]->CalculateProbability(1.,2.,3.) << std::endl;

	std::cout<<"-------- Trying with some sensible data ---------------" << std::endl;
	Tomography::GMM1D *gmm2 = new Tomography::GMM1D(3);
	double mean1 = -1.;
	double mean2 = 0.;
	double mean3 = 3.;

	double covars1 = 2.25;
	double covars2 = 1;
	double covars3 = 0.25;

	gmm2->GetGaussianVector()[0]->SetMean(mean1);
	gmm2->GetGaussianVector()[1]->SetMean(mean2);
	gmm2->GetGaussianVector()[2]->SetMean(mean3);


	gmm2->GetGaussianVector()[0]->SetCovars(covars1);
	gmm2->GetGaussianVector()[1]->SetCovars(covars2);
	gmm2->GetGaussianVector()[2]->SetCovars(covars3);


	std::cout << "Probabilty w.r.t gaussian 0 : "<< gmm2->GetGaussianVector()[0]->CalculateProbability(2.7) << std::endl;
	std::cout << "Probabilty w.r.t gaussian 1 : "<< gmm2->GetGaussianVector()[1]->CalculateProbability(2.7) << std::endl;
	std::cout << "Probabilty w.r.t gaussian 2 : "<< gmm2->GetGaussianVector()[2]->CalculateProbability(2.7) << std::endl;




	return 0;
}


