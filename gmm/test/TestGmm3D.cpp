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
	mean(0) = 0.;
	mean(1) = 0.;
	mean(2) = 0.;
	Eigen::MatrixXd covars;
	covars.resize(3,3);
	covars(0,0) = 100.;
	covars(0,1) = 0;
	covars(0,2) = 0;
	covars(1,0) = 0;
	covars(1,1) = 100.;
	covars(1,2) = 0;
	covars(2,0) = 0;
	covars(2,1) = 0.;
	covars(2,2) = 100;

	Tomography::Gaussian3D *gauss3 = new Tomography::Gaussian3D(mean,covars,0.7);
	std::cout << "Mean : " << std::endl << gauss3->GetMean() << std::endl;
	//std::cout << "Covars : " << std::endl << gauss3->GetCovars() << std::endl;

	Eigen::Vector3d data;
	data(0) = 12.;
	data(1) = 10.;
	data(1) = 5.;
	std::cout<<"Covars Determinant : " << covars.determinant() << std::endl;
	std::cout <<"Prob : " << gauss3->CalculateProbability(data) << std::endl;

	std::cout<<"*******************************************************************" << std::endl;

	Tomography::GMM3D *gmm3 = new Tomography::GMM3D(mean,covars,4);
#if(1)
	mean(0) = 0;
	mean(1) = 0;
	mean(2) = 0;

	covars(0,0) = 200.;
	covars(0,1) = 0;
	covars(0,2) = 0.;
	covars(1,0) = 0.;
	covars(1,1) = 200.;
	covars(1,2) = 0.;
	covars(2,0) = 0.;
	covars(2,1) = 0.;
	covars(2,2) = 200.;


	std::cout<<"Covars Determinant : " << covars.determinant() << std::endl;

	gmm3->GetGaussianVector()[0]->SetMean(mean);
	gmm3->GetGaussianVector()[0]->SetCovars(covars);


	mean(0) = 0;
	mean(1) = 0;
	mean(2) = 0;

	covars(0,0) = 400.;
	covars(0,1) = 0.;
	covars(0,2) = 0.;
	covars(1,0) = 0.;
	covars(1,1) = 400.;
	covars(1,2) = 0.;
	covars(2,0) = 0.;
	covars(2,1) = 0.;
	covars(2,2) = 400.;


	gmm3->GetGaussianVector()[1]->SetMean(mean);
	gmm3->GetGaussianVector()[1]->SetCovars(covars);

	covars(0,0) = -400.;
		covars(0,1) = 0.;
		covars(0,2) = 0.;
		covars(1,0) = 0.;
		covars(1,1) = -400.;
		covars(1,2) = 0.;
		covars(2,0) = 0.;
		covars(2,1) = 0.;
		covars(2,2) = -400.;

	mean(0) = 10;
	mean(1) = 20;
	mean(2) = 30;
	gmm3->GetGaussianVector()[2]->SetMean(mean);
	gmm3->GetGaussianVector()[2]->SetCovars(covars);

	covars(0,0) = -200.;
		covars(0,1) = 0.;
		covars(0,2) = 0.;
		covars(1,0) = 0.;
		covars(1,1) = -200.;
		covars(1,2) = 0.;
		covars(2,0) = 0.;
		covars(2,1) = 0.;
		covars(2,2) = -200.;

	mean(0) = 40;
	mean(1) = 20;
	mean(2) = 100;
	gmm3->GetGaussianVector()[3]->SetMean(mean);
	gmm3->GetGaussianVector()[3]->SetCovars(covars);


/*	covars(0,0) = -200.;
			covars(0,1) = 0.;
			covars(0,2) = 0.;
			covars(1,0) = 0.;
			covars(1,1) = -10.;
			covars(1,2) = 0.;
			covars(2,0) = 0.;
			covars(2,1) = 0.;
			covars(2,2) = -20.;

		mean(0) = 60;
		mean(1) = 90;
		mean(2) = 10;
		gmm3->GetGaussianVector()[4]->SetMean(mean);
		gmm3->GetGaussianVector()[4]->SetCovars(covars);*/



/*

	mean(0) = 100;
	mean(1) = 0;
	mean(2) = 0;
	gmm3->GetGaussianVector()[2]->SetMean(mean);
	gmm3->GetGaussianVector()[2]->SetCovars(covars);


	mean(0) = -100;
	mean(1) = 0;
	mean(2) = 0;
	gmm3->GetGaussianVector()[3]->SetMean(mean);
	gmm3->GetGaussianVector()[3]->SetCovars(covars);

	mean(0) = -50;
	mean(1) = 0;
	mean(2) = 50;
	gmm3->GetGaussianVector()[4]->SetMean(mean);
	gmm3->GetGaussianVector()[4]->SetCovars(covars);

*/


	std::cout<<"Covars Determinant : " << covars.determinant() << std::endl;
#endif

	//return 0;
	//Filling DataVector
	//Reading data and filling std::vector

	//std::vector<Eigen::Vector3d> dataVector;
	std::vector<Tracking::Vector3D<double>> dataVector;

	std::ifstream infile("PocaPt.txt");
	//int count = 5536;
	double valX = 0., valY = 0. , valZ = 0., color = 0.;
	//while(count){
	while(!infile.eof()){
		infile >> valX >> valY >> valZ >> color;
		//Eigen::Vector3d val;
		//val << valX, valY , valZ;
		Tracking::Vector3D<double> val(valX,valY,valZ,color);
		dataVector.push_back(val);
		//count--;
	}

	gmm3->SetData(dataVector);

	std::cout<<"----------- STARTED WITH --------------" << std::endl;
	Print(gmm3);



	for(int i = 0 ; i < 100 ; i++){
		std::cout << "-------------- Iteration no : "<< (i+1) <<" -------------------" << std::endl;
		gmm3->Start();
		std::cout<<"GMM Processing Done.............." << std::endl;
		Print(gmm3);
	}


	//Trying to apply filteration now
	gmm3->ApplyFilteration();


	return 0;

}


