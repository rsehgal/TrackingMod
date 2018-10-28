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
#include "VoxelV2.h"
#include "Voxelator_Evolution.h"
#include <G4ThreeVector.hh>
#include <algorithm>
#include <TF1.h>
#include "base/Global.h"
#include "TColor.h"
#include "TStyle.h"
#include "DetectorMapping.h"
using Tracking::Vector3D;

namespace CommonFunc{


static Precision Distance(Vector3D<Precision> p, Vector3D<Precision> q){
	return (p-q).Mag();
}

class Functions{
	static Functions *finstance;
	Functions(){}
public:

static std::string GetFileName(std::string fullFilename){
	int length = fullFilename.length();
	std::string filename = fullFilename.substr(0,length-4);
	return filename;
}

Tomography::Track CreateTrackFromHitPointVector(std::vector<Tracking::Vector3D<double>> hitPointVector){
	for(int i = 0 ; i < hitPointVector.size() ; i++){
		return Tomography::Track(hitPointVector[hitPointVector.size()-1],hitPointVector[0]);
	}
}

//Converting the double value corresponds to
//Scatering value or RL or SD to proper color value from
//ROOT Color palette. Giving good results
static double CreateColorVal(double color){
		 TColor::SetPalette(1, 0);
		 //gStyle->SetPalette(kDarkBodyRadiator);
		//gStyle->SetPalette(kTemperatureMap);
		 const Int_t nCol = 10;//TColor::GetNumberOfColors();
		 float min = 0, max = 50.; // your range of values
		 double colorVal = TColor::GetColorPalette((color - min)/(max-min) * nCol);
		 return colorVal;
}

//Below to converter functions needs to be checked.
//Function to convert RGB value to a double
static double PackColor(Vector3D<double> color) {
	    return color.x() + color.y() * 256.0 + color.z() * 256.0 * 256.0;
	}
//Function to convert the double to RGB components
static Vector3D<double> UnpackColor(double f) {
		Vector3D<double> color;
		double b = std::floor(f / 256.0 / 256.0);
		double g = std::floor((f - b * 256.0 * 256.0) / 256.0);
		double r = std::floor(f - b * 256.0 * 256.0 - g * 256.0);
		// now we have a vec3 with the 3 components in range [0..255]. Let's normalize it!
		b /= 255.0;
		g /= 255.0;
		r /= 255.0;

		return Vector3D<double>(r, g, b);
		//return color / 255.0;

}

template <bool ForSimulation>
static bool IsFalsePositivePoca(Tracking::Vector3D<double> fPocaPt){
	bool truePositive = true;
	if(!ForSimulation)
		return !truePositive;
	else{
		//Logic to check if the Calculated PoCA lie with the extent of any scatterer
		std::vector<Tracking::Vector3D<double>> scattererMinExtentVector = Tomography::DetectorMapping::create("testMapping.txt")->GetScattererMinExtent();
		std::vector<Tracking::Vector3D<double>> scattererMaxExtentVector = Tomography::DetectorMapping::instance()->GetScattererMaxExtent();

		for(int i = 0 ; i < scattererMinExtentVector.size() ; i++){
			truePositive &=    (fPocaPt.x() >= scattererMinExtentVector[i].x() && fPocaPt.x() <= scattererMaxExtentVector[i].x())
						    && (fPocaPt.y() >= scattererMinExtentVector[i].y() && fPocaPt.y() <= scattererMaxExtentVector[i].y())
							&& (fPocaPt.z() >= scattererMinExtentVector[i].z() && fPocaPt.z() <= scattererMaxExtentVector[i].z());
			if(truePositive)
				break;
		}
		return !truePositive;
	}

}


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

#if(1)
double StandardDeviation(std::vector<double> scatteringVect,bool forVoxel = false, int numOfBins = 1000){
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

#if(0)
double StandardDeviation(std::vector<double> scatteringVect,bool forVoxel = false, int numOfBins = 1000){
	std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@ SD Called........ @@@@@@@@@@@@@@@@@@@@@@@" << std::endl;

	std::cout << "Found Scattering Vector Size : " << scatteringVect.size() << " : " << __FILE__ << " : " << __LINE__ << std::endl;
	if(scatteringVect.size() <= 1){

		return 0;
	}
	double max = *std::max_element(scatteringVect.begin(),scatteringVect.end());
	double min = *std::min_element(scatteringVect.begin(),scatteringVect.end());
	//std::cout<<"Min : " << min <<" : Max : "<< max << std::endl;
	TH1F *histogram = new TH1F("Hist","Hist",numOfBins,min,max);
	for(int i = 0 ; i < scatteringVect.size() ; i++)
		histogram->Fill(scatteringVect[i]);
	double sd = histogram->GetStdDev();
	std::cout << "====================== SD of Voxel : " << sd <<" =====================" << std::endl;
	if(forVoxel){
		delete histogram;
		return sd;
	}
	//return sd;
	//delete histogram;
	std::cout<<"AYUSH SD from hist : " << sd << std::endl;
	histogram->Fit("gaus","","",-2*sd, 2*sd);
	TF1 *fit = histogram->GetFunction("gaus");
	double chi2 = fit->GetChisquare();
	double p1 = fit->GetParameter(2);
	delete histogram;
	return p1;
}
#endif

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

/*
void WriteToFile(std::string fileName,std::vector<Tomography::Voxel*> voxelsVector,bool filteredVoxels = true){
   std::cout<<"Writing file " << fileName << "....  ";
   float dummy = 0.;
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
         fileHandle << dim.x() << " " << dim.y() << " " << dim.z() << " " << dummy << std::endl;
         fileHandle << voxCenter.x() << " " << voxCenter.y() << " " << voxCenter.z()
                    //<< " " << voxelsVector[i]->GetRadiationLength() <<  std::endl;
						<< " " << voxelsVector[i]->GetRadiationLength() <<  " " <<
						voxelsVector[i]->GetScatteringDensity() <<  std::endl;
      }else{
      fileHandle << voxCenter.x() << " " << voxCenter.y() << " " << voxCenter.z()
                 //<< " " << voxelsVector[i]->GetRadiationLength() <<  std::endl;
								<< " " << voxelsVector[i]->GetRadiationLength() <<  " " <<
								voxelsVector[i]->GetScatteringDensity() <<  std::endl;
   }
   }}else{
 	  count++;
   if(count==0){
      Tracking::Vector3D<int> dim = Tomography::evolution::Voxelator::instance()->GetEachVoxelDim();
      fileHandle << dim.x() << " " << dim.y() << " " << dim.z() << std::endl;
      fileHandle << voxCenter.x() << " " << voxCenter.y() << " " << voxCenter.z()
                 //<< " " << voxelsVector[i]->GetRadiationLength() <<  std::endl;
								<< " " << voxelsVector[i]->GetRadiationLength() <<  " " <<
								voxelsVector[i]->GetScatteringDensity() <<  std::endl;
   }else{
   fileHandle << voxCenter.x() << " " << voxCenter.y() << " " << voxCenter.z()
              //<< " " << voxelsVector[i]->GetRadiationLength() <<  std::endl;
								<< " " << voxelsVector[i]->GetRadiationLength() <<  " " <<
								voxelsVector[i]->GetScatteringDensity() <<  std::endl;
}
}

   }
   }

   fileHandle.close();
   std::cout<< "Writing DONE, file CLOSE !! " << std::endl;
}
*/


void WriteToFile(std::string fileName,std::vector<Tomography::Voxel_V2*> voxelsVector ){
   std::cout<<"Writing file " << fileName << "....  ";
   float dummy = 0.;
   std::ofstream fileHandle(fileName);
   int count=-1;;
   std::cout<<"VoxelVector Size : " << voxelsVector.size() << std::endl;
   if(voxelsVector.size()){
   for(int i = 0 ; i < voxelsVector.size() ; i++){
      Tracking::Vector3D<double> voxCenter = voxelsVector[i]->GetVoxelCenter();
      count++;
      double sd = voxelsVector[i]->GetStandardDeviation();
      if(count==0){
         Tracking::Vector3D<double> dim = Tomography::evolution::Voxelator::instance()->GetEachVoxelDim();
         fileHandle << dim.x() << " " << dim.y() << " " << dim.z() << " " << dummy << std::endl;
         //Storing values only when SD is NOT equal to ZERO, to avoid division by zero
         if(sd != 0.){
         fileHandle << std::setprecision(20.) << voxCenter.x() << " " << voxCenter.y() << " " << voxCenter.z();
  		 fileHandle << " " << sd <<  " " << voxelsVector[i]->GetRadiationLength() <<  std::endl;
			//voxelsVector[i]->GetScatteringDensity() <<  std::endl;
		}
      }else{
    	  if(sd != 0.){
    		  fileHandle << std::setprecision(20.) << voxCenter.x() << " " << voxCenter.y() << " " << voxCenter.z() ;
              fileHandle << " " << sd <<  " " << voxelsVector[i]->GetRadiationLength() <<  std::endl;
      		  //voxelsVector[i]->GetScatteringDensity() <<  std::endl;
    	  }
      //std::cout << "UNEXPECTED VALUE SHOULD COME FROM HERE.......... : " << voxelsVector[i]->GetScatteringDensity() << std::endl;
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

int GetVoxelNum(Tracking::Vector3D<double> pt){
	int voxNum = Tomography::evolution::Voxelator::instance()->GetVoxelNumber(pt); //logica to calculate voxel num
	return voxNum;
}

//#define USE_ENCLOSING_VOXELS
#ifdef USE_ENCLOSING_VOXELS
std::vector<Tracking::Vector3D<double>> GetEightCorners_Of_ImaginaryVoxel_CentredAtPocaPoint(Tracking::Vector3D<double> pocaPt){
	int count = 0;
	Tracking::Vector3D<double> voxelDim = Tomography::evolution::Voxelator::instance()->GetEachVoxelDim();
	std::vector<Tracking::Vector3D<double>> corners;
	for(int i=-1 ; i <= 1 ; i=i+2  ) // X axis
		  for(int j=-1 ; j <= 1 ; j=j+2  ) // Y axis
			  for(int k=-1 ; k <= 1 ; k=k+2  ){ // Z axis
				  count++;
				  Tracking::Vector3D<double> corner(pocaPt.x()+(i*voxelDim.x()/2.) , pocaPt.y()+(j*voxelDim.y()/2.) , pocaPt.z()+(k*voxelDim.z()/2.) );
				  //corners.push_back(Tracking::Vector3D<double>(pocaPt.x()+(i*fDim.x()/2.) , pocaPt.y()+(j*fDim.y()/2.) , pocaPt.z()+(k*fDim.z()/2.) ));
				  corners.push_back(corner);
				  //std::cout<<"Corner "<< count << " :: " << corner.x() << " , " << corner.y() << " , " << corner.z() << std::endl;
			  }

	return corners;
}


std::vector<Tomography::Voxel*> GetEnclosingVoxelWithScatteringIntensity(Tracking::Vector3D<double> pocaPt){
	std::vector<Tracking::Vector3D<double>> imaginaryVoxelsCorners = GetEightCorners_Of_ImaginaryVoxel_CentredAtPocaPoint(pocaPt);
	std::vector<Tomography::Voxel*> enclosingVoxelsVector;
	double sumOfInverseDistancesFromEnclosingVoxels = 0.;
	for(int i =0 ; i  < imaginaryVoxelsCorners.size() ; i++){
		int voxelNum = GetVoxelNum(imaginaryVoxelsCorners[i]);
		int voxNum = Tomography::Voxel::IfVoxelExist(voxelNum);
		if(voxNum < 0.){
			enclosingVoxelsVector.push_back(new Tomography::Voxel(imaginaryVoxelsCorners[i],voxelNum,true));
			//oxel::InsertVoxel(new Voxel(fPocaPt,voxelNum),voxelNum);
		}else{
			enclosingVoxelsVector.push_back(Tomography::Voxel::GetVoxelVector()[voxNum]);
		}
		Tracking::Vector3D<double> encVoxCenter = enclosingVoxelsVector[i]->GetVoxelCenter();
		Tracking::Vector3D<double> corner = imaginaryVoxelsCorners[i];
//		std::cout<<"@@@@@@@@ POCAPt : " << fPocaPt.x() <<" , "<< fPocaPt.y() <<" , "<< fPocaPt.z() << std::endl;
//		std::cout<<"@@@@@@@@ encVoxCenter : " << encVoxCenter.x() <<" , "<< encVoxCenter.y() <<" , "<< encVoxCenter.z() << std::endl;
//		std::cout<<"@@@@@@@@ ImaginaryVoxelCorner : " << corner.x() <<" , "<< corner.y() <<" , "<< corner.z() << std::endl;
//		std::cout<<"@@@@@@@@ RAMAN DIST : " << CommonFunc::Distance(enclosingVoxelsVector[i]->GetVoxelCenter(),fPocaPt) << std::endl;
//		std::cout<<"@@@@@@@@ SHACHI InvDIST : " << 1./CommonFunc::Distance(enclosingVoxelsVector[i]->GetVoxelCenter(),fPocaPt) << std::endl;
		sumOfInverseDistancesFromEnclosingVoxels += 1./CommonFunc::Distance(enclosingVoxelsVector[i]->GetVoxelCenter(),pocaPt);
	}

	std::cout<< " @@@@@@@@@ AYUSH sumOfInverseDistancesFromEnclosingVoxels : " << sumOfInverseDistancesFromEnclosingVoxels << std::endl;

	for(int i =0 ; i  < enclosingVoxelsVector.size() ; i++){
		enclosingVoxelsVector[i]->SetScatteringDensity(((1./CommonFunc::Distance(enclosingVoxelsVector[i]->GetVoxelCenter(),
														pocaPt))/sumOfInverseDistancesFromEnclosingVoxels));//*pocaPt.GetColor());

	}

	return enclosingVoxelsVector;
}

	/*
	 * Above logic to calculate the inverse ratios, is taken from table from the following link
	 * https://math.stackexchange.com/questions/2479777/find-the-inverse-ratio-of-a-group-of-numbers
	 */

#endif



};


}



#endif /* HELPERS_INC_COMMONFUNC_H_ */
