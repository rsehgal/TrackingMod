/*
 * GMM1D.h
 *
 *  Created on: Mar 9, 2018
 *      Author: rsehgal
 */

#ifndef GMM_INCLUDE_GMM1D_H_
#define GMM_INCLUDE_GMM1D_H_

#include "Gaussian1D.h"
#include "vector"

namespace Tomography {

struct X{
	double sX;
	std::vector<double> sGaussProb;
    void SetProbability(int gaussNum, double probValue){
    	sGaussProb[gaussNum] = probValue;
    }
    std::vector<double>  GetProbabilityVector()const {return sGaussProb;}
};

class GMM1D {
private:
	unsigned int fNumOfGaussians;
	std::vector<X> fPointVector;
	//Lets have a std::vector of Gaussian1D ie.
	std::vector<Gaussian1D*> fGaussianVector;


public:
	GMM1D(){}
	GMM1D(int numofGaussians): fNumOfGaussians(numofGaussians){
		for(int i = 0 ; i < fNumOfGaussians ; i++)
			fGaussianVector.push_back(new Gaussian1D(0.,0.,1./fNumOfGaussians));
	}

	int GetNumOfGaussians() const {return fNumOfGaussians;}
	std::vector<Gaussian1D*> GetGaussianVector() const {return fGaussianVector;}

	void DoExpectation(){
		for(int i= 0 ; i < fPointVector.size() ; i++){
			for(int j = 0 ; j < fGaussianVector.size() ; j++){
				fPointVector[i].SetProbability(j,fGaussianVector[i]->CalculateProbability(fPointVector[i].sX));
			}
		}
	}

	void DoMaximization(){
		/* Here basically we have to update the following three parameters:
		 * 1) fWeight
		 * 2) fMean
		 * 3) fCovars
		 */

		std::vector<double> sum, sumProbX, sumProbDev2;
		for(int i = 0 ; i < fNumOfGaussians ; i++){
			sum[i] = 0.;
			sumProbX[i] = 0.;
			sumProbDev2[i] = 0.;
		}

		for(int i = 0 ; i < fPointVector.size() ; i++){
			std::vector<double> probVect = fPointVector[i].GetProbabilityVector();
			for(int j = 0 ; j < fNumOfGaussians ; j++){
				sum[j] += probVect[j];
				sumProbX[j] += probVect[j] * fPointVector[i].sX;
				sumProbDev2[j] += probVect[j] * (fPointVector[i].sX-fGaussianVector[j]->GetMean())*(fPointVector[i].sX-fGaussianVector[j]->GetMean());
			}
		}


		std::vector<double> updatedWeightVector, updatedMeanVector, updatedCovarsVector;
		for(int j = 0 ; j < fNumOfGaussians ; j++){

			//Maximization of fWeight
			//updatedWeightVector[j] = sum[j]/fPointVector.size();
			fGaussianVector[j]->UpdateWeight(sum[j]/fPointVector.size());

			//Maximization of fMean
			//updatedMeanVector[j] = sumProbX[j]/sum[j];
			fGaussianVector[j]->UpdateMean(sumProbX[j]/sum[j]);

			//Maximization of fCovars
			//updatedCovarsVector[j] = sumProbDev2[j]/sum[j];
			fGaussianVector[j]->UpdateCovars(sumProbDev2[j]/sum[j]);

		}
	}


	virtual ~GMM1D(){}
};

} /* namespace Tomography */

#endif /* GMM_INCLUDE_GMM1D_H_ */
