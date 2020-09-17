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

	Point3D hitPosition;
	Point3D fittedHitPosition;

	//Estimated Path length within each Bar
	double pathLength;


public:
	ScintillatorBar_V2();
	ScintillatorBar_V2(const ScintillatorBar_V2 &sbar);
	ScintillatorBar_V2(unsigned short l_channelNear, unsigned short l_channelFar,
						ULong64_t l_tstampNear, ULong64_t l_tstampFar,
						UInt_t l_qlongNear,	UInt_t l_qlongFar);

	void EstimateHitPositionAlongX();
	void EstimateHitPositionAlongY();

	void Print();
	virtual ~ScintillatorBar_V2();
};

#endif /* ISMRAN_ANALYSIS_INC_SCINTILLATORBAR_V2_H_ */