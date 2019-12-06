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
#include <fstream>
//#include "Delta.h"

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

  TH1F *fWeightedCountHist;

  //Added another histogram to use the idea for selecting threshold based on lower layers
  TH1F *fNoisyLayerPointCountHist;


  //Histogram of Normlaized Weighted counts
  //TH1F* fNormalizedWeightedHist;

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

  /*
   * This data member is required for acceptance calculation,
   * where we want to know the counts in individual voxel,
   *
   *  ** It has NOTHING to do with Voxel_V2 **
   *
   *  The length of this should be equal to the total number
   *  of Voxels, and vector index represent Voxel index.
   *
   *  Now the value at individual index represent the muon counts
   *  that traversed this voxel of particular index
   */
  std::vector<unsigned int> fEachVoxelCountForAcceptance;


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


  //Data member to store the predicted threshold.
  int fThresholdVal;

  double fWeightedThresholdVal;


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
  Tracking::Vector3D<double> GetVoxelCenter(double x, double y, double z, bool &valid);
  Tracking::Vector3D<double> GetVoxelCenter(Tracking::Vector3D<double> vox, bool &valid);
  Tracking::Vector3D<double> GetVoxelCenter(int voxelNum, bool &valid);
  int GetVoxelNumber(int x, int y, int z);
  int GetVoxelNumber(double x,double y, double z);
  int GetVoxelNumber(Tracking::Vector3D<double> vox);
  int GetTotalNumberOfVoxels(){return fVoxelatorDim.x()*fVoxelatorDim.y()*fVoxelatorDim.z();}
  std::vector<unsigned int> GetEachVoxelCountForAcceptance() const {return fEachVoxelCountForAcceptance;}


  //Function to return the std::vector of filled Voxels
 std::vector<Voxel_V2*> GetVoxelVector() const {
	  //return Voxel::GetVoxelVector();
	  return fVoxelVector;
  }

 std::vector<int> GetVisitedVoxelNumVector() const {
 	  return fVisitedVoxelNumVector;
   }

 Voxel_V2* GetVoxelFromVoxelator(int voxelNum){
	 for(int i = 0 ; i < fVoxelVector.size(); i++){
		 if(fVoxelVector[i]->GetVoxelNum() == voxelNum){
			 return fVoxelVector[i];
		 }
	 }
	 std::cerr << "REQUIRED Voxel not found in fVoxelVector of Voxelator...... Kindly check......." << std::endl;
	 exit(1);
 }

 void PrintCleanVoxelNumAndCleanCount(std::vector<int> cleanHittedVoxelNumVector);

  //Some new functions taken from Voxel. This is correct place for them
  int IfVoxelExist(int voxelNum);
  void Insert(Tracking::Vector3D<double> point, int voxelNum, bool useEnclosingVoxels = false , double scatteringDensity = 0.);
  //void Insert(Tracking::Vector3D<double>, int voxelNum);
  void InsertCleanVoxel(int voxelNum);

  void Reset();
  std::vector<Voxel_V2*> GetFilteredVoxelVector();
  std::vector<Tracking::Vector3D<double>> GetFilteredPocaPtVector();
  //Getting filtered PoCA from the specifed filteredVoxelVector
  std::vector<Tracking::Vector3D<double>> GetFilteredPocaPtVector(std::vector<Voxel_V2*> filteredVoxelVector);

  //Another Clean Voxel based filteration function
  std::vector<Voxel_V2*> GetFilteredVoxelVectorUsingCleanVoxel();
  //Another version that will filter the specified Voxel Vector based on Clean Voxel
  std::vector<Voxel_V2*> GetFilteredVoxelVectorUsingCleanVoxel(std::vector<Voxel_V2*> voxelVector);

  //std::vector<Tracking::Vector3D<double>>
  void GetFilteredPocaPtVectorUsingCleanedVoxel(std::vector<Tracking::Vector3D<double>> &pocaPtVector);

  std::vector<unsigned int> GetFilteredTrackIndexVector();

  //Function to select the voxel based on the fWeightedCount of the Voxel
  std::vector<Voxel_V2*> GetFilteredVoxelVectorBasedOnWeightedCount();
  std::vector<Voxel_V2*> GetFilteredVoxelVectorBasedOnWeightedCount(TH1F *hist);

  //Function to calculate the NormalizedCount in a Voxel
  void NormalizeEachVoxelCount();
  //Function to select the voxel based on the fNormalizedCount of the Voxel
  std::vector<Voxel_V2*> GetFilteredVoxelVectorBasedOnNormalizedCount();

  //Function to calculate NormalizedScattering in each voxel
  void NormalizeEachVoxelScatteringValue();
  //Function to select the voxel based on the fNormalizedCount of the Voxel
  std::vector<Voxel_V2*> GetFilteredVoxelVectorBasedOnNormalizedScatteringValue();





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
  void InsertCleanVoxels(std::vector<Vector3D<double>> ptVect);
  void AverageOut();
  TH3F* GetVoxelizedHist(){return histVoxelValue;}
  TH3F* GetVoxelizedCount(){return histVoxelCount;}
  bool IsGenuine(Tracking::Vector3D<double> pocaPt);

  Vector3D<double> GetVoxelizedVolumeDim(){return fVoxelizedVolumeDim;}
  Vector3D<double> GetEachVoxelDim(){return fEachVoxelDim;}
  Vector3D<double> GetVoxelatorDim(){return fVoxelatorDim;}

  //Member function to predict the threshold
  void PredictThreshold();
  int GetThresholdVal() const {return fThresholdVal;}

  //Member function to predict the threshold for Weighted Count
  void PredictWeightedThreshold(TH1F *hist);
  double GetWeightedThresholdVal() const {return fWeightedThresholdVal;}




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

  // This version is for Acceptance calculation, and hence will make use of only the
  // incoming track to find the candidate voxel and return a vector of their index
  std::vector<int> FindCandidateVoxels(Track incoming);

  //Function to test the total count in all the voxel,
  //can be used for simple test like when all muon comes vertically down
  //without any scatterer.
  unsigned int GetTotalVoxelCountForAcceptance(){
	  unsigned int sum_of_elems = 0;
	  for (auto& n : fEachVoxelCountForAcceptance)
	      sum_of_elems += n;
	  return sum_of_elems;
  }

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

/*
void TestWrite(){
	std::ofstream outfile("filteredTestWrite.txt");
	int voxelVectorSize = fVoxelVector.size();
	outfile << "VoxelVectorSize : " << voxelVectorSize << std::endl;
	for(int i = 0 ; i < voxelVectorSize; i++){
		outfile << "VoxelIndex : " << i <<" :: VoxelNum : " << fVoxelVector[i]->GetVoxelNum() << " :: IsVisited : " <<  std::endl;
		std::vector<Tracking::Vector3D<double>> ptVector = fVoxelVector[i]->GetPocaPointsVector();
		for(int j = 0 ; j < ptVector.size() ; j++){
			outfile << ptVector[j].x() << "  " << ptVector[j].y() << "  " << ptVector[j].z() << " 0.0" <<  std::endl;
		}
	}
	outfile.close();
}
*/

};

}//end of evolution namespace
} // end of Tomography namespace
#endif  // INC_VOXELATOR__

