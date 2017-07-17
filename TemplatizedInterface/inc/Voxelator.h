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

using Slice = std::vector<std::vector<Tracking::Vector3D<double>>>;
using VoxelCenters = std::vector<Slice>;

using Tracking::Vector3D;
namespace Tomography{
class Voxelator{
private:
  Vector3D<int> fVoxelizedVolumeDim; // This should be set using the detector size
  Vector3D<int> fEachVoxelDim; // voxel width in x, y and z
  Vector3D<int> fVoxelatorDim;
  TH3F *histVoxelValue; // = new TH3F("glvoxel", "glvoxel", 50, -500., 500., 50, -500., 500., 50, -500., 500.);
  TH3F *histVoxelCount;

  //VoxelCenters fVoxelCenters;
  std::vector<Tracking::Vector3D<double>> fVoxelCenters;
public:
  Voxelator();
  ~Voxelator();
  void SetVoxelizedVolumeDim(double halfX,double halfY, double halfZ);
  void SetVoxelDim(double x,double y, double z);
  void SetVoxelatorDim(double x,double y, double z);
  void SetVoxelator(double voxelizedVolHalfX,double voxelizedVolHalfY, double voxelizedVolHalfZ,
  							 double voxelX,double voxelY, double voxelZ);

  //VoxelCenters
  std::vector<Tracking::Vector3D<double>> GetVoxelCenters(){return fVoxelCenters; }


  // This needs NOT to be called by user program
  void CreateHistogram();
  void CalculateVoxelCenters();

  void PrintVoxelCenters();

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
  Vector3D<int> GetVoxelizedVolumeDim(){return fVoxelizedVolumeDim;}
  Vector3D<int> GetEachVoxelDim(){return fEachVoxelDim;}

};

#endif  // INC_VOXELATOR__
} // end of Tomography namespace

