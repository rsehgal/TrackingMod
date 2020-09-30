/*
 * ScintillatorBar_V2.cpp
 *
 *  Created on: 03-Sep-2020
 *      Author: rsehgal
 */

#include "ScintillatorBar_V2.h"
#include "HardwareNomenclature.h"

bool verbose = false;

ScintillatorBar_V2::ScintillatorBar_V2() {
	// TODO Auto-generated constructor stub

}

ScintillatorBar_V2::~ScintillatorBar_V2() {
	// TODO Auto-generated destructor stub
}

/*
 * This constructor will be useful for simulation
 */
ScintillatorBar_V2::ScintillatorBar_V2(unsigned int bIndex){
	barHitted = false;
	barIndex = bIndex;
	layerIndex = barIndex/numOfBarsInEachLayer;
	unsigned short l_channelNear = 2*barIndex;
	unsigned short l_channelFar = l_channelNear+1;
	//scintName="PsBar"+std::to_string(barIndex)+"-"+std::to_string(l_channelNear)+"-"+std::to_string(l_channelFar);
	scintName = vecOfBarsNamess[barIndex];
	/* qlongMean, the most import parameter, should be filled as total energy
	 * deposited in a bar, which is basically the sum of energy deposited in
	 * individual steps
	 *
	 * remaining data members may be set to zero.
	 */
	qlongMean = 0;
	qlongNear = 0;
	qlongFar = 0;
	tstampNear=0;
	tstampFar=0;
	tsmallTimeStamp = 0;
	deltaTstamp=0;
	//time=0;
	//EstimateHitPosition();
}

ScintillatorBar_V2::ScintillatorBar_V2(unsigned short l_channelNear, unsigned short l_channelFar,
					ULong64_t l_tstampNear, ULong64_t l_tstampFar,
					UInt_t l_qlongNear,	UInt_t l_qlongFar){

		barIndex = l_channelNear/2;
		layerIndex = barIndex/numOfBarsInEachLayer;
		//scintName="PsBar"+std::to_string(barIndex)+"-"+std::to_string(l_channelNear)+"-"+std::to_string(l_channelFar);
		scintName = vecOfBarsNamess[barIndex];
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
		if(verbose)
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
					  << "Energy Deposited : " << qlongMeanCorrected << " : Path Length : " << pathLength << std::endl;


}

void ScintillatorBar_V2::EstimateHitPositionAlongX(){
		//hitPosition.x = (layerIndex+1)*10.;
		hitPosition.x = vecOfScintXYCenter[barIndex].x;
		hitPositionError.x = errorX;
}

void ScintillatorBar_V2::EstimateHitPositionAlongY(){
		//hitPosition.y = 0.;
		hitPosition.y = vecOfScintXYCenter[barIndex].y;
		hitPositionError.y = errorY;
}
