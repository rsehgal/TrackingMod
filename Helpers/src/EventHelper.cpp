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
#ifdef FIND_CANDIDATE_VOXEL
	CalculateCandidateVoxels();
	WriteToFile();
#endif


}

void EventHelper::CalculatePOCA(){
	fPocaPt = fIm.POCA(fIncoming,fOutgoing);
	//std::cout << "Scattering NewArch : " << fScatteringAngle << std::endl;
	fPocaPt.SetColor(fScatteringAngle);
	std::cout<< "POCA from EventHELPER : " ; fPocaPt.Print();
}

void EventHelper::CalculateScatterAngle(){
	//fScatteringAngle = CommonFunc::Functions::instance()->GetAngleInRadian(fIncoming,fOutgoing);
	Tomography::Track ref(Vector3D<double>(0.,0.,0.),Vector3D<double>(0.,0.,-1.));
	double angleIncoming = CommonFunc::Functions::instance()->GetAngleInRadian(fIncoming,ref);
	double angleOutgoing = CommonFunc::Functions::instance()->GetAngleInRadian(fOutgoing,ref);
	fScatteringAngle = angleOutgoing-angleIncoming;
	//std::cout << "ScatteringAngle from EventHELPER : " << fScatteringAngle << std::endl;
}

/*
 * If point lie outside the voxelized region then it should be rejected
 * there itself. This will make the processing faster
 */
void EventHelper::CalculateVoxel(){
	int voxelNum = GetVoxelNum();
	int voxNum = Voxel::IfVoxelExist(voxelNum);
    //std::cout<<"VoxNum : " << voxNum << std::endl;

	//Checking if the POCA point lie within the Voxelized Region,
	//If yes then only insert it in the desired Voxel.
	if(Tomography::evolution::Voxelator::instance()->IsGenuine(fPocaPt)){
		if(voxNum < 0.){
			fVoxel = new Voxel(fPocaPt,voxelNum);
		//oxel::InsertVoxel(new Voxel(fPocaPt,voxelNum),voxelNum);
		}else{
		//Voxel::GetVoxelVector()[voxNum]->Insert(fPocaPt,voxelNum);
			fVoxel = Voxel::GetVoxelVector()[voxNum];
			fVoxel->Insert(fPocaPt,voxelNum);
		}
	}

/*
//#define USE_ENCLOSING_VOXELS
#ifdef USE_ENCLOSING_VOXELS
	std::vector<Tracking::Vector3D<double>> imaginaryVoxelsCorners = fVoxel->GetEightCorners_Of_ImaginaryVoxel_CentredAtPocaPoint(fPocaPt);
	std::vector<Voxel*> enclosingVoxelsVector;
	double sumOfInverseDistancesFromEnclosingVoxels = 0.;
	for(int i =0 ; i  < imaginaryVoxelsCorners.size() ; i++){
		voxelNum = GetVoxelNum(imaginaryVoxelsCorners[i]);
		voxNum = Voxel::IfVoxelExist(voxelNum);
		if(voxNum < 0.){
			enclosingVoxelsVector.push_back(new Voxel(imaginaryVoxelsCorners[i],voxelNum,true));
			//oxel::InsertVoxel(new Voxel(fPocaPt,voxelNum),voxelNum);
		}else{
			enclosingVoxelsVector.push_back(Voxel::GetVoxelVector()[voxNum]);
		}
		Tracking::Vector3D<double> encVoxCenter = enclosingVoxelsVector[i]->GetVoxelCenter();
		Tracking::Vector3D<double> corner = imaginaryVoxelsCorners[i];
//		std::cout<<"@@@@@@@@ POCAPt : " << fPocaPt.x() <<" , "<< fPocaPt.y() <<" , "<< fPocaPt.z() << std::endl;
//		std::cout<<"@@@@@@@@ encVoxCenter : " << encVoxCenter.x() <<" , "<< encVoxCenter.y() <<" , "<< encVoxCenter.z() << std::endl;
//		std::cout<<"@@@@@@@@ ImaginaryVoxelCorner : " << corner.x() <<" , "<< corner.y() <<" , "<< corner.z() << std::endl;
//		std::cout<<"@@@@@@@@ RAMAN DIST : " << CommonFunc::Distance(enclosingVoxelsVector[i]->GetVoxelCenter(),fPocaPt) << std::endl;
//		std::cout<<"@@@@@@@@ SHACHI InvDIST : " << 1./CommonFunc::Distance(enclosingVoxelsVector[i]->GetVoxelCenter(),fPocaPt) << std::endl;
		sumOfInverseDistancesFromEnclosingVoxels += 1./CommonFunc::Distance(enclosingVoxelsVector[i]->GetVoxelCenter(),fPocaPt);
	}

	std::cout<< " @@@@@@@@@ AYUSH sumOfInverseDistancesFromEnclosingVoxels : " << sumOfInverseDistancesFromEnclosingVoxels << std::endl;

	for(int i =0 ; i  < enclosingVoxelsVector.size() ; i++){
		enclosingVoxelsVector[i]->SetScatteringDensity((1./CommonFunc::Distance(enclosingVoxelsVector[i]->GetVoxelCenter(),
														fPocaPt))/sumOfInverseDistancesFromEnclosingVoxels);

	}


	 * Above logic to calculate the inverse ratios, is taken from table from the following link
	 * https://math.stackexchange.com/questions/2479777/find-the-inverse-ratio-of-a-group-of-numbers


#endif
*/
}

#ifdef FIND_CANDIDATE_VOXEL
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
#endif

int EventHelper::GetVoxelNum(){
	int voxNum = Tomography::evolution::Voxelator::instance()->GetVoxelNumber(fPocaPt); //logica to calculate voxel num
	return voxNum;
}

int EventHelper::GetVoxelNum(Tracking::Vector3D<double> pt){
	int voxNum = Tomography::evolution::Voxelator::instance()->GetVoxelNumber(pt); //logica to calculate voxel num
	return voxNum;
}

} /* namespace Tomography */
