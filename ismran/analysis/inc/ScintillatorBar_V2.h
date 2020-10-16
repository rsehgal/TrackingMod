/*
 * ScintillatorBar_V2.h
 *
 *  Created on: 03-Sep-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_INC_SCINTILLATORBAR_V2_H_
#define ISMRAN_ANALYSIS_INC_SCINTILLATORBAR_V2_H_

#include "includes.hh"
#include "PsBar.h"
#include "HitPositionAndError.h"

class Calibration;

class ScintillatorBar_V2 {

//Keeping data member also PUBLIC
public:
	std::string  scintName;    //! board #  and channel number ( its packed in as follows ) //! board*16 + chno.

	//Charge info may not be required, but keeping it for the time being.
	UInt_t    qlongNear;   //! integrated charge in long gate 88 nsec
	UInt_t    qlongFar;   //! integrated charge in long gate 88 nsec
	//UInt_t    qlongMean;   //! integrated charge in long gate 88 nsec
	Double_t    qlongMean;   //! integrated charge in long gate 88 nsec
	Double_t    qlongMeanCorrected;   //! integrated charge in long gate 88 nsec


	ULong64_t tstampNear;  //! time stamp in pico sec.
	ULong64_t tstampFar;  //! time stamp in pico sec.
	ULong64_t tsmallTimeStamp;  //! time stamp in pico sec.
	Long64_t deltaTstamp;  //! time stamp in pico sec.
	Long64_t deltaTstampCorrected;

	unsigned short barIndex;
	unsigned short layerIndex;



	/*These data members may be removed once
	 * the new mechanism starts working.
	 */
	Point3D hitPosition;
	Point3D hitPositionParam;
	Point3D hitPositionError;
	Point3D fittedHitPosition;

	/*
	 * Data members for new mechanism
	 */
	HitPositionAndError fhitPositionParam; //To Store estimated hit position from the parameterization
	HitPositionAndError fhitPostionSOL; //To Store the estimated hit postition from the Speed Of Light logic
	HitPositionAndError fhitPostionMean; //To Store the meanHit Position from simulation


	/*
	 * Introduced ONLY FOR SIMULATION to store the hits within
	 * each scintillator, must be cleared at the end of each
	 * event, after calculation of delT
	 */
	std::vector<Point3D*> hitsVectorInAnEventInABar;
	Point3D meanHitPosition;
	HitPositionAndError meanHitPositionAndError;

	void CalculateVariousPhysicalParameters(unsigned long muonNum);


	//Estimated Path length within each Bar
	double pathLength;
	bool barHitted;


public:
	ScintillatorBar_V2();
	ScintillatorBar_V2(unsigned int bIndex);
	ScintillatorBar_V2(const ScintillatorBar_V2 &sbar);
	ScintillatorBar_V2(unsigned short l_channelNear, unsigned short l_channelFar,
						ULong64_t l_tstampNear, ULong64_t l_tstampFar,
						UInt_t l_qlongNear,	UInt_t l_qlongFar);
	void EstimateHitPosition(Calibration *fCalib);

	//Constructor to generate Scintillator from simulated data
	ScintillatorBar_V2(ULong64_t l_tstampNear, ULong64_t l_tstampFar,
					   Double_t qlongmean, unsigned short barIndex);
	ScintillatorBar_V2(ULong64_t tstampnear, ULong64_t tstampfar,
					   Double_t qlongmean, unsigned short barindex, double hitx,double hity, double hitz);

	void EstimateHitPositionAlongX();
	void EstimateHitPositionAlongX(Point3D *temp, Point3D *tempError);
	void EstimateHitPositionAlongY();
	void EstimateHitPositionAlongY(Point3D *temp, Point3D *tempError);

	void EstimateHitPosition_V2(Calibration *fCalib);

	void Print();
	virtual ~ScintillatorBar_V2();
};

#endif /* ISMRAN_ANALYSIS_INC_SCINTILLATORBAR_V2_H_ */
