/*
 * SingleMuonTrack.cpp
 *
 *  Created on: 10-Nov-2020
 *      Author: rsehgal
 */

#include "SingleMuonTrack.h"
#include "ScintillatorBar_V2.h"

ClassImp(lite_interface::SingleMuonTrack);

namespace lite_interface {

SingleMuonTrack::SingleMuonTrack() {
	// TODO Auto-generated constructor stub

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
	for(unsigned int j = 0 ; j < fSingleMuonTrack.size() ; j++){
		fSingleMuonTrack[j]->Print();
	}
	//std::cout << "@@@@@@@@@2 Total Energy Deposited : " << fTotalEnergyDeposited << " @@@@@@@@@" <<  std::endl;
}

std::vector<ScintillatorBar_V2*> SingleMuonTrack::GetMuonTrack()const{
	return fSingleMuonTrack;
}

} /* namespace lite_interface */
