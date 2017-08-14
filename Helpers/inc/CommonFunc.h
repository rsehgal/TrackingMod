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
//#include <G4ThreeVector.hh>
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

/*static double GetAngleInRadian(G4ThreeVector incoming, G4ThreeVector outgoing){
   TVector3 TIncoming(incoming.x(),incoming.y(),incoming.z());
   TVector3 TOutgoing(outgoing.x(),outgoing.y(),outgoing.z());
   return TIncoming.Angle(TOutgoing);
}*/

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

static void WriteToFile(std::string fileName, std::vector<double> scatteringVect){
   std::ofstream fileHandle(fileName);
   for( int i = 0 ; i < scatteringVect.size() ; i++){
      fileHandle << scatteringVect[i] << " ";
   }
   fileHandle.close();
}

static void WriteToFile(std::string fileName, std::vector<Vector3D<double>> ptVect){
   std::ofstream fileHandle(fileName);
   for(int i =0  ; i < ptVect.size() ; i++){
      fileHandle << ptVect[i].x() << " " << ptVect[i].y() << " " << ptVect[i].z() <<  " " << ptVect[i].GetColor() <<std::endl;
   }
   fileHandle.close();
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

static void WriteToFile(std::string fileName,std::vector<Tomography::Voxel*> voxelsVector){
   std::ofstream fileHandle(fileName);
   int count=-1;;
   for(int i = 0 ; i < voxelsVector.size() ; i++){
      Tracking::Vector3D<double> voxCenter = voxelsVector[i]->GetVoxelCenter();
      if(!(voxelsVector[i]->IsOutlier())){
    	  count++;
      if(count==0){
         Tracking::Vector3D<int> dim = Tomography::Voxelator::instance()->GetEachVoxelDim();
         fileHandle << dim.x() << " " << dim.y() << " " << dim.z() << std::endl;
         fileHandle << voxCenter.x() << " " << voxCenter.y() << " " << voxCenter.z()
                    << " " << voxelsVector[i]->GetRadiationLength() <<  std::endl;
      }else{
      fileHandle << voxCenter.x() << " " << voxCenter.y() << " " << voxCenter.z()
                 << " " << voxelsVector[i]->GetRadiationLength() <<  std::endl;
   }
   }

   }

   fileHandle.close();
}

static double RadiationLength(double sd){
	//std::cout << "Voxel Depth : " << (Tomography::Voxelator::instance()->GetEachVoxelDim()).z() << std::endl;
	return ((15.*15.)/(3000.*3000.))*((Tomography::Voxelator::instance()->GetEachVoxelDim()).z()/(sd*sd));
}

static double RadiationLength(std::vector<double> scatteringAngleVector, double depth){
	std::cout << "Voxel Depth : " << depth << std::endl;
	double sd = StandardDeviation(scatteringAngleVector);
	return ((15.*15.)/(3000.*3000.))*(depth/(sd*sd));
}

};

}



#endif /* HELPERS_INC_COMMONFUNC_H_ */
