/*
 * Fit2DLinear.cpp
 *
 *  Created on: Mar 5, 2018
 *      Author: rsehgal
 */

#include "Fit2DLinear.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include "TGraph.h"
#include "TF1.h"

namespace Tomography {

Fit2DLinear::Fit2DLinear() {
	// TODO Auto-generated constructor stub
	fA = 0.;
	fB = 0.;
	fC = 0.;
	FillParams();

}

void Fit2DLinear::FillParams(){
	fParams.clear();
	fParams.push_back(fA);
	fParams.push_back(fB);
	fParams.push_back(fC);
}

Fit2DLinear::Fit2DLinear(double a, double b, double c){
	fA = a;
	fB = b;
	fC = c;
	FillParams();
}

std::vector<Tracking::Vector3D<double>> Fit2DLinear::EstimatePreFitter(std::vector<Tracking::Vector3D<double>> hitPointVect){
	std::vector<Tracking::Vector3D<double>> newHitsVector;

	//3-2 -> 1
	Tracking::Vector3D<double> dir = (hitPointVect[1]-hitPointVect[2]);
	double dist = (hitPointVect[0].z()-hitPointVect[2].z())/dir.z();
	Tracking::Vector3D<double> newPt = hitPointVect[2]+dir*dist;
	newHitsVector.push_back(newPt);

	//1-3 -> 2
	dir = (hitPointVect[2]-hitPointVect[0]);
	dist = (hitPointVect[1].z()-hitPointVect[0].z())/dir.z();
	newPt = hitPointVect[0]+dir*dist;
	newHitsVector.push_back(newPt);

	//1-2 --> 3
	dir = (hitPointVect[1]-hitPointVect[0]);
	dist = (hitPointVect[2].z()-hitPointVect[0].z())/dir.z();
	newPt = hitPointVect[0]+dir*dist;
	newHitsVector.push_back(newPt);


	return newHitsVector;
}

void Fit2DLinear::Fit(std::vector<double> vectX, std::vector<double> vectY){
#ifdef USE_ROOT_FITTING
	FitROOT(vectX, vectY);
#else
	double vectSizeInv = 1./vectX.size();
	double x = SumX(vectX) * vectSizeInv;
	double y = SumY(vectY) * vectSizeInv;
	double xy = SumXY(vectX, vectY) * vectSizeInv;;
	double x2 = SumX2(vectX) * vectSizeInv;;
	double y2 = SumY2(vectY) * vectSizeInv;;
	//std::cout << x <<" " << y << " " << xy << " " << x2 << " " << y2 << std::endl;
	fA = -xy + x*y ;
	double bX = x2 - x*x;
	double bY = y2 - y*y;
	if(std::fabs(bX) < std::fabs(bY) ){
	    fB = bY;
	    std::swap(fA,fB);
	}else{
	    fB = bX;
	}
	fC = -(fA*x + fB*y);
	FillParams();
#endif

}

#ifdef USE_ROOT_FITTING
void Fit2DLinear::FitROOT(std::vector<double> vectX, std::vector<double> vectY){
	 TGraph *gr = new TGraph(vectX.size(), &vectX[0],&vectY[0]);
	 TF1 *fit = new TF1("ffit1", "pol1", -500, 500);
//#define ROBUST_FIT
#ifdef ROBOUST_FIT
	 gr->Fit(fit, "+rob=0.95");
#else
	 gr->Fit(fit);
#endif
	 fIntercept = fit->GetParameter(0);
	 fSlope = fit->GetParameter(1);
	 std::cout << "Slope : " << fSlope <<" : Intercept : " << fIntercept << std::endl;
}
#endif

double Fit2DLinear::GetSum(std::vector<double> vect){

	double sum = 0.;
	for(unsigned int i = 0 ; i < vect.size() ; i++){
		sum += vect[i];
	}
	return sum;
}

double Fit2DLinear::SumXY(std::vector<double> vectX, std::vector<double> vectY){
	double sum = 0.;
	for(unsigned int i = 0 ; i < vectX.size() ; i++){
		sum += vectX[i] * vectY[i];
	}
	return sum;
}

void Fit2DLinear::PrintFitModel(){
	std::cout << "\n Linear Fit is of form : " << fA <<"x + "<<fB<<"y + "<<fC << std::endl;
	std::cout << "\n Linear Fit in Slopef form : " << (-fA/fB) <<"x + "<<(-fC/fB) << std::endl;
}

double Fit2DLinear::Residual(double x, double y){
	double fittedX = (-fC - fB*y)/fA;
	double residual = (fittedX - x);
	std::cout << "FittedValue : "<< fittedX << " : X : " << x << " : Residual : " << residual <<" : Y : " << y << std::endl;
	return residual;
}

void Fit2DLinear::CalculateResiduals(std::vector<Tracking::Vector3D<double>> hitPointVect){
	std::vector<double> xVect, yVect, zVect;
	for(int i = 0 ; i < hitPointVect.size() ; i++){
		xVect.push_back(hitPointVect[i].x());
		yVect.push_back(hitPointVect[i].y());
		zVect.push_back(hitPointVect[i].z());
	}
	WriteXZ(xVect,zVect);
	Fit(xVect,zVect);
	std::cout<<"\n------------------ Residuals X Plane ----------------" << std::endl;
	PrintFitModel();
	for(int i = 0 ; i < hitPointVect.size() ; i++){
		fResidualXVect.push_back(Residual(hitPointVect[i].x(),hitPointVect[i].z()));
	}
	Fit(yVect,zVect);
	std::cout<<"\n------------------ Residuals Y Plane ----------------" << std::endl;
	PrintFitModel();
	for(int i = 0 ; i < hitPointVect.size() ; i++){
		fResidualYVect.push_back(Residual(hitPointVect[i].y(),hitPointVect[i].z()));
	}
}

void Fit2DLinear::WriteAB(std::vector<double> x, std::vector<double> z,std::string filename){
	std::ofstream outfile(filename);
	for(int i = 0 ; i < x.size() ; i++){
		outfile << x[i] << " " << z[i] << std::endl;
	}
	outfile.close();

}

void Fit2DLinear::PrintResiduals(){
	for(int i = 0 ; i < fResidualXVect.size() ; i++){
		std::cout<<"\n XResidual : " << fResidualXVect[i] << " : YResidual : " << fResidualYVect[i] << std::endl;
	}
}

std::vector<Tracking::Vector3D<double>> Fit2DLinear::GetFittedTrack(std::vector<Tracking::Vector3D<double>> hitPointVect){
	std::vector<double> xVect, yVect, zVect;
	for(int i = 0 ; i < hitPointVect.size() ; i++){
		xVect.push_back(hitPointVect[i].x());
		yVect.push_back(hitPointVect[i].y());
		zVect.push_back(hitPointVect[i].z());
	}

	Fit(xVect,zVect);
	std::vector<double> xValueFitted = GetFittedValue(zVect);

	Fit(yVect,zVect);
	std::vector<double> yValueFitted = GetFittedValue(zVect);

	std::vector<Tracking::Vector3D<double>> fittedHitValueVector;
	for(int i = 0 ; i < hitPointVect.size() ; i++){
		Tracking::Vector3D<double> temp(xValueFitted[i], yValueFitted[i], zVect[i]);
		//fittedHitValueVector.push_back(Tracking::Vector3D<double>(xValueFitted[i], yValueFitted[i], zVect[i] ));
		if(isinf(xValueFitted[i]) || isinf(yValueFitted[i])){
			temp.SetColor(-100000);
		}
		fittedHitValueVector.push_back(temp);
	}
	return fittedHitValueVector;

}


std::vector<double> Fit2DLinear::GetFittedValue(std::vector<double> zVect){
	std::vector<double> fitValueVector;
	fitValueVector.clear();
	for(int i = 0 ; i < zVect.size() ; i++){
#ifdef USE_ROOT_FITTING
		fitValueVector.push_back((zVect[i]-fIntercept)/fSlope);
#else
		fitValueVector.push_back((-fC - fB*zVect[i])/fA);
#endif
	}
	return fitValueVector;
}

Fit2DLinear::~Fit2DLinear() {
	// TODO Auto-generated destructor stub
}

} /* namespace Tomography */
