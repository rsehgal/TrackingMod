/*
 * TestGmm3D.cpp
 *
 *  Created on: Mar 15, 2018
 *      Author: rsehgal
 */

#include <Eigen/Dense>
#include <iostream>
#include "Gaussian3D.h"
#include "GMM3D.h"

void Print(Tomography::GMM3D *gmm){
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
	mean.resize(3);
	mean(0) = 6.;
	mean(1) = 8.;
	mean(2) = 4.;
	Eigen::MatrixXd covars;
	covars.resize(3,3);
	covars(0,0) = 1.;
	covars(0,1) = 0.5;
	covars(0,2) = 0.5;
	covars(1,0) = 0.5;
	covars(1,1) = 2.;
	covars(1,2) = 0.5;
	covars(2,0) = 0.5;
	covars(2,1) = 1.;
	covars(2,2) = 0.5;

	Tomography::Gaussian3D *gauss3 = new Tomography::Gaussian3D(mean,covars,0.7);
	std::cout << "Mean : " << std::endl << gauss3->GetMean() << std::endl;
	//std::cout << "Covars : " << std::endl << gauss3->GetCovars() << std::endl;

	Eigen::Vector3d data;
	data(0) = 12.;
	data(1) = 10.;
	data(1) = 5.;
	//std::cout<<"Covars Determinant : " << covars.determinant() << std::endl;
	std::cout <<"Prob : " << gauss3->CalculateProbability(data) << std::endl;

	std::cout<<"*******************************************************************" << std::endl;

	Tomography::GMM3D *gmm3 = new Tomography::GMM3D(mean,covars,2);

	mean(0) = -2;
	mean(1) = -4;
	mean(2) = -1;
	covars(0,0) = 2.;
	covars(0,1) = 1.5;
	covars(0,2) = 1.5;
	covars(1,0) = 0.5;
	covars(1,1) = 2.;
	covars(1,2) = 1.5;
	covars(2,0) = 1.5;
	covars(2,1) = 1.;
	covars(2,2) = 1.8;

	std::cout<<"Covars Determinant : " << covars.determinant() << std::endl;

	gmm3->GetGaussianVector()[0]->SetMean(mean);
	gmm3->GetGaussianVector()[0]->SetCovars(covars);

	//Filling DataVector
	//Reading data and filling std::vector
	std::vector<Eigen::Vector3d> dataVector;
	std::ifstream infile("PocaPt.txt");
	int count = 3327;
	double valX = 0., valY = 0. , valZ = 0., color = 0.;
	while(count){
		infile >> valX >> valY >> valZ >> color;
		Eigen::Vector3d val;
		val << valX, valY , valZ;
		dataVector.push_back(val);
		count--;
	}

	gmm3->SetData(dataVector);

	std::cout<<"----------- STARTED WITH --------------" << std::endl;
	Print(gmm3);



	for(int i = 0 ; i < 30 ; i++){
		std::cout << "-------------- Iteration no : "<< (i+1) <<" -------------------" << std::endl;
		gmm3->Start();
		std::cout<<"GMM Processing Done.............." << std::endl;
		Print(gmm3);
	}


	//Trying to apply filteration now
	gmm3->ApplyFilteration();


	return 0;

}


