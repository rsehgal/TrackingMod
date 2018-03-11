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
#include <fstream>

void Print(Tomography::GMM1D *gmm){
	std::cout << "---------------------------------------------" << std::endl;
	for(int i = 0 ; i < gmm->GetNumOfGaussians() ; i++){
		std::cout<<"============ Gaussian No. : " << (i+1) << "=====================" << std::endl;
		gmm->GetGaussianVector()[i]->PrintMean();
		gmm->GetGaussianVector()[i]->PrintCovars();
		gmm->GetGaussianVector()[i]->PrintWeight();
		std::cout << "Sigma of Gaussian : " << i << " : " << std::sqrt(gmm->GetGaussianVector()[i]->GetCovars()) << std::endl;
	}

}

int main(){
#if(0)
	int numOfGaussians = 5;
	int dimension = 3;
	Tomography::GMM *gmm = new Tomography::GMM(numOfGaussians,dimension);

	std::cout << "No. of Gaussian : " << gmm->GetNumOfGaussians() << std::endl;
	for(int i = 0 ; i < numOfGaussians ; i++){
		std::cout<<"-----------------------------------------------------"<<std::endl;
		std::cout << "Mean of Gaussian : " << i << " : " ; gmm->GetGaussianVector()[i]->PrintMean();
		std::cout << "Covars of Gaussian : " << i << " : " << std::endl; gmm->GetGaussianVector()[i]->PrintCovars();
		std::cout << "Sigma of Gaussian : " << i << " : " << std::sqrt(gmm->GetGaussianVector()[i]->GetCovars()) << std::endl; //gmm->GetGaussianVector()[i]->PrintCovars();
		std::cout << "Weight of Gaussian : " << i << " : " << gmm->GetGaussianVector()[i]->GetWeight() << std::endl;
	}
	std::cout<<"-----------------------------------------------------"<<std::endl;
	std::cout << "Prob : "<< gmm->GetGaussianVector()[0]->CalculateProbability(1.,2.,3.) << std::endl;

#endif
	std::cout<<"-------- Trying with some sensible data ---------------" << std::endl;
	Tomography::GMM1D *gmm2 = new Tomography::GMM1D(2);

/*	std::vector<double> meann;
	meann.push_back(2.); meann.push_back(4.);

	std::vector<double> covarss;
	covarss.push_back(3.); covarss.push_back(5.);

	std::vector<double> weightt;
	weightt.push_back(0.35); weightt.push_back(0.65);

	std::vector<double> dataVector = gmm2->Sample(100,meann,covarss,weightt);
	std::ofstream outfile1("gauss3.txt");
	for(int i = 0 ; i < dataVector.size() ; i++){
		outfile1 << dataVector[i] << std::endl;
	}
	outfile1.close();
*/

/*
	double mean1 = -0.5;
	double mean2 = -0.5;
	double mean3 = -0.5;
*/

	double mean1 = -1.;
	double mean2 = 7.;
//	double mean3 = 1.;

	double covars1 = 0.5; //0.25;
	double covars2 = 1.0; //1.2;
//	double covars3 = 1.5; //2.25;

	gmm2->GetGaussianVector()[0]->SetMean(mean1);
	gmm2->GetGaussianVector()[1]->SetMean(mean2);
//	gmm2->GetGaussianVector()[2]->SetMean(mean3);


	gmm2->GetGaussianVector()[0]->SetCovars(covars1);
	gmm2->GetGaussianVector()[1]->SetCovars(covars2);
//	gmm2->GetGaussianVector()[2]->SetCovars(covars3);

	//Reading data and filling std::vector
	std::vector<double> dataVector;
	std::ifstream infile("gauss.txt");
	int count = 55000;
	double val = 0.;
	while(count){
		infile >> val;
		dataVector.push_back(val);
		count--;
	}


/*
	//printing just for testing
	for(int i = 0 ; i < dataVector.size() ; i++){
	//for(int i = 0 ; i < 700 ; i++){
		std::cout << dataVector[i] << std::endl;
	5
*/

	//Giving data vector to GMM
	gmm2->SetData(dataVector);


	Print(gmm2);
	for(int i = 0 ; i < 250 ; i++){
		std::cout << "-------------- Iteration no : "<< (i+1) <<" -------------------" << std::endl;
	gmm2->Start();


	std::cout<<"GMM Processing Done.............." << std::endl;
	Print(gmm2);
	//std::cout << "Sigma of Gaussian : " << i << " : " << std::sqrt(gmm2->GetGaussianVector()[i]->GetCovars()) << std::endl;
	}

	std::cout << "====================================================================" << std::endl;
 	//gmm2->PrintPointVector();

//	std::cout << "Probabilty w.r.t gaussian 0 : "<< gmm2->GetGaussianVector()[0]->CalculateProbability(2.7) << std::endl;
//	std::cout << "Probabilty w.r.t gaussian 1 : "<< gmm2->GetGaussianVector()[1]->CalculateProbability(2.7) << std::endl;
//	std::cout << "Probabilty w.r.t gaussian 2 : "<< gmm2->GetGaussianVector()[2]->CalculateProbability(2.7) << std::endl;




	return 0;
}


