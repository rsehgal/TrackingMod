/*
 * EventHelper.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: rsehgal
 */

#include "EventHelper.h"
#include "CommonFunc.h"
#include "Voxelator_Evolution.h"

using Tracking::Vector3D;

namespace Tomography {

EventHelper::EventHelper() {
	// TODO Auto-generated constructor stub

}

EventHelper::~EventHelper() {
	// TODO Auto-generated destructor stub
}

EventHelper::EventHelper(Track incoming, Track outgoing){
	fIncoming = incoming;
	fOutgoing = outgoing;
	CalculateScatterAngle();
	//std::cout<<"ScattAngle from EventHelper : " << fScatteringAngle << std::endl;
	CalculatePOCA();
	CalculateVoxel();


	//Uncomment them if one want Candidate voxel for an event
	//CalculateCandidateVoxels();
	//WriteToFile();

}

void EventHelper::CalculatePOCA(){
	fPocaPt = fIm.POCA(fIncoming,fOutgoing);
	//std::cout << "Scattering NewArch : " << fScatteringAngle << std::endl;
	fPocaPt.SetColor(fScatteringAngle);
	//std::cout<< "POCA from EventHELPER : " ; fPocaPt.Print();
}

void EventHelper::CalculateScatterAngle(){
	//fScatteringAngle = CommonFunc::Functions::instance()->GetAngleInRadian(fIncoming,fOutgoing);
	Tomography::Track ref(Vector3D<double>(0.,0.,0.),Vector3D<double>(0.,0.,1.));
	double angleIncoming = CommonFunc::Functions::instance()->GetAngleInRadian(fIncoming,ref);
	double angleOutgoing = CommonFunc::Functions::instance()->GetAngleInRadian(fOutgoing,ref);
	fScatteringAngle = angleOutgoing-angleIncoming;
	//std::cout << "ScatteringAngle from EventHELPER : " << fScatteringAngle << std::endl;
}

void EventHelper::CalculateVoxel(){
	int voxelNum = GetVoxelNum();
	int voxNum = Voxel::IfVoxelExist(voxelNum);
    //std::cout<<"VoxNum : " << voxNum << std::endl;
	if(voxNum < 0.){
		fVoxel = new Voxel(fPocaPt,voxelNum);
		//oxel::InsertVoxel(new Voxel(fPocaPt,voxelNum),voxelNum);
	}else{
		//Voxel::GetVoxelVector()[voxNum]->Insert(fPocaPt,voxelNum);
		fVoxel = Voxel::GetVoxelVector()[voxNum];
		fVoxel->Insert(fPocaPt,voxelNum);
	}
}

void EventHelper::CalculateCandidateVoxels(){
	fCandidateVoxelNumVector = Tomography::evolution::Voxelator::instance()->FindCandidateVoxels(fIncoming,fOutgoing,fIncomingHitPoint,fOutgoingHitPoint);
}

void EventHelper::WriteToFile(){
	std::vector<Tracking::Vector3D<double>> voxelCenterVector;
	Tracking::Vector3D<int> dim = Tomography::evolution::Voxelator::instance()->GetEachVoxelDim();
	//Pushing Voxel Dimension
	voxelCenterVector.push_back(Tracking::Vector3D<double>((double)dim.x(),(double)dim.y(),(double)dim.z()));
	voxelCenterVector.push_back(fIncomingHitPoint);
	voxelCenterVector.push_back(fOutgoingHitPoint);
	voxelCenterVector.push_back(fPocaPt);


//Uncomment block below to store all voxel informations.
/*
	for(int i = 0 ;  i <  Tomography::Voxelator::instance()->GetTotalNumberOfVoxels() ; i++ ){
		Tracking::Vector3D<double> center = Tomography::Voxelator::instance()->GetVoxelCenter(i);
		center.SetColor(0);
		voxelCenterVector.push_back(center);

	}
*/


	for(int i = 0 ; i < fCandidateVoxelNumVector.size() ; i++){
		std::cout<< (i+1) <<" Candidate Voxel Num : " << fCandidateVoxelNumVector[i] << std::endl;
		Tracking::Vector3D<double> center = Tomography::evolution::Voxelator::instance()->GetVoxelCenter(fCandidateVoxelNumVector[i]);
		center.SetColor(4.);
		voxelCenterVector.push_back(center);
	}

	//Now pushing PocaPt
	Tracking::Vector3D<double> pocaCenter = Tomography::evolution::Voxelator::instance()->GetVoxelCenter(fPocaPt);
	//intentionally setting blue color for pocalVoxel
	pocaCenter.SetColor(2.);
	voxelCenterVector.push_back(pocaCenter);

	CommonFunc::Functions::instance()->WriteToFile("candidatesVoxels.txt",voxelCenterVector);
}

int EventHelper::GetVoxelNum(){
	int voxNum = Tomography::evolution::Voxelator::instance()->GetVoxelNumber(fPocaPt); //logica to calculate voxel num
	return voxNum;
}

} /* namespace Tomography */
