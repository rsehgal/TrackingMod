/*
 * GMM.cpp
 *
 *  Created on: Feb 13, 2018
 *      Author: rsehgal
 */

#include "GMM.h"
#include <Eigen/Dense>
#include "Gaussian.h"

namespace Tomography {

GMM::GMM() {
	// TODO Auto-generated constructor stub

}

GMM::GMM(int numOfGaussians, int dimension){
	fNumOfGaussians = numOfGaussians;
	fDimension = dimension;

	//Filling the MeanVector and CovarianceMatrix
	for(int i = 0 ; i < fNumOfGaussians ; i++){
		Eigen::VectorXd mean;
		mean.resize(fDimension);
		mean = InitializeMean();
		Eigen::MatrixXd covars;
		covars.resize(fDimension,fDimension);
		covars = InitializeCovars();
		//fMeanVector.push_back(mean);
		//fCovarsVector.push_back(covars);

		//fGaussianVector.push_back(new Gaussian(mean,covars,1./fNumOfGaussians));
		//std::cout << "--------------------------------------------------------" << std::endl;
		//std::cout<<"Mean from constructor : " << std::endl << InitializeMean() << std::endl;
		//fGaussianVector.push_back(new Gaussian(InitializeMean(),InitializeCovars(),1./fNumOfGaussians));
		//(new Gaussian(mean,covars,1./fNumOfGaussians))->PrintMean();
		fGaussianVector.push_back(new Gaussian(mean,covars,1./fNumOfGaussians));
	}



}

void GMM::DoExpectation(){
	for(int i= 0 ; i < fPointVector.size() ; i++){
		for(int j = 0 ; j < fGaussianVector.size() ; j++){
			fPointVector[i].SetProbability(j,fGaussianVector[i]->CalculateProbability(fPointVector[i].x(),fPointVector[i].y(),fPointVector[i].z()));
		}
	}

}

void GMM::DoMaximization(){

}

//Function to fill MeanVector
Eigen::VectorXd GMM::InitializeMean(){
	Eigen::VectorXd mean;
	mean.resize(fDimension);
	for(int i = 0 ; i < fDimension ; i++){
		mean(i) = 0.5 ;//+ i*0.2;
	}
	//std::cout << "Mean from InitializeMean : " << mean << std::endl;
	return mean;
}

//Function to fill CovarianceMatrix
Eigen::MatrixXd GMM::InitializeCovars(){
	Eigen::MatrixXd covars;
	covars.resize(fDimension,fDimension);
	for(int i = 0 ; i < fDimension ; i++){
		for(int j = 0 ; j < fDimension ; j++){
			covars(i,j) = 0.5;
		}
	}
	return covars;
}

GMM::~GMM() {
	// TODO Auto-generated destructor stub
}

} /* namespace Tomography */
