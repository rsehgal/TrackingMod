/*
 * ScintillatorBar_V2.cpp
 *
 *  Created on: 03-Sep-2020
 *      Author: rsehgal
 */

#include "ScintillatorBar_V2.h"
#include "HardwareNomenclature.h"
#include "G4SystemOfUnits.hh"
#include "Calibration.h"
#include "colors.h"
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
		hitsVectorInAnEventInABar = sbar.hitsVectorInAnEventInABar;

	}

/*
 * Constructor to be used only for Simulated data
 */
ScintillatorBar_V2::ScintillatorBar_V2(ULong64_t tstampnear, ULong64_t tstampfar,
					   Double_t qlongmean, unsigned short barindex){

		barIndex = barindex;
		layerIndex = barIndex/numOfBarsInEachLayer;
		scintName = vecOfBarsNamess[barIndex];
		qlongMean = qlongmean;
		qlongMeanCorrected = qlongMean*1000.;
		qlongNear = 0;
		qlongFar = 0;
		tstampNear = tstampnear;
		tstampFar = tstampfar;
		tsmallTimeStamp = (tstampNear < tstampFar) ? tstampNear : tstampFar;
		deltaTstamp=tstampNear-tstampFar;
		deltaTstampCorrected=deltaTstamp;
		pathLength = 0.;
}

ScintillatorBar_V2::ScintillatorBar_V2(ULong64_t tstampnear, ULong64_t tstampfar,
					   Double_t qlongmean, unsigned short barindex, double hitx,double hity, double hitz){

		barIndex = barindex;
		layerIndex = barIndex/numOfBarsInEachLayer;
		scintName = vecOfBarsNamess[barIndex];
		qlongMean = qlongmean;
		qlongMeanCorrected = qlongMean*1000.;
		qlongNear = 0;
		qlongFar = 0;
		tstampNear = tstampnear;
		tstampFar = tstampfar;
		tsmallTimeStamp = (tstampNear < tstampFar) ? tstampNear : tstampFar;
		deltaTstamp=tstampNear-tstampFar;
		deltaTstampCorrected=deltaTstamp;
		pathLength = 0.;

		//May be remove lateron, with new schemem in place
		meanHitPosition.x = hitx;
		meanHitPosition.y = hity;
		meanHitPosition.z = hitz;

		fhitPostionMean.SetHitPosition(hitx,hity,hitz);


}
void ScintillatorBar_V2::Print(){
			std::cout <<"*************************************************************" << std::endl;
			std::cout << scintName << " , " << barIndex << " , " << qlongNear << " , " << qlongFar << " , " << qlongMean << " , "
					  << tstampNear  << " , " << tstampFar << " , " << tsmallTimeStamp << " , " << deltaTstamp << " , "
					  << " ( " << hitPosition.x << " , " << hitPosition.y << " , " << hitPosition.z << " )  : "
					  << "Energy Deposited : " << qlongMeanCorrected << " : Path Length : " << pathLength << std::endl;


}

void ScintillatorBar_V2::EstimateHitPosition_V2(Calibration *fCalib){

	TF1 *param = fCalib->GetCalibrationDataOf(barIndex)->fParameterization_F;
	double correctedDelT = deltaTstampCorrected/1000.;

	double estZparam = param->Eval(correctedDelT);
	const double *errors = param->GetParErrors();
	double estZparamError = errors[0]+errors[1]*correctedDelT+errors[2]*pow(correctedDelT,2)+errors[3]*pow(correctedDelT,3) ;

	double estZSOL = 0.5*correctedDelT*fCalib->GetCalibrationDataOf(barIndex)->fVelocityInsideScintillator;
	double estZSOLError = 0.;

	//std::cout << "========= : EstZSOL : " << estZSOL <<" :=================" << std::endl;
	/*
	 * Put a check on the estimated Z of the hit point
	 */
	if (estZSOL > -50. && estZSOL < 50.) {

		fhitPositionParam.SetHitPositionAndError(vecOfScintXYCenter[barIndex].x, vecOfScintXYCenter[barIndex].y, estZparam,
												 errorX, errorY, estZparamError);
		fhitPostionSOL.SetHitPositionAndError(vecOfScintXYCenter[barIndex].x, vecOfScintXYCenter[barIndex].y, estZSOL,
											  errorX, errorY, estZSOLError);

		verbose = false;
		if(verbose)
		{
			std::cout <<"==============================================================================================" << std::endl;
			std::cout<<"Errors : (" << errors[0] << " : " << errors[1] <<" : " << errors[2] <<" : " << errors[3] << ")" << std::endl;
			std::cout << "ScintName : " << scintName << " : TNear : " << tstampNear <<" : TFar : " << tstampFar << " : CorrectedDelT : "
					  << correctedDelT << GREEN << " : Mean Hit Position Z : " << meanHitPosition.z << YELLOW << " : Estimated Z : "
					  << estZSOL << MAGENTA << " : EstZParam : " << estZparam  << RESET << RED
					  << " : Diff between two methods of estimation : " << (estZSOL-estZparam) << RESET
					  << " : Error in estimated Z postion from parameterization : " << estZparamError << " : " << __FILE__ <<" : " << __LINE__ << std::endl;
			std::cout <<"==============================================================================================" << std::endl;
		}

	}


}

void ScintillatorBar_V2::EstimateHitPositionAlongX(Point3D *temp, Point3D *tempError){
		temp->x = vecOfScintXYCenter[barIndex].x;
		tempError->x = errorX;
}

void ScintillatorBar_V2::EstimateHitPositionAlongY(Point3D *temp, Point3D *tempError){
		temp->y = vecOfScintXYCenter[barIndex].y;
		tempError->y = errorY;
}


void ScintillatorBar_V2::EstimateHitPosition(Calibration *fCalib){
	TF1 *param = fCalib->GetCalibrationDataOf(barIndex)->fParameterization_F;
	//long double correctedDelT = scint->deltaTstampCorrected / 1000.;
	//double correctedDelT = scint->deltaTstampCorrected / 1000.;
	double correctedDelT = deltaTstampCorrected/1000.;

	float estZparm = param->Eval(correctedDelT);
	//Using  3rd order polynomial
	//float estZ = param->Eval(correctedDelT);


	float estZ2 = 0.5*correctedDelT*fCalib->GetCalibrationDataOf(barIndex)->fVelocityInsideScintillator;
	//Using new logic based on traversal of light in scintillator
	float estZ = 0.5*correctedDelT*fCalib->GetCalibrationDataOf(barIndex)->fVelocityInsideScintillator;

	//Using y = mx + c
	//float estZ = fCalib->GetCalibrationDataOf(barIndex)->c +
		//		 correctedDelT*fCalib->GetCalibrationDataOf(barIndex)->fVelocityInsideScintillator;

	const double *errors = param->GetParErrors();
	double estZError = errors[0]+errors[1]*correctedDelT+errors[2]*pow(correctedDelT,2)+errors[3]*pow(correctedDelT,3) ;
	//param->EvalPar(&correctedDelT,param->GetParErrors());
	//std::cout << "Corrected DelT : " << correctedDelT << " : Hit Position along Z : " << estZ << std::endl;
	/*(scint->hitPosition).z=estZ;
	scint->EstimateHitPositionAlongY();
	scint->EstimateHitPositionAlongX();
	 */
	/*
	 * Put a check on the estimated Z of the hit point
	 */
	if (estZ > -50. && estZ < 50.) {
		(hitPosition).z=estZ;
		hitPositionParam.z = estZparm;
		(hitPositionError).z=estZError;
		verbose = false;
		if(verbose)
		{
			std::cout <<"==============================================================================================" << std::endl;
			std::cout<<"Errors : (" << errors[0] << " : " << errors[1] <<" : " << errors[2] <<" : " << errors[3] << ")" << std::endl;
			std::cout << "ScintName : " << scintName << " : TNear : " << tstampNear <<" : TFar : " << tstampFar << " : CorrectedDelT : "
					  << correctedDelT << GREEN << " : Mean Hit Position Z : " << meanHitPosition.z << YELLOW << " : Estimated Z : "
					  << estZ2 << MAGENTA << " : EstZParam : " << estZparm  << RESET << RED
					  << " : Diff between two methods of estimation : " << (estZ2-estZparm) << RESET
					  << " : Error in Z postion : " << estZError << " : " << __FILE__ <<" : " << __LINE__ << std::endl;
			std::cout <<"==============================================================================================" << std::endl;
		}

		EstimateHitPositionAlongY();
		EstimateHitPositionAlongX();
	}
}

void ScintillatorBar_V2::EstimateHitPositionAlongX(){
		//hitPosition.x = (layerIndex+1)*10.;
		hitPosition.x = vecOfScintXYCenter[barIndex].x;
		hitPositionParam.x =  vecOfScintXYCenter[barIndex].x;
		hitPositionError.x = errorX;


}

void ScintillatorBar_V2::EstimateHitPositionAlongY(){
		//hitPosition.y = 0.;
		hitPosition.y = vecOfScintXYCenter[barIndex].y;
		hitPositionParam.y = vecOfScintXYCenter[barIndex].y;
		hitPositionError.y = errorY;
}


 //Commented for new schemem
  void ScintillatorBar_V2::CalculateVariousPhysicalParameters(unsigned long muonNum){
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
	//std::cout << "TimeStampNear : " << tstampNear <<" : TimeStampFar : " << tstampFar << std::endl;
	tsmallTimeStamp = (tstampNear < tstampFar) ? tstampNear : tstampFar;
	deltaTstamp = tstampNear-tstampFar;
	deltaTstampCorrected = deltaTstamp;	
}

/*
void ScintillatorBar_V2::CalculateVariousPhysicalParameters(unsigned long muonNum){
	qlongMeanCorrected = qlongMean*1000.;
	Point3D meanHitPosition_l;
	meanHitPosition_l.x = 0.;
	meanHitPosition_l.y = 0.;
	meanHitPosition_l.z = 0.;
	for(unsigned int i = 0 ; i < hitsVectorInAnEventInABar.size() ; i++){
		//std::cout << "Hit point Vec from ScintillatorBar_V2 : "; hitsVectorInAnEventInABar[i]->Print();
		meanHitPosition_l.x += hitsVectorInAnEventInABar[i]->x;
		meanHitPosition_l.y += hitsVectorInAnEventInABar[i]->y;
		meanHitPosition_l.z += hitsVectorInAnEventInABar[i]->z;
	}
	meanHitPosition_l.x /= hitsVectorInAnEventInABar.size();
	meanHitPosition_l.y /= hitsVectorInAnEventInABar.size();
	meanHitPosition_l.z /= hitsVectorInAnEventInABar.size();

	unsigned long int startTime = muonNum*timeBetweenTwoMuonTracks;
	//std::cout << "Muon Number : " << muonNum << " : startTime : " << startTime << " : " << __FILE__ <<" : " << __LINE__ << std::endl;
	//std::cout << "hitsVector.size() : " << hitsVectorInAnEventInABar.size() << std::endl;
	//meanHitPosition.Print();

	tstampNear = startTime + ((barLength/2. * cm + meanHitPosition.z)/(barLength*cm))*timeDiffNearFar;
	tstampFar = startTime + ((barLength/2. * cm - meanHitPosition.z)/(barLength*cm))*timeDiffNearFar;
	//std::cout << "TimeStampNear : " << tstampNear <<" : TimeStampFar : " << tstampFar << std::endl;
	tsmallTimeStamp = (tstampNear < tstampFar) ? tstampNear : tstampFar;
	deltaTstamp = tstampNear-tstampFar;
	deltaTstampCorrected = deltaTstamp;
	fhitPostionMean.SetHitPosition(meanHitPosition_l);
}
*/
