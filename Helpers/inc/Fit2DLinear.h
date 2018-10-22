/*
 * Fit2DLinear.h
 *
 *  Created on: Mar 5, 2018
 *      Author: rsehgal
 *
 *  Equation of Line is Ax + By + C = 0
 */

#ifndef HELPERS_INC_FIT2DLINEAR_H_
#define HELPERS_INC_FIT2DLINEAR_H_

#include <vector>
#include <base/Vector3D.h>
namespace Tomography {

class Fit2DLinear {
private:
	double fA;
	double fB;
	double fC;
	std::vector<double> fParams;

#ifdef USE_ROOT_FITTING
	double fSlope;
	double fIntercept;
#endif

	void FillParams();
	double SumX(std::vector<double> vect){
		return GetSum(vect);
	}
	double SumY(std::vector<double> vect){
		return GetSum(vect);
	}
	double SumX2(std::vector<double> vect){
		return SumXY(vect,vect);
	}
	double SumY2(std::vector<double> vect){
		return SumXY(vect,vect);
	}
	double SumXY(std::vector<double> vectX, std::vector<double> vectY);

	double GetSum(std::vector<double> vect);

	double Residual(double x, double y);

	void WriteAB(std::vector<double> x, std::vector<double> z,std::string filename="regData.txt" );
	void WriteXZ(std::vector<double> x, std::vector<double> z, std::string filename = "xz.txt"){
		WriteAB(x,z,filename);
	}
	void WriteYZ(std::vector<double> y, std::vector<double> z, std::string filename = "yz.txt"){
		WriteAB(y,z,filename);
	}

public:
	Fit2DLinear();
	virtual ~Fit2DLinear();
	Fit2DLinear(double a, double b, double c);

	double GetA() const {return fA;}
	double GetB() const {return fB;}
	double GetC() const {return fC;}
	std::vector<double> GetParameters() const { return fParams;}
	void PrintFitModel();

	void Fit(std::vector<double> vectX, std::vector<double> vectY);
	void FitROOT(std::vector<double> vectX, std::vector<double> vectY);


	//-------------- Specific to our Application --------------------
private:
	std::vector<double> fResidualXVect;
	std::vector<double> fResidualYVect;


public:
	std::vector<double> GetResidualInX() const {return fResidualXVect;}
	std::vector<double> GetResidualInY() const {return fResidualYVect;}

	void CalculateResiduals(std::vector<Tracking::Vector3D<double>> hitPointVect);
	void PrintResiduals();

	//Trying to get fitted track
	std::vector<Tracking::Vector3D<double>> GetFittedTrack(std::vector<Tracking::Vector3D<double>> hitPointVect);
	std::vector<double> GetFittedValue(std::vector<double> zVect);

	std::vector<Tracking::Vector3D<double>> EstimatePreFitter(std::vector<Tracking::Vector3D<double>> hitPointVect);


};

} /* namespace Tomography */

#endif /* HELPERS_INC_FIT2DLINEAR_H_ */
