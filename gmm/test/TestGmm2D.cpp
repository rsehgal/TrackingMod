/*
 * TestGmm2D.cpp
 *
 *  Created on: Mar 12, 2018
 *      Author: rsehgal
 */

#include <Eigen/Dense>
#include <iostream>
#include "Gaussian2D.h"
#include "GMM2D.h"

void Print(Tomography::GMM2D *gmm){
	std::cout << "---------------------------------------------" << std::endl;
	for(int i = 0 ; i < gmm->GetNumOfGaussians() ; i++){
		std::cout<<"============ Gaussian No. : " << (i+1) << "=====================" << std::endl;
		gmm->GetGaussianVector()[i]->PrintMean();
		gmm->GetGaussianVector()[i]->PrintCovars();
		gmm->GetGaussianVector()[i]->PrintWeight();
		//std::cout << "Sigma of Gaussian : " << i << " : " << std::sqrt(gmm->GetGaussianVector()[i]->GetCovars()) << std::endl;
	}

}

int main(){

	Eigen::VectorXd mean;//
	mean.resize(2);
	mean(0) = 6.;
	mean(1) = 8.;
	Eigen::MatrixXd covars;
	covars.resize(2,2);
	covars(0,0) = 1.;
	covars(0,1) = 0.5;
	covars(1,0) = 0.5;
	covars(1,1) = 1.;

	//	gmm2->GetGaussianVector()[0]->SetMean(mean);
	//	gmm2->GetGaussianVector()[0]->SetCovars(covars);
	//Tomography::GMMND *gmm2 = new Tomography::GMMND(1,mean,covars);
	Tomography::Gaussian2D *gauss2 = new Tomography::Gaussian2D(mean,covars,0.7);
	std::cout << "Mean : " << std::endl << gauss2->GetMean() << std::endl;
	std::cout << "Covars : " << std::endl << gauss2->GetCovars() << std::endl;

	Eigen::Vector2d data;
	data(0) = 12.;
	data(1) = 10.;
	std::cout <<"Prob : " << gauss2->CalculateProbability(data) << std::endl;

	std::cout<<"*******************************************************************" << std::endl;

	Tomography::GMM2D *gmm2 = new Tomography::GMM2D(mean,covars,4);
//	std::cout << "Mean : " << std::endl << gmm2->GetGaussianVector()[0]->GetMean() << std::endl;
//	std::cout << "Covars : " << std::endl << gmm2->GetGaussianVector()[0]->GetCovars() << std::endl;


/*	mean(0) = 0;
	mean(1) = 2;
	covars(0,0) = 400.;
	covars(0,1) = 0.;
	covars(1,0) = 0.;
	covars(1,1) = 400.;*/

	mean(0) = 3;
	mean(1) = 2;
	covars(0,0) = -250.;
	covars(0,1) = 0.;
	covars(1,0) = 0.;
	covars(1,1) = 250.;

	gmm2->GetGaussianVector()[0]->SetMean(mean);
	gmm2->GetGaussianVector()[0]->SetCovars(covars);


	/*mean(0) = -100.;
	mean(1) = -200.;*/
	mean(0) = -10.;
		mean(1) = -2.;
	gmm2->GetGaussianVector()[1]->SetMean(mean);
	gmm2->GetGaussianVector()[1]->SetCovars(covars);

	/*mean(0) = 100.;
		mean(1) = -200.;*/
	mean(0) = -100.;
			mean(1) = -80.;
		gmm2->GetGaussianVector()[2]->SetMean(mean);
		gmm2->GetGaussianVector()[2]->SetCovars(covars);

		/*mean(0) = 10.;
				mean(1) = 40.;*/
		mean(0) = 3.;
						mean(1) = 4.;
				gmm2->GetGaussianVector()[3]->SetMean(mean);
				gmm2->GetGaussianVector()[3]->SetCovars(covars);


		/*		mean(0) = -10.;
							mean(1) = -40.;
							gmm2->GetGaussianVector()[3]->SetMean(mean);
							gmm2->GetGaussianVector()[3]->SetCovars(covars);
*/
/*
	mean(0) = 100.;
	mean(1) = -200.;
	gmm2->GetGaussianVector()[2]->SetMean(mean);
	gmm2->GetGaussianVector()[2]->SetCovars(covars);
*/

/*
	mean(0) = -100.;
	mean(1) = 200.;
	gmm2->GetGaussianVector()[3]->SetMean(mean);
	gmm2->GetGaussianVector()[3]->SetCovars(covars);

	mean(0) = 100.;
	mean(1) = 200.;
	gmm2->GetGaussianVector()[4]->SetMean(mean);
	gmm2->GetGaussianVector()[4]->SetCovars(covars);

	mean(0) = 50.;
	mean(1) = 50.;
	gmm2->GetGaussianVector()[5]->SetMean(mean);
	gmm2->GetGaussianVector()[5]->SetCovars(covars);
*/


/*
	mean(0) = 5.0;
	mean(1) = 6.9;
	covars(0,0) = 5.5;
	covars(0,1) = 5.5;
	covars(1,0) = 6.2;
	covars(1,1) = 6.5;

	gmm2->GetGaussianVector()[1]->SetMean(mean);
	gmm2->GetGaussianVector()[1]->SetCovars(covars);
*/



	std::cout<<"----------- STARTED WITH --------------" << std::endl;
	Print(gmm2);

	//Reading data and filling std::vector
	std::vector<Eigen::Vector2d> dataVector;
	//std::ifstream infile("gauss2D.txt");
	std::ifstream infile("PocaPt.txt");
	//int count = 6149;
	double valX = 0., valY = 0., valZ = 0., color = 0.;
	//while(count){
	while(!infile.eof()){
		infile >> valX >> valY >> valZ >> color;
		Eigen::Vector2d val;
		val << valX, valY;
		dataVector.push_back(val);
		//count--;
	}

	gmm2->SetData(dataVector);
	for(int i = 0 ; i < 100 ; i++){
		std::cout << "-------------- Iteration no : "<< (i+1) <<" -------------------" << std::endl;
		gmm2->Start();
		std::cout<<"GMM Processing Done.............." << std::endl;
		Print(gmm2);
	}

	std::cout << "====================================================================" << std::endl;

/*
	std::cout << "----- Probability calculation using generated model ----------------" << std::endl;
	data(0) = 10.;
	data(1) = 0.;
	int numOfGaussians = gmm2->GetNumOfGaussians();
	for(int i = 0 ;  i < numOfGaussians ; i++){
		std::cout <<"Prob w.r.t gaussian : " << (i+1) << " : " << gmm2->GetGaussianVector()[i]->CalculateProbability(data) << std::endl;
	}
	std::cout<<"----- Trying another point that lie in one of the guassians------" <<std::endl;
	data(0) = -2.;
	data(1) = 1.;
	for(int i = 0 ;  i < numOfGaussians ; i++){
		std::cout <<"Prob w.r.t gaussian : " << (i+1) << " : " << gmm2->GetGaussianVector()[i]->CalculateProbability(data) << std::endl;
	}

	std::cout<<"----- Trying another point that lie in one of the guassians------" <<std::endl;
		data(0) = 0.;
		data(1) = 0.;
		for(int i = 0 ;  i < numOfGaussians ; i++){
			std::cout <<"Prob w.r.t gaussian : " << (i+1) << " : " << gmm2->GetGaussianVector()[i]->CalculateProbability(data) << std::endl;
		}

	std::cout<<"----- Trying another point that lie in one of the guassians------" <<std::endl;
		data(0) = -5.76434;
		data(1) = 10.7713;
		for(int i = 0 ;  i < numOfGaussians ; i++){
			std::cout <<"Prob w.r.t gaussian : " << (i+1) << " : " << gmm2->GetGaussianVector()[i]->CalculateProbability(data) << std::endl;
		}
*/

	gmm2->ApplyFilteration();



	return 0;
}


