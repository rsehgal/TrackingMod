#include "ScintillatorBar_V2.h"

namespace lite_interface{

ScintillatorBar_V2::ScintillatorBar_V2(){

}

ScintillatorBar_V2::ScintillatorBar_V2(unsigned int bIndex){

}

ScintillatorBar_V2::ScintillatorBar_V2(const ScintillatorBar_V2 &sbar){

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

} /* End of lite_interface */
