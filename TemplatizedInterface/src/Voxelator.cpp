#include "Voxelator.h"

namespace Tomography {

Voxelator::Voxelator(){
fVoxelizedVolumeDim.Set(1000,1000,150); //Currently setting to values used in Simulations
fEachVoxelDim.Set(10,10,10); // Ideally this should corresponds to strip size of detectors
			  // so setting it to 3

fVoxelatorDim.Set(int(2*fVoxelizedVolumeDim.x()/fEachVoxelDim.x()),int(2*fVoxelizedVolumeDim.y()/fEachVoxelDim.y()), int(2*fVoxelizedVolumeDim.z()/fEachVoxelDim.z()) );

//Create the required 3D Hist which corresponds to voxelized volume
histVoxelValue = new  TH3F("histVoxelValue", "HistVoxelValue", fVoxelatorDim.x(), -fVoxelizedVolumeDim.x(), fVoxelizedVolumeDim.x(), fVoxelatorDim.y(), -fVoxelizedVolumeDim.y(), fVoxelizedVolumeDim.y(),fVoxelatorDim.z(), -fVoxelizedVolumeDim.z(), fVoxelizedVolumeDim.z()) ;

histVoxelCount = new TH3F("histVoxelCount", "HistVoxelCount", fVoxelatorDim.x(), -fVoxelizedVolumeDim.x(), fVoxelizedVolumeDim.x(), fVoxelatorDim.y(), -fVoxelizedVolumeDim.y(), fVoxelizedVolumeDim.y(),fVoxelatorDim.z(), -fVoxelizedVolumeDim.z(), fVoxelizedVolumeDim.z()) ;
}

Voxelator::~Voxelator(){
delete histVoxelValue;
delete histVoxelCount;
}

double Voxelator::GetAverageScatteringAngleInAVoxel(Vector3D<double> vox){
return GetAverageScatteringAngleInAVoxel(vox.x(),vox.y(),vox.z());
}

double Voxelator::GetAverageScatteringAngleInAVoxel(double x, double y, double z){
return histVoxelValue->GetBinContent(x,y,z);
}

void Voxelator::Insert(double x, double y, double z, double w){
histVoxelCount->Fill(x,y,z);
histVoxelValue->Fill(x,y,z,w);

}

void Voxelator::Insert(Vector3D<double> vox){
Insert(vox.x(),vox.y(),vox.z(),vox.w());
}

void Voxelator::Insert(double x, double y, double z, int w){
Insert(x,y,z,double(w));
}

//Very important. Used to set the average value in each voxel
void Voxelator::AverageOut(){
  for(int x = 0 ; x < fVoxelatorDim.x() ; x++){
    for(int y = 0 ; y < fVoxelatorDim.y() ; y++){
      for(int z = 0 ; z < fVoxelatorDim.z() ; z++){
	if(histVoxelCount->GetBinContent(x,y,z)){
	double averageVal = histVoxelValue->GetBinContent(x,y,z)/histVoxelCount->GetBinContent(x,y,z);
	histVoxelValue->SetBinContent(x,y,z,averageVal);
	}
      }
    }
  }
}

void Voxelator::Insert(std::vector<Vector3D<double>> pocaPtVect){
int size = pocaPtVect.size();
for (auto &pocaPt : pocaPtVect) {
      Insert(pocaPt);
  }
AverageOut();
}

} // end of Tomography namespace
