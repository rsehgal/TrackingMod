/* author : rsehgal
** date : 17.01.2017
*/

/* Idea of creation of this class is to use it calculate the
** average Scattering angle of PocaPoint in each voxel.
*/

#ifndef HELPERS_INC_VOXELATOR_EVOLUTION_H_
#define HELPERS_INC_VOXELATOR_EVOLUTION_H_

#include "base/Vector3D.h"
#include "TH3F.h"
#include "TH2F.h"
#include "ObjectChecker.h"
#include "Track.h"
#include <TGraph.h>
#include "VoxelV2.h"

using Slice = std::vector<std::vector<Tracking::Vector3D<double>>>;
using VoxelCenters = std::vector<Slice>;

using Tracking::Vector3D;
using Tomography::Track;


namespace Tomography{
namespace evolution{
class Voxelator{
private:
  Vector3D<double> fVoxelizedVolumeDim; // This should be set using the detector size
  Vector3D<double> fEachVoxelDim; // voxel width in x, y and z
  Vector3D<double> fVoxelatorDim;
  TH3F *histVoxelValue; // = new TH3F("glvoxel", "glvoxel", 50, -500., 500., 50, -500., 500., 50, -500., 500.);
  TH3F *histVoxelCount;
  TH1F *fVoxelsIn1D;
  TH1F *fVoxelsIn1DCount;

  //Some more histogram to that represent SD and RL for each voxel
  TH1F *fSDInVoxels;
  TH1F *fRLInVoxels;

  //Data member to store max and min SD and RL
  //Currently these will be used by EveVisualizer
  double fMinSD, fMaxSD;
  double fMinRL, fMaxRL;

  //TGraph *fGraphSD;// = new TGraph(dim, &x[0], &y[0]);


  ObjectChecker fObjChecker;

  std::vector<Tracking::Vector3D<double>> fVoxelCenters;


  // Hit point to VoxelizedVolume
  Vector3D<double> fHitPtInput;
  Vector3D<double> fHitPtOutput;


  //Some data member taken from Voxe. This is the correct place for them
  std::vector<int> fVisitedVoxelNumVector ;
  std::vector<Voxel_V2*> fVoxelVector ;

  static Voxelator *s_instance;
  Voxelator();
  Voxelator(double voxelizedVolHalfX,double voxelizedVolHalfY, double voxelizedVolHalfZ,
  			 double voxelX,double voxelY, double voxelZ);


public:
  static Voxelator *instance(double voxelizedVolHalfX,double voxelizedVolHalfY, double voxelizedVolHalfZ,
			 double voxelX,double voxelY, double voxelZ);
  static Voxelator *Create(double voxelizedVolHalfX,double voxelizedVolHalfY, double voxelizedVolHalfZ,
  			 double voxelX,double voxelY, double voxelZ);
  static Voxelator *instance();

  ObjectChecker GetObjectChecker(){return fObjChecker;}
  /*Voxelator();
  Voxelator(double voxelizedVolHalfX,double voxelizedVolHalfY, double voxelizedVolHalfZ,
			 double voxelX,double voxelY, double voxelZ);
*/
  ~Voxelator();
  void SetVoxelizedVolumeDim(double halfX,double halfY, double halfZ);
  void SetVoxelDim(double x,double y, double z);
  void SetVoxelatorDim(double x,double y, double z);
  void SetVoxelator(double voxelizedVolHalfX,double voxelizedVolHalfY, double voxelizedVolHalfZ,
  							 double voxelX,double voxelY, double voxelZ);

  //VoxelCenters
  TH1F* GetVoxelIn1D(){return fVoxelsIn1D;}
  TH1F* GetVoxelIn1DCount(){return fVoxelsIn1DCount;}
  TH1F* GetSDInVoxelsHist(){return fSDInVoxels;}
  TH1F* GetRLInVoxelsHist(){return fRLInVoxels;}
  void FillSDAndRLHist();
  void SetMaxMinSDAndRL();
  double GetMaxSD() const {return fMaxSD;}
  double GetMinSD() const {return fMinSD;}
  double GetMaxRL() const {return fMaxRL;}
  double GetMinRL() const {return fMinRL;}


  std::vector<Tracking::Vector3D<double>> GetVoxelCenters(){return fVoxelCenters; }
  Tracking::Vector3D<double> GetVoxelCenter(double x, double y, double z);
  Tracking::Vector3D<double> GetVoxelCenter(Tracking::Vector3D<double> vox);
  Tracking::Vector3D<double> GetVoxelCenter(int voxelNum);
  int GetVoxelNumber(int x, int y, int z);
  int GetVoxelNumber(double x,double y, double z);
  int GetVoxelNumber(Tracking::Vector3D<double> vox);
  int GetTotalNumberOfVoxels(){return fVoxelatorDim.x()*fVoxelatorDim.y()*fVoxelatorDim.z();}


  //Function to return the std::vector of filled Voxels
 std::vector<Voxel_V2*> GetVoxelVector() const {
	  //return Voxel::GetVoxelVector();
	  return fVoxelVector;
  }

 std::vector<int> GetVisitedVoxelNumVector() const {
 	  return fVisitedVoxelNumVector;
   }


  //Some new functions taken from Voxel. This is correct place for them
  int IfVoxelExist(int voxelNum);
  void Insert(Tracking::Vector3D<double> point, int voxelNum, bool useEnclosingVoxels = false , double scatteringDensity = 0.);
  //void Insert(Tracking::Vector3D<double>, int voxelNum);
  void Reset();
  std::vector<Voxel_V2*> GetFilteredVoxelVector();
  std::vector<Tracking::Vector3D<double>> GetFilteredPocaPtVector();





  // This needs NOT to be called by user program
  void CreateHistogram();
  void CalculateVoxelCenters();
  //void CreateSDGraph();

  void PrintVoxelCenters();


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
  bool IsGenuine(Tracking::Vector3D<double> pocaPt);

  Vector3D<double> GetVoxelizedVolumeDim(){return fVoxelizedVolumeDim;}
  Vector3D<double> GetEachVoxelDim(){return fEachVoxelDim;}
  Vector3D<double> GetVoxelatorDim(){return fVoxelatorDim;}




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


   //Vector3D<double> GetInsersection(Track t, double zVal, int select);

#ifdef FIND_CANDIDATE_VOXEL
  std::vector<int> FindCandidateVoxels(Track incoming, Track outgoing,
			Tracking::Vector3D<double> &inComingHitPt,
			Tracking::Vector3D<double> &outGoingHitPt);
#endif
/*
	VectorOfVoxelsForAnEvent vectOfVoxels;
  //Detecting HitPoints in Voxelized Volume
  Tracking::Vector3D<double> inComingHitPt = GetIntersection(incoming,-1.*(run->GetVoxelator().GetVoxelizedVolumeDim().z()/2.),1);
  Tracking::Vector3D<double> outGoingHitPt = GetIntersection(incoming,run->GetVoxelator().GetVoxelizedVolumeDim().z()/2.,2);
  std::cout << "InComingHitPoint : ";
  inComingHitPt.Print();
  std::cout << "OutgoingHitPoint : " ;
  outGoingHitPt.Print();
*/

  /*
   * Function to detect if the point lie within the VoxelizedRegion
   */


};

}//end of evolution namespace
} // end of Tomography namespace
#endif  // INC_VOXELATOR__

