/*
 * TestGmmND.cpp
 *
 *  Created on: Mar 12, 2018
 *      Author: rsehgal
 */

#include "GMMND.h"
#include <iostream>
#include <Eigen/Dense>
#include <fstream>

int main(){
	std::cout<<"-------- Trying with some sensible data ---------------" << std::endl;

//	Tomography::GMMND *gmm2 = new Tomography::GMMND(1);

	Eigen::VectorXd mean;//
	mean.resize(2);
	mean(0) = 12;
	mean(1) = 10.;
	Eigen::MatrixXd covars;
	covars.resize(2,2);
	covars(0,0) = 0.5;
	covars(0,1) = 0.5;
	covars(1,0) = 0.5;
	covars(1,1) = 0.5;

//	gmm2->GetGaussianVector()[0]->SetMean(mean);
//	gmm2->GetGaussianVector()[0]->SetCovars(covars);
	Tomography::GMMND *gmm2 = new Tomography::GMMND(1,mean,covars);
	std::cout << "Mean : " << std::endl << gmm2->GetGaussianVector()[0]->GetMean() << std::endl;
	std::cout << "Covars : " << std::endl << gmm2->GetGaussianVector()[0]->GetCovars() << std::endl;
/*

	//Reading data and filling std::vector
	std::vector<Eigen::VectorXd> dataVector;
	std::ifstream infile("gauss2D.txt");
	int count = 20;
	double valX = 0., valY = 0.;
	while(count){
		infile >> valX >> valY;
		Eigen::VectorXd val;
		val.resize(2);
		val << valX, valY;
		dataVector.push_back(val);
		count--;
	}

	gmm2->SetData(dataVector);
	for(int i = 0 ; i < 5 ; i++){
		std::cout << "-------------- Iteration no : "<< (i+1) <<" -------------------" << std::endl;
		gmm2->Start();
		std::cout<<"GMM Processing Done.............." << std::endl;
	}

	std::cout << "====================================================================" << std::endl;

*/



}



