/*
 * RunHelper.h
 *
 *  Created on: Aug 10, 2017
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_RUNHELPER_H_
#define HELPERS_INC_RUNHELPER_H_

#include <vector>
#include "base/Vector3D.h"
#include "VoxelV2.h"
#include "Voxelator_Evolution.h"
#include "EventHelper.h"
#include "TH1F.h"
#include "TH3F.h"
#include <TMath.h>
//#include "Voxel.h"

using Tracking::Vector3D;

namespace Tomography {

class RunHelper {

	TH1F *fWeightedCountHist;
	TH1F *fNormalizedWeightedHist;
	//The histogram is actually of normalization of mean of scattering value
	TH1F *fNormalizedScatteringValueHist;

	//Bare Scattering angle histogram just to visualize scattering info
	TH1F *fBareScatteringAngleHist;

	//Some additional helper histogram
	TH1F *fWHist;

	//Introduced during second round of review
	  TH1F *fHistOfCount;
	  TH3F *fHist3DCount;
	  TH2F *fHist2DXY;
	  TH2F *fHist2DYZ;
	  TH2F *fHist2DXZ;
	  TH1F *fHistVoxelSD;
	  TH1F *fHistVoxelsScattering;




 	std::vector<double> fScatteringAngleVector;
	std::vector<Vector3D<double>> fPocaPtVector;
	std::vector<double> fSDOfVoxelsVector;
	std::vector<double> fRLOfVoxelsVector;
	//std::vector<Voxel_V2*> fVoxelVector;
	std::string fFileType;

	//Data member to store TruePositive PocaPt and false positive PocaPt
	std::vector<Vector3D<double>> fTruePositivePocaPtVector;
	std::vector<Vector3D<double>> fFalsePositivePocaPtVector;
	int fTruePositiveCount;
	int fFalsePositiveCount;

	//Data member to store max and min SD and RL
	//Currently these will be used by EveVisualizer
	double fMinSD, fMaxSD;
	double fMinRL, fMaxRL;

	//To make RunHelper SingleTon
	RunHelper();
	RunHelper(std::string fileType);
	static RunHelper *s_instance;

#ifdef VOXELIZE
	//std::vector<Voxel*> fVoxelVector;
#endif


 // std::vector<EventHelper*> fEventHelperVector;
public:
	std::vector<Voxel_V2*> fVoxelVector;
	static RunHelper *instance(std::string filename);
	static RunHelper *instance();
//	RunHelper();
//	RunHelper(std::string fileType);

	std::vector<double> GetScatteringAngleVector() const {return fScatteringAngleVector;}
#ifdef VOXELIZE
	std::vector<Voxel_V2*> GetVoxelVector() const {return fVoxelVector;}
#endif
	std::vector<Vector3D<double>> GetPocaPtVector() const {return
			fPocaPtVector;
	}

	void Insert(double scatteringAngle){ fScatteringAngleVector.push_back(scatteringAngle);}
	void Insert(Vector3D<double> pocaPt) { fPocaPtVector.push_back(pocaPt);}
	void FillScatteringAngleVector();
	void FillPocaVector();
	void FillSDVector();
	void CalcSDOfEachVoxel();
	void CalcRLOfEachVoxel();
	void FillRLVector();
	void WriteToFile();
	void SetMaxMinSDAndRL();

	//Getters
	double GetMaxSD() const {return fMaxSD;}
	double GetMinSD() const {return fMinSD;}
	double GetMaxRL() const {return fMaxRL;}
	double GetMinRL() const {return fMinRL;}
	void PrintMaxMinSDAndRL() const;

	//Detect True and False positive PocaPts
	void DetectorTrueAndFalsePositive();
	std::vector<Tracking::Vector3D<double>> GetTruePositivePocaPtVector()const {return fTruePositivePocaPtVector;}
	std::vector<Tracking::Vector3D<double>> GetFalsePositivePocaPtVector()const {return fFalsePositivePocaPtVector;}
	int GetTruePositivePocaPtCount()const {return fTruePositiveCount;}
	int GetFalsePositivePocaPtCount()const {return fFalsePositiveCount;}

	//Filtering based on ThetaCut
	std::vector<Tracking::Vector3D<double>> ThetaCutFiltering();

	int GetCountInAVoxel(unsigned int voxelNum) const {return fVoxelVector[voxelNum]->GetPointCount();}

	//Function used to do resetting
	void Reset();
#ifdef STORE
	void Store();
#endif

#ifdef VOXELIZE
	//void Insert(Voxel *obj){ fVoxelVector.push_back(obj); }
#endif


//	std::vector<EventHelper*> GetEventHelperVector(){return fEventHelperVector;}
//	void Insert(EventHelper *obj){fEventHelperVector.push_back(obj);}
	virtual ~RunHelper();
	static int npeaks;

	static Double_t fpeaks2(Double_t *x, Double_t *par) {
	   Double_t result = 0.1;
	   for (Int_t p=0;p<npeaks;p++) {
	      Double_t norm   = par[5*p+0];
	      Double_t mean1  = par[5*p+1];
	      Double_t sigma1 = par[5*p+2];
	      Double_t mean2  = par[5*p+3];
	      Double_t sigma2 = par[5*p+4];
	      result += norm*TMath::Gaus(x[0],mean1,sigma1)*TMath::Gaus(x[1],mean2,sigma2);
	   }
	   return result;
	}
};

} /* namespace Tomography */

#endif /* HELPERS_INC_RUNHELPER_H_ */
