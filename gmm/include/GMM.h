/*
 * GMM.h
 *
 *  Created on: Feb 13, 2018
 *      Author: rsehgal
 */

#ifndef GMM_INCLUDE_GMM_H_
#define GMM_INCLUDE_GMM_H_

#include <vector>
#include <Eigen/Dense>
#include "base/Vector3D.h"

namespace Tomography {

class Gaussian;

class GMM {
private:
	unsigned int fNumOfGaussians;
	unsigned int fDimension;

	std::vector<Tracking::Vector3D<double>> fPointVector;

	//Lets have a std::vector of Gaussians ie.
	std::vector<Gaussian*> fGaussianVector;


	Eigen::MatrixXd InitializeCovars();
	Eigen::VectorXd InitializeMean();

public:
	GMM();
	GMM(int numOfGaussians, int dimension);

	int GetNumOfGaussians() const {return fNumOfGaussians;}
	std::vector<Gaussian*> GetGaussianVector() const {return fGaussianVector;}

	void DoExpectation();
	void DoMaximization();
	virtual ~GMM();
};

} /* namespace Tomography */

#endif /* GMM_INCLUDE_GMM_H_ */
