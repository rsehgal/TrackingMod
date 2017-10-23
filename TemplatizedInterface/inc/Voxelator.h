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
#include "TH2F.h"
#include "ObjectChecker.h"
#include "Track.h"
#include <TGraph.h>

using Slice = std::vector<std::vector<Tracking::Vector3D<double>>>;
using VoxelCenters = std::vector<Slice>;

using Tracking::Vector3D;
using Tomography::Track;


namespace Tomography{
class Voxelator{
private:
  Vector3D<int> fVoxelizedVolumeDim; // This should be set using the detector size
  Vector3D<int> fEachVoxelDim; // voxel width in x, y and z
  Vector3D<int> fVoxelatorDim;
  TH3F *histVoxelValue; // = new TH3F("glvoxel", "glvoxel", 50, -500., 500., 50, -500., 500., 50, -500., 500.);
  TH3F *histVoxelCount;
  TH1F *fVoxelsIn1D;
  TH1F *fVoxelsIn1DCount;
  //TGraph *fGraphSD;// = new TGraph(dim, &x[0], &y[0]);


  ObjectChecker fObjChecker;

  std::vector<Tracking::Vector3D<double>> fVoxelCenters;


  // Hit point to VoxelizedVolume
  Vector3D<double> fHitPtInput;
  Vector3D<double> fHitPtOutput;

public:

  ObjectChecker GetObjectChecker(){return fObjChecker;}
  Voxelator();
  Voxelator(double voxelizedVolHalfX,double voxelizedVolHalfY, double voxelizedVolHalfZ,
			 double voxelX,double voxelY, double voxelZ);

  ~Voxelator();
  void SetVoxelizedVolumeDim(double halfX,double halfY, double halfZ);
  void SetVoxelDim(double x,double y, double z);
  void SetVoxelatorDim(double x,double y, double z);
  void SetVoxelator(double voxelizedVolHalfX,double voxelizedVolHalfY, double voxelizedVolHalfZ,
  							 double voxelX,double voxelY, double voxelZ);

  //VoxelCenters
  TH1F* GetVoxelIn1D(){return fVoxelsIn1D;}
  TH1F* GetVoxelIn1DCount(){return fVoxelsIn1DCount;}
  std::vector<Tracking::Vector3D<double>> GetVoxelCenters(){return fVoxelCenters; }
  Tracking::Vector3D<double> GetVoxelCenter(double x, double y, double z);
  Tracking::Vector3D<double> GetVoxelCenter(Tracking::Vector3D<double> vox);
  Tracking::Vector3D<double> GetVoxelCenter(int voxelNum);
  int GetVoxelNumber(int x, int y, int z);
  int GetVoxelNumber(double x,double y, double z);
  int GetVoxelNumber(Tracking::Vector3D<double> vox);
  int GetTotalNumberOfVoxels(){return fVoxelatorDim.x()*fVoxelatorDim.y()*fVoxelatorDim.z();}



  // This needs NOT to be called by user program
  void CreateHistogram();
  void CalculateVoxelCenters();
  //void CreateSDGraph();

  void PrintVoxelCenters();

  Vector3D<int> GetVoxelatorDim(){return fVoxelatorDim;}
  double GetAverageScatteringAngleInAVoxel(Vector3D<double> vox);
  double GetAverageScatteringAngleInAVoxel(double x, double y, double z){}
  double GetAverageScatteringAngleInAVoxel(int x, int y, int z);
  void Insert(double x, double y, double z, double w);
  void Insert(double x, double y, double z, int w);
  void Insert(Vector3D<double> vox);
  void Insert(std::vector<Vector3D<double>> pocaPtVect);
  void AverageOut();
  TH3F* GetVoxelizedHist(){return histVoxelValue;}
  TH3F* GetVoxelizedCount(){return histVoxelCount;}
  Vector3D<int> GetVoxelizedVolumeDim(){return fVoxelizedVolumeDim;}
  Vector3D<int> GetEachVoxelDim(){return fEachVoxelDim;}



  //Functions to detect Candidate Voxels that influenced the Muon direction
  /*
   * @Input : Track
   * @Input : Z value to detect the intersection at
   * @Input : An integer defining following
   * 		  1) Incoming Track (to detect incoming Hit Point at Voxelized Volume)
   * 		  2) Outgoing Track (to detect outgoing Hit Point at Voxelized Volume)
   * 		  3) Intersection with real Voxels
   *
   */

   //Curretnly
   //Vector3D<double> GetInsersection(Track t, double zVal, int select);

};


} // end of Tomography namespace
#endif  // INC_VOXELATOR__

