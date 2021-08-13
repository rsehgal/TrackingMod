/*
 * PsBar.h
 *
 *  Created on: 14-Aug-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_INC_PSBAR_H_
#define ISMRAN_INC_PSBAR_H_
#include "includes.hh"
//#include "Point3D.h"
extern bool IsSimulation;
// namespace ismran{
extern const int offset;      //=0;
extern const double kDelTBar; //= 32.0; //! ns
extern TStopwatch timer;
extern unsigned int numOfLayers;          //=3;
extern unsigned int numOfBarsInEachLayer; //=9;
extern float barWidth;
extern float barHeight;
extern float barLength;
extern double scattererPlaneY;
extern float offsetYLowerDetectors;
extern float offsetYUpperDetectors;
extern unsigned int numOfLowerLayers;
extern unsigned int numOfUpperLayers;
extern std::vector<unsigned int> incomingLayerIndices;
extern std::vector<unsigned int> outgoingLayerIndices;
extern std::vector<unsigned int> vecOfRequiredLayers;
extern std::vector<unsigned int> vecOfAntiCoincideneScint;

extern std::map<std::string, TH1D *> barsEnergyMap;
extern std::vector<TH1D *> vecOfEnergyHist;
extern std::vector<TH1D *> vecOfTimeDiffHist;

extern std::vector<unsigned int> vecOfdeltaTMin;
extern std::vector<unsigned int> vecOfdeltaTMax;

// energy range to select the particle
extern double qmeanCorrThreshold;
extern double qmeanCorrThresholdUpper;
extern int qstart; // = 4000;
extern int qend;   // = 18000;
// time difference between Far and Near PMT of a PsBar
extern int timeDiffNearFar; // =25000 (ps)
extern int muonEnergyPeak;
extern int nbins;
extern int minEnergyDepositedByMuonTrack; // = 170000;
extern int maxEnergyDepositedByMuonTrack; // = 190000;
extern unsigned long int timeBetweenTwoMuonTracks;
extern double xyzLimit;

extern double fwhm; // = 2.355 * 5.66353 ;

extern double errorX;
extern double errorY;

extern double errorX_Exact;
extern double errorY_Exact;
extern double errorZ_Exact;
extern double errorX_Smeared;
extern double errorY_Smeared;
extern double errorZ_Smeared;

extern std::vector<double> zCorrOffsetVector;

extern unsigned short exactTrack;
extern unsigned short smearedTrack;

extern bool planeXY;
extern bool planeZY;

struct Point3D {
  double x;
  double y;
  double z;

  Point3D(double s_x, double s_y, double s_z)
  {
    x = s_x;
    y = s_y;
    z = s_z;
  }

  Point3D()
  {
    x = 100;
    y = 100.;
    z = 100.;
  }
  void Print() { std::cout << "( " << x << " , " << y << " , " << z << " )" << std::endl; }

  double Distance(Point3D p2)
  {
    return sqrt((p2.x - this->x) * (p2.x - this->x) + (p2.y - this->y) * (p2.y - this->y) +
                (p2.z - this->z) * (p2.z - this->z));
  }

  void SetZero()
  {
    x = 0;
    y = 0;
    z = 0;
  }
  void InCm()
  {
    x /= 10.;
    y /= 10.;
    z /= 10.;
  }
};

// extern std::string FileFrag[];
extern std::vector<std::string> vecOfBarsName;

struct ScintillatorBar {
  std::string scintName; //! board #  and channel number ( its packed in as follows ) //! board*16 + chno.

  // Charge info may not be required, but keeping it for the time being.
  UInt_t qlongNear; //! integrated charge in long gate 88 nsec
  UInt_t qlongFar;  //! integrated charge in long gate 88 nsec
  // UInt_t    qlongMean;   //! integrated charge in long gate 88 nsec
  Double_t qlongMean; //! integrated charge in long gate 88 nsec

  ULong64_t tstampNear;      //! time stamp in pico sec.
  ULong64_t tstampFar;       //! time stamp in pico sec.
  ULong64_t tsmallTimeStamp; //! time stamp in pico sec.
  Long64_t deltaTstamp;      //! time stamp in pico sec.
  UInt_t time;               //! real computer time in sec

  unsigned short barIndex;
  unsigned short layerIndex;

  Point3D hitPosition;
  Point3D fittedHitPosition;

  bool barHitted;
  bool validPair;

  bool corrected;
  long deltaTstampCorrected;

  ScintillatorBar(unsigned short l_channelNear, unsigned short l_channelFar, ULong64_t l_tstampNear,
                  ULong64_t l_tstampFar, UInt_t l_qlongNear, UInt_t l_qlongFar, UInt_t l_time)
  {

    barHitted  = true;
    corrected  = false;
    barIndex   = l_channelNear / 2;
    layerIndex = barIndex / numOfBarsInEachLayer;
    scintName =
        "PsBar" + std::to_string(barIndex) + "-" + std::to_string(l_channelNear) + "-" + std::to_string(l_channelFar);
    qlongNear        = l_qlongNear;
    qlongFar         = l_qlongFar;
    qlongMean        = sqrt(qlongNear * qlongFar);
    tstampNear       = l_tstampNear;
    tstampFar        = l_tstampFar;
    tsmallTimeStamp  = (tstampNear < tstampFar) ? tstampNear : tstampFar;
    deltaTstamp      = tstampNear - tstampFar;
    bool validEnergy = (qlongMean > qstart && qlongMean < qend);

    /*
     * Added following checks to declare the pair as VALID
     * 1) Absolute value of time differ must be smaller than 2500 : abs(deltaTstamp) < timeDiffNearFar)
     * 2) Near channel number must be smaller than far channel number and the diff should be -1 :
     * ((l_channelNear-l_channelFar) == -1) 3) Since we are considering only MUONS to energy deposited kept between 4000
     * to 18000 : NOT THE CALIBRATED ONE 4) Near channel number should be small then far channel number, ideally it
     * should be even as per the current nomenclature
     */
    validPair = (abs(deltaTstamp) < timeDiffNearFar) && ((l_channelNear - l_channelFar) == -1) && validEnergy &&
                !(l_channelNear % 2);
    time = l_time;
    // EstimateHitPosition();
  }

  /*
   * This constructor will be useful for simulation
   */
  ScintillatorBar(unsigned int bIndex)
  {
    barHitted                    = false;
    barIndex                     = bIndex;
    layerIndex                   = barIndex / numOfBarsInEachLayer;
    unsigned short l_channelNear = 2 * barIndex;
    unsigned short l_channelFar  = l_channelNear + 1;
    scintName =
        "PsBar" + std::to_string(barIndex) + "-" + std::to_string(l_channelNear) + "-" + std::to_string(l_channelFar);

    /* qlongMean, the most import parameter, should be filled as total energy
     * deposited in a bar, which is basically the sum of energy deposited in
     * individual steps
     *
     * remaining data members may be set to zero.
     */
    qlongMean       = 0;
    qlongNear       = 0;
    qlongFar        = 0;
    tstampNear      = 0;
    tstampFar       = 0;
    tsmallTimeStamp = 0;
    deltaTstamp     = 0;
    time            = 0;
    // EstimateHitPosition();
  }

  ScintillatorBar() {}

  ScintillatorBar(const ScintillatorBar &sbar)
  {
    std::cout << "======= COPY CONSTRUCTOR CaLLED ==========" << std::endl;
    barHitted       = sbar.barHitted;
    barIndex        = sbar.barIndex;
    layerIndex      = sbar.layerIndex;
    scintName       = sbar.scintName;
    qlongMean       = sbar.qlongMean;
    qlongNear       = sbar.qlongNear;
    qlongFar        = sbar.qlongFar;
    tstampNear      = sbar.tstampNear;
    tstampFar       = sbar.tstampFar;
    tsmallTimeStamp = sbar.tsmallTimeStamp;
    deltaTstamp     = sbar.deltaTstamp;
    time            = sbar.time;
  }

  void Print()
  {
    // barHitted=true;
    if (barHitted) {
      std::cout << barHitted << " , " << scintName << " , " << barIndex << " , " << qlongNear << " , " << qlongFar
                << " , " << qlongMean << " , " << tstampNear << " , " << tstampFar << " , " << tsmallTimeStamp << " , "
                << deltaTstamp << " , " << time << " , "
                << " ( " << hitPosition.x << " , " << hitPosition.y << " , " << hitPosition.z << " ) " << std::endl;
    }
  }

  /*
   * Calculates the estimated hit position in cm along Y,
   * deltaTstamp should be in ns
   *
   * IDEA : Map the deltaT to the corresponding position along Y
   */
  void EstimateHitPositionAlongZ() { hitPosition.z = 0; }

  /*
   * Calculates the estimated hit position in cm along X,
   *
   * For the time being, this can be mid point along X.
   * can be 5cm
   */
  void EstimateHitPositionAlongX()
  {
    // hitPosition.x = 0;
    hitPosition.x = (layerIndex + 1) * 10.;
  }

  /*
   * Calculates the estimated hit position in cm along Z,
   *
   * For the time being, this can be level of top surface along Z.
   * i.e. multiple of 10,
   * Consider the bottom most layer is at 0, then its Z = 1*10cm
   * General logic
   * Z=(layerIndex+1)*10
   */
  void EstimateHitPositionAlongY()
  {
    // hitPosition.z = (layerIndex+1)*10.;
    hitPosition.y = 0;
  }

  void EstimateHitPosition()
  {
    EstimateHitPositionAlongX();
    EstimateHitPositionAlongY();
    EstimateHitPositionAlongZ();
  }
};

// extern std::vector<ScintillatorBar*> eventsVec2;

// extern void PrintPsBarVector();
/*void PrintPsBarVector(){
  std::cout <<"6666666666 Printing from PrintPsBarVector 66666666" << std::endl;
  for(unsigned int i = 0 ; i < eventsVec2.size(); i++){
    eventsVec2[i]->Print();
  }
}*/
//}

namespace Tomography {

class PsBar {
public:
  PsBar();
  virtual ~PsBar();
};

} /* namespace Tomography */

#endif /* ISMRAN_INC_PSBAR_H_ */
