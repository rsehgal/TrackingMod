/*
 * GMM3D.h
 *
 *  Created on: Mar 15, 2018
 *      Author: rsehgal
 */

#ifndef GMM_INCLUDE_GMM3D_H_
#define GMM_INCLUDE_GMM3D_H_

#include "Gaussian3D.h"
#include <Eigen/Dense>
#include <iostream>
#include "base/Vector3D.h"

namespace Tomography {

struct X{
	Eigen::Vector3d sX;
	std::vector<double> sGaussProb;
    void SetProbability(int gaussNum, double probValue){
    	sGaussProb[gaussNum] = probValue;
    }
    std::vector<double>  GetProbabilityVector()const {return sGaussProb;}
};

Eigen::Vector3d ConvertToEigenVector3d(Tracking::Vector3D<double> point){
	Eigen::Vector3d pointEigen;
	pointEigen(0) = point.x();
	pointEigen(1) = point.y();
	pointEigen(2) = point.z();
	return pointEigen;
}

class GMM3D {
private:
	unsigned int fNumOfGaussians;
	std::vector<X> fPointVector;
	std::vector<Gaussian3D*> fGaussianVector;

public:
	GMM3D(){}
	GMM3D(Eigen::Vector3d mean, Eigen::Matrix3d covars,int numofGaussians): fNumOfGaussians(numofGaussians){
		for(int i = 0 ; i < fNumOfGaussians ; i++)
			fGaussianVector.push_back(new Gaussian3D(mean,covars,1./fNumOfGaussians));
	}
	int GetNumOfGaussians() const {return fNumOfGaussians;}
	std::vector<Gaussian3D*> GetGaussianVector() const {return fGaussianVector;}

	void SetData(std::vector<Eigen::Vector3d> data){
		for(int i = 0 ; i < data.size() ; i++){
			X x;
			x.sX = data[i];
			for(int j = 0 ; j < fNumOfGaussians ; j++)
				x.sGaussProb.push_back(0.);
			fPointVector.push_back(x);
		}
	}

	void SetData(std::vector<Tracking::Vector3D<double>> data){
		for(int i = 0 ; i < data.size() ; i++){
			X x;
			//Converting Vector3D to Eigen::Vector3d
			x.sX = ConvertToEigenVector3d(data[i]);
			for(int j = 0 ; j < fNumOfGaussians ; j++)
				x.sGaussProb.push_back(0.);
			fPointVector.push_back(x);
		}
	}

	void ApplyFilteration(){
		std::ofstream  outfile("filtered3D.txt");
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
				int maxElementIndex = std::max_element(fPointVector[i].sGaussProb.begin(),fPointVector[i].sGaussProb.end()) - fPointVector[i].sGaussProb.begin();
				outfile << fPointVector[i].sX(0) << " " << fPointVector[i].sX(1) << " " << fPointVector[i].sX(2) << " " << (maxElementIndex+2) << std::endl;
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
				double probNumer = probVector[j];
				if(probNumer == 0. && denoSum == 0.)
					fPointVector[i].SetProbability(j,0.);
				else
					fPointVector[i].SetProbability(j,probVector[j]/denoSum);
				//std::cout << "ProbNumer : " << probVector[j] <<" : DenoSum : " << denoSum << std::endl;
				//if(fPointVector[i].sGaussProb[j])
				//	std::cout<<"Prob value wrt gauss : " << (j+1) <<" : " << fPointVector[i].sGaussProb[j] << std::endl;
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
		std::vector<Eigen::Vector3d> sumProbX;
		std::vector<Eigen::Matrix3d> sumProbDev2;
		for(int i = 0 ; i < fNumOfGaussians ; i++){
			sum.push_back(0.);
			Eigen::Vector3d zero ; zero(0) = 0.; zero(1) = 0.; zero(2) = 0.;
			sumProbX.push_back(zero);

			Eigen::Matrix3d zeromatrix;
			zeromatrix(0,0) = 0.;
			zeromatrix(0,1) = 0.;
			zeromatrix(0,2) = 0.;
			zeromatrix(1,0) = 0.;
			zeromatrix(1,1) = 0.;
			zeromatrix(1,2) = 0.;
			zeromatrix(2,0) = 0.;
			zeromatrix(2,1) = 0.;
			zeromatrix(2,2) = 0.;

			sumProbDev2.push_back(zeromatrix);
		}

		for(int i = 0 ; i < fPointVector.size() ; i++){
			std::vector<double> probVect = fPointVector[i].GetProbabilityVector();
			for(int j = 0 ; j < fNumOfGaussians ; j++){
				sum[j] += probVect[j];

				Eigen::Vector3d probX = (probVect[j] * fPointVector[i].sX);
				sumProbX[j] += probX;


				Eigen::Matrix3d dev = (fPointVector[i].sX-fGaussianVector[j]->GetMean())*((fPointVector[i].sX-fGaussianVector[j]->GetMean()).transpose());
				Eigen::Matrix3d probDev = probVect[j] * dev;
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

	virtual ~GMM3D(){}
};

} /* namespace Tomography */

#endif /* GMM_INCLUDE_GMM3D_H_ */
