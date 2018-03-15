/*
 * Gaussian3D.h
 *
 *  Created on: Mar 15, 2018
 *      Author: rsehgal
 */

#ifndef GMM_INCLUDE_GAUSSIAN3D_H_
#define GMM_INCLUDE_GAUSSIAN3D_H_

#include <cmath>
#include <iostream>
#include <fstream>
#include "TRandom3.h"
#include <cstdlib>
#include <Eigen/Dense>

namespace Tomography {

class Gaussian3D {
private:
	Eigen::Vector3d fMean;
	Eigen::Matrix3d fCovars;
	double fWeight;

public:
	Gaussian3D(){}
	Gaussian3D(Eigen::Vector3d mean, Eigen::Matrix3d covars):fMean(mean), fCovars(covars){}
	Gaussian3D(Eigen::Vector3d mean, Eigen::Matrix3d covars,double weight):Gaussian3D(mean,covars){
		fWeight = weight;
	}
	Gaussian3D(double weight){
		fWeight = weight;
	}
	void SetMean(Eigen::Vector3d mean) {fMean = mean;}
	void SetCovars(Eigen::Matrix3d covars) {fCovars = covars;}
	void SetWeight(double weight) {fWeight = weight;}
	Eigen::Vector3d GetMean() const {return fMean;}
	Eigen::Matrix3d GetCovars() const {return fCovars;}
	double GetWeight() const {return fWeight;}
	void PrintMean() const {std::cout << "Mean : "<< std::endl <<fMean << std::endl;}
	void PrintCovars() const {std::cout << "Covars : " << std::endl << fCovars << std::endl;}
	void PrintWeight() const {std::cout << "Weight : " << fWeight << std::endl;}
	void UpdateWeight(double weight){fWeight = weight;}
	void UpdateMean(Eigen::Vector3d mean) {fMean = mean;}
	void UpdateCovars(Eigen::Matrix3d covars) {fCovars = covars;}

	double CalculateProbability(Eigen::Vector3d x){
		double deno = 2*M_PI*std::sqrt(std::fabs(fCovars.determinant()));
		Eigen::Vector3d devMean = (x-fMean);
		double pw = -0.5 * devMean.transpose() * fCovars.inverse() * devMean ;
		//std::cout <<"Pw : " << pw << std::endl;

		double numer = std::exp(pw);
		//std::cout << "Numer : " << numer << std::endl;

		double prob = numer/deno;
		return prob;


	}

	virtual ~Gaussian3D(){}
};

} /* namespace Tomography */

#endif /* GMM_INCLUDE_GAUSSIAN3D_H_ */
