#include "ScintillatorBar_V2.h"
#include "Calibration.h"
#include "HardwareNomenclature.h"
#include "Point3D.h"
#include "Analyzer.h"
#include "colors.h"
#define cm 10
#ifndef FOR_SIMULATION
ClassImp(lite_interface::ScintillatorBar_V2);
#endif
namespace lite_interface {

ScintillatorBar_V2::ScintillatorBar_V2()
{
  // std::cout << "SCINTILLATORBAR_V2 : Default Constructor called" << std::endl;
  fBarIndex        = 1234;
  fQlongNear       = 0;
  fQlongMean       = 0;
  fTSmallTimeStamp = 0;
  fDelTstamp       = 0;
  hitX             = 0.;
  hitY             = 0.;
  hitZ             = 0.;
}

ScintillatorBar_V2::ScintillatorBar_V2(unsigned int bIndex)
    : fBarIndex(bIndex), fQlongNear(0), fQlongMean(0), fTSmallTimeStamp(0), fDelTstamp(0), hitX(0.), hitY(0.), hitZ(0.)

#ifdef FOR_SIMULATION
      ,
      fBarHitted(false), exactHitX(0.), exactHitY(0.), exactHitZ(0.)
#endif
{
#ifdef FOR_SIMULATION
  fExactHitPosition = new Point3D();
#endif
}

ScintillatorBar_V2::ScintillatorBar_V2(ushort barIndex, ushort qlongNear, ushort qlongMean, ULong64_t tstampSmall,
                                       Long_t delTStamp)
    : fBarIndex(barIndex), fQlongNear(qlongNear), fQlongMean(qlongMean), fTSmallTimeStamp(tstampSmall),
      fDelTstamp(delTStamp), hitX(0.), hitY(0.), hitZ(0.)
{

  // fittedLinear = new lite_interface::Point3D();
  // fittedParam = new lite_interface::Point3D();
  // fittedMean = new lite_interface::Point3D();
}

#if defined(FOR_SIMULATION) || defined(USE_FOR_SIMULATION)
ScintillatorBar_V2::ScintillatorBar_V2(ushort barIndex, ushort qlongNear, ushort qlongMean, ULong64_t tstampSmall,
                                       Long_t delTStamp, double hitx, double hity, double hitz)
    : fBarIndex(barIndex), fQlongNear(qlongNear), fQlongMean(qlongMean), fTSmallTimeStamp(tstampSmall),
      fDelTstamp(delTStamp), hitX(hitx), hitY(hity), hitZ(hitz)
{

  /*
    fittedLinear = new lite_interface::Point3D();
    fittedParam = new lite_interface::Point3D();
    fittedMean = new lite_interface::Point3D();
  */
  // Print();
}

ScintillatorBar_V2::ScintillatorBar_V2(ushort barIndex, ushort qlongNear, ushort qlongMean, ULong64_t tstampSmall,
                                       Long_t delTStamp, double hitx, double hity, double hitz, double exacthitX,
                                       double exacthitY, double exacthitZ)
    : fBarIndex(barIndex), fQlongNear(qlongNear), fQlongMean(qlongMean), fTSmallTimeStamp(tstampSmall),
      fDelTstamp(delTStamp), hitX(hitx), hitY(hity), hitZ(hitz), exactHitX(exacthitX), exactHitY(exacthitY),
      exactHitZ(exacthitZ)
{

  /*
    fittedLinear = new lite_interface::Point3D();
    fittedParam = new lite_interface::Point3D();
    fittedMean = new lite_interface::Point3D();
  */
  // Print();
}
ScintillatorBar_V2::ScintillatorBar_V2(ushort barIndex, ushort qlongNear, ushort qlongMean, ULong64_t tstampSmall,
                                       Long_t delTStamp, double hitx, double hity, double hitz, double exacthitX,
                                       double exacthitY, double exacthitZ,double logQFarByQNear)
    : fBarIndex(barIndex), fQlongNear(qlongNear), fQlongMean(qlongMean), fTSmallTimeStamp(tstampSmall),
      fDelTstamp(delTStamp), hitX(hitx), hitY(hity), hitZ(hitz), exactHitX(exacthitX), exactHitY(exacthitY),
      exactHitZ(exacthitZ),fLogQFarByQNear(logQFarByQNear)
{
//std::cout <<"@@@@@@@@@ PRINT FROM NEW CONSTRUCTOR @@@@@@@@@@@@@" << std::endl;
//Print();
}

#endif

ScintillatorBar_V2::ScintillatorBar_V2(const ScintillatorBar_V2 &sbar)
{
  // if(verbose)
  // std::cout << "======= COPY CONSTRUCTOR CaLLED ==========" << std::endl;
  // std::cout << "SCINTILLATORBAR_V2 : Copy Constructor called" << std::endl;
  fBarIndex        = sbar.fBarIndex;
  fQlongNear       = sbar.fQlongNear;
  fQlongMean       = sbar.fQlongMean;
  fTSmallTimeStamp = sbar.fTSmallTimeStamp;
  fDelTstamp       = sbar.fDelTstamp;
  fLogQFarByQNear  = sbar.fLogQFarByQNear;
  // fittedLinear = sbar.fittedLinear;
  // fittedParam = sbar.fittedParam;
#if defined(FOR_SIMULATION) || defined(USE_FOR_SIMULATION)

  // fittedMean = sbar.fittedMean;
  fBarHitted                = sbar.fBarHitted;
  hitsVectorInAnEventInABar = sbar.hitsVectorInAnEventInABar;
  hitX                      = sbar.hitX;
  hitY                      = sbar.hitY;
  hitZ                      = sbar.hitZ;
  exactHitX                 = sbar.exactHitX;
  exactHitY                 = sbar.exactHitY;
  exactHitZ                 = sbar.exactHitZ;
  fExactHitPosition         = sbar.fExactHitPosition;
#endif
}

// void ScintillatorBar_V2::EstimateHitPosition(Calibration *fCalib);

lite_interface::Point3D *ScintillatorBar_V2::EstimateHitPosition()
{
  //(1.0*100)/22.0) = 545454545
  double zval = 0.;
  if (IsSimulation)
    zval = (4.545454545 * (GetDelTCorrected() / 1000. + 11.)) - 50.;
  else
    zval = (4.545454545 * (GetDelTCorrected() / 1000. + 11.)) - 50.;
  return (new lite_interface::Point3D(vecOfScintXYCenter[fBarIndex].x, vecOfScintXYCenter[fBarIndex].y, zval));
}

lite_interface::Point3D *ScintillatorBar_V2::EstimateHitPosition_Param()
{
  //(1.0*100)/22.0) = 545454545
  // double zval = ( 4.545454545 * ( GetDelTCorrected()/1000. + 11.)) - 50.;
  // std::cout << "BAR INDEX : " << fBarIndex << " : " << __FILE__ << " : " << __LINE__ << std::endl;
  TF1 *param     = lite_interface::Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fParameterization_F;
  double zval    = 0.;
  double xOrZval = 0.;

  if (IsSimulation)
    xOrZval = param->Eval(GetDelTCorrected() / 1000.);
  else
    xOrZval = param->Eval(GetDelTCorrected() / 1000.);

#ifdef STAGGERED_GEOM
  if (GetLayerIndex() % 2) {
    return (new lite_interface::Point3D(xOrZval, vecOfScintXYCenter[fBarIndex].y, vecOfScintXYCenter[fBarIndex].x));
  } else {
    return (new lite_interface::Point3D(vecOfScintXYCenter[fBarIndex].x, vecOfScintXYCenter[fBarIndex].y, xOrZval));
  }

#else
  return (new lite_interface::Point3D(vecOfScintXYCenter[fBarIndex].x, vecOfScintXYCenter[fBarIndex].y, xOrZval));
#endif
}

lite_interface::Point3D *ScintillatorBar_V2::EstimateHitPosition_QParam()
{
  TF1 *param     = lite_interface::Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fQParameterization_F;
  double zval    = 0.;
  double xOrZval = 0.;

#ifdef USE_FOR_SIMULATION
  double qval = GetLogQNearByQFar_ForSimulation();
#else

  double qval = GetLogQNearByQFar();
#endif
  if (IsSimulation)
    xOrZval = param->Eval(qval);
  else
    xOrZval = param->Eval(qval);

#ifdef STAGGERED_GEOM
  if (GetLayerIndex() % 2) {
    return (new lite_interface::Point3D(xOrZval, vecOfScintXYCenter[fBarIndex].y, vecOfScintXYCenter[fBarIndex].x));
  } else {
	//lite_interface::Point3D *pt = new lite_interface::Point3D(vecOfScintXYCenter[fBarIndex].x, vecOfScintXYCenter[fBarIndex].y, xOrZval);
	//pt->Print();
    return (new lite_interface::Point3D(vecOfScintXYCenter[fBarIndex].x, vecOfScintXYCenter[fBarIndex].y, xOrZval));
  }

#else
  return (new lite_interface::Point3D(vecOfScintXYCenter[fBarIndex].x, vecOfScintXYCenter[fBarIndex].y, xOrZval));
#endif
}

void ScintillatorBar_V2::EstimateHitPositionAlongX() {}

void ScintillatorBar_V2::EstimateHitPositionAlongX(Point3D *temp, Point3D *tempError)
{
  temp->x      = vecOfScintXYCenter[fBarIndex].x;
  tempError->x = errorX;
}

void ScintillatorBar_V2::EstimateHitPositionAlongY() {}

void ScintillatorBar_V2::EstimateHitPositionAlongY(Point3D *temp, Point3D *tempError)
{
  temp->y      = vecOfScintXYCenter[fBarIndex].y;
  tempError->y = errorY;
}

// void ScintillatorBar_V2::EstimateHitPosition_V2(Calibration *fCalib);

void ScintillatorBar_V2::Print()
{
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << "BarIndex : " << fBarIndex << " : BAR Name : " << vecOfBarsNamess[fBarIndex]
            << " :  Energy :  " << GetQMeanCorrected() << " : DelT : " << GetDelT() << std::endl;
#ifdef USE_FOR_SIMULATION
  std::cout << "Mean Hit Position : " << hitX << " , " << hitY << " , " << hitZ << std::endl;
  std::cout << "Exact Hit Position individual : " << exactHitX << " , " << exactHitY << " , " << exactHitZ << std::endl;
  std::cout << "LogQFarByQNear : Stored Value  : " << fLogQFarByQNear <<" : Calculated value : " << GetLogQNearByQFar_ForSimulation() << std::endl;
  // std::cout << "Fitted Mean Hit position : " ; fittedMean->Print();
#else
#ifdef FOR_SIMULATION
  std::cout << "Mean Hit Position : ";
  fMeanHitPosition->Print();
  std::cout << "Exact Hit Position : ";
  fExactHitPosition->Print();
#endif

#endif

  //	std::cout << "Estimated Hit Position Linear : " ; EstimateHitPosition()->Print();

  //#ifdef USE_FOR_SIMULATION
  // std::cout << "Fitted Linear Hit position : " ; fittedLinear->Print();
  //#endif

  // std::cout << "Estimated Hit Position Param : " ; EstimateHitPosition_Param()->Print();

  //#ifdef USE_FOR_SIMULATION
  // std::cout << "Fitted Param Hit position : " ; fittedParam->Print();
  //#endif

#if defined(FOR_SIMULATION) || defined(USE_FOR_SIMULATION)

  std::cout << "############# Hit positioins within BAR : " << vecOfBarsNamess[fBarIndex] << "##############"
            << std::endl;
  for (unsigned int i = 0; i < hitsVectorInAnEventInABar.size(); i++) {
    hitsVectorInAnEventInABar[i]->Print();
  }
#endif
}

ScintillatorBar_V2::~ScintillatorBar_V2() {}

ushort ScintillatorBar_V2::GetBarIndex() const
{
  return fBarIndex;
}
ushort ScintillatorBar_V2::GetLayerIndex()
{
  return (fBarIndex / numOfBarsInEachLayer);
}

ushort ScintillatorBar_V2::GetBarIndexInLayer()
{
  return (fBarIndex % numOfBarsInEachLayer);
}

UInt_t ScintillatorBar_V2::GetQLongNear() const
{
  return fQlongNear;
}
UInt_t ScintillatorBar_V2::GetQLongFar()
{
  return (fQlongMean * fQlongMean) / fQlongNear;
}
Double_t ScintillatorBar_V2::GetQLongMean() const
{
  return fQlongMean;
}
ULong64_t ScintillatorBar_V2::GetTSmallTimestamp() const
{
  return fTSmallTimeStamp;
}

ULong64_t ScintillatorBar_V2::GetTAverage()
{
  /*if(fDelTstamp < 0.)
    return (2*fTSmallTimeStamp-fDelTstamp)/2.;
  else
    return (2*fTSmallTimeStamp+fDelTstamp)/2.;*/
  return (GetTNearCorr() / 2. + GetTFarCorr() / 2.);
}

ULong64_t ScintillatorBar_V2::GetTNearCorr()
{
  return GetTNear() - GetOffsetCorrection() / 2.;
}

ULong64_t ScintillatorBar_V2::GetTFarCorr()
{
  return GetTFar() + GetOffsetCorrection() / 2.;
}

ULong64_t ScintillatorBar_V2::GetTNear()
{
  if (fDelTstamp < 0.)
    return fTSmallTimeStamp;
  else
    return (fDelTstamp + fTSmallTimeStamp);
}

ULong64_t ScintillatorBar_V2::GetTFar()
{
  if (fDelTstamp > 0.)
    return fTSmallTimeStamp;
  else
    return (fTSmallTimeStamp - fDelTstamp);
}

Long_t ScintillatorBar_V2::GetDelT() const
{
  return fDelTstamp;
}

double ScintillatorBar_V2::GetOffsetCorrection()
{
  return Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fDeltaTCorr * 1000;
}

Long_t ScintillatorBar_V2::GetDelTCorrected()
{
  if (IsSimulation) {
    // std::cout << "FROM IF : IS_SIMULATION SET TO TRUE : " << __FILE__ << " : " << __LINE__ << std::endl;
#ifdef USE_CALIBRATION
    // return (fDelTstamp - Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fDeltaTCorr * 1000);
    return fDelTstamp;
#else
    return fDelTstamp;
#endif
  } else {
    // std::cout << "FROM ELSE : " << Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fDeltaTCorr * 1000 << "
    // : "
    //        << __FILE__ << " : " << __LINE__ << std::endl;
    return (fDelTstamp - Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fDeltaTCorr * 1000);
  }
}
Double_t ScintillatorBar_V2::GetQMeanCorrected()
{
  // return (fQlongMean + Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fEnergyCalibrationFactor);

  if (IsSimulation)
    return fQlongMean;
  else {
    Double_t ener = 0.;
#ifndef SINGLE_POINT_CALIBRATION
    // std::cout << "ENTERED GetQMeanCorrected : " << __FILE__ <<" : " << __LINE__ << std::endl;
    // std::cout << "BAR INDEX : " << fBarIndex << std::endl;
    TF1 *enercalibFormula = Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fEnergyCalibration_F;
    /*std::cout << "BARIndex" << fBarIndex <<  " : BarName : " << vecOfBarsNamess[fBarIndex] <<  " : QlongMean : " <<
       fQlongMean <<  " : Parameters : "
        << enercalibFormula->GetParameter(0) << " , " <<  enercalibFormula->GetParameter(1) << std::endl;*/
    ener = (enercalibFormula->Eval(fQlongMean));
    // std::cout << "Predicted Energy : " << ener << std::endl;
#else
    // Using single point calibration, and using equation of straight line to get y correspoinding to a x
    ener = (20. / (1. * GetPeakPos(vecOfBarsNamess[fBarIndex]))) * fQlongMean;
#endif
    return ener;
  }
  /*TGraph *gr = Calibration::instance()->GetCalibrationDataOf(fBarIndex)->fGraphWithMuonPoint;
  new TCanvas();
  gr->SetMarkerStyle(8);
  gr->Draw();
  */

  // std::cout <<"QlongMean : " << fQlongMean <<  " : Corrected Energy : " << ener << std::endl;
}
//#ifdef USE_FOR_SIMULATION
//#if defined(USE_FOR_SIMULATION) || defined(FOR_SIMULATION)

lite_interface::Point3D *ScintillatorBar_V2::GetMeanHitPosition()
{
#ifdef USE_FOR_SIMULATION
  return (new lite_interface::Point3D(hitX, hitY, hitZ));
#endif

#ifdef FOR_SIMULATION
  // fMeanHitPosition->Print();
  return fMeanHitPosition;
#endif
}

lite_interface::Point3D *ScintillatorBar_V2::GetExactHitPosition()
{
  // std::cout <<"X : " << exactHitX <<" : Y : " << exactHitX <<" : Z : " << exactHitZ << std::endl;
#ifdef USE_FOR_SIMULATION
  return (new lite_interface::Point3D(exactHitX, exactHitY, exactHitZ));
#endif
#ifdef FOR_SIMULATION
  // fMeanHitPosition->Print();
  return fExactHitPosition;
#endif
}
//#endif

#ifdef USE_CALIBRATION

void ScintillatorBar_V2::CalculateVariousPhysicalParameters(unsigned long muonNum, lite_interface::Calibration *calib)
{
  fMeanHitPosition = new Point3D();
  fMeanHitPosition->SetZero();

  // std::cout << "############# Hits Vector in Bar : " << fBarIndex << " #################" << std::endl;
  for (unsigned int i = 0; i < hitsVectorInAnEventInABar.size(); i++) {
    lite_interface::Point3D *hitpt = hitsVectorInAnEventInABar[i];
    // hitpt->Print();
    fMeanHitPosition->SetXYZ((fMeanHitPosition->GetX() + hitpt->GetX()), (fMeanHitPosition->GetY() + hitpt->GetY()),
                             fMeanHitPosition->GetZ() + hitpt->GetZ());
  }
  int n = hitsVectorInAnEventInABar.size();
  fMeanHitPosition->Divide(n);
  // std::cout << "--------------------------------------" << std::endl;
  // std::cout <<"Mean Hit Position : " ; fMeanHitPosition->Print();

  unsigned long int startTime = muonNum * timeBetweenTwoMuonTracks;

  ULong64_t tstampNear =
      startTime + ((barLength / 2. * cm + fMeanHitPosition->GetZ()) / (barLength * cm)) * timeDiffNearFar;
  ULong64_t tstampFar =
      startTime + ((barLength / 2. * cm - fMeanHitPosition->GetZ()) / (barLength * cm)) * timeDiffNearFar;

  /*
   * Below two member to be set using the calibration data.
   */
  // deltaTstamp = tstampNear-tstampFar;
  // deltaTstampCorrected = deltaTstamp;
  lite_interface::CalibrationData *calibDataOfScint = calib->GetCalibrationDataVector()[fBarIndex];
  int quotient                                      = (((int)fMeanHitPosition->GetZ()) / 10) / 10;
  // int formulaIndex = (quotient > 0) ? (quotient+1) : quotient;

  int formulaIndex = quotient + 4;
  if (formulaIndex < 0) formulaIndex = 0;
  if (formulaIndex > 8) formulaIndex = 8;
  // std::cout << "FormulaIndex : " << formulaIndex << std::endl;
  // TF1 *formula = (calibDataOfScint->fVectorOfDelT_F)[formulaIndex];
  // fDelTstamp   = (formula->GetRandom()) * 1000.;
  {
    TF1 *formulaFor = calibDataOfScint->fParameterization_F;
    TF1 *formulaForQ = calibDataOfScint->fQParameterization_F;
    TF1 *formulaRev = calibDataOfScint->fParameterization_F_Rev;
    // std::cout << "Mean Hit Position : "; fMeanHitPosition->Print();
    double delTMeanFromParam  = formulaRev->Eval(fMeanHitPosition->GetZ() / 10.) * 1000.;
    double delTSigmaFromParam = calibDataOfScint->fDelTCorr_F->GetParameter(2) * 1000.;
    double deltTstampValue    = GetGaussianRandomSample(delTMeanFromParam, delTSigmaFromParam);
    double smeared            = 510.;
    if (vecOfLayersOrientation[GetLayerIndex()]) {
      smeared = 510;
      while (smeared > 500. || smeared < -500)
        // smeared = GenRandom(fExactHitPosition->GetZ()-220, fExactHitPosition->GetZ()+220);
        // Should be GAUSSIAN SAMPLING
        smeared = GetGaussianRandomSample(fExactHitPosition->GetZ(), 180);

      // smeared = GetGaussianRandomSample(fMeanHitPosition->GetZ(), 140);

      // smeared = fMeanHitPosition->GetZ();//GetGaussianRandomSample(fMeanHitPosition->GetZ(), 10);
      // std::cout << RED << "Smeared Value of Z : " << smeared << RESET << std::endl;
      fDelTstamp = formulaFor->GetX(smeared / 10.) * 1000.;
      // fDelTstamp = formulaFor->GetX(fMeanHitPosition->GetZ() / 10.) * 1000.;
    } else {
      smeared = 510;
      // fDelTstamp = formulaFor->GetX(fMeanHitPosition->GetX() / 10.) * 1000.;
      while (smeared > 500. || smeared < -500.)
        // smeared = GetGaussianRandomSample(fExactHitPosition->GetX()-220, fExactHitPosition->GetX()+220);
        // Should be GAUSSIAN SAMPLING
        smeared = GetGaussianRandomSample(fExactHitPosition->GetX(), 180);
   	fLogQFarByQNear = formulaForQ->GetX(smeared/10.);
        //std::cout << MAGENTA << __FILE__ << " : " << __LINE__ <<" : LogQFarByQNear from Param : " << fLogQFarByQNear << RESET << std::endl;

      // smeared = GetGaussianRandomSample(fMeanHitPosition->GetX(), 140);

      // smeared = fMeanHitPosition->GetX();//GetGaussianRandomSample(fMeanHitPosition->GetX(), 10);
      // std::cout << BLUE << "Smeared Value of X : " << smeared << RESET << std::endl;
      fDelTstamp = formulaFor->GetX(smeared / 10.) * 1000.;
    }
  }
  fTSmallTimeStamp = (tstampNear < tstampFar) ? tstampNear : tstampFar;
  // std::cout << "DeltaTstamp : " << deltaTstamp << std::endl;
  // deltaTstampCorrected = deltaTstamp - calibDataOfScint->fDeltaTCorr;
}
#endif

#ifdef FOR_SIMULATION
void ScintillatorBar_V2::CalculateVariousPhysicalParameters(unsigned long muonNum)
{

  fMeanHitPosition = new Point3D();

  fMeanHitPosition->SetZero();
  for (unsigned int i = 0; i < hitsVectorInAnEventInABar.size(); i++) {
    lite_interface::Point3D *hitpt = hitsVectorInAnEventInABar[i];
    // if(i==(hitsVectorInAnEventInABar.size()-1))
    //		if(i==0){
    //			std::cout << "================ Bar Index : " << fBarIndex << " =========================" << std::endl;
    //			//std::cout << "First Hit : " ; hitpt->Print() ;
    //		}
    //		hitpt->Print();
    fMeanHitPosition->SetXYZ((fMeanHitPosition->GetX() + hitpt->GetX()), (fMeanHitPosition->GetY() + hitpt->GetY()),
                             fMeanHitPosition->GetZ() + hitpt->GetZ());
  }
  int n = hitsVectorInAnEventInABar.size();
  fMeanHitPosition->Divide(n);
  // std::cout << "Mean Hit : " ; fMeanHitPosition->Print();

  unsigned long int startTime = muonNum * timeBetweenTwoMuonTracks;

  ULong64_t tstampNear =
      startTime + ((barLength / 2. * cm + fMeanHitPosition->GetZ()) / (barLength * cm)) * timeDiffNearFar;
  ULong64_t tstampFar =
      startTime + ((barLength / 2. * cm - fMeanHitPosition->GetZ()) / (barLength * cm)) * timeDiffNearFar;
  if (n > 0) {
    // std::cout << "TimeStampNear : " << tstampNear <<" : TimeStampFar : " << tstampFar << std::endl;
  }
  fTSmallTimeStamp = (tstampNear < tstampFar) ? tstampNear : tstampFar;
  fDelTstamp       = tstampNear - tstampFar;
}
#endif

double ScintillatorBar_V2::GetSmearedZ()
{
  lite_interface::Calibration *calib                = lite_interface::Calibration::instance();
  lite_interface::CalibrationData *calibDataOfScint = calib->GetCalibrationDataVector()[fBarIndex];
  TF1 *zparam                                       = calibDataOfScint->fParameterization_F;
  /*if (vecOfLayersOrientation[GetLayerIndex()])
    return zparam->Eval(fDelTstamp / 1000.);
  else {
    return GetGaussianRandomSample(vecOfScintXYCenter[fBarIndex].x * 10, 20) / 10.;
  }*/

  if (vecOfLayersOrientation[GetLayerIndex()]) {
    // return GetExactHitPosition()->GetZ(); // zparam->Eval(fDelTstamp / 1000.) * 10.;
    return zparam->Eval(fDelTstamp / 1000.) * 10.;
  } else {
    // return GetGaussianRandomSample(vecOfScintXYCenter[fBarIndex].x * 10, 50.);
    double center = vecOfScintXYCenter[fBarIndex].x * 10;
    return GenRandom(center - 50, center + 50.);
  }
}

double ScintillatorBar_V2::GetSmearedX()
{
  lite_interface::Calibration *calib                = lite_interface::Calibration::instance();
  lite_interface::CalibrationData *calibDataOfScint = calib->GetCalibrationDataVector()[fBarIndex];
  TF1 *zparam                                       = calibDataOfScint->fParameterization_F;
  /*if (vecOfLayersOrientation[GetLayerIndex()])
    return GetGaussianRandomSample(vecOfScintXYCenter[fBarIndex].x * 10, 20) / 10.;
  else {
    return zparam->Eval(fDelTstamp / 1000.);
  }*/

  if (vecOfLayersOrientation[GetLayerIndex()]) {
    // return GetGaussianRandomSample(vecOfScintXYCenter[fBarIndex].x * 10, 50.);
    double center = vecOfScintXYCenter[fBarIndex].x * 10;
    return GenRandom(center - 50, center + 50.);
  } else {
    // return GetExactHitPosition()->GetX(); // zparam->Eval(fDelTstamp / 1000.) * 10.;
    return zparam->Eval(fDelTstamp / 1000.) * 10.;
  }
}

lite_interface::Point3D *ScintillatorBar_V2::GetSmearedHitPosition()
{
  return new lite_interface::Point3D(GetSmearedX(), GetY(), GetSmearedZ());
}
double ScintillatorBar_V2::GetY()
{
  return vecOfLayersYPos[GetLayerIndex()] * 10.;
}

double ScintillatorBar_V2::GetRandomValueAlongWidth() {}

double ScintillatorBar_V2::GetLogQNearByQFar_ForSimulation()
{
  lite_interface::Calibration *calib                = lite_interface::Calibration::instance();
  lite_interface::CalibrationData *calibDataOfScint = calib->GetCalibrationDataVector()[fBarIndex];
  TF1 *zparam                                       = calibDataOfScint->fParameterization_F;
  TF1 *qparam                                       = calibDataOfScint->fQParameterization_F;
  double smearedZ_InCm                              = zparam->Eval(fDelTstamp / 1000.);
  double qval                                       = qparam->GetX(smearedZ_InCm);
  return qval;
}

} // namespace lite_interface
