#include "Voxelator.h"
#include <cmath>

namespace Tomography {

Voxelator::Voxelator(){
//fVoxelsIn1D = new TH1F();
fVoxelizedVolumeDim.Set(500,500,400); //Currently setting to values used in Simulations
fEachVoxelDim.Set(10,10,80); // Ideally this should corresponds to strip size of detectors
			  // so setting it to 3
//fVoxelatorDim.Set(int(fVoxelizedVolumeDim.x()/fEachVoxelDim.x()),int(fVoxelizedVolumeDim.y()/fEachVoxelDim.y()),
	//			      int(fVoxelizedVolumeDim.z()/fEachVoxelDim.z()) );

fVoxelatorDim.Set((fVoxelizedVolumeDim.x()/fEachVoxelDim.x()),(fVoxelizedVolumeDim.y()/fEachVoxelDim.y()),
				      (fVoxelizedVolumeDim.z()/fEachVoxelDim.z()) );


CreateHistogram();

}

Voxelator::Voxelator(double voxelizedVolHalfX,double voxelizedVolHalfY, double voxelizedVolHalfZ,
			 double voxelX,double voxelY, double voxelZ){
	SetVoxelator(voxelizedVolHalfX,voxelizedVolHalfY,voxelizedVolHalfZ,voxelX,voxelY,voxelZ);
	fVoxelizedVolumeDim.Print();
	fVoxelatorDim.Print();

}

Voxelator::~Voxelator(){
//delete histVoxelValue;
//delete histVoxelCount;
}

void Voxelator::SetVoxelDim(double x, double y , double z){
	fEachVoxelDim.Set(x,y,z);
}

void Voxelator::SetVoxelizedVolumeDim(double halfX, double halfY, double halfZ){
	fVoxelizedVolumeDim.Set(halfX,halfY,halfZ);
}

void Voxelator::SetVoxelatorDim(double x, double y , double z){
	//fVoxelatorDim.Set(int(2*fVoxelizedVolumeDim.x()/fEachVoxelDim.x()),int(2*fVoxelizedVolumeDim.y()/fEachVoxelDim.y()),
		//			  int(2*fVoxelizedVolumeDim.z()/fEachVoxelDim.z()) );
	fVoxelatorDim.Set((2*fVoxelizedVolumeDim.x()/fEachVoxelDim.x()),(2*fVoxelizedVolumeDim.y()/fEachVoxelDim.y()),
						  (2*fVoxelizedVolumeDim.z()/fEachVoxelDim.z()) );
}

void Voxelator::SetVoxelator(double voxelizedVolHalfX,double voxelizedVolHalfY, double voxelizedVolHalfZ,
							 double voxelX,double voxelY, double voxelZ){
	fVoxelizedVolumeDim.Set(2*voxelizedVolHalfX,2*voxelizedVolHalfY,2*voxelizedVolHalfZ);
	fEachVoxelDim.Set(voxelX,voxelY,voxelZ);
	fVoxelatorDim.Set(int(fVoxelizedVolumeDim.x()/fEachVoxelDim.x()),int(fVoxelizedVolumeDim.y()/fEachVoxelDim.y()),
			          int(fVoxelizedVolumeDim.z()/fEachVoxelDim.z()) );
	CreateHistogram();
}

/*
Vector3D<double> Voxelator::GetInsersection(Track t, double zVal, int select){
	Vector3D<double> ptOfIntersection;
	double x = 0.,y = 0., z = 0., dist = 0. ;

	if(select == 1){
		dist = (zVal - t.GetP2().z())/t.GetDirection().z();
		ptOfIntersection = t.GetP2() + t.GetDirection()*dist;
	}

	if(select == 2){
		dist = (zVal - t.GetP1().z())/(-t.GetDirection().z());
		ptOfIntersection = t.GetP1() - t.GetDirection()*dist;
	}

	if(select == 3){
		dist = (zVal - t.GetP1().z())/t.GetDirection().z();
		ptOfIntersection = t.GetP1() + t.GetDirection()*dist;
	}

	return ptOfIntersection;
}
*/

int Voxelator::GetVoxelNumber(double x, double y, double z){
	int onX = (int)(x+fVoxelizedVolumeDim.x()/2.)/fEachVoxelDim.x();// + 1;
	int onY = (int)(y+fVoxelizedVolumeDim.y()/2.)/fEachVoxelDim.y();// + 1;
	int onZ = (int)(z+fVoxelizedVolumeDim.z()/2.)/fEachVoxelDim.z();// + 1;

	// std::cout<< onX <<" : "<< onY << " : " << onZ << std::endl;
	// return fVoxelatorDim.x()*fVoxelatorDim.y()*(onZ+fVoxelizedVolumeDim.z()/2.) +
	// 	   fVoxelatorDim.x()*(onY+fVoxelizedVolumeDim.y()/2.) + (onX+fVoxelizedVolumeDim.x()/2.) ;
	return GetVoxelNumber(onX,onY,onZ);

}

int Voxelator::GetVoxelNumber(int x, int y, int z){

	return fVoxelatorDim.x()*fVoxelatorDim.y()*z +
		   fVoxelatorDim.x()*y + x ;

}

int Voxelator::GetVoxelNumber(Tracking::Vector3D<double> vox){
	return GetVoxelNumber(vox.x(),vox.y(),vox.z());
}

Tracking::Vector3D<double> Voxelator::GetVoxelCenter(int voxelNum){

	return fVoxelCenters[voxelNum];
}

Tracking::Vector3D<double> Voxelator::GetVoxelCenter(double x, double y, double z){

	return fVoxelCenters[GetVoxelNumber(x,y,z)];
}

Tracking::Vector3D<double> Voxelator::GetVoxelCenter(Tracking::Vector3D<double> vox){
	return GetVoxelCenter(vox.x(),vox.y(),vox.z());
}

void Voxelator::CalculateVoxelCenters(){

//	std::cout << fVoxelizedVolumeDim.x() << " : " << fVoxelatorDim.x() << std::endl;
//	std::cout << fVoxelizedVolumeDim.y() << " : " << fVoxelatorDim.y() << std::endl;
//	std::cout << fVoxelizedVolumeDim.z() << " : " << fVoxelatorDim.z() << std::endl;
	//fVoxelizedVolumeDim.Print();
	for(int k = 0 ; k < fVoxelatorDim.z() ; k++){
		for(int j = 0 ; j < fVoxelatorDim.y() ; j++){
			for(int i = 0 ; i < fVoxelatorDim.x() ; i++){
				double x = -fVoxelizedVolumeDim.x()/2. + fEachVoxelDim.x()/2. + (fEachVoxelDim.x() * (i));
				double y = -fVoxelizedVolumeDim.y()/2. + fEachVoxelDim.y()/2. + (fEachVoxelDim.y() * (j));
				double z = -fVoxelizedVolumeDim.z()/2. + fEachVoxelDim.z()/2. + (fEachVoxelDim.z() * (k));
				fVoxelCenters.push_back(Tracking::Vector3D<double>(x,y,z));
			}
		}
	}
}

void Voxelator::PrintVoxelCenters(){
	std::cout<<"Size of VoxelCenters : " << fVoxelCenters.size() << std::endl;
	for(int i = 0 ; i < fVoxelCenters.size() ; i++){
		fVoxelCenters[i].Print();
	}
}


/*
void Voxelator::CreateSDGraph(){
	std::vector<int> x;
	int numOfVoxels = fVoxelatorDim.x()*fVoxelatorDim.y()*fVoxelatorDim.z();
	for(int i = 0 ; i < numOfVoxels ; i++){
		x.push_back(i);
	}
	//fGraphSD = new TGraph(numOfVoxels, &x[0],);
}
*/

void Voxelator::CreateHistogram(){
	//Create the required 3D Hist which corresponds to voxelized volume
	histVoxelValue = new  TH3F("histVoxelValue", "HistVoxelValue", fVoxelatorDim.x(), -fVoxelizedVolumeDim.x(), fVoxelizedVolumeDim.x(),
						                                           fVoxelatorDim.y(), -fVoxelizedVolumeDim.y(), fVoxelizedVolumeDim.y(),
																   fVoxelatorDim.z(), -fVoxelizedVolumeDim.z(), fVoxelizedVolumeDim.z()) ;

	histVoxelCount = new TH3F("histVoxelCount", "HistVoxelCount", fVoxelatorDim.x(), -fVoxelizedVolumeDim.x(), fVoxelizedVolumeDim.x(),
																  fVoxelatorDim.y(), -fVoxelizedVolumeDim.y(), fVoxelizedVolumeDim.y(),
																  fVoxelatorDim.z(), -fVoxelizedVolumeDim.z(), fVoxelizedVolumeDim.z()) ;

	std::cout << "Total num of voxel for simulations : " << GetTotalNumberOfVoxels() << std::endl;
	fVoxelsIn1D = new TH1F("IDHistOfVoxels","IDHistOfVoxels",GetTotalNumberOfVoxels(), 0, GetTotalNumberOfVoxels());
	fVoxelsIn1DCount = new TH1F("IDHistOfVoxelsCount","IDHistOfVoxelsCount",GetTotalNumberOfVoxels(), 0, GetTotalNumberOfVoxels());

}

double Voxelator::GetAverageScatteringAngleInAVoxel(Vector3D<double> vox){
return GetAverageScatteringAngleInAVoxel(vox.x(),vox.y(),vox.z());
}

double Voxelator::GetAverageScatteringAngleInAVoxel(int x, int y, int z){
//return histVoxelValue->GetBinContent(x,y,z);
	return fVoxelsIn1D->GetBinContent(GetVoxelNumber(x,y,z));
}

void Voxelator::Insert(double x, double y, double z, double w){
histVoxelCount->Fill(x,y,z);
histVoxelValue->Fill(x,y,z,w);
int voxelNumber = GetVoxelNumber(x,y,z);
fVoxelsIn1D->Fill(voxelNumber,std::fabs(w));
fVoxelsIn1DCount->Fill(voxelNumber);

#ifdef STORE_SLICE
int numOfVoxelsInASlice = fVoxelatorDim.x()*fVoxelatorDim.y();
//std::cout<<"NumofVoxelsInAslice : " << numOfVoxelsInASlice << std::endl;
int sliceNum = 0;
if(voxelNumber % numOfVoxelsInASlice)
	sliceNum = (voxelNumber/numOfVoxelsInASlice) + 1;
else
	sliceNum = (voxelNumber/numOfVoxelsInASlice);

/*if(sliceNum != 6 && sliceNum != 7 && sliceNum != 8 ){
	std::cout << "Not expected SliceNum : "<< sliceNum << std::endl;
}*/

if(sliceNum < 0 || sliceNum > fVoxelatorDim.z()){
	std::cout<<"Unexpected slice : "<< sliceNum <<"  : For VoxelNum : "<< voxelNumber << ".... Skipping this guy....." << std::endl;
	return;
}
std::string histName = "ZSlice_"+std::to_string(sliceNum);
fObjChecker.CheckAndInsert(histName.c_str(),histName.c_str(),fVoxelatorDim.x(), -fVoxelatorDim.x()/2.,fVoxelatorDim.x()/2.,
															 fVoxelatorDim.y(), -fVoxelatorDim.y()/2.,fVoxelatorDim.y()/2.,
															 (x/fEachVoxelDim.x()),(y/fEachVoxelDim.y()),w);
#endif

}

void Voxelator::Insert(Vector3D<double> vox){
Insert(vox.x(),vox.y(),vox.z(),vox.w());
}

void Voxelator::Insert(double x, double y, double z, int w){
Insert(x,y,z,double(w));
}

//Very important. Used to set the average value in each voxel
void Voxelator::AverageOut(){
/*  for(int x = 0 ; x < fVoxelatorDim.x() ; x++){
    for(int y = 0 ; y < fVoxelatorDim.y() ; y++){
      for(int z = 0 ; z < fVoxelatorDim.z() ; z++){
	if(histVoxelCount->GetBinContent(x,y,z)){
	double averageVal = histVoxelValue->GetBinContent(x,y,z)/histVoxelCount->GetBinContent(x,y,z);
	histVoxelValue->SetBinContent(x,y,z,averageVal);
	}
      }
    }
  }
  */

  //Using Root histogram divide function
  histVoxelValue->Divide(histVoxelCount);
}

void Voxelator::Insert(std::vector<Vector3D<double>> pocaPtVect){
int size = pocaPtVect.size();
for (auto &pocaPt : pocaPtVect) {
      Insert(pocaPt);
  }
AverageOut();
}

} // end of Tomography namespace
