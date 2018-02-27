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
	double *fMean;//[2];
	double **fCovars;//[2][2];

	void AllocateMemory();

public:
	Gaussian();
	Gaussian(double *mean, double **covars);
	Gaussian(double mean[2], double covars[2][2]);
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
