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

ClassImp(lite_interface::SingleMuonTrack)

namespace lite_interface {

SingleMuonTrack::SingleMuonTrack() {
	// TODO Auto-generated constructor stub
	//std::cout << "SINGLEMUONTRACK : DEfault Contructor called" << std::endl;

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

std::vector<lite_interface::Point3D*> SingleMuonTrack::Get3DHitPointVector(){
	std::vector<lite_interface::Point3D*> vectorOf3DHitPoint;
	std::vector<ScintillatorBar_V2*>::iterator itr;
	for(itr = fSingleMuonTrack.begin() ; itr != fSingleMuonTrack.end() ; itr++){
		vectorOf3DHitPoint.push_back((*itr)->EstimateHitPosition());
	}
	return vectorOf3DHitPoint;
}

} /* namespace lite_interface */
