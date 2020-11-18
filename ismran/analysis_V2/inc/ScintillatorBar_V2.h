/*
 * ScintillatorBar_V2.h
 *
 *  Created on: 08-Nov-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_V2_INC_SCINTILLATORBAR_V2_H_
#define ISMRAN_ANALYSIS_V2_INC_SCINTILLATORBAR_V2_H_

#include "includes.hh"

//#include "PsBar.h"
#include "Point3D.h"
//#include "HitPositionAndError.h"

//class Calibration;
#pragma once
#include <TROOT.h>

class TH1F;

namespace lite_interface{

class Point3D;
class Calibration;

class ScintillatorBar_V2 {

//Keeping data member also PUBLIC
public:
	 //! board #  and channel number ( its packed in as follows ) //! board*16 + chno.
	ushort  fBarIndex;
	 //! integrated charge in long gate 88 nsec
	UInt_t    fQlongNear;
	 //! integrated charge in long gate 88 nsec
	Double_t  fQlongMean;
	 //! time stamp in pico sec.
	ULong64_t fTSmallTimeStamp;
	 //! time stamp in pico sec.
	Long_t fDelTstamp;

	//Only for Simulation
	bool fBarHitted;


public:
	ScintillatorBar_V2();
	ScintillatorBar_V2(unsigned int bIndex);
	ScintillatorBar_V2(const ScintillatorBar_V2 &sbar);
	ScintillatorBar_V2(ushort barIndex, ushort qlongNear, ushort qlongMean,
						ULong64_t tstampSmall, Long_t delTStamp);
	//ScintillatorBar_V2(const ScintillatorBar_V2 &sbar);
	//void EstimateHitPosition(Calibration *fCalib);


	lite_interface::Point3D* EstimateHitPosition();
	void EstimateHitPositionAlongX();
	void EstimateHitPositionAlongX(Point3D *temp, Point3D *tempError);
	void EstimateHitPositionAlongY();
	void EstimateHitPositionAlongY(Point3D *temp, Point3D *tempError);
	lite_interface::Point3D* EstimateHitPosition_Param();

	/* 
	 * Required Getters
	 */
	ushort GetBarIndex()const;
	ushort GetLayerIndex() ;
	UInt_t GetQLongNear()const;
	UInt_t GetQLongFar();
	Double_t GetQLongMean() const;
	ULong64_t GetTSmallTimestamp() const;
	Long_t GetDelT() const;
	Long_t GetDelTCorrected();
	Double_t GetQMeanCorrected();

	//void EstimateHitPosition_V2(Calibration *fCalib);

	void Print();
	/*
 	 * Some histogram specific to Scintillator
	 * opt : 0 -> QMean
	 * opt : 1 -> QNear
	 * opt : 2 -> QFar
	 */
	//TH1F* GetHistQ_012(ushort opt);

	virtual ~ScintillatorBar_V2();

	void CalculateVariousPhysicalParameters(unsigned long muonNum, Calibration *calib);
	void CalculateVariousPhysicalParameters(unsigned long muonNum);

	ClassDef(ScintillatorBar_V2,1)
};
} /* End of lite_interface namespace */
#endif /* ISMRAN_ANALYSIS_V2_INC_SCINTILLATORBAR_V2_H_ */
