/*
 * Gaussian1D.h
 *
 *  Created on: Mar 9, 2018
 *      Author: rsehgal
 */

#ifndef GMM_INCLUDE_GAUSSIAN1D_H_
#define GMM_INCLUDE_GAUSSIAN1D_H_

#include <cmath>
#include <iostream>

namespace Tomography {

class Gaussian1D {
private:
	double fMean;
	double fCovars;
	double fWeight;

public:
	Gaussian1D(){}
	Gaussian1D(double mean, double covars):fMean(mean), fCovars(covars){}
	Gaussian1D(double mean, double covars,double weight):Gaussian1D(mean,covars){
		 fWeight = weight;
	}
	void SetMean(double mean) {fMean = mean;}
	void SetCovars(double covars) {fCovars = covars;}
	void SetWeight(double weight) {fWeight = weight;}
	double GetMean() const {return fMean;}
	double GetCovars() const {return fCovars;}
	double GetWeight() const {return fWeight;}
	void PrintMean() const {std::cout << "Mean : "<< fMean << std::endl;}
	void PrintCovars() const {std::cout << "Covars : " << fCovars << std::endl;}
	void PrintWeight() const {std::cout << "Weight : " << fWeight << std::endl;}
	void UpdateWeight(double weight){fWeight = weight;}
	void UpdateMean(double mean) {fMean = mean;}
	void UpdateCovars(double covars) {fCovars = covars;}
	double CalculateProbability(double x){
		double prob = 0.;
		double deno = std::sqrt(2*M_PI*fCovars);
		double devMean = (x-fMean);
		double pw = (-0.5 * devMean*devMean) / fCovars ;// (fCovars*fCovars);
		double numer = std::exp(pw);
		prob = numer/deno;
		return prob*fWeight;
	}


	virtual ~Gaussian1D(){}
};

} /* namespace Tomography */

#endif /* GMM_INCLUDE_GAUSSIAN1D_H_ */
