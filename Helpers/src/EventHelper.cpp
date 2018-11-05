/*
 * EventHelper.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: rsehgal
 */

#include "EventHelper.h"
#include "CommonFunc.h"
#include "Voxelator_Evolution.h"
#include "Files.h"
#include "DetectorMapping.h"

using Tracking::Vector3D;

namespace Tomography {


EventHelper::EventHelper() {
	// TODO Auto-generated constructor stub

}

EventHelper::~EventHelper() {
	// TODO Auto-generated destructor stub
}

void EventHelper::Test2EventHelper(Track incoming, Track outgoing){
	fIncoming = incoming;
	fOutgoing = outgoing;
	CalculateScatterAngle();
	//std::cout<<"ScattAngle from EventHelper : " << fScatteringAngle << std::endl;
	if(fScatteringAngle!=0.){
		CalculatePOCA();
		CalculateVoxel_V2();
	}


	//Uncomment them if one want Candidate voxel for an event
#ifdef FIND_CANDIDATE_VOXEL
	CalculateCandidateVoxels();
	WriteToFile();
#endif


}

//void EventHelper::TestEventHelper(Track incoming, Track outgoing, std::string filename) : EventHelper(incoming,outgoing){
void EventHelper::TestEventHelper(Track incoming, Track outgoing, std::string filename) {
	Test2EventHelper(incoming,outgoing);
	if(fScatteringAngle!=0.){
	Tomography::Files::instance()->Write(filename,4, fPocaPt.x(), fPocaPt.y(),
																   fPocaPt.z(), fPocaPt.GetColor());

	//Storing InfoForMLEM, required by TUSHAR
	Tomography::Track ref(Tracking::Vector3D<double>(0.,0.,0.),Tracking::Vector3D<double>(0.,0.,-1.));
	double inAngle = CommonFunc::Functions::instance()->GetAngleInRadian(incoming,ref);
	double outAngle = CommonFunc::Functions::instance()->GetAngleInRadian(outgoing,ref);
/*
	Tomography::Files::instance()->Write("InfoForMLEM.txt",12,
										 incoming.GetP2().x(),incoming.GetP2().y(),incoming.GetP2().z(),
										 inAngle,
										 outgoing.GetP1().x(),outgoing.GetP1().y(),outgoing.GetP1().z(),
										 outAngle,
										 fPocaPt.x(), fPocaPt.y(),fPocaPt.z(),
										 fScatteringAngle);
*/
	//if(!IsFalsePositivePoca<true>()){
		//		genuinePocaCounter++;
			//	std::cout << "Genunie PocaPt Counter : " << genuinePocaCounter << std::endl;
	//}
	}
}

EventHelper::EventHelper(std::string fileToRead, std::string fileToWrite,bool forSimulation){
	genuinePocaCounter = 0;
	/* This should go through the event loop and call other constructors
	 * and create the desired file
	 */
	std::ifstream infile(fileToRead);
	//12 variable to read xyz for two tracks
	double incomingTrackP1X = 0.,incomingTrackP1Y = 0.,incomingTrackP1Z = 0.;
	double incomingTrackP2X = 0.,incomingTrackP2Y = 0.,incomingTrackP2Z = 0.;
	double outgoingTrackP1X = 0.,outgoingTrackP1Y = 0.,outgoingTrackP1Z = 0.;
	double outgoingTrackP2X = 0.,outgoingTrackP2Y = 0.,outgoingTrackP2Z = 0.;
	double scattererHitted = 0.;

	//Opening the file to store All PocaPt
	Tomography::Files::instance()->Open(fileToWrite,Tomography::operation::write);

	std::string filename = CommonFunc::Functions::instance()->GetFileName(fileToRead);

	if(forSimulation){
	//Opening the file to store P2 of incoming track, P1 of outgoing track, PoCA Point
	//and scattering angle. Required by TUSHAR, may be useful information for MLEM.
	Tomography::Files::instance()->Open(filename+"-InfoForMLEM.txt",Tomography::operation::write);
	}

	/* Opening the file to store Only Genuine PocaPt
	 * Genuinie PoCA points are those which lie within the scatterer region
	 *
	 * Can be done only when you know the exact location of scatterers,
	 * For Simulation certainly yes, but for real experiments,
	 * it may be useful for test, when we know the location of scatterer.
	 */

	std::string genuinefileName = filename+"PocaPtGenuine.txt";
	Tomography::Files::instance()->Open(genuinefileName,Tomography::operation::write);


	//Resetting Voxel in the beginning of event loop
    //Voxel::Reset();
	//Resetting Voxelator in the beginning of event loop
	//Tomography::evolution::Voxelator::instance()->Reset();

	int hitCounter = 0.;
	//int genuinePocaCounter = 0;

	std::ofstream parHandle("Par.txt",std::ios::app);

	while(!infile.eof()){
		infile >> incomingTrackP1X >> incomingTrackP1Y >> incomingTrackP1Z
			   >> incomingTrackP2X >> incomingTrackP2Y >> incomingTrackP2Z
			   >> outgoingTrackP1X >> outgoingTrackP1Y >> outgoingTrackP1Z
			   >> outgoingTrackP2X >> outgoingTrackP2Y >> outgoingTrackP2Z
			   >> scattererHitted;

		if(scattererHitted == 1)
			hitCounter++;

		Tomography::Track incoming(Tracking::Vector3D<double>(incomingTrackP1X,incomingTrackP1Y,incomingTrackP1Z),
								   Tracking::Vector3D<double>(incomingTrackP2X,incomingTrackP2Y,incomingTrackP2Z));

		Tomography::Track outgoing(Tracking::Vector3D<double>(outgoingTrackP1X,outgoingTrackP1Y,outgoingTrackP1Z),
								   Tracking::Vector3D<double>(outgoingTrackP2X,outgoingTrackP2Y,outgoingTrackP2Z));

		std::cout << "INCOMING : " ; incoming.Print();
		std::cout << "OUTGOING : " ; outgoing.Print();


		TestEventHelper(incoming,outgoing,fileToWrite);

		//if(!IsFalsePositivePoca<true>()){
		if(!CommonFunc::Functions::instance()->IsFalsePositivePoca<true>(fPocaPt)){
			genuinePocaCounter++;
			Tomography::Files::instance()->Write(genuinefileName,4, fPocaPt.x(), fPocaPt.y(),
																			   fPocaPt.z(), fPocaPt.GetColor());
		}

	}

	Tomography::Files::instance()->Close(fileToWrite);
	Tomography::Files::instance()->Close(genuinefileName);
	//Tomography::Files::instance()->Close("InfoForMLEM.txt");
	infile.close();
	std::cout << "ActualHit Counter : " << hitCounter << " :: GenuinePoca Counter : " << genuinePocaCounter << std::endl;
	double par = double(genuinePocaCounter)/hitCounter;
	std::cout << "PoCAAccuracyRatio : " << par << std::endl;
	parHandle << par << " ";
	parHandle.close();

}


/*
bool EventHelper::IsFalsePositivePoca(){
	Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::create("testMapping.txt");
	std::vector<Tomography::Mapping::Scatterer*>  scattererVector = detectorMap->GetScattererVector();

}
*/

void EventHelper::CalculatePOCA(){
	fPocaPt = fIm.POCA(fIncoming,fOutgoing);
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
	if(Tomography::evolution::Voxelator::instance()->IsGenuine(fPocaPt)){
	int voxelNum = GetVoxelNum();
	int voxNum = Voxel::IfVoxelExist(voxelNum);
    	if(voxNum < 0.){
			fVoxel = new Voxel(fPocaPt,voxelNum);
		}else{
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

void EventHelper::CalculateVoxel_V2(){
	bool isGenuine = Tomography::evolution::Voxelator::instance()->IsGenuine(fPocaPt);
	if(isGenuine){
	int voxelNum = GetVoxelNum();
	int voxNum = Tomography::evolution::Voxelator::instance()->IfVoxelExist(voxelNum);
    	if(voxNum < 0.){
			//fVoxel = new Voxel(fPocaPt,voxelNum);
    		//Tomography::evolution::Voxelator::instance()->Insert(fPocaPt,voxelNum);
    		//Tomography::evolution::Voxelator::instance()->GetVoxelVector().push_back(new Voxel(fPocaPt,voxelNum));
    		//Tomography::evolution::Voxelator::instance()->GetVisitedVoxelNumVector().push_back(voxelNum);
    		Tomography::evolution::Voxelator::instance()->Insert(fPocaPt,voxelNum);
		}else{
			//fVoxel = Voxel::GetVoxelVector()[voxNum];
			Tomography::evolution::Voxelator::instance()->GetVoxelVector()[voxNum]->Insert(fPocaPt);
			//fVox->Insert(fPocaPt,voxelNum);
		}
	}
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
