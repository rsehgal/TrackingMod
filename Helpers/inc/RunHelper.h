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
//#include "Voxel.h"

using Tracking::Vector3D;

namespace Tomography {

class RunHelper {

 	std::vector<double> fScatteringAngleVector;
	std::vector<Vector3D<double>> fPocaPtVector;
	std::vector<double> fSDOfVoxelsVector;
	std::vector<double> fRLOfVoxelsVector;
	std::vector<Voxel_V2*> fVoxelVector;
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
};

} /* namespace Tomography */

#endif /* HELPERS_INC_RUNHELPER_H_ */
