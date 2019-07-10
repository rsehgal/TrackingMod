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
#include "VoxelNavigator.h"
#include "Delta.h"
#include <G4SystemOfUnits.hh>
#include "base/Global.h"


using Tracking::Vector3D;


namespace Tomography {
int EventHelper::fUnscatteredCounter = 0;

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
	std::cout<<"FABS of ScattAngle from EventHelper : " << std::fabs(fScatteringAngle) << "  : Momentum : " << fMomentum << std::endl;



	if (std::fabs(fScatteringAngle) < Tomography::unscatteringThreshold){
#ifdef FIND_CANDIDATE_VOXEL
		CalculateCandidateVoxelsForAcceptance();
#endif

#ifdef USE_UNSCATTERED_TRACKS
	//Threshold below which the tracks are considered unscattered, defined in Global.h

		fUnscatteredCounter++;
		CalculateVoxel_V3(incoming,outgoing);
#endif
		}else{

		CalculatePOCA();
		CalculateVoxel_V2();
#ifdef FIND_CANDIDATE_VOXEL
	CalculateCandidateVoxels();
	WriteToFile();
#endif
		}

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
	fTrackId = 0;
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
	double momentum = 0.;

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

	while (!infile.eof()) {
		fTrackId++;
		infile >> incomingTrackP1X >> incomingTrackP1Y >> incomingTrackP1Z
				>> incomingTrackP2X >> incomingTrackP2Y >> incomingTrackP2Z
				>> outgoingTrackP1X >> outgoingTrackP1Y >> outgoingTrackP1Z
				>> outgoingTrackP2X >> outgoingTrackP2Y >> outgoingTrackP2Z
				>> scattererHitted >> momentum;

		fMomentum = momentum;

#ifndef TPR_ANALYSIS
		if (scattererHitted == 1)
#endif
		{
			hitCounter++;

			Tomography::Track incoming(
					Tracking::Vector3D<double>(incomingTrackP1X,incomingTrackP1Y, incomingTrackP1Z),
					Tracking::Vector3D<double>(incomingTrackP2X,incomingTrackP2Y, incomingTrackP2Z));

			Tomography::Track outgoing(
					Tracking::Vector3D<double>(outgoingTrackP1X,outgoingTrackP1Y, outgoingTrackP1Z),
					Tracking::Vector3D<double>(outgoingTrackP2X,outgoingTrackP2Y, outgoingTrackP2Z));

			std::cout << "INCOMING : ";			incoming.Print();
			std::cout << "OUTGOING : ";			outgoing.Print();

			TestEventHelper(incoming, outgoing, fileToWrite);

			//if(!IsFalsePositivePoca<true>()){
			if (!CommonFunc::Functions::instance()->IsFalsePositivePoca<true>(fPocaPt)) {
				genuinePocaCounter++;
				Tomography::Files::instance()->Write(genuinefileName, 4,
						fPocaPt.x(), fPocaPt.y(), fPocaPt.z(),
						fPocaPt.GetColor());
			}
		}

	}

	Tomography::Files::instance()->Close(fileToWrite);
	Tomography::Files::instance()->Close(genuinefileName);
	//Tomography::Files::instance()->Close("InfoForMLEM.txt");
	infile.close();
	std::cout << "ActualHit Counter : " << hitCounter << " :: GenuinePoca Counter : " << genuinePocaCounter << std::endl;
	double par = double(genuinePocaCounter)/hitCounter;
	std::cout << "PoCAAccuracyRatio : " << par << std::endl;
	//double error = par/std::sqrt(genuinePocaCounter);
	double error = par*std::sqrt((1./double(genuinePocaCounter))+(1./double(hitCounter)));
	parHandle << par << " " << error << " ";
	parHandle.close();

}


/*
bool EventHelper::IsFalsePositivePoca(){
	Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::create("testMapping.txt");
	std::vector<Tomography::Mapping::Scatterer*>  scattererVector = detectorMap->GetScattererVector();

}
*/

void EventHelper::CalculatePOCA(){
	Tracking::Vector3D<double> test = fIm.POCA(fIncoming,fOutgoing);
    fPocaPt = test;
    fPocaPt.SetDoCA(test.GetDoCA());
	std::cout << "DOCA : " << fPocaPt.GetDoCA() << std::endl;


	fPocaPt.SetColor(fScatteringAngle);
	fPocaPt.SetTrackId(fTrackId);
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
	if(voxelNum < 0 || voxelNum > Tomography::evolution::Voxelator::instance()->GetTotalNumberOfVoxels())
		return;
	int voxNum = Tomography::evolution::Voxelator::instance()->IfVoxelExist(voxelNum);
	if (std::fabs(fScatteringAngle) > Tomography::unscatteringThreshold){
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
}

void EventHelper::CalculateVoxel_V3(Tomography::Track trackIncoming,Tomography::Track trackOutgoing){
std::cout << "Entered CalculateVoxel_V3 ..........." << std::endl;
std::cout << "Scattering VAlue : " << fScatteringAngle << std::endl;
	//if (fScatteringAngle < 1e-8)
	{


			Tomography::evolution::Voxelator *voxelator = Tomography::evolution::Voxelator::instance();
			Tracking::Vector3D<double> inComingHitPt = Delta::GetIntersection(trackIncoming,voxelator->GetVoxelizedVolumeDim().z()/2.,1);
			Tracking::Vector3D<double> outGoingHitPt = Delta::GetIntersection(trackOutgoing,-1*(voxelator->GetVoxelizedVolumeDim().z()/2.),2);
			std::cout << "Incoming Hit Point : " << __FILE__ <<" : " << __LINE__ <<"  :  "; inComingHitPt.Print();
			std::cout << "Outgoing Hit Point : " << __FILE__ <<" : " << __LINE__  << "  :  "; outGoingHitPt.Print();
			std::vector<int> cleanHittedVoxelNumVect = VoxelFinder(Tomography::Track(inComingHitPt,outGoingHitPt));
			for (int i = 0; i < cleanHittedVoxelNumVect.size(); i++) {
				if(cleanHittedVoxelNumVect[i] > voxelator->GetTotalNumberOfVoxels() || cleanHittedVoxelNumVect[i] < 0.){
					std::cout << "BAD Voxel Encountered, hence skipping the event......." << std::endl;
					return;
				}

				int voxelNum = cleanHittedVoxelNumVect[i];
				int voxNum = Tomography::evolution::Voxelator::instance()->IfVoxelExist(voxelNum);
				if (voxNum < 0. ) {
					Tomography::evolution::Voxelator::instance()->InsertCleanVoxel(voxelNum);
					//Tomography::evolution::Voxelator::instance()->Insert(Tracking::Vector3D<double>(0.,0.,0.),voxelNum);
				} else {
					Tomography::evolution::Voxelator::instance()->GetVoxelVector()[voxNum]->IncrementCleanCount(); // Insert(fPocaPt);
				}
			}

			Tomography::evolution::Voxelator::instance()->PrintCleanVoxelNumAndCleanCount(cleanHittedVoxelNumVect);


	}
}

std::vector<int> EventHelper::VoxelFinder(Tomography::Track track){

	std::cout << "Entered Voxel Finder : " << __FILE__ <<" : " << __LINE__ << std::endl;

	VoxelNavigator voxelNavigator;
	std::vector<int> cleanHittedVoxelNumVector;
	Tracking::Vector3D<double> dir = track.GetDirCosine();
	double epsilon = 1e-8;
	Tracking::Vector3D<double> startPoint = track.GetP1() + dir * epsilon;
	Tracking::Vector3D<double> endPoint = track.GetP2() - dir * epsilon;
	Tomography::evolution::Voxelator *voxelator = Tomography::evolution::Voxelator::instance();
	int endPointVoxelNum = voxelator->GetVoxelNumber(endPoint);
	int hitPointVoxelNum = voxelator->GetVoxelNumber(startPoint);
	std::cout << "HitPointVoxelNum : " << hitPointVoxelNum <<" : endPointVoxelNum : " << endPointVoxelNum << " : " << __FILE__ <<" : " << __LINE__ << std::endl;

	//The condition should be applied only to valid tracks
	bool valid=true;
	while(hitPointVoxelNum != endPointVoxelNum){
		int voxelNum = voxelator->GetVoxelNumber(startPoint);
		cleanHittedVoxelNumVector.push_back(voxelNum);
		Tracking::Vector3D<double> voxelCenter = voxelator->GetVoxelCenter(voxelNum,valid);
		double step = voxelNavigator.ComputeStep(startPoint, dir, voxelCenter);
		startPoint = startPoint + dir * (step+epsilon);
		hitPointVoxelNum = voxelator->GetVoxelNumber(startPoint);

	}
	cleanHittedVoxelNumVector.push_back(endPointVoxelNum);
	return cleanHittedVoxelNumVector;
}

#ifdef FIND_CANDIDATE_VOXEL
void EventHelper::CalculateCandidateVoxels(){
	fCandidateVoxelNumVector = Tomography::evolution::Voxelator::instance()->FindCandidateVoxels(fIncoming,fOutgoing,fIncomingHitPoint,fOutgoingHitPoint);
}

void EventHelper::CalculateCandidateVoxelsForAcceptance(){
	std::vector<int> candidateVoxelNumVectorForAcc = Tomography::evolution::Voxelator::instance()->FindCandidateVoxels(fIncoming);
}

void EventHelper::WriteToFile(){
	std::vector<Tracking::Vector3D<double>> voxelCenterVector;
	Tracking::Vector3D<double> dim = Tomography::evolution::Voxelator::instance()->GetEachVoxelDim();
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
		bool valid=true;
		Tracking::Vector3D<double> center = Tomography::evolution::Voxelator::instance()->GetVoxelCenter(fCandidateVoxelNumVector[i],valid);
		center.SetColor(4.);
		if(valid)
			voxelCenterVector.push_back(center);
	}

	//Now pushing PocaPt
	bool valid=true;
	Tracking::Vector3D<double> pocaCenter = Tomography::evolution::Voxelator::instance()->GetVoxelCenter(fPocaPt,valid);
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
