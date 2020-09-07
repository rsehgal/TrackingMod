/*
 * ScintillatorBar_V2.cpp
 *
 *  Created on: 03-Sep-2020
 *      Author: rsehgal
 */

#include "ScintillatorBar_V2.h"


ScintillatorBar_V2::ScintillatorBar_V2() {
	// TODO Auto-generated constructor stub

}

ScintillatorBar_V2::~ScintillatorBar_V2() {
	// TODO Auto-generated destructor stub
}

ScintillatorBar_V2::ScintillatorBar_V2(unsigned short l_channelNear, unsigned short l_channelFar,
					ULong64_t l_tstampNear, ULong64_t l_tstampFar,
					UInt_t l_qlongNear,	UInt_t l_qlongFar){

		barIndex = l_channelNear/2;
		layerIndex = barIndex/numOfBarsInEachLayer;
		scintName="PsBar"+std::to_string(barIndex)+"-"+std::to_string(l_channelNear)+"-"+std::to_string(l_channelFar);
		qlongNear=l_qlongNear;
		qlongFar=l_qlongFar;
		qlongMean=sqrt(qlongNear*qlongFar);
		tstampNear=l_tstampNear;
		tstampFar=l_tstampFar;
		tsmallTimeStamp = (tstampNear < tstampFar) ? tstampNear : tstampFar;
		deltaTstamp=tstampNear-tstampFar;
		bool validEnergy = (qlongMean > qstart && qlongMean < qend);
		pathLength = 0.;

}


ScintillatorBar_V2::ScintillatorBar_V2(const ScintillatorBar_V2 &sbar){
		std::cout << "======= COPY CONSTRUCTOR CaLLED ==========" << std::endl;
		//barHitted = sbar.barHitted;
		barIndex = sbar.barIndex;
		layerIndex = sbar.layerIndex;
		scintName = sbar.scintName;
		qlongMean = sbar.qlongMean;
		qlongNear = sbar.qlongNear;
		qlongFar = sbar.qlongFar;
		tstampNear = sbar.tstampNear;
		tstampFar = sbar.tstampFar;
		tsmallTimeStamp = sbar.tsmallTimeStamp;
		deltaTstamp = sbar.deltaTstamp;
		pathLength = 0.;

	}

void ScintillatorBar_V2::Print(){

			std::cout << scintName << " , " << barIndex << " , " << qlongNear << " , " << qlongFar << " , " << qlongMean << " , "
					  << tstampNear  << " , " << tstampFar << " , " << tsmallTimeStamp << " , " << deltaTstamp << " , "
					  << " ( " << hitPosition.x << " , " << hitPosition.y << " , " << hitPosition.z << " )  : "
					  << "Energy Deposited : " << qlongMean << " : Path Length : " << pathLength << std::endl;


}

void ScintillatorBar_V2::EstimateHitPositionAlongX(){
		hitPosition.x = (layerIndex+1)*10.;
}

void ScintillatorBar_V2::EstimateHitPositionAlongY(){
		hitPosition.y = 0.;
}
