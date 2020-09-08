/*
 * SingleMuonTrack.cpp
 *
 *  Created on: 07-Sep-2020
 *      Author: rsehgal
 */

#include "SingleMuonTrack.h"
#include "ScintillatorBar_V2.h"

SingleMuonTrack::SingleMuonTrack() {
	// TODO Auto-generated constructor stub
	fTotalEnergyDeposited = 0.;

}

SingleMuonTrack::~SingleMuonTrack() {
	// TODO Auto-generated destructor stub
}

void SingleMuonTrack::push_back(ScintillatorBar_V2 *hittedBar){
	fSingleMuonTrack.push_back(hittedBar);
}

void SingleMuonTrack::CalculateTotalEnergyDeposited(){
	fTotalEnergyDeposited = 0.;
	for(unsigned int i = 0 ; i < fSingleMuonTrack.size() ; i++){
		//fTotalEnergyDeposited += fSingleMuonTrack[i]->qlongMean;
		fTotalEnergyDeposited += fSingleMuonTrack[i]->qlongMeanCorrected;
		if(i < (fSingleMuonTrack.size()-1))
			fSingleMuonTrack[i]->pathLength = fSingleMuonTrack[i]->hitPosition.Distance(fSingleMuonTrack[i+1]->hitPosition);
		else
			fSingleMuonTrack[i]->pathLength = 0.;
	}
	//std::cout << "*********** Total Energy Deposited : " << fTotalEnergyDeposited << " **************" << std::endl;

}

bool SingleMuonTrack::CompareBarIndexInScintillator(ScintillatorBar_V2 *i1, ScintillatorBar_V2 *i2)
{
	return (i1->barIndex > i2->barIndex);
}

void SingleMuonTrack::Sort(){
	std::sort(fSingleMuonTrack.begin(), fSingleMuonTrack.end(),CompareBarIndexInScintillator);
}

void SingleMuonTrack::clear(){
	fSingleMuonTrack.clear();
}

unsigned long int SingleMuonTrack::size(){
	fSingleMuonTrack.size();
}

void SingleMuonTrack::Print(){
	for(unsigned int j = 0 ; j < fSingleMuonTrack.size() ; j++){
		fSingleMuonTrack[j]->Print();
	}
	std::cout << "@@@@@@@@@2 Total Energy Deposited : " << fTotalEnergyDeposited << " @@@@@@@@@" <<  std::endl;
}
