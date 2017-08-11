/*
 * EventHelper.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: rsehgal
 */

#include "EventHelper.h"
#include "CommonFunc.h"
#include "Voxelator_Evolution.h"

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
	CalculatePOCA();
	CalculateVoxel();

}

void EventHelper::CalculatePOCA(){
	fPocaPt = fIm.POCA(fIncoming,fOutgoing);
}

void EventHelper::CalculateScatterAngle(){
	fScatteringAngle = CommonFunc::Functions::GetAngleInRadian(fIncoming,fOutgoing);
}

void EventHelper::CalculateVoxel(){
	int voxelNum = GetVoxelNum();
	int voxNum = Voxel::IfVoxelExist(voxelNum);
    std::cout<<"VoxNum : " << voxNum << std::endl;
	if(voxNum < 0.){
		fVoxel = new Voxel(fPocaPt,voxelNum);
		//oxel::InsertVoxel(new Voxel(fPocaPt,voxelNum),voxelNum);
	}else{
		//Voxel::GetVoxelVector()[voxNum]->Insert(fPocaPt,voxelNum);
		fVoxel = Voxel::GetVoxelVector()[voxNum];
		fVoxel->Insert(fPocaPt,voxelNum);
	}
}

int EventHelper::GetVoxelNum(){
	int voxNum = Voxelator::instance()->GetVoxelNumber(fPocaPt); //logica to calculate voxel num
	return voxNum;
}

} /* namespace Tomography */
