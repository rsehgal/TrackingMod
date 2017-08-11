/*
 * CommonFunc.h
 *
 *  Created on: Aug 8, 2017
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_COMMONFUNC_H_
#define HELPERS_INC_COMMONFUNC_H_

#include <TH1F.h>
#include <TVector3.h>
#include "base/Vector3D.h"
#include "Track.h"
using Tracking::Vector3D;

namespace CommonFunc{
class Functions{
public:
static double FWHM(TH1F *histogram){
	int bin1 = histogram->FindFirstBinAbove(histogram->GetMaximum()/2);
	int bin2 = histogram->FindLastBinAbove(histogram->GetMaximum()/2);
	double fwhm = histogram->GetBinCenter(bin2) - histogram->GetBinCenter(bin1);
	return fwhm;
}


static double GetAngleInRadian(Vector3D<double>In1, Vector3D<double>In2, Vector3D<double>In3, Vector3D<double>In4){
	TVector3 tvect1(In1.x(),In1.y(),In1.z());
	TVector3 tvect2(In2.x(),In2.y(),In2.z());
	TVector3 tvect3(In3.x(),In3.y(),In3.z());
	TVector3 tvect4(In4.x(),In4.y(),In4.z());
	return (tvect2-tvect1).Angle(tvect4-tvect3);
}

static double GetAngleInRadian(Vector3D<double>InComing, Vector3D<double>Outgoing){
	TVector3 incoming(InComing.x(),InComing.y(),InComing.z());
	TVector3 outgoing(Outgoing.x(),Outgoing.y(),Outgoing.z());
	return incoming.Angle(outgoing);
}

static double GetAngleInRadian(Tomography::Track InComing, Tomography::Track Outgoing){
	return GetAngleInRadian(InComing.GetDirection(),Outgoing.GetDirection());
}


static double Mean(std::vector<double> scatteringVect){
   double sum = 0.;
   for(int i = 0 ; i < scatteringVect.size() ; i++){
     sum += scatteringVect[i];
   }
   return sum/scatteringVect.size();
}

static double StandardDeviation(std::vector<double> scatteringVect){
   double mean = Mean(scatteringVect);
   for(int i = 0 ; i < scatteringVect.size() ; i++){
   scatteringVect[i] -= mean;
   }

   for(int i = 0 ; i < scatteringVect.size() ; i++){
   scatteringVect[i] *= scatteringVect[i];
   }
   mean = Mean(scatteringVect);
   return std::sqrt(mean);
}

};

}



#endif /* HELPERS_INC_COMMONFUNC_H_ */