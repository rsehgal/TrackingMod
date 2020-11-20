#include "ScintillatorBar_V2.h"
#include "Calibration.h"
#include "HardwareNomenclature.h"
#include "Point3D.h"
#include "Analyzer.h"
#define cm 10
#ifndef FOR_SIMULATION
ClassImp(lite_interface::ScintillatorBar_V2);
#endif
namespace lite_interface{

ScintillatorBar_V2::ScintillatorBar_V2(){
	//std::cout << "SCINTILLATORBAR_V2 : Default Constructor called" << std::endl;
	fBarIndex = 1234;
		fQlongNear = 0;
		fQlongMean = 0;
		fTSmallTimeStamp = 0;
		fDelTstamp = 0;

}

ScintillatorBar_V2::ScintillatorBar_V2(unsigned int bIndex):
		fBarIndex(bIndex),
		fQlongNear(0),
		fQlongMean(0),
		fTSmallTimeStamp(0),
		fDelTstamp(0)
#ifdef FOR_SIMULATION
		,fBarHitted(false)
#endif
		{


}

ScintillatorBar_V2::ScintillatorBar_V2(ushort barIndex, ushort qlongNear, 
									   ushort qlongMean,ULong64_t tstampSmall, 
									   Long_t delTStamp):
fBarIndex(barIndex),
fQlongNear(qlongNear),
fQlongMean(qlongMean),
fTSmallTimeStamp(tstampSmall),
fDelTstamp(delTStamp){


}

#if defined(FOR_SIMULATION) || defined(USE_FOR_SIMULATION)
ScintillatorBar_V2::ScintillatorBar_V2(ushort barIndex, ushort qlongNear,
									   ushort qlongMean,ULong64_t tstampSmall,
									   Long_t delTStamp, double hitx, double hity, double hitz):
fBarIndex(barIndex),
fQlongNear(qlongNear),
fQlongMean(qlongMean),
fTSmallTimeStamp(tstampSmall),
fDelTstamp(delTStamp),
hitX(hitx), hitY(hity), hitZ(hitz){

//Print();
}
#endif

ScintillatorBar_V2::ScintillatorBar_V2(const ScintillatorBar_V2 &sbar){
		//if(verbose)
			//std::cout << "======= COPY CONSTRUCTOR CaLLED ==========" << std::endl;
	//std::cout << "SCINTILLATORBAR_V2 : Copy Constructor called" << std::endl;
			fBarIndex = sbar.fBarIndex;
			fQlongNear = sbar.fQlongNear;
			fQlongMean = sbar.fQlongMean;
			fTSmallTimeStamp = sbar.fTSmallTimeStamp;
			fDelTstamp = sbar.fDelTstamp;
#if defined(FOR_SIMULATION) || defined(USE_FOR_SIMULATION)
			fBarHitted = sbar.fBarHitted;
			hitsVectorInAnEventInABar = sbar.hitsVectorInAnEventInABar;
			hitX = sbar.hitX;
			hitY = sbar.hitY;
			hitZ = sbar.hitZ;
#endif

}


//void ScintillatorBar_V2::EstimateHitPosition(Calibration *fCalib);

lite_interface::Point3D* ScintillatorBar_V2::EstimateHitPosition(){
	//(1.0*100)/22.0) = 545454545
	double zval = 0.;
	if(IsSimulation)
		zval = ( 4.545454545 * ( GetDelTCorrected()/1000. + 11.)) - 50.;
	else
		zval = ( 4.545454545 * ( GetDelTCorrected()/1000. + 11.)) - 50.;
	return (new lite_interface::Point3D(vecOfScintXYCenter[fBarIndex].x,
						 vecOfScintXYCenter[fBarIndex].y,
						 zval));
}

lite_interface::Point3D* ScintillatorBar_V2::EstimateHitPosition_Param(){
	//(1.0*100)/22.0) = 545454545
	//double zval = ( 4.545454545 * ( GetDelTCorrected()/1000. + 11.)) - 50.;
	TF1 *param = lite_interface::Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fParameterization_F;
	double zval = 0.;
	if(IsSimulation)
		zval = param->Eval(GetDelTCorrected()/1000.);
	else
		zval = param->Eval(GetDelTCorrected()/1000.);

	return (new lite_interface::Point3D(vecOfScintXYCenter[fBarIndex].x,
						 vecOfScintXYCenter[fBarIndex].y,
						 zval));
}

void ScintillatorBar_V2::EstimateHitPositionAlongX(){

}

void ScintillatorBar_V2::EstimateHitPositionAlongX(Point3D *temp, Point3D *tempError){
	temp->x = vecOfScintXYCenter[fBarIndex].x;
	tempError->x = errorX;
}

void ScintillatorBar_V2::EstimateHitPositionAlongY(){

}

void ScintillatorBar_V2::EstimateHitPositionAlongY(Point3D *temp, Point3D *tempError){
	temp->y = vecOfScintXYCenter[fBarIndex].y;
	tempError->y = errorY;
}

//void ScintillatorBar_V2::EstimateHitPosition_V2(Calibration *fCalib);

void ScintillatorBar_V2::Print(){
	std::cout <<"BarIndex : " << fBarIndex <<  " :  Energy :  " << GetQMeanCorrected() << " : DelT : " << GetDelT() << std::endl;
	std::cout << "Mean Hit Position : " << hitX <<" , " << hitY << " , " << hitZ << std::endl;

}

ScintillatorBar_V2::~ScintillatorBar_V2(){

}

ushort ScintillatorBar_V2::GetBarIndex()const{
	return fBarIndex;
}
ushort ScintillatorBar_V2::GetLayerIndex(){
	return  (fBarIndex/numOfBarsInEachLayer);
}
UInt_t ScintillatorBar_V2::GetQLongNear()const{
	return fQlongNear;
}
UInt_t ScintillatorBar_V2::GetQLongFar(){
	return (fQlongMean*fQlongMean)/fQlongNear;
}
Double_t ScintillatorBar_V2::GetQLongMean() const{
	return fQlongMean;
}
ULong64_t ScintillatorBar_V2::GetTSmallTimestamp() const{
	return fTSmallTimeStamp;
}
Long_t ScintillatorBar_V2::GetDelT() const{
	return fDelTstamp;
}
Long_t ScintillatorBar_V2::GetDelTCorrected(){
	if(IsSimulation)
#ifdef USE_CALIBRATION
		return (fDelTstamp - Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fDeltaTCorr*1000);
#else
		return fDelTstamp;
#endif
	else
		return (fDelTstamp - Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fDeltaTCorr*1000);
}
Double_t ScintillatorBar_V2::GetQMeanCorrected(){
	//return (fQlongMean + Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fEnergyCalibrationFactor);

	if(IsSimulation)
		return fQlongMean;
	else{
		TF1 *enercalibFormula = Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fEnergyCalibration_F;
		Double_t ener = (enercalibFormula->Eval(fQlongMean)) ;
		return ener;
	}
	/*TGraph *gr = Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fGraphWithMuonPoint;
	new TCanvas();
	gr->SetMarkerStyle(8);
	gr->Draw();
	*/

	//std::cout <<"QlongMean : " << fQlongMean <<  " : Corrected Energy : " << ener << std::endl;


}
#ifdef USE_FOR_SIMULATION


lite_interface::Point3D* ScintillatorBar_V2::GetMeanHitPosition(){
	return (new lite_interface::Point3D(hitX, hitY, hitZ));
}
#endif

#ifdef USE_CALIBRATION

void ScintillatorBar_V2::CalculateVariousPhysicalParameters(unsigned long muonNum, lite_interface::Calibration *calib){
/*
	qlongMeanCorrected = qlongMean*1000.;
		meanHitPosition.x = 0.;
		meanHitPosition.y = 0.;
		meanHitPosition.z = 0.;
		for(unsigned int i = 0 ; i < hitsVectorInAnEventInABar.size() ; i++){
			//std::cout << "Hit point Vec from ScintillatorBar_V2 : "; hitsVectorInAnEventInABar[i]->Print();
			meanHitPosition.x += hitsVectorInAnEventInABar[i]->x;
			meanHitPosition.y += hitsVectorInAnEventInABar[i]->y;
			meanHitPosition.z += hitsVectorInAnEventInABar[i]->z;
		}
		meanHitPosition.x /= hitsVectorInAnEventInABar.size();
		meanHitPosition.y /= hitsVectorInAnEventInABar.size();
		meanHitPosition.z /= hitsVectorInAnEventInABar.size();

		unsigned long int startTime = muonNum*timeBetweenTwoMuonTracks;
		//std::cout << "Muon Number : " << muonNum << " : startTime : " << startTime << " : " << __FILE__ <<" : " << __LINE__ << std::endl;
		//std::cout << "hitsVector.size() : " << hitsVectorInAnEventInABar.size() << std::endl;
		//meanHitPosition.Print();

		tstampNear = startTime + ((barLength/2. * cm + meanHitPosition.z)/(barLength*cm))*timeDiffNearFar;
		tstampFar = startTime + ((barLength/2. * cm - meanHitPosition.z)/(barLength*cm))*timeDiffNearFar;
*/
		//std::cout << "TimeStampNear : " << tstampNear <<" : TimeStampFar : " << tstampFar << std::endl;
//		tsmallTimeStamp = (tstampNear < tstampFar) ? tstampNear : tstampFar;

	fMeanHitPosition = new Point3D();
	fMeanHitPosition->SetZero();
	for(unsigned int i = 0 ; i < hitsVectorInAnEventInABar.size() ; i++){
		lite_interface::Point3D *hitpt = hitsVectorInAnEventInABar[i];
		fMeanHitPosition->SetXYZ((fMeanHitPosition->GetX() + hitpt->GetX()),
								 (fMeanHitPosition->GetY() + hitpt->GetY()),
								 fMeanHitPosition->GetZ() + hitpt->GetZ());

	}
	int n = hitsVectorInAnEventInABar.size();
	fMeanHitPosition->Divide(n);

	unsigned long int startTime = muonNum*timeBetweenTwoMuonTracks;

	ULong64_t tstampNear = startTime + ((barLength/2. * cm + fMeanHitPosition->GetZ())/(barLength*cm))*timeDiffNearFar;
	ULong64_t tstampFar = startTime + ((barLength/2. * cm - fMeanHitPosition->GetZ())/(barLength*cm))*timeDiffNearFar;

		/*
		 * Below two member to be set using the calibration data.
		 */
		//deltaTstamp = tstampNear-tstampFar;
		//deltaTstampCorrected = deltaTstamp;
		lite_interface::CalibrationData *calibDataOfScint = calib->GetCalibrationDataVector()[fBarIndex];
		int quotient = (((int)fMeanHitPosition->GetZ())/10)/10;
		//int formulaIndex = (quotient > 0) ? (quotient+1) : quotient;

		int formulaIndex = quotient + 4;
		if(formulaIndex < 0)
			formulaIndex = 0;
		if(formulaIndex > 8)
			formulaIndex = 8;
		//std::cout << "FormulaIndex : " << formulaIndex << std::endl;
		TF1 *formula = (calibDataOfScint->fVectorOfDelT_F)[formulaIndex];
		fDelTstamp = (formula->GetRandom())*1000.;
		//std::cout << "DeltaTstamp : " << deltaTstamp << std::endl;
		//deltaTstampCorrected = deltaTstamp - calibDataOfScint->fDeltaTCorr;


}
#endif

#ifdef FOR_SIMULATION
  void ScintillatorBar_V2::CalculateVariousPhysicalParameters(unsigned long muonNum){

	fMeanHitPosition = new Point3D();
	fMeanHitPosition->SetZero();
	for(unsigned int i = 0 ; i < hitsVectorInAnEventInABar.size() ; i++){
		lite_interface::Point3D *hitpt = hitsVectorInAnEventInABar[i];
		fMeanHitPosition->SetXYZ((fMeanHitPosition->GetX() + hitpt->GetX()),
								 (fMeanHitPosition->GetY() + hitpt->GetY()),
								 fMeanHitPosition->GetZ() + hitpt->GetZ());

	}
	int n = hitsVectorInAnEventInABar.size();
	fMeanHitPosition->Divide(n);

	unsigned long int startTime = muonNum*timeBetweenTwoMuonTracks;

	ULong64_t tstampNear = startTime + ((barLength/2. * cm + fMeanHitPosition->GetZ())/(barLength*cm))*timeDiffNearFar;
	ULong64_t tstampFar = startTime + ((barLength/2. * cm - fMeanHitPosition->GetZ())/(barLength*cm))*timeDiffNearFar;
	if(n > 0){
	//std::cout << "TimeStampNear : " << tstampNear <<" : TimeStampFar : " << tstampFar << std::endl;
	}
	fTSmallTimeStamp = (tstampNear < tstampFar) ? tstampNear : tstampFar;
	fDelTstamp = tstampNear-tstampFar;

}
#endif
} /* End of lite_interface */
