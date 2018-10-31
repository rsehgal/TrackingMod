/*
 * EventHelper.h
 *
 *  Created on: Aug 10, 2017
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_EVENTHELPER_H_
#define HELPERS_INC_EVENTHELPER_H_

#include "Track.h"
#include "base/Vector3D.h"
#include "Imaging.h"
#include "Voxel.h"
#include "DetectorMapping.h"
using Tracking::ImageReconstruction;

using Tomography::Track;
using Tracking::Vector3D;
namespace Tomography {

class EventHelper {
	Track fIncoming;
	Track fOutgoing;
	Vector3D<double> fPocaPt;
	double fScatteringAngle;
	ImageReconstruction fIm;
	std::vector<int> fCandidateVoxelNumVector;
	Tracking::Vector3D<double> fIncomingHitPoint;
	Tracking::Vector3D<double> fOutgoingHitPoint;
	int genuinePocaCounter ;

//#ifdef VOXELIZE
	Voxel *fVoxel;
//#endif


public:
	EventHelper();
	EventHelper(Track incoming, Track outgoing){}
	void Test2EventHelper(Track incoming, Track outgoing);
	EventHelper(Track incoming, Track outgoing, std::string filename){}
	void TestEventHelper(Track incoming, Track outgoing, std::string filename);

	//This is a very special constructor and will be used during offline analysis
	EventHelper(std::string fileToRead, std::string fileToWrite,bool forSimulation=true);

	//Getters
	Track GetIncomingVector() const {return fIncoming;}
	Track GetOutgoingVector() const {return fOutgoing;}
	Vector3D<double> GetPocaPt() const {return fPocaPt;}
	double GetScatteringAngle() const {return fScatteringAngle;}
	std::vector<int> GetCandidateVoxels()const{return fCandidateVoxelNumVector;}
//#ifdef VOXELIZE
	Voxel* GetVoxel(){return fVoxel;}
//#endif

	//Calculators
	void CalculatePOCA();
	void CalculateScatterAngle();
	void CalculateVoxel();
	void CalculateVoxel_V2();
	int GetVoxelNum();
	int GetVoxelNum(Tracking::Vector3D<double> pt);
	void CalculateCandidateVoxels();
	void WriteToFile();

	//To detect if the calculated PoCA is false positive or genuine
	//Will be useful only during Simulation,
	//Template parameter should be provided as true for Simulation and false
	//for real data processing, because in real scenario we don't know the location
	//of scatter os it should just all PoCA as truePositive and hence this function
	//should just return false
	template <bool ForSimulation>
	bool IsFalsePositivePoca(){
		bool truePositive = true;
		if(!ForSimulation)
			return !truePositive;
		else{
			//Logic to check if the Calculated PoCA lie with the extent of any scatterer
			std::vector<Tracking::Vector3D<double>> scattererMinExtentVector = Tomography::DetectorMapping::create("testMapping.txt")->GetScattererMinExtent();
			std::vector<Tracking::Vector3D<double>> scattererMaxExtentVector = Tomography::DetectorMapping::create("testMapping.txt")->GetScattererMaxExtent();

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

	virtual ~EventHelper();
};

} /* namespace Tomography */

#endif /* HELPERS_INC_EVENTHELPER_H_ */
