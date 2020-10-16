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
	delete grxy;
	delete grzy;
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
	//std::cout <<"================= Trying to create the Fitted Track from Class FittedTracks ==================== : " << __FILE__ << " : " << __LINE__ << std::endl;
	grxy = new TGraphErrors(xVec.size(), &xVec[0], &yVec[0],&xVecErr[0],&yVecErr[0]);
	//std::cout << "++++++++++++++ Fit XY ++++++++++++++++" << std::endl;
	std::vector<double> fittedX = GetFittedXorZ(grxy);
	grzy = new TGraphErrors(zVec.size(), &zVec[0], &yVec[0], &zVecErr[0], &yVecErr[0]);
	//std::cout << "++++++++++++++ Fit ZY ++++++++++++++++" << std::endl;
	std::vector<double> fittedZ = GetFittedXorZ(grzy);
	for (unsigned int i = 0; i < xVec.size(); i++) {
		/*Point3D *pt = new Point3D(fittedX[i],yVec[i],fittedZ[i]);
		std::cout << "From CreateFitted Track : " << __FILE__ <<" : " << __LINE__ ; pt->Print();
		fittedMuonTrack.push_back(pt);*/
		fittedMuonTrack.push_back(new Point3D(fittedX[i],yVec[i],fittedZ[i]));

	}
}

std::vector<double> FittedTracks::GetFittedXorZ(TGraphErrors *gr){
	TF1 *formula = new TF1("Formula",LinearFit,-45,45,2);
	gr->Fit(formula,"qn");
	//gr->Fit(formula,"r");
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

void FittedTracks::PlotTrack(){
	TCanvas *can = new TCanvas();
	can->Divide(2,2);
	can->cd(1);
	DrawGrid("Muon Track in XY plane; X axis ; Y axis", 9, 9);
	grxy->Draw("p");
	can->cd(2);
	DrawGrid("Muon Track in ZY plane; X axis ; Y axis", 9, 9);
	grzy->Draw("p");


}

void FittedTracks::Print(){
	for(unsigned int  i = 0 ; i < xVec.size() ; i++){
		std::cout << "Point : (" << xVec[i] << "," << yVec[i] << "," << zVec[i] << ") :: Error : (" << xVecErr[i] << "," << yVecErr[i] << "," << zVecErr[i] << ")" << std::endl;
	}
}
