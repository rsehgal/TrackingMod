/*
 * GMMND.h
 *
 *  Created on: Mar 12, 2018
 *      Author: rsehgal
 */

#ifndef GMM_INCLUDE_GMMND_H_
#define GMM_INCLUDE_GMMND_H_

#include "Gaussian.h"
#include <Eigen/Dense>
#include <iostream>

namespace Tomography {

struct X{
	Eigen::VectorXd sX;
	std::vector<double> sGaussProb;
    void SetProbability(int gaussNum, double probValue){
    	sGaussProb[gaussNum] = probValue;
    }
    std::vector<double>  GetProbabilityVector()const {return sGaussProb;}
};


class GMMND {
	unsigned int fNumOfGaussians;
	std::vector<X> fPointVector;
	std::vector<Gaussian*> fGaussianVector;

public:
	GMMND(){}
	GMMND(int numofGaussians): fNumOfGaussians(numofGaussians){
		for(int i = 0 ; i < fNumOfGaussians ; i++){
			Eigen::VectorXd mean;
			Eigen::MatrixXd covars;
			fGaussianVector.push_back(new Gaussian(mean,covars,1./fNumOfGaussians));
		}
	}

	GMMND(int numofGaussians,Eigen::VectorXd mean, Eigen::MatrixXd covars): fNumOfGaussians(numofGaussians){
		for(int i = 0 ; i < fNumOfGaussians ; i++)
			fGaussianVector.push_back(new Gaussian(mean,covars,1./fNumOfGaussians));
	}

	int GetNumOfGaussians() const {return fNumOfGaussians;}
	std::vector<Gaussian*> GetGaussianVector() const {return fGaussianVector;}

	void SetData(std::vector<Eigen::VectorXd> data){
		for(int i = 0 ; i < data.size() ; i++){
			X x;
			x.sX = data[i];
			for(int j = 0 ; j < fNumOfGaussians ; j++)
				x.sGaussProb.push_back(0.);
			fPointVector.push_back(x);
		}
	}

	void Start(){
		DoExpectation();
		DoMaximization();
	}

	//The return value will be used by Expectation function of GMM
	double CalculateGaussianProbability(int gaussNum, Eigen::VectorXd val){
		double prob = fGaussianVector[gaussNum]->CalculateProbability(val);
		return prob;
	}

	void DoExpectation(){
		std::cout<<"Doing Expectation............. Point Vector size : " << fPointVector.size() << std::endl;

		std::vector<double> probVector;

		for(int i= 0 ; i < fPointVector.size() ; i++){
			probVector.clear();
			double denoSum = 0.;
			for(int j = 0 ; j < fNumOfGaussians ; j++){
				probVector.push_back(CalculateGaussianProbability(j,fPointVector[i].sX));
				denoSum += probVector[j];
			}
			//std::cout << "Processing Point : " << i << std::endl;
			for(int j = 0 ; j < fGaussianVector.size() ; j++){
				//fPointVector[i].SetProbability(j,fGaussianVector[j]->CalculateProbability(fPointVector[i].sX));
				fPointVector[i].SetProbability(j,probVector[j]/denoSum);
			}
		}
	}

	void DoMaximization(){
		std::cout<<"Doing Maximization............. @@@@@@@@ " << std::endl;
		/* Here basically we have to update the following three parameters:
		 * 1) fWeight
		 * 2) fMean
		 * 3) fCovars
		 */

		std::vector<double> sum;
		std::vector<Eigen::VectorXd> sumProbX, sumProbDev2;
		for(int i = 0 ; i < fNumOfGaussians ; i++){
			int dim = fPointVector[0].sX.size();
			Eigen::VectorXd zero;
			zero.resize(2);
			for(int j = 0 ; j < dim ; j++)
				zero(j) = 0.;
			sum.push_back(0.);
			sumProbX.push_back(zero);
			sumProbDev2.push_back(zero);
		}

		for(int i = 0 ; i < fPointVector.size() ; i++){
			std::vector<double> probVect = fPointVector[i].GetProbabilityVector();
			for(int j = 0 ; j < fNumOfGaussians ; j++){
				sum[j] += probVect[j];
				sumProbX[j].resize(2);
				Eigen::VectorXd prbVect;
				prbVect.resize(2);
				prbVect(0)=probVect[j];
				prbVect(1)=probVect[j];
				//sumProbX[j] += probVect[j] * fPointVector[i].sX;
				//sumProbX[j] += prbVect * fPointVector[i].sX;
				sumProbX[j](0) += prbVect(0) * fPointVector[i].sX(0);
				sumProbX[j](1) += prbVect (1)* fPointVector[i].sX(1);
				sumProbDev2[j].resize(2);
				//sumProbDev2[j] += probVect[j] * (fPointVector[i].sX-fGaussianVector[j]->GetMean())*(fPointVector[i].sX-fGaussianVector[j]->GetMean());

				//sumProbDev2[j] += prbVect * (fPointVector[i].sX-fGaussianVector[j]->GetMean())*(fPointVector[i].sX-fGaussianVector[j]->GetMean());
				sumProbDev2[j](0) += prbVect(0) * (fPointVector[i].sX(0)-fGaussianVector[j]->GetMean()(0))*(fPointVector[i].sX(0)-fGaussianVector[j]->GetMean()(0));
				sumProbDev2[j](1) += prbVect(1) * (fPointVector[i].sX(1)-fGaussianVector[j]->GetMean()(1))*(fPointVector[i].sX(1)-fGaussianVector[j]->GetMean()(1));
			}
		}


		std::vector<double> updatedWeightVector, updatedMeanVector, updatedCovarsVector;
		for(int j = 0 ; j < fNumOfGaussians ; j++){

			//Maximization of fWeight
			//updatedWeightVector[j] = sum[j]/fPointVector.size();
			fGaussianVector[j]->UpdateWeight(sum[j]/fPointVector.size());
			std::cout << "Weights Before: " << std::endl << fGaussianVector[j]->GetWeight() << std::endl;
			std::cout<<"===============================================================================" << std::endl;
			std::cout << "UPDATED Weights : " << std::endl << fGaussianVector[j]->GetWeight() << std::endl;

			//Maximization of fMean
			//updatedMeanVector[j] = sumProbX[j]/sum[j];
			fGaussianVector[j]->UpdateMean(sumProbX[j]/sum[j]);
			std::cout << "Mean Before: " << std::endl << fGaussianVector[j]->GetMean() << std::endl;
			std::cout<<"===============================================================================" << std::endl;
			std::cout << "UPDATED Mean : " << std::endl << fGaussianVector[j]->GetMean() << std::endl;

			//Maximization of fCovars
			//updatedCovarsVector[j] = sumProbDev2[j]/sum[j];
			fGaussianVector[j]->UpdateCovars(sumProbDev2[j]/sum[j]);
			std::cout << "Covars before: " << std::endl << fGaussianVector[j]->GetCovars() << std::endl;
			std::cout<<"===============================================================================" << std::endl;
			std::cout << "UPDATED Covars : " << std::endl << fGaussianVector[j]->GetCovars() << std::endl;

		}
	}



	virtual ~GMMND(){}
};

} /* namespace Tomography */

#endif /* GMM_INCLUDE_GMMND_H_ */
