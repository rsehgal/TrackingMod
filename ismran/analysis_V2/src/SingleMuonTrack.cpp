/*
 * SingleMuonTrack.cpp
 *
 *  Created on: 10-Nov-2020
 *      Author: rsehgal
 */

#include "SingleMuonTrack.h"
#include "ScintillatorBar_V2.h"
#include <iterator>
#include "PsBar.h"
#include "Point3D.h"
#include "HardwareNomenclature.h"
#include <TVector3.h>
#include "Plotter.h"
#ifndef FOR_SIMULATION
ClassImp(lite_interface::SingleMuonTrack)
#endif
namespace lite_interface {

SingleMuonTrack::SingleMuonTrack() {
	// TODO Auto-generated constructor stub
	//std::cout << "SINGLEMUONTRACK : DEfault Contructor called" << std::endl;

}

SingleMuonTrack::SingleMuonTrack(std::vector<ScintillatorBar_V2*> vecOfScintBars){
	fSingleMuonTrack = vecOfScintBars;
}

SingleMuonTrack::SingleMuonTrack(const SingleMuonTrack &smt){
	//std::cout << "SINGLEMUONTRACK : Copy Contructor called" << std::endl;
	//smt.GetMuonTrack()[0]->Print();
	for(unsigned int i = 0 ; i < smt.GetMuonTrack().size() ; i++){
		//fSingleMuonTrack.push_back(new ScintillatorBar_V2(*smt.GetMuonTrack()[i]));
		fSingleMuonTrack.push_back(smt.GetMuonTrack()[i]);
	}
}

SingleMuonTrack::~SingleMuonTrack() {
	// TODO Auto-generated destructor stub
}

void SingleMuonTrack::push_back(ScintillatorBar_V2 *hittedBar){
	fSingleMuonTrack.push_back(hittedBar);
}

bool SingleMuonTrack::CompareBarIndexInScintillator(ScintillatorBar_V2 *i1, ScintillatorBar_V2 *i2)
{
	return (i1->fBarIndex > i2->fBarIndex);
}

void SingleMuonTrack::Sort(){
	std::sort(fSingleMuonTrack.begin(), fSingleMuonTrack.end(),CompareBarIndexInScintillator);
}

void SingleMuonTrack::clear(){
	fSingleMuonTrack.clear();
}

unsigned long int SingleMuonTrack::size(){
	return fSingleMuonTrack.size();
}

void SingleMuonTrack::Print(){
	std::cout << "==== Print from SingleMuonTrack =====" << std::endl;
	for(unsigned int j = 0 ; j < fSingleMuonTrack.size() ; j++){
		fSingleMuonTrack[j]->Print();
	}
	//std::cout << "@@@@@@@@@2 Total Energy Deposited : " << fTotalEnergyDeposited << " @@@@@@@@@" <<  std::endl;
}

std::vector<ScintillatorBar_V2*> SingleMuonTrack::GetMuonTrack()const{
	return fSingleMuonTrack;
}

std::vector<unsigned short> SingleMuonTrack::GetLayersHitCountVector(){
	std::vector<unsigned short> layerHitCountVector;
	for(unsigned short i = 0 ; i < numOfLayers ; i++)
		layerHitCountVector.push_back(0);

	std::vector<ScintillatorBar_V2*>::iterator itr;
	for(itr = fSingleMuonTrack.begin() ; itr != fSingleMuonTrack.end() ; itr++){
		layerHitCountVector[(*itr)->GetLayerIndex()]++;
	}
	return layerHitCountVector;
}

bool SingleMuonTrack::IsClearTrack(){
	std::vector<unsigned short> layerHitCountVector = GetLayersHitCountVector();
	std::vector<unsigned short>::iterator itr;
	bool clear = true;
	for(itr = layerHitCountVector.begin() ; itr!=layerHitCountVector.end(); itr++){
		clear &= ((*itr)==1);
		if(!clear)
			break;
	}
	return clear;
}

#ifdef USE_FOR_SIMULATION
std::vector<lite_interface::Point3D*>  SingleMuonTrack::GetMean3DHitPointVector(){
	std::vector<lite_interface::Point3D*> vectorOf3DHitPoint;
	std::vector<ScintillatorBar_V2*>::iterator itr;
	//int count = 0 ;
	for(itr = fSingleMuonTrack.begin() ; itr != fSingleMuonTrack.end() ; itr++){

		vectorOf3DHitPoint.push_back((*itr)->GetMeanHitPosition());
		//vectorOf3DHitPoint[count]->Print();
		//count++;
	}
	return vectorOf3DHitPoint;
}
#endif

std::vector<lite_interface::Point3D*> SingleMuonTrack::Get3DHitPointVector(){
	std::vector<lite_interface::Point3D*> vectorOf3DHitPoint;
	std::vector<ScintillatorBar_V2*>::iterator itr;
	for(itr = fSingleMuonTrack.begin() ; itr != fSingleMuonTrack.end() ; itr++){
		vectorOf3DHitPoint.push_back((*itr)->EstimateHitPosition());
	}
	return vectorOf3DHitPoint;
}

std::vector<lite_interface::Point3D*> SingleMuonTrack::Get3DHitPointVector_Param(){
	std::vector<lite_interface::Point3D*> vectorOf3DHitPoint;
	std::vector<ScintillatorBar_V2*>::iterator itr;
	for(itr = fSingleMuonTrack.begin() ; itr != fSingleMuonTrack.end() ; itr++){
		vectorOf3DHitPoint.push_back((*itr)->EstimateHitPosition_Param());
	}
	return vectorOf3DHitPoint;
}

void SingleMuonTrack::SetFittedMembers(int opt){
	std::vector<lite_interface::Point3D*> fittedTrack;
	if(opt == 0){
		//For Linear
		fittedTrack = lite_interface::CreateFittedTrack(Get3DHitPointVector());
		for(unsigned int i = 0 ; i < size() ; i++){
			fSingleMuonTrack[i]->fittedLinear = fittedTrack[i];
		}
	}
	if(opt == 1){
		//For Param
		fittedTrack = lite_interface::CreateFittedTrack(Get3DHitPointVector_Param());
		for(unsigned int i = 0 ; i < size() ; i++){
			fSingleMuonTrack[i]->fittedParam = fittedTrack[i];
		}
	}
#ifdef USE_FOR_SIMULATION
	if(opt == 2){
		//For Mean
		fittedTrack = lite_interface::CreateFittedTrack(GetMean3DHitPointVector());
		for(unsigned int i = 0 ; i < size() ; i++){
			fSingleMuonTrack[i]->fittedMean = fittedTrack[i];
		}
	}
#endif


}




double SingleMuonTrack::GetEnergySum(){
	double energySum = 0;
	std::vector<ScintillatorBar_V2*>::iterator itr;
	for(itr = fSingleMuonTrack.begin() ; itr != fSingleMuonTrack.end() ; itr++){
		energySum += (*itr)->GetQMeanCorrected();
	}
	//std::cout << "Energy Sum : " << energySum << std::endl;
	return energySum;
}

std::vector<std::string> SingleMuonTrack::GetBarNamesVector(){
	std::vector<std::string> barNamesVector;
	std::vector<ScintillatorBar_V2*>::iterator itr;
	for(itr = fSingleMuonTrack.begin() ; itr != fSingleMuonTrack.end() ; itr++){
		barNamesVector.push_back(vecOfBarsNamess[(*itr)->GetBarIndex()]);
	}
	return barNamesVector;
}

std::vector<double> SingleMuonTrack::GetDepositedEnergyVector(){
	std::vector<double> energyVec;
	std::vector<ScintillatorBar_V2*>::iterator itr;
	for(itr = fSingleMuonTrack.begin() ; itr != fSingleMuonTrack.end() ; itr++){
		energyVec.push_back((*itr)->GetQMeanCorrected());
	}
	return energyVec;
}

double SingleMuonTrack::GetZenithAngle(std::vector<lite_interface::Point3D*> vecOfPoint3D){
	TVector3 ref(0.,-1.,0.);
	Point3D *startPoint = vecOfPoint3D[0];
	Point3D *endPoint = vecOfPoint3D[vecOfPoint3D.size() - 1];
	TVector3 muonDir(TVector3(endPoint->GetX(), endPoint->GetY(), endPoint->GetZ())	- TVector3(startPoint->GetX(), startPoint->GetY(),startPoint->GetZ()));
	return muonDir.Angle(ref);
}

double SingleMuonTrack::GetZenithAngle(int opt){
	if(opt == 1){
		return GetZenithAngle(CreateFittedTrack(Get3DHitPointVector()));
	}
	if(opt == 2){
		return GetZenithAngle(CreateFittedTrack(Get3DHitPointVector_Param()));
	}
#ifdef USE_FOR_SIMULATION
	if(opt == 3){
		std::vector<lite_interface::Point3D*> pt3DVec = GetMean3DHitPointVector();
		/*std::cout << "========= Mean Hit Points of a track ========== " << std::endl;
		for(unsigned int j = 0 ; j < pt3DVec.size() ; j++){
			pt3DVec[j]->Print();
		}*/

		return GetZenithAngle(CreateFittedTrack(pt3DVec));
	}
#endif
}
double SingleMuonTrack::GetZenithAngle_Linear(){
	return GetZenithAngle(1);
}
double SingleMuonTrack::GetZenithAngle_Param(){
	return GetZenithAngle(2);
}
#ifdef USE_FOR_SIMULATION
double SingleMuonTrack::GetZenithAngle_MeanHitPoint(){
	return GetZenithAngle(3);
}
#endif

} /* namespace lite_interface */
