#include "ScintillatorBar_V2.h"
#include "Calibration.h"
#include "HardwareNomenclature.h"
#include "Point3D.h"

ClassImp(lite_interface::ScintillatorBar_V2);

namespace lite_interface{

ScintillatorBar_V2::ScintillatorBar_V2(){
	//std::cout << "SCINTILLATORBAR_V2 : Default Constructor called" << std::endl;
	fBarIndex = 1234;
		fQlongNear = 0;
		fQlongMean = 0;
		fTSmallTimeStamp = 0;
		fDelTstamp = 0;

}

ScintillatorBar_V2::ScintillatorBar_V2(unsigned int bIndex){

}

ScintillatorBar_V2::ScintillatorBar_V2(ushort barIndex, ushort qlongNear, 
									   ushort qlongMean,ULong64_t tstampSmall, 
									   Long_t delTStamp):fBarIndex(barIndex),
fQlongNear(qlongNear),
fQlongMean(qlongMean),
fTSmallTimeStamp(tstampSmall),
fDelTstamp(delTStamp){


}

ScintillatorBar_V2::ScintillatorBar_V2(const ScintillatorBar_V2 &sbar){
		//if(verbose)
			//std::cout << "======= COPY CONSTRUCTOR CaLLED ==========" << std::endl;
	//std::cout << "SCINTILLATORBAR_V2 : Copy Constructor called" << std::endl;
			fBarIndex = sbar.fBarIndex;
			fQlongNear = sbar.fQlongNear;
			fQlongMean = sbar.fQlongMean;
			fTSmallTimeStamp = sbar.fTSmallTimeStamp;
			fDelTstamp = sbar.fDelTstamp;

}


//void ScintillatorBar_V2::EstimateHitPosition(Calibration *fCalib);

lite_interface::Point3D* ScintillatorBar_V2::EstimateHitPosition(){
	//(1.0*100)/22.0) = 545454545
	double zval = ( 4.545454545 * ( GetDelTCorrected()/1000. + 11.)) - 50.;
	return (new lite_interface::Point3D(vecOfScintXYCenter[fBarIndex].x,
						 vecOfScintXYCenter[fBarIndex].y,
						 zval));
}

lite_interface::Point3D* ScintillatorBar_V2::EstimateHitPosition_Param(){
	//(1.0*100)/22.0) = 545454545
	//double zval = ( 4.545454545 * ( GetDelTCorrected()/1000. + 11.)) - 50.;
	TF1 *param = lite_interface::Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fParameterization_F;
	double zval = param->Eval(GetDelTCorrected()/1000.);

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
	std::cout <<"BarIndex : " << fBarIndex <<  " :  Energy :  " << GetQMeanCorrected() << std::endl;

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
	return (fDelTstamp - Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fDeltaTCorr*1000);
}
Double_t ScintillatorBar_V2::GetQMeanCorrected(){
	//return (fQlongMean + Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fEnergyCalibrationFactor);

	TF1 *enercalibFormula = Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fEnergyCalibration_F;
	Double_t ener = (enercalibFormula->Eval(fQlongMean)) ;

	return ener;
	/*TGraph *gr = Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fGraphWithMuonPoint;
	new TCanvas();
	gr->SetMarkerStyle(8);
	gr->Draw();
	*/

	//std::cout <<"QlongMean : " << fQlongMean <<  " : Corrected Energy : " << ener << std::endl;


}
} /* End of lite_interface */
