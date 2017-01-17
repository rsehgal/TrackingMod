/* author : rsehgal
** date : 17.01.2017
*/

/* Idea of creation of this class is to use it calculate the
** average Scattering angle of PocaPoint in each voxel.
*/

#ifndef TEMPLATIZEDINTERFACE_INC_VOXELATOR_H_
#define TEMPLATIZEDINTERFACE_INC_VOXELATOR_H_

#include "base/Vector3D.h"
#include "TH3F.h"

using Tracking::Vector3D;
namespace Tomography{
class Voxelator{
private:
  Vector3D<int> fVoxelizedVolumeDim; // This should be set using the detector size
  Vector3D<int> fEachVoxelDim; // voxel width in x, y and z
  Vector3D<int> fVoxelatorDim;
  TH3F *histVoxelValue; // = new TH3F("glvoxel", "glvoxel", 50, -500., 500., 50, -500., 500., 50, -500., 500.);
  TH3F *histVoxelCount;
public:
  Voxelator();
  ~Voxelator();
  Vector3D<int> GetVoxelatorDim(){return fVoxelatorDim;}
  double GetAverageScatteringAngleInAVoxel(Vector3D<double> vox);
  double GetAverageScatteringAngleInAVoxel(double x, double y, double z);
  void Insert(double x, double y, double z, double w);
  void Insert(double x, double y, double z, int w);
  void Insert(Vector3D<double> vox);
  void Insert(std::vector<Vector3D<double>> pocaPtVect);
  void AverageOut();
  TH3F* GetVoxelizedHist(){return histVoxelValue;}
  TH3F* GetVoxelizedCount(){return histVoxelCount;}

};

#endif  // INC_VOXELATOR__
} // end of Tomography namespace

