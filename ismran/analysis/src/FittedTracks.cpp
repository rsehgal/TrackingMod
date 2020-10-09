/*
 * FittedTracks.cpp
 *
 *  Created on: 09-Oct-2020
 *      Author: rsehgal
 */

#include "FittedTracks.h"
#include "HelperFunctions.h"
#include "PsBar.h"

FittedTracks::FittedTracks() {
	// TODO Auto-generated constructor stub

}

FittedTracks::~FittedTracks() {
	// TODO Auto-generated destructor stub
}

FittedTracks::FittedTracks(std::vector<Double_t> xvec, std::vector<Double_t> yvec, std::vector<Double_t> zvec) : xVec(xvec),yVec(yvec),zVec(zvec){
	for(unsigned int i = 0 ; i < xVec.size() ; i++){
		xVecErr[i] = 0.;
		yVecErr[i] = 0.;
		zVecErr[i] = 0.;
	}
	CreateFittedTrack();

}

FittedTracks::FittedTracks(std::vector<Double_t> xvec, std::vector<Double_t> yvec, std::vector<Double_t> zvec,
			     std::vector<Double_t> xvecerror, std::vector<Double_t> yvecerror, std::vector<Double_t> zvecerror)
				 : xVec(xvec),yVec(yvec),zVec(zvec), xVecErr(xvecerror), yVecErr(yvecerror), zVecErr(zvecerror){
	CreateFittedTrack();
}

void FittedTracks::CreateFittedTrack(){
	TGraphErrors *grxy = new TGraphErrors(xVec.size(), &xVec[0], &yVec[0],&xVecErr[0],&yVecErr[0]);
	std::vector<double> fittedX = GetFittedXorZ(grxy);
	TGraphErrors *grzy = new TGraphErrors(zVec.size(), &zVec[0], &yVec[0], &zVecErr[0], &yVecErr[0]);
	std::vector<double> fittedZ = GetFittedXorZ(grzy);
	for (unsigned int i = 0; i < xVec.size(); i++) {
		fittedMuonTrack.push_back(new Point3D(fittedX[i],yVec[i],fittedZ[i]));
	}
}

std::vector<double> FittedTracks::GetFittedXorZ(TGraphErrors *gr){
	TF1 *formula = new TF1("Formula",LinearFit,-45,45,2);
	gr->Fit(formula,"qn");
	double c = formula->GetParameter(0);
	double m = formula->GetParameter(1);
	delete formula;

	std::vector<double> vecOfEstimatedXorZ;
	for(unsigned int i = 0 ; i < xVec.size() ; i++){
		vecOfEstimatedXorZ.push_back( (yVec[i] - c)/m );
	}
	return vecOfEstimatedXorZ;
}

std::vector<Point3D*> FittedTracks::GetFittedTrack() const{
	return fittedMuonTrack;
}
