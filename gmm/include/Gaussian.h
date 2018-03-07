/*
 * Gaussian.h
 *
 *  Created on: Feb 13, 2018
 *      Author: rsehgal
 */

#ifndef GMM_INCLUDE_GAUSSIAN_H_
#define GMM_INCLUDE_GAUSSIAN_H_

namespace Tomography {

struct Vector2D{
public:
	double sX;
	double sY;
};

class Gaussian {
private:
	int fDim;
	double *fMean;//[2];
	double **fCovars;//[2][2];

	void AllocateMemory();
	void AllocateMemory3();

public:
	Gaussian();
	Gaussian(double *mean, double **covars);
	Gaussian(double mean[2], double covars[2][2]);
	Gaussian(double mean[3], double covars[3][3]);
	virtual ~Gaussian();
	Vector2D Sample();

	//Getters
	double* GetMean() {return fMean;}
	double** GetCovars() {return fCovars;}

	//Printers
	void PrintMean() const;
	void PrintCovars() const;

};

} /* namespace Tomography */

#endif /* GMM_INCLUDE_GAUSSIAN_H_ */
