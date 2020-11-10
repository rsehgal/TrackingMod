#include "ScintillatorBar_V2.h"
#include "Calibration.h"

ClassImp(lite_interface::ScintillatorBar_V2);

namespace lite_interface{

ScintillatorBar_V2::ScintillatorBar_V2(){

}

ScintillatorBar_V2::ScintillatorBar_V2(unsigned int bIndex){

}

ScintillatorBar_V2::ScintillatorBar_V2(ushort barIndex, ushort qlongNear, 
									   ushort qlongMean,ULong64_t tstampSmall, 
									   Long_t delTStamp){
	fBarIndex = barIndex;
	fQlongNear = qlongNear;
	fQlongMean = qlongMean;
	fTSmallTimeStamp = tstampSmall;
	fDelTstamp = delTStamp;

}

ScintillatorBar_V2::ScintillatorBar_V2(const ScintillatorBar_V2 &sbar){
		//if(verbose)
			std::cout << "======= COPY CONSTRUCTOR CaLLED ==========" << std::endl;
			fBarIndex = sbar.fBarIndex;
			fQlongNear = sbar.fQlongNear;
			fQlongMean = sbar.fQlongMean;
			fTSmallTimeStamp = sbar.fTSmallTimeStamp;
			fDelTstamp = sbar.fDelTstamp;

}


//void ScintillatorBar_V2::EstimateHitPosition(Calibration *fCalib);


void ScintillatorBar_V2::EstimateHitPositionAlongX(){

}

void ScintillatorBar_V2::EstimateHitPositionAlongX(Point3D *temp, Point3D *tempError){

}

void ScintillatorBar_V2::EstimateHitPositionAlongY(){

}

void ScintillatorBar_V2::EstimateHitPositionAlongY(Point3D *temp, Point3D *tempError){

}

//void ScintillatorBar_V2::EstimateHitPosition_V2(Calibration *fCalib);

void ScintillatorBar_V2::Print(){
	std::cout <<"BarIndex : " << fBarIndex << std::endl;
}

ScintillatorBar_V2::~ScintillatorBar_V2(){

}

ushort ScintillatorBar_V2::GetBarIndex()const{
	return fBarIndex;
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
