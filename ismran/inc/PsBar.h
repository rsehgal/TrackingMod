/*
 * PsBar.h
 *
 *  Created on: 14-Aug-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_INC_PSBAR_H_
#define ISMRAN_INC_PSBAR_H_


const int offset=0;
const double kDelTBar = 32.0; //! ns
TStopwatch timer;
unsigned int numOfLayers=3;
unsigned int numOfBarsInEachLayer=9;

std::map<std::string,TH1D*> barsEnergyMap;
std::vector<TH1D*> vecOfEnergyHist;

double fwhm = 2.355 * 5.66353 ;

struct Point3D{
	double x;
	double y;
	double z;

	Point3D(double s_x, double s_y, double s_z){
		x = s_x;
		y = s_y;
		z = s_z;
	}

	Point3D(){}

};


struct ScintillatorBar{
	std::string  scintName;    //! board #  and channel number ( its packed in as follows ) //! board*16 + chno.

	//Charge info may not be required, but keeping it for the time being.
	UInt_t    qlongNear;   //! integrated charge in long gate 88 nsec
	UInt_t    qlongFar;   //! integrated charge in long gate 88 nsec
	UInt_t    qlongMean;   //! integrated charge in long gate 88 nsec


	ULong64_t tstampNear;  //! time stamp in pico sec.
	ULong64_t tstampFar;  //! time stamp in pico sec.
	ULong64_t tsmallTimeStamp;  //! time stamp in pico sec.
	Long64_t deltaTstamp;  //! time stamp in pico sec.
	UInt_t    time;    //! real computer time in sec

	unsigned short barIndex;
	unsigned short layerIndex;

	Point3D hitPosition;

	bool barHitted;


	ScintillatorBar(unsigned short l_channelNear, unsigned short l_channelFar,
					ULong64_t l_tstampNear, ULong64_t l_tstampFar,
					UInt_t l_qlongNear,	UInt_t l_qlongFar, UInt_t l_time){

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
		time=l_time;
		EstimateHitPosition();

	}

	/*
	 * This constructor will be useful for simulation
	 */
	ScintillatorBar(unsigned short bIndex){
		barHitted = false;
		barIndex = bIndex;
		layerIndex = barIndex/numOfBarsInEachLayer;
		unsigned short l_channelNear = 2*barIndex;
		unsigned short l_channelFar = l_channelNear+1;
		scintName="PsBar"+std::to_string(barIndex)+"-"+std::to_string(l_channelNear)+"-"+std::to_string(l_channelFar);

		/* qlongMean, the most import parameter, should be filled as total energy
		 * deposited in a bar, which is basically the sum of energy deposited in
		 * individual steps
		 *
		 * remaining data members may be set to zero.
		 */
		qlongMean = 0;
		tstampNear=0;
		tstampFar=0;
		tsmallTimeStamp = 0;
		deltaTstamp=0;
		time=0;
		//EstimateHitPosition();


	}

	ScintillatorBar(){}

	void Print(){

		std::cout << scintName << " , " << qlongNear << " , " << qlongFar << " , " << qlongMean << " , "
				  << tstampNear  << " , " << tstampFar << " , " << tsmallTimeStamp << " , " << deltaTstamp << " , "
				  << time << " , " << " ( " << hitPosition.x << " , " << hitPosition.y << " , " << hitPosition.z << " ) " << std::endl;

	}

	/*
	 * Calculates the estimated hit position in cm along Y,
	 * deltaTstamp should be in ns
	 */
	void EstimateHitPositionAlongY(){

		hitPosition.y = 30*(deltaTstamp/1000.0);///fwhm;
		if(hitPosition.y < 0){
			hitPosition.y = -1.*hitPosition.y;
		}
	}

	/*
	 * Calculates the estimated hit position in cm along X,
	 *
	 * For the time being, this can be mid point along X.
	 * can be 5cm
	 */
	void EstimateHitPositionAlongX(){
		hitPosition.x = 5;
	}

	/*
	 * Calculates the estimated hit position in cm along Z,
	 *
	 * For the time being, this can be level of top surface along Z.
	 * i.e. multiple of 10,
	 * Consider the bottom most layer is at 0, then its Z = 1*10cm
	 * General logic
	 * Z=(layerIndex+1)*10
	 */
	void EstimateHitPositionAlongZ(){
		hitPosition.z = (layerIndex+1)*10.;
	}

	void EstimateHitPosition(){
		EstimateHitPositionAlongX();
		EstimateHitPositionAlongY();
		EstimateHitPositionAlongZ();
	}




};


namespace Tomography {

class PsBar {
public:
	PsBar();
	virtual ~PsBar();
};

} /* namespace Tomography */

#endif /* ISMRAN_INC_PSBAR_H_ */
