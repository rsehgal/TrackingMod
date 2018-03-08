/*
 * Gaussian.cpp
 *
 *  Created on: Feb 13, 2018
 *      Author: rsehgal
 */

#include "Gaussian.h"
#include <iostream>
#include <cmath>

//#define USE_EIGEN

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
#ifndef USE_EIGEN
	fMean = mean;
	fCovars = covars;
#endif
}

#ifndef USE_EIGEN
void Gaussian::AllocateMemory2(){
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

#endif

Gaussian::Gaussian(double mean[2], double covars[2][2]){
	fDim = 2;
#ifndef USE_EIGEN
	AllocateMemory2();
	*fMean = mean[0];
	*(fMean+1) = mean[1];
	*(fCovars+0) = covars[0];
	*(fCovars+1) = covars[1];
	*(fCovars+2) = covars[2];
	*(fCovars+3) = covars[3];
#else
	//FillEigenDataType();
	fMean.resize(fDim);
	fCovars.resize(fDim,fDim);
	for(unsigned int i = 0 ; i < fDim ; i++){
	   fMean(i) = mean[i];
	   for(unsigned int j = 0 ; j < fDim ; j++){
		   fCovars(i,j) = covars[i][j];
	   }
	 }
#endif

	CalculateDeterminant();
}

Gaussian::Gaussian(double mean[2], double covars[2][2],double weight){
	Gaussian(mean,covars);
	fWeight = weight;
}

Gaussian::Gaussian(double mean[3], double covars[3][3], double weight){
	Gaussian(mean,covars);
	fWeight = weight;
}


#ifdef USE_EIGEN
Gaussian::Gaussian(Eigen::VectorXd mean, Eigen::MatrixXd covars){
	//std::cout << "Size from another constructor : " << mean.size() << std::endl;
	fDim = mean.size();
	fMean.resize(fDim);
	fMean = mean;
	//std::cout << "Mean from Another Constructor : " << std::endl << fMean << std::endl;
	fCovars.resize(fDim,fDim);
	fCovars = covars;
	std::cout << "Covariance from Another Constructor : " << std::endl << fCovars << std::endl;
}

Gaussian::Gaussian(Eigen::VectorXd  mean, Eigen::MatrixXd  covars, double weight){
	//std::cout << "Mean from Gaussian : " << std::endl << mean << std::endl;
	//std::cout << "Covariance from Gaussian : " << std::endl << covars << std::endl;
	Gaussian(mean,covars);
	fWeight = weight;
}
#endif

Gaussian::Gaussian(double mean[3], double covars[3][3]){
	fDim = 3;
#ifndef USE_EIGEN
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
#else
	//FillEigenDataType();
	fMean.resize(fDim);
	fCovars.resize(fDim,fDim);
	for(unsigned int i = 0 ; i < fDim ; i++){
	   fMean(i) = mean[i];
	   for(unsigned int j = 0 ; j < fDim ; j++){
		   fCovars(i,j) = covars[i][j];
	   }
	}
#endif

	CalculateDeterminant();

}

void Gaussian::PrintMean() const{
#ifndef USE_EIGEN
	for(int i = 0 ; i < fDim ; i++)
		std::cout << fMean[i] << " , " ;
	std::cout << std::endl;
#else
	std::cout << fMean << std::endl;
#endif
}

void Gaussian::PrintCovars() const{
#ifndef USE_EIGEN
	for(int i = 0 ; i < fDim ; i++){
		for(int j = 0 ; j < fDim ; j++){
			std::cout << fCovars[i][j] << " , " ;
		}
		std::cout << std::endl;
	}
#else
	std::cout << fCovars << std::endl;
#endif
}

double Gaussian::CalculateDeterminant(){
#ifndef USE_EIGEN

#else
	fDet = fCovars.determinant();
#endif

	return fDet;
}

#ifndef USE_EIGEN

#else
Eigen::MatrixXd Gaussian::CalculateCovarsInverse(){

	return fCovars.inverse();

}

Eigen::MatrixXd Gaussian::CalculateCovarsTranspose(){

	return fCovars.transpose();

}

Eigen::VectorXd Gaussian::CalculateVectorTranspose(Eigen::VectorXd vect){
	return vect.transpose();
}

#endif

double Gaussian::CalculateProbability(double x, double y){
	double prob = 0.;
#ifndef USE_EIGEN

#else
	Eigen::Vector2d X(x,y);
	double deno = 2*M_PI*std::sqrt(std::fabs(CalculateDeterminant()));
	Eigen::Vector2d devMean = (X-fMean);
	double pw = -0.5 * devMean.transpose() * fCovars.inverse() * devMean ;
	double numer = std::exp(pw);
	prob = numer/deno;

#endif

	return prob;
}

double Gaussian::CalculateProbability(double x, double y, double z){
	double prob = 0.;
#ifndef USE_EIGEN

#else
	Eigen::Vector3d X(x,y,z);
	double deno = 2*M_PI*std::sqrt(2*M_PI)*std::sqrt(std::fabs(CalculateDeterminant()));
	Eigen::Vector3d devMean = (X-fMean);
	double pw = -0.5 * devMean.transpose() * fCovars.inverse() * devMean ;
	double numer = std::exp(pw);
	prob = numer/deno;

#endif

	return prob;
}

Gaussian::~Gaussian() {
	// TODO Auto-generated destructor stub
#ifndef USE_EIGEN
	delete [] fMean;
	delete [] fCovars;
#endif
}

} /* namespace Tomography */
