/*
 * Gaussian.h
 *
 *  Created on: Feb 13, 2018
 *      Author: rsehgal
 */

#ifndef GMM_INCLUDE_GAUSSIAN_H_
#define GMM_INCLUDE_GAUSSIAN_H_

#include <Eigen/Dense>

//#define USE_EIGEN

namespace Tomography {

struct Vector2D{
public:
	double sX;
	double sY;
};

//class Eigen::MatrixXd;

class Gaussian {
private:
	int fDim;
	double fDet;
	double fWeight; //i.e. P(G)


#ifndef USE_EIGEN

	double *fMean;//[2];
	double **fCovars;//[2][2];

	void AllocateMemory2();
	void AllocateMemory3();
#else

	//Taking Eigen for matrix stuff
	Eigen::MatrixXd fCovars;
	Eigen::VectorXd fMean;


#endif

//	void FillEigenDataType();
	double CalculateDeterminant();


public:
	Gaussian();

	Gaussian(double *mean, double **covars);

	Gaussian(double mean[2], double covars[2][2]);
	Gaussian(double mean[2], double covars[2][2],double weigth);

	Gaussian(double mean[3], double covars[3][3]);
	Gaussian(double mean[3], double covars[3][3], double weight);

	//Constructor to directly take Eigen matrices, will be used by GMM
#ifdef USE_EIGEN
	Gaussian(Eigen::VectorXd mean, Eigen::MatrixXd covars);
	Gaussian(Eigen::VectorXd  mean, Eigen::MatrixXd  covars, double weight);
#endif

	void SetWeight(double weight){fWeight = weight;}

#ifdef USE_EIGEN
	//Updating function, required by Maximization step
	void UpdateWeight(double weight){fWeight = weight;}
	void UpdateMean(Eigen::VectorXd mean) {fMean = mean;}
	void UpdateCovars(Eigen::MatrixXd covars) {fCovars = covars;}

	void SetMean(Eigen::VectorXd mean) {fMean = mean;}
	void SetCovars(Eigen::MatrixXd covars) {fCovars = covars;}
#endif

	virtual ~Gaussian();
	Vector2D Sample();

#ifndef USE_EIGEN
	//Getters
	double* GetMean() {return fMean;}
	double** GetCovars() {return fCovars;}

#else

	Eigen::MatrixXd CalculateCovarsInverse();
	Eigen::MatrixXd CalculateCovarsTranspose();
	Eigen::VectorXd CalculateVectorTranspose(Eigen::VectorXd vect);


#endif

	//Common Getters
	double GetDeterminant() const {return fDet;}
	double GetWeight() const {return fWeight;}

	//Printers
	void PrintMean() const;
	void PrintCovars() const;

	//Code for Probability Density function, to get the probability of a
	//point belonging to a Gaussian.
	double CalculateProbability(double x, double y);
	double CalculateProbability(double x, double y,double z);
#ifdef USE_EIGEN
	double CalculateProbability(Eigen::VectorXd dataSample);
#endif

};

} /* namespace Tomography */

#endif /* GMM_INCLUDE_GAUSSIAN_H_ */
