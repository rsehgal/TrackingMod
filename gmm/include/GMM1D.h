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

	}


	virtual ~GMM1D(){}
};

} /* namespace Tomography */

#endif /* GMM_INCLUDE_GMM1D_H_ */
