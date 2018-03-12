/*
 * Gaussian2D.h
 *
 *  Created on: Mar 12, 2018
 *      Author: rsehgal
 */

#ifndef GMM_INCLUDE_GAUSSIAN2D_H_
#define GMM_INCLUDE_GAUSSIAN2D_H_

#include <cmath>
#include <iostream>
#include <fstream>
#include "TRandom3.h"
#include <cstdlib>
#include <Eigen/Dense>


namespace Tomography {

class Gaussian2D {
private:
	Eigen::Vector2d fMean;
	Eigen::Matrix2d fCovars;
	double fWeight;
	//double fSigma; //basically used for sampling

public:
	Gaussian2D(){}
	Gaussian2D(Eigen::Vector2d mean, Eigen::Matrix2d covars):fMean(mean), fCovars(covars){}
	Gaussian2D(Eigen::Vector2d mean, Eigen::Matrix2d covars,double weight):Gaussian2D(mean,covars){
		fWeight = weight;
	}
	Gaussian2D(double weight){
		fWeight = weight;
	}
	void SetMean(Eigen::Vector2d mean) {fMean = mean;}
	void SetCovars(Eigen::Matrix2d covars) {fCovars = covars;}
	void SetWeight(double weight) {fWeight = weight;}
	Eigen::Vector2d GetMean() const {return fMean;}
	Eigen::Matrix2d GetCovars() const {return fCovars;}
	double GetWeight() const {return fWeight;}
	void PrintMean() const {std::cout << "Mean : "<< std::endl <<fMean << std::endl;}
	void PrintCovars() const {std::cout << "Covars : " << std::endl << fCovars << std::endl;}
	void PrintWeight() const {std::cout << "Weight : " << fWeight << std::endl;}
	void UpdateWeight(double weight){fWeight = weight;}
	void UpdateMean(Eigen::Vector2d mean) {fMean = mean;}
	void UpdateCovars(Eigen::Matrix2d covars) {fCovars = covars;}
	double CalculateProbability(Eigen::Vector2d x){
/*
		double prob = 0.;
		double deno = std::sqrt(2*M_PI*fCovars);
		double devMean = (x-fMean);
		double pw = (-0.5 * devMean*devMean) / fCovars ;// (fCovars*fCovars);
		double numer = std::exp(pw);
		prob = numer/deno;
		return prob*fWeight;
*/

		double deno = 2*M_PI*std::sqrt(std::fabs(fCovars.determinant()));
		Eigen::Vector2d devMean = (x-fMean);
		double pw = -0.5 * devMean.transpose() * fCovars.inverse() * devMean ;
		//std::cout <<"Pw : " << pw << std::endl;

		double numer = std::exp(pw);
		//std::cout << "Numer : " << numer << std::endl;

		double prob = numer/deno;
		return prob;

	}


	virtual ~Gaussian2D(){}
};

} /* namespace Tomography */

#endif /* GMM_INCLUDE_GAUSSIAN2D_H_ */
