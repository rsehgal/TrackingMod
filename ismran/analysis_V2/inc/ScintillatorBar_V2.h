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
//#include "HitPositionAndError.h"

//class Calibration;
class TH1F;

namespace lite_interface{

class ScintillatorBar_V2 {

//Keeping data member also PUBLIC
public:
	ushort  fBarIndex;    //! board #  and channel number ( its packed in as follows ) //! board*16 + chno.
	UInt_t    fQlongNear;   //! integrated charge in long gate 88 nsec
	Double_t  fQlongMean;   //! integrated charge in long gate 88 nsec
	ULong64_t fTSmallTimeStamp;  //! time stamp in pico sec.
	Long_t fDelTstamp;  //! time stamp in pico sec.


public:
	ScintillatorBar_V2();
	ScintillatorBar_V2(unsigned int bIndex);
	ScintillatorBar_V2(const ScintillatorBar_V2 &sbar);
	ScintillatorBar_V2(ushort barIndex, ushort qlongNear, ushort qlongMean,
						ULong64_t tstampSmall, Long_t delTStamp);
	//void EstimateHitPosition(Calibration *fCalib);


	void EstimateHitPositionAlongX();
	void EstimateHitPositionAlongX(Point3D *temp, Point3D *tempError);
	void EstimateHitPositionAlongY();
	void EstimateHitPositionAlongY(Point3D *temp, Point3D *tempError);

	/* 
	 * Required Getters
	 */
	ushort GetBarIndex()const;
	UInt_t GetQLongNear()const;
	UInt_t GetQLongFar();
	Double_t GetQLongMean() const;
	ULong64_t GetTSmallTimestamp() const;
	Long_t GetDelT() const;

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
};
} /* End of lite_interface namespace */
#endif /* ISMRAN_ANALYSIS_V2_INC_SCINTILLATORBAR_V2_H_ */
