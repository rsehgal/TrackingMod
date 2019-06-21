/*
 * MLEM.cpp
 *
 *  Created on: 19-Dec-2018
 *      Author: rsehgal
 */

#include "MLEM.h"
#include "Track.h"
#include <TMath.h>
#include "Track.h"
#include "Imaging.h"
#include "CommonFunc.h"
#include "Voxelator_Evolution.h"
#include "Delta.h"
#include <G4SystemOfUnits.hh>
#include "VoxelNavigator.h"
#include "ScatteringDensityV2.h"
//#include "VoxelV2.h"

namespace Tomography {

MLEM::MLEM() {
	// TODO Auto-generated constructor stub
	Tomography::ScatteringDensityV2::instance()->Create()->Initialize();

}

MLEM::MLEM(std::string tracksFileName) {
	// TODO Auto-generated constructor stub
	fTracksFileName = tracksFileName;
}

MLEM::~MLEM() {
	// TODO Auto-generated destructor stub

}

void MLEM::DoExpectation(){

}

void MLEM::DoMaximization(){

}

void MLEM::Print(){
	for(int i = 0 ; i < fVectorOfLTVector.size() ; i++){
		std::cout <<"====================== MUON : " << i <<" ========================="<< std::endl;
		for(int j = 0 ; j < fVectorOfLTVector[i].size() ; j++){
			fVectorOfLTVector[i][j].Print();
		}

	}
}

void MLEM::Print2(){
	for(int i = 0 ; i < fVectorOfMuonData.size() ; i++){
		fVectorOfMuonData[i].Print();
	}
}

void MLEM::CreateWeightedMatrix(){
	for(int i = 0 ; i < fVectorOfLTVector.size() ; i++){
		int size = fVectorOfLTVector[i].size();
		for(int j = 0 ; j < size ; j++){
			double totalPathLength = fVectorOfLTVector[i][size-1].sT;
			fVectorOfLTVector[i][j].CreateWeightedMatrix(totalPathLength);
		}
	}
}

//This should do the work of finding the voxel and L,T pair for each muon and store the
//data in fVectorOfLTVector
void MLEM::VoxelFinder(Tomography::Track trackIncoming,Tomography::Track trackOutgoing, double pr){

	//Very important
	fLTVectorForOneTrack.clear();

	//Calculating the Scattering data in X and Y direction.
	//The first step of MLEM Algo
	ScatteringData s = SetScatteringData(trackIncoming,trackOutgoing,pr);
	fVectorOfMuonTrack.push_back(MuonTrack(trackIncoming,trackOutgoing,pr));


	std::cout << "======== Entering Voxel Finder=======" << std::endl;
	Tomography::evolution::Voxelator *voxelator = Tomography::evolution::Voxelator::instance();
	std::vector<int> vectOfVoxels;
	Tracking::Vector3D<double> inComingHitPt = Delta::GetIntersection(trackIncoming,voxelator->GetVoxelizedVolumeDim().z()/2.,1);
	Tracking::Vector3D<double> outGoingHitPt = Delta::GetIntersection(trackOutgoing,-1*(voxelator->GetVoxelizedVolumeDim().z()/2.),2);

	VoxelNavigator voxelNavigator;
	EInside incomingSurfacePt = voxelNavigator.IsPointOnSurfaceOfVoxelizedVolume(inComingHitPt);
	EInside outgoingSurfacePt = voxelNavigator.IsPointOnSurfaceOfVoxelizedVolume(outGoingHitPt);

	bool validTracks = (incomingSurfacePt == EInside::kSurface);
	validTracks &= (outgoingSurfacePt == EInside::kSurface);

	//std::cout <<"Surface Point : " << surfacePt << std::endl;
	if(!validTracks)
		return;

	std::cout << "InComingHitPoint : ";
	inComingHitPt.Print();
	std::cout << "OutgoingHitPoint : " ;
	outGoingHitPt.Print();
	Tracking::ImageReconstruction im;
	Tracking::Vector3D<double> pocaPt = im.POCA(trackIncoming,trackOutgoing);
	std::cout << "PocaPt : "; pocaPt.Print();
	bool isGenuinePocaPt = voxelator->IsGenuine(pocaPt);
	if(!isGenuinePocaPt){
		std::cout << "*********** False Positive PoCA encountered ***************" << std::endl;
		return;
	}


	double T = 0.;
	std::cout << "-------------------- Start to Mid ---------------------------" << std::endl;
	double epsilon = 0.00001;
	double T1 = 0.;
	double pocaStep = 0.;
	//////////////////////////////////////////////////////////////////////////////////
	{
//		VoxelNavigator voxelNavigator;
		Tomography::Track tr(inComingHitPt, pocaPt);
		Tracking::Vector3D<double> startPoint = tr.GetP1();
		Tracking::Vector3D<double> midPoint = tr.GetP2();
	//	double T = 0.;
		std::cout << "VoxelatorDim.Z : " << voxelator->GetVoxelatorDim().z() << std::endl;
//		Tracking::Vector3D<double> voxelHitPt = startPoint + dir * epsilon;
		int pocaPtVoxelNum = voxelator->GetVoxelNumber(pocaPt);
		int hitPointVoxelNum = 0.;
		Tracking::Vector3D<double> dir = tr.GetDirCosine();
		Tracking::Vector3D<double> voxelHitPt = startPoint + dir * epsilon;
		hitPointVoxelNum = voxelator->GetVoxelNumber(voxelHitPt);
		while (pocaPtVoxelNum != hitPointVoxelNum) {
			//voxelHitPt.SetZ(voxelHitPt.z() - epsilon);
			//
			int voxelNum = voxelator->GetVoxelNumber(voxelHitPt);
			Tracking::Vector3D<double> voxelCenter = voxelator->GetVoxelCenter(voxelNum);
			std::cout << "Voxel Num : " << voxelNum << std::endl;
			std::cout << "VoxelCenter : ";
			voxelCenter.Print();

			double step = voxelNavigator.ComputeStep(voxelHitPt, dir,
					voxelCenter);
			std::cout << "Calculated Step value:  Start To MID :  " << step << std::endl;
			voxelHitPt = voxelHitPt + dir * (step+epsilon);
			std::cout << "Moved POINT : "; voxelHitPt.Print();
			hitPointVoxelNum = voxelator->GetVoxelNumber(voxelHitPt);

			if(step <= 1e-15){
				std::cout << "BAD EVENT.......... "; tr.Print();
				exit(1);
			}
			double L = step;
			T += L;
			T1 += L;
			fLTVectorForOneTrack.push_back(LTOfEachVoxel(voxelNum,L,T));
			//fLTVectorForOneTrack.push_back(LTOfEachVoxel(voxelNum,L,T,trackIncoming,trackOutgoing));
		}

		std::cout << "Final Voxel Hit Point : "; voxelHitPt.Print();

		double stepFinal = CommonFunc::Distance(voxelHitPt,pocaPt);
		std::cout << "Final Step to reach PocaPt : " <<  stepFinal << std::endl;
		if (stepFinal <= 1e-15) {
			std::cout << "BAD EVENT detected 0 length of last step while going from Start to Mid .......... ";
			tr.Print();
			exit(1);
		}

		Tracking::Vector3D<double> finalPocaPt = voxelHitPt + dir * stepFinal;
		std::cout << "Final Poca Point : "; finalPocaPt.Print();
		double L = stepFinal;
		pocaStep = stepFinal;
		T += L;
		T1 += L;
		//fLTVectorForOneTrack.push_back(LTOfEachVoxel(pocaPtVoxelNum,L,T));


		//fLTVectorForOneTrack.push_back(LTOfEachVoxel(pocaPtVoxelNum,L,T,trackIncoming,trackOutgoing));

	}
	std::cout << "Distance from Start To Mid : " << CommonFunc::Distance(inComingHitPt, pocaPt) <<" :: Cumulative distance Calculated in Steps : " << T << std::endl;

	std::cout << "-------------------- Mid To End ---------------------------" << std::endl;
	double T2 = 0.;
		{
			VoxelNavigator voxelNavigator;
			Tomography::Track tr(pocaPt,outGoingHitPt);
			Tracking::Vector3D<double> startPoint = tr.GetP1();
			Tracking::Vector3D<double> midPoint = tr.GetP2();
		//	double T = 0.;
			std::cout << "VoxelatorDim.Z : " << voxelator->GetVoxelatorDim().z() << std::endl;
	//		Tracking::Vector3D<double> voxelHitPt = startPoint + dir * epsilon;
			int endPtVoxelNum = voxelator->GetVoxelNumber(outGoingHitPt);
			int hitPointVoxelNum = 0.;
			Tracking::Vector3D<double> dir = tr.GetDirCosine();
			Tracking::Vector3D<double> voxelHitPt = startPoint + dir * epsilon;
			hitPointVoxelNum = voxelator->GetVoxelNumber(voxelHitPt);
			int count = 0;
			while (endPtVoxelNum != hitPointVoxelNum) {
				count++;

				//voxelHitPt.SetZ(voxelHitPt.z() - epsilon);
				//
				int voxelNum = voxelator->GetVoxelNumber(voxelHitPt);
				Tracking::Vector3D<double> voxelCenter = voxelator->GetVoxelCenter(voxelNum);
				std::cout << "Voxel Num : " << voxelNum << std::endl;
				std::cout << "VoxelCenter : ";
				voxelCenter.Print();

				double step = voxelNavigator.ComputeStep(voxelHitPt, dir,
						voxelCenter);
				std::cout << "Calculated Intermediate Step value from Mid To End Region :  " << step << std::endl;
			if (step <= 1e-15) {
				std::cout << "BAD EVENT.......... ";
				tr.Print();
				exit(1);
			}

				voxelHitPt = voxelHitPt + dir * (step+epsilon);
				std::cout << "Moved POINT : "; voxelHitPt.Print();
				hitPointVoxelNum = voxelator->GetVoxelNumber(voxelHitPt);;
				double L = step;
				T += L;
				//std::cout << "T : " << T << std::endl;
				T2 += L;

				if (count == 1) {
				  pocaStep += step;
				  fLTVectorForOneTrack.push_back(LTOfEachVoxel(voxelNum,pocaStep,T));
			   }else{
				fLTVectorForOneTrack.push_back(LTOfEachVoxel(voxelNum,L,T));
			   }
				//fLTVectorForOneTrack.push_back(LTOfEachVoxel(voxelNum,L,T,trackIncoming,trackOutgoing));

			}

			std::cout << "Final Voxel Hit Point : "; voxelHitPt.Print();

			double stepFinal = CommonFunc::Distance(voxelHitPt,outGoingHitPt);
		if (stepFinal <= 1e-15) {
			std::cout << "BAD EVENT detected 0 length of last step while going from Mid To End .......... ";
			tr.Print();
			exit(1);
		}
			std::cout << "Final Step to reach PocaPt : " <<  stepFinal << std::endl;

			std::cout << "Actual Exit Point : " ; outGoingHitPt.Print();

			Tracking::Vector3D<double> finalExitPt = voxelHitPt + dir * stepFinal;
			std::cout << "Calculated Final Exit Point : "; finalExitPt.Print();

			double L = stepFinal;
			T += L;
			//std::cout << "T : " << T << std::endl;
			T2 += L;
			fLTVectorForOneTrack.push_back(LTOfEachVoxel(endPtVoxelNum,L,T));
			//fLTVectorForOneTrack.push_back(LTOfEachVoxel(endPtVoxelNum,L,T,trackIncoming,trackOutgoing));


		}

		std::cout << "Distance from Mid To End : " << CommonFunc::Distance(pocaPt, outGoingHitPt) <<" :: Cumulative distance Calculated in Steps : " << T2 << std::endl;

		std::cout << "Total Distance Travelled T1 + T2 : " << (T1+T2) <<" :: T : " << T << std::endl;



		fVectorOfLTVector.push_back(fLTVectorForOneTrack);

		//Below block should be the part of EMUpdate
		/*for(unsigned int i = 0 ; i < fVectorOfLTVector.size() ; i++){
			//Below logic should be applied to all the muons
			EachMuonData muonData(fVectorOfLTVector[i],s);
			//muonData.sLTVectorForEachMuon
			fVectorOfMuonData.push_back(muonData);
		}*/

	//////////////////////////////////////////////////////////////////////////////////////

}

//------------------------------------------------------------------------------------------------------------
//LambdaUpdater
LambdaUpdater::LambdaUpdater() {
	// TODO Auto-generated constructor stub
	Tomography::evolution::Voxelator* voxelator = Tomography::evolution::Voxelator::instance();
	fTotalNumOfVoxels = voxelator->GetTotalNumberOfVoxels();
	for(int i = 0 ; i < fTotalNumOfVoxels ; i++){
		fVectorOfVoxelData.push_back(VoxelData());
	}

}

LambdaUpdater::LambdaUpdater(std::vector<Tomography::EachMuonData> vectorOfMuonData){
	Tomography::evolution::Voxelator* voxelator = Tomography::evolution::Voxelator::instance();
		fTotalNumOfVoxels = voxelator->GetTotalNumberOfVoxels();
		fVectorOfVoxelData.clear();
		fVectorOfMuonData.clear();
		for(int i = 0 ; i < fTotalNumOfVoxels ; i++){
			fVectorOfVoxelData.push_back(VoxelData());
	}

		fVectorOfMuonData = vectorOfMuonData;
}

void LambdaUpdater::CalculateNewLambda(){
	for(int i = 0 ; i < fVectorOfMuonData.size() ; i++){
		Tomography::EachMuonData muonData = fVectorOfMuonData[i];
		for(int j = 0 ; j < muonData.sLTVectorForEachMuon.size() ; j++){
			int voxelNum = muonData.sLTVectorForEachMuon[j].sVoxelNum;
			fVectorOfVoxelData[voxelNum].sLambda += muonData.sNewLambdaFromMuon[j];
			fVectorOfVoxelData[voxelNum].count++;
		}
	}
	for(int j = 0 ; j < fVectorOfVoxelData.size() ; j++){
		if(fVectorOfVoxelData[j].count != 0)
			fVectorOfVoxelData[j].sLambda /= (2*fVectorOfVoxelData[j].count);
	}
}

void LambdaUpdater::Print(){
	Tomography::evolution::Voxelator* voxelator = Tomography::evolution::Voxelator::instance();
	int numOfVoxels = voxelator->GetTotalNumberOfVoxels();
	int numOfVoxelsInOneSlice = voxelator->GetVoxelatorDim().x()*voxelator->GetVoxelatorDim().y();
	for(unsigned int i = 0 ; i < numOfVoxels ; i++){

				std::cout << fVectorOfVoxelData[i].sLambda << " ";
				if(!((i+1)%numOfVoxelsInOneSlice) && i!=0)
					std::cout << std::endl << " i :  " << i << std::endl;
			}
}

LambdaUpdater::~LambdaUpdater() {
	// TODO Auto-generated destructor stub
}

//-------------------------------------------------------------------------------------------------------------

void MLEM::EMUpdate(){

	//Iteration loop
	for (int i = 0; i < 1; i++) {

		std::cout << "ITERNO : " << i << std::endl;
		std::cout << "VectorOfLTVector Size : " << fVectorOfLTVector.size() << std::endl;

		fVectorOfMuonData.clear();

		//Muon Event loop
		for (unsigned int j = 0; j < fVectorOfLTVector.size(); j++) {
			ScatteringData s = SetScatteringData(
					fVectorOfMuonTrack[j].sIncoming,
					fVectorOfMuonTrack[j].sOutgoing,
					fVectorOfMuonTrack[j].sPr);
			//Below logic should be applied to all the muons
			EachMuonData muonData(fVectorOfLTVector[j], s);
			muonData.Print();
			//muonData.sLTVectorForEachMuon
			if(muonData.sIsInvertibleCovarianceMatrix){
				fVectorOfMuonData.push_back(muonData);
			}

		}
		LambdaUpdater lambdaUpdater(fVectorOfMuonData);
		std::cout << "======================== OLD Lambda =======================" << std::endl;
		lambdaUpdater.Print();
		lambdaUpdater.CalculateNewLambda();
		std::cout << "======================== NEW Lambda =======================" << std::endl;
		lambdaUpdater.Print();

		//Now before Next Iteration start the actual lambda should be replaced with new lambda
		UpdateScatteringDensity(lambdaUpdater);
		//Printing the updated scattering density
		std::cout << "@@@@@@@@@@@@@@@@@@@ Printing the UPDATED Scattering Density  @@@@@@@@@@@@@@@@@@" << std::endl;
		Tomography::ScatteringDensityV2::instance()->Print();
		std::cout <<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;


	}

}

void MLEM::UpdateScatteringDensity(LambdaUpdater lambdaUpdater) {
	Tomography::ScatteringDensityV2* scatteringDensity = Tomography::ScatteringDensityV2::instance();
	Tomography::evolution::Voxelator* voxelator = Tomography::evolution::Voxelator::instance();
	int numOfVoxels = voxelator->GetTotalNumberOfVoxels();
	for(unsigned int i = 0 ; i < numOfVoxels ; i++){
		//scatteringDensity->FillLambdaValue(i,fVectorOfVoxelData[i].sLambda);
		scatteringDensity->FillLambdaValue(i,lambdaUpdater.GetVectorOfVoxelData()[i].sLambda);
	}
}

//-------------------------------------------------------------------------------------------------------------
ScatteringData MLEM::SetScatteringData(Tomography::Track trackIncoming,Tomography::Track trackOutgoing, double pr){

	ScatteringData s;

	Tracking::Vector3D<double> entryPoint = trackIncoming.GetP2();
	Tracking::Vector3D<double> exitPoint = trackOutgoing.GetP1();
	double myL = (exitPoint.z() - entryPoint.z())/trackIncoming.GetDirCosine().z();

	Tracking::Vector3D<double> incomingDirRatio = trackIncoming.GetDirectionRatio();
	double Tx_in = incomingDirRatio.x()/incomingDirRatio.z();
	double Ty_in = incomingDirRatio.y()/incomingDirRatio.z();
	double H= std::fabs(exitPoint.z()-entryPoint.z());
	double Lxy= sqrt(1+Tx_in*Tx_in+Ty_in*Ty_in);
	double L= H*Lxy;

	std::cout << "myL : " << myL <<" :: L : " << L << std::endl;

	Tracking::Vector3D<double> entryDir = trackIncoming.GetDirCosine();
	Tracking::Vector3D<double> exitDir = trackOutgoing.GetDirCosine();
	Tracking::Vector3D<double> P= entryPoint + entryDir*L;
	double ThetaX0= TMath::ATan2(entryDir.x(),entryDir.z());//in rad
	double ThetaY0= TMath::ATan2(entryDir.y(),entryDir.z());//in rad
	double ThetaX= TMath::ATan2(exitDir.x(),exitDir.z());//in rad
	double ThetaY= TMath::ATan2(exitDir.y(),exitDir.z());//in rad
	double DeltaThetaX= ThetaX-ThetaX0;
	double DeltaThetaY= ThetaY-ThetaY0;

	double DeltaX= (exitPoint.x()-P.x())*cos(ThetaX0)*Lxy*cos(ThetaX0+DeltaThetaX)/cos(DeltaThetaX);
	double DeltaY= (exitPoint.y()-P.y())*cos(ThetaY0)*Lxy*cos(ThetaY0+DeltaThetaY)/cos(DeltaThetaY);

	double Ex= 0.; //4.*pow(fSigmaPosX*sqrt(12)/OuterPlaneZSeparation,2);//rad^2
	double Ey= 0.; //2.*(1+InnerPlaneZSeparation/OuterPlaneZSeparation+pow(InnerPlaneZSeparation/OuterPlaneZSeparation,2))*pow(fSigmaPosY*sqrt(12),2);//cm^2
	double Exy= 0.; //2.*pow(fSigmaPosZ*sqrt(12)/OuterPlaneZSeparation,2)*InnerPlaneZSeparation;//rad*cm

	//Set it carefully
	//double pr = 1.; //Now pr is comming from VoxelFinder

	pr/=Tomography::refMomentum;

	std::cout <<"RamanPr : " << pr << std::endl;

	s.SetScatteringData(ThetaX0,ThetaY0,ThetaX,ThetaY,DeltaThetaX,DeltaThetaY,DeltaX,DeltaY,pr,Ex,Ey,Exy);
	fScatteringDataVector.push_back(s);
	return s;
	//fScatteringDataVector.push_back(ScatteringData(DeltaThetaX,DeltaThetaY,DeltaX,DeltaY,pr,Ex,Ey,Exy));
/*
	if(fVerbosity>1){
			cout<<"EMTomographyRecoThreadObj::SetScatteringData(): INFO:: Ex="<<Ex<<"  Ey="<<Ey<<"  Exy="<<Exy<<endl;
	}
*/

}


//Temporary function, may be deleted later
void MLEM::Debugger(){
	Tomography::Track trackIncoming(Tracking::Vector3D<double>(10.,20.,900.),
									Tracking::Vector3D<double>(20.,30.,800));
	Tomography::Track trackOutgoing(Tracking::Vector3D<double>(40.,50.,-800.),
									Tracking::Vector3D<double>(30.,40.,-900));

/*
	Tracking::Vector3D<double> entryPoint = trackIncoming.GetP2();
	Tracking::Vector3D<double> exitPoint = trackOutgoing.GetP1();
	double myL = (exitPoint.z() - entryPoint.z())/trackIncoming.GetDirCosine().z();

	Tracking::Vector3D<double> incomingDirRatio = trackIncoming.GetDirectionRatio();
	double Tx_in = incomingDirRatio.x()/incomingDirRatio.z();
	double Ty_in = incomingDirRatio.y()/incomingDirRatio.z();
	double H= std::fabs(exitPoint.z()-entryPoint.z());
	double Lxy= sqrt(1+Tx_in*Tx_in+Ty_in*Ty_in);
	double L= H*Lxy;

	std::cout << "myL : " << myL <<" :: L : " << L << std::endl;

	Tracking::Vector3D<double> entryDir = trackIncoming.GetDirCosine();
	Tracking::Vector3D<double> exitDir = trackOutgoing.GetDirCosine();
	Tracking::Vector3D<double> P= entryPoint + entryDir*L;
	double ThetaX0= TMath::ATan2(entryDir.x(),entryDir.z());//in rad
	double ThetaY0= TMath::ATan2(entryDir.y(),entryDir.z());//in rad
	double ThetaX= TMath::ATan2(exitDir.x(),exitDir.z());//in rad
	double ThetaY= TMath::ATan2(exitDir.y(),exitDir.z());//in rad
	double DeltaThetaX= ThetaX-ThetaX0;
	double DeltaThetaY= ThetaY-ThetaY0;

	double DeltaX= (exitPoint.x()-P.x())*cos(ThetaX0)*Lxy*cos(ThetaX0+DeltaThetaX)/cos(DeltaThetaX);
	double DeltaY= (exitPoint.y()-P.y())*cos(ThetaY0)*Lxy*cos(ThetaY0+DeltaThetaY)/cos(DeltaThetaY);

	std::cout << "============= Raman : Print Scattering values from Debugger ============ " << std::endl;
	std::cout << "DeltaThetaX : " << DeltaThetaX <<" : DeltaThetaY : " << DeltaThetaY <<" : DeltaX :" <<  DeltaX << " : DeltaY : " << DeltaY << std::endl;

	Tomography::evolution::Voxelator::Create(50*cm,50*cm,75*cm,5*cm,5*cm,7.5*cm);
*/

	std::cout <<"================== Executing VoxelFinder =====================" << std::endl;
/*
	VoxelFinder(trackIncoming,trackOutgoing);
	trackIncoming.Set(Tracking::Vector3D<double>(20.,40.,900),Tracking::Vector3D<double>(50.,30.,800));
	trackOutgoing.Set(Tracking::Vector3D<double>(20.,30.,-800),Tracking::Vector3D<double>(40.,50.,-900));
	VoxelFinder(trackIncoming,trackOutgoing);
	std::cout <<"===========================================================================================" << std::endl;
	Print();
	std::cout <<"===========================================================================================" << std::endl;
	CreateWeightedMatrix();
	std::cout <<"===========================================================================================" << std::endl;
	Print();
*/


/*
	//Going Good
	VoxelFinder(trackIncoming,trackOutgoing);
	std::cout <<"================================= Processing Second Track================================" << std::endl;
	trackIncoming.Set(Tracking::Vector3D<double>(20.,40.,900),Tracking::Vector3D<double>(50.,30.,800));
	trackOutgoing.Set(Tracking::Vector3D<double>(20.,30.,-800),Tracking::Vector3D<double>(40.,50.,-900));
	VoxelFinder(trackIncoming,trackOutgoing);
*/


	std::cout <<"================================= Processing Second Track Manually ================================" << std::endl;
	//338.198 -26.4614 1060 199.706 38.0842 460 -8.07893 134.916 -440 -146.596 199.468 -1040
	trackIncoming.Set(Tracking::Vector3D<double>(338.198, -26.4614, 1060),Tracking::Vector3D<double>(199.706, 38.0842, 460));
	trackOutgoing.Set(Tracking::Vector3D<double>(-8.07893, 134.916, -440),Tracking::Vector3D<double>(-146.596, 199.468, -1040));
	VoxelFinder(trackIncoming,trackOutgoing);

	//170.888 430.05 1060 164.463 275.888 460 155.209 47.1923 -440 149.365 -103.194 -1040 1


/*  //Going Good
	std::cout <<"================================= Processing Third Track Manually ================================" << std::endl;
	trackIncoming.Set(Tracking::Vector3D<double>(170.888,430.05, 1060),Tracking::Vector3D<double>(164.463, 275.888, 460));
		trackOutgoing.Set(Tracking::Vector3D<double>(155.209, 47.1923, -440),Tracking::Vector3D<double>(-149.365, -103.194, -1040));
		VoxelFinder(trackIncoming,trackOutgoing);
*/

		//355.129 -260.27 1060 369.008 -289.545 460 389.826 -333.459 -440 403.701 -362.731 -1040 0

	 //Started working
	 std::cout <<"================================= Processing Fourth Track Manually ================================" << std::endl;
			trackIncoming.Set(Tracking::Vector3D<double>(355.129, -260.27, 1060),Tracking::Vector3D<double>(369.008 ,-289.545, 460));
				trackOutgoing.Set(Tracking::Vector3D<double>(389.826, -333.459, -440),Tracking::Vector3D<double>(403.701, -362.731, -1040));
				VoxelFinder(trackIncoming,trackOutgoing);

/*
	std::vector<Tomography::Track> incomingTrackVector,outgoingTrackVector;
	CommonFunc::GetTracksVector("TrackExact.txt",incomingTrackVector,outgoingTrackVector);
	//for(int i = 0 ; i < incomingTrackVector.size() ; i++){
	for(int i = 0 ; i < 1 ; i++){
		VoxelFinder(incomingTrackVector[i],outgoingTrackVector[i]);
	}
*/
	//Calling EMUdate
	EMUpdate();
	std::cout << " ++++++++++++++++++++ Printing After VoxelFinder : Using Print2() ++++++++++++++++++++" << std::endl;
	Print2();
	std::cout <<"===========================================================================================" << std::endl;


}

} /* namespace Tomography */
