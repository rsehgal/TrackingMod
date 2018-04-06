/*
 * GMM2D.h
 *
 *  Created on: Mar 12, 2018
 *      Author: rsehgal
 */

#ifndef GMM_INCLUDE_GMM2D_H_
#define GMM_INCLUDE_GMM2D_H_

#include "Gaussian2D.h"
#include <Eigen/Dense>
#include <iostream>


namespace Tomography {

struct X{
	Eigen::Vector2d sX;
	std::vector<double> sGaussProb;
    void SetProbability(int gaussNum, double probValue){
    	sGaussProb[gaussNum] = probValue;
    }
    std::vector<double>  GetProbabilityVector()const {return sGaussProb;}
};


class GMM2D {
	unsigned int fNumOfGaussians;
	std::vector<X> fPointVector;
	std::vector<Gaussian2D*> fGaussianVector;

public:
	GMM2D(){}
	GMM2D(Eigen::Vector2d mean, Eigen::Matrix2d covars,int numofGaussians): fNumOfGaussians(numofGaussians){
		for(int i = 0 ; i < fNumOfGaussians ; i++)
			fGaussianVector.push_back(new Gaussian2D(mean,covars,1./fNumOfGaussians));
	}
	int GetNumOfGaussians() const {return fNumOfGaussians;}
	std::vector<Gaussian2D*> GetGaussianVector() const {return fGaussianVector;}

	void SetData(std::vector<Eigen::Vector2d> data){
		for(int i = 0 ; i < data.size() ; i++){
			X x;
			x.sX = data[i];
			for(int j = 0 ; j < fNumOfGaussians ; j++)
				x.sGaussProb.push_back(0.);
			fPointVector.push_back(x);
		}
	}

	void ApplyFilteration(){
		std::ofstream  outfile("filtered.txt");
		for(int i = 0 ; i < fPointVector.size() ; i++){
			std::cout << "Point : " << fPointVector[i].sX.transpose() << " : Probs : ";
			double totalProb = 0.;
			for(int j = 0 ; j < fNumOfGaussians ; j++){
				totalProb += fPointVector[i].sGaussProb[j];
				std::cout << fPointVector[i].sGaussProb[j] << " : ";
			}

			//if(fPointVector[i].sGaussProb[0] > 0.30)
			if(0)
				continue;
			else{
				//Getting the index of maximum probability gaussian.
				//if(std::fabs(fPointVector[i].sGaussProb[1]-fPointVector[i].sGaussProb[0]) > 0.2){
				int maxElementIndex = std::max_element(fPointVector[i].sGaussProb.begin(),fPointVector[i].sGaussProb.end()) - fPointVector[i].sGaussProb.begin();
				outfile << fPointVector[i].sX(0) << " " << fPointVector[i].sX(1) << " " << (maxElementIndex+2) << std::endl;
/*
				for(int col = 0 ; col < fPointVector[i].sGaussProb.size() ; col++){
					outfile << fPointVector[i].sX(0) << " " << fPointVector[i].sX(1) << " " << (fPointVector[i].sGaussProb[col]*10) << std::endl;
				//outfile << fPointVector[i].sX(0) << " " << fPointVector[i].sX(1) << " " << (maxElementIndex+2) << std::endl;
				}
*/
				//}
			}

			std::cout<<"Total : "<< totalProb << std::endl;
		}
		outfile.close();
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
			for(int j = 0 ; j < fGaussianVector.size() ; j++){
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
		std::vector<Eigen::Vector2d> sumProbX;
		std::vector<Eigen::Matrix2d> sumProbDev2;
		for(int i = 0 ; i < fNumOfGaussians ; i++){
			sum.push_back(0.);
			Eigen::Vector2d zero ; zero(0) = 0.; zero(1) = 0.;
			sumProbX.push_back(zero);

			Eigen::Matrix2d zeromatrix;
			zeromatrix(0,0) = 0.;
			zeromatrix(0,1) = 0.;
			zeromatrix(1,0) = 0.;
			zeromatrix(1,1) = 0.;

			sumProbDev2.push_back(zeromatrix);
		}

		for(int i = 0 ; i < fPointVector.size() ; i++){
			std::vector<double> probVect = fPointVector[i].GetProbabilityVector();
			for(int j = 0 ; j < fNumOfGaussians ; j++){
				sum[j] += probVect[j];

				Eigen::Vector2d probX = (probVect[j] * fPointVector[i].sX);
				sumProbX[j] += probX;


				Eigen::Matrix2d dev = (fPointVector[i].sX-fGaussianVector[j]->GetMean())*((fPointVector[i].sX-fGaussianVector[j]->GetMean()).transpose());
				Eigen::Matrix2d probDev = probVect[j] * dev;
				sumProbDev2[j] += probDev;

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


	virtual ~GMM2D(){}
};

} /* namespace Tomography */

#endif /* GMM_INCLUDE_GMM2D_H_ */
