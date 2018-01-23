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
#include <fstream>
#include "Voxel.h"
#include "Voxelator_Evolution.h"
#include <G4ThreeVector.hh>
#include <algorithm>
#include <TF1.h>
#include "base/Global.h"
using Tracking::Vector3D;

namespace CommonFunc{

static Precision Distance(Vector3D<Precision> p, Vector3D<Precision> q){
	return (p-q).Mag();
}

class Functions{
	static Functions *finstance;
	Functions(){}
public:


static Functions *instance(){
	if(!finstance){
		finstance = new Functions;
	}
	return finstance;
}

double FWHM(TH1F *histogram){
	int bin1 = histogram->FindFirstBinAbove(histogram->GetMaximum()/2);
	int bin2 = histogram->FindLastBinAbove(histogram->GetMaximum()/2);
	double fwhm = histogram->GetBinCenter(bin2) - histogram->GetBinCenter(bin1);
	return fwhm;
}

/*static double GetAngleInRadian(G4ThreeVector incoming, G4ThreeVector outgoing){
   TVector3 TIncoming(incoming.x(),incoming.y(),incoming.z());
   TVector3 TOutgoing(outgoing.x(),outgoing.y(),outgoing.z());
   return TIncoming.Angle(TOutgoing);
}*/

double GetAngleInRadian(Vector3D<double>In1, Vector3D<double>In2, Vector3D<double>In3, Vector3D<double>In4){
	TVector3 tvect1(In1.x(),In1.y(),In1.z());
	TVector3 tvect2(In2.x(),In2.y(),In2.z());
	TVector3 tvect3(In3.x(),In3.y(),In3.z());
	TVector3 tvect4(In4.x(),In4.y(),In4.z());
	return (tvect2-tvect1).Angle(tvect4-tvect3);
}

double GetAngleInRadian(Vector3D<double>InComing, Vector3D<double>Outgoing){
	TVector3 incoming(InComing.x(),InComing.y(),InComing.z());
	TVector3 outgoing(Outgoing.x(),Outgoing.y(),Outgoing.z());
	return incoming.Angle(outgoing);
}

double GetAngleInRadian(Tomography::Track InComing, Tomography::Track Outgoing){
	return GetAngleInRadian(InComing.GetDirection(),Outgoing.GetDirection());
}

template<bool TopToDown=true>
double GetAngleWithVertical(Vector3D<double>direction){
	TVector3  dir(direction.x(),direction.y(),direction.z());
	TVector3  vertical(0.,0.,0.);
	if(TopToDown)
		vertical.SetXYZ(0.,0.,-1);
	else
		vertical.SetXYZ(0.,0.,1);

	return dir.Angle(vertical);
}

double Mean(std::vector<double> scatteringVect){
   double sum = 0.;
   for(int i = 0 ; i < scatteringVect.size() ; i++){
     sum += scatteringVect[i];
   }
   return sum/scatteringVect.size();
}

#if(0)
double StandardDeviation(std::vector<double> scatteringVect){
   double mean = Mean(scatteringVect);
   std::cout<<"Mean : " << mean << std::endl;
   for(int i = 0 ; i < scatteringVect.size() ; i++){
   scatteringVect[i] -= mean;
   }

   for(int i = 0 ; i < scatteringVect.size() ; i++){
   scatteringVect[i] *= scatteringVect[i];
   }
   mean = Mean(scatteringVect);
   return std::sqrt(mean);
}
#endif


double StandardDeviation(std::vector<double> scatteringVect,bool forVoxel = false, int numOfBins = 1000){
	//std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@ SD Called........ @@@@@@@@@@@@@@@@@@@@@@@" << std::endl;

	if(scatteringVect.size() <= 1)
		return 0;
	double max = *std::max_element(scatteringVect.begin(),scatteringVect.end());
	double min = *std::min_element(scatteringVect.begin(),scatteringVect.end());
	//std::cout<<"Min : " << min <<" : Max : "<< max << std::endl;
	TH1F *histogram = new TH1F("Hist","Hist",numOfBins,min,max);
	for(int i = 0 ; i < scatteringVect.size() ; i++)
		histogram->Fill(scatteringVect[i]);
	double sd = histogram->GetStdDev();
	if(forVoxel){
		delete histogram;
		return sd;
	}
	//return sd;
	//delete histogram;
	std::cout<<"SD from hist : " << sd << std::endl;
	histogram->Fit("gaus","","",-2*sd, 2*sd);
	TF1 *fit = histogram->GetFunction("gaus");
	double chi2 = fit->GetChisquare();
	double p1 = fit->GetParameter(2);
	delete histogram;
	return p1;
}


void WriteToFile(std::string fileName, std::vector<double> scatteringVect){
   std::cout<<"Writing file " << fileName << "....  ";
   std::ofstream fileHandle(fileName);
   for( int i = 0 ; i < scatteringVect.size() ; i++){
      fileHandle << scatteringVect[i] << " ";
   }
   fileHandle.close();
   std::cout<< "Writing DONE, file CLOSE !! " << std::endl;
}

void WriteToFile(std::string fileName, std::vector<Vector3D<double>> ptVect, double cut=0.3){
   std::cout<<"Writing file " << fileName << "....  ";
   int count = 0 ;
   std::ofstream fileHandle(fileName);
   //std::cout<<"------------ RAMAN -------------" << std::endl;
   for(int i =0  ; i < ptVect.size() ; i++){
	   if(std::fabs(ptVect[i].GetColor())*1000 > cut){
		  // std::cout << ptVect[i].x() << " " << ptVect[i].y() << " " << ptVect[i].z() <<  " " << ptVect[i].GetColor() <<std::endl;
		  fileHandle << ptVect[i].x() << " " << ptVect[i].y() << " " << ptVect[i].z() <<  " " << ptVect[i].GetColor() <<std::endl;
	   }
	  else
		  count++;
   }
   fileHandle.close();
   std::cout<< "Writing DONE, file CLOSE !! " << std::endl;
   std::cout<<"Num of Points with Scattering < 1e-4 : " << count << std::endl;
}



/*static void WriteToFile(std::string fileName,std::vector<Tomography::Voxel*> voxelsVector){
   std::ofstream fileHandle(fileName);
   for(int i = 0 ; i < voxelsVector.size() ; i++){
      Tracking::Vector3D<double> voxCenter = voxelsVector[i]->GetVoxelCenter();
      if(i==0){
         Tracking::Vector3D<int> dim = Tomography::Voxelator::instance()->GetEachVoxelDim();
         fileHandle << dim.x() << " " << dim.y() << " " << dim.z() << std::endl;
         fileHandle << voxCenter.x() << " " << voxCenter.y() << " " << voxCenter.z() 
                    << " " << voxelsVector[i]->GetStandardDeviation()*10000. <<  std::endl;
      }else{
      fileHandle << voxCenter.x() << " " << voxCenter.y() << " " << voxCenter.z() 
                 << " " << voxelsVector[i]->GetStandardDeviation()*10000. <<  std::endl;
   }

   }

   fileHandle.close();
}*/

void WriteToFile(std::string fileName,std::vector<Tomography::Voxel*> voxelsVector,bool filteredVoxels = true){
   std::cout<<"Writing file " << fileName << "....  ";
   std::ofstream fileHandle(fileName);
   int count=-1;;
   if(voxelsVector.size()){
   for(int i = 0 ; i < voxelsVector.size() ; i++){
      Tracking::Vector3D<double> voxCenter = voxelsVector[i]->GetVoxelCenter();
      if(filteredVoxels){
      if(!(voxelsVector[i]->IsOutlier())){
    	  count++;
      if(count==0){
         Tracking::Vector3D<int> dim = Tomography::evolution::Voxelator::instance()->GetEachVoxelDim();
         fileHandle << dim.x() << " " << dim.y() << " " << dim.z() << std::endl;
         fileHandle << voxCenter.x() << " " << voxCenter.y() << " " << voxCenter.z()
                    << " " << voxelsVector[i]->GetRadiationLength() <<  std::endl;
      }else{
      fileHandle << voxCenter.x() << " " << voxCenter.y() << " " << voxCenter.z()
                 << " " << voxelsVector[i]->GetRadiationLength() <<  std::endl;
   }
   }}else{
 	  count++;
   if(count==0){
      Tracking::Vector3D<int> dim = Tomography::evolution::Voxelator::instance()->GetEachVoxelDim();
      fileHandle << dim.x() << " " << dim.y() << " " << dim.z() << std::endl;
      fileHandle << voxCenter.x() << " " << voxCenter.y() << " " << voxCenter.z()
                 << " " << voxelsVector[i]->GetRadiationLength() <<  std::endl;
   }else{
   fileHandle << voxCenter.x() << " " << voxCenter.y() << " " << voxCenter.z()
              << " " << voxelsVector[i]->GetRadiationLength() <<  std::endl;
}
}

   }
   }

   fileHandle.close();
   std::cout<< "Writing DONE, file CLOSE !! " << std::endl;
}

double RadiationLength(double sd){
	//std::cout << "Voxel Depth : " << (Tomography::Voxelator::instance()->GetEachVoxelDim()).z() << std::endl;
	return ((15.*15.)/(3000.*3000.))*((Tomography::evolution::Voxelator::instance()->GetEachVoxelDim()).z()/(sd*sd));
}

double RadiationLength(std::vector<double> scatteringAngleVector, double depth){
	std::cout << "Voxel Depth : " << depth << std::endl;
	double sd = StandardDeviation(scatteringAngleVector);
	return ((15.*15.)/(3000.*3000.))*(depth/(sd*sd));
}

Tracking::Vector3D<double> ConvertToVector3D(G4ThreeVector g4vect3d){
		return Tracking::Vector3D<double>(g4vect3d.x(),g4vect3d.y(),g4vect3d.z());
}
};


}



#endif /* HELPERS_INC_COMMONFUNC_H_ */
