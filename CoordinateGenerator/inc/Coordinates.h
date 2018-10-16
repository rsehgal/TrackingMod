#ifndef COORDINATEGENERATOR_INC_COORDINATES_H
#define COORDINATEGENERATOR_INC_COORDINATES_H

#include <iostream>
#include <vector>
//#include "TemplatizedInterface/inc/SetupManager.h"
#include "base/Vector3D.h"
#include "Properties.h"
#include "SetupManager.h"
#include "ScintillatorPlane.h"
#include "DetectorMapping.h"

typedef Tomography::Properties Detector;
using namespace Tracking;
// using std::vector;
namespace Tomography {
class Coordinates {
private:
  int N; // Number of Detectors
  int totalDetector;
  int M;
  int signal = 0;
  double temp;
  // std::vector<double> detectorLength;
  // std::vector<double> detectorWidth;

  std::vector<double> totalStrip;
  Vector3D<double> InitialPoint;
  std::vector<Detector *> detectors;
  std::vector<Tracking::Vector3D<double>> coordinate;
  std::vector<Tracking::Vector3D<double>> stripcoord;
  std::vector<Tracking::Vector3D<double>> strip;
  std::vector<Detector *> det;

  Vector3D<double> fP1;
  Vector3D<double> fP2;
  // vector<vector <double>> coordinate;
  // vector<vector <double>> stripcoord;    //generated coordinates from midpoints of 2-D grid of each detectors
  // vector<vector <double>> strip;

public:
  Coordinates();     //ooooooooooooooooooooooooooooo
  Coordinates(std::vector<Detector *> det);
  // Coordinates(Vector3D<double> init);   ooooooooooooooooooooo
  Coordinates(std::vector<Detector *> det, Vector3D<double> init);
  Coordinates(Vector3D<double> p1, Vector3D<double> p2 );
  //~Coordinates(Vector3D<double> init);
  void InitializeVectors();


  //Function to calculation extrapolated point of intersection with Detector plan
  //std::vector<Vector3D<double>> GetPOI();
  Vector3D<double> GetPOI(Detector *det, bool topToDown=true);
  void SetPoints(std::vector<Tracking::Vector3D<double>>tempVect){
	  fP1 = tempVect[0];
	  fP2 = tempVect[tempVect.size()-1];
	  fP1.Print();
	  fP2.Print();
  }
  void SetP1(Vector3D<double> p){fP1 = p;}
  void SetP2(Vector3D<double> p){fP2 = p;}


  // void CoGenerator(std::vector <Detector*> det,int N1=0);
  void CoGenerator();
  Vector3D<double> GetCoordinate(int detector); // Enter value between 0 < detector < total detectors
  Vector3D<double> GetStripCoordinate(double x, double y, double z = 0. );
  //Vector3D<double> GetStripCoordinate(double x, double y);
  Vector3D<double> GetStripCoordinate(Detector *det, double x, double y, double z);
  void SetStrips(double x, double y, double z);
  void SetStripCoordinates();
  Tracking::Vector3D<double> GetStripCoordinate(int detector);
  void SetStrips();
  Tracking::Vector3D<double> GetStrip(int detector);
  double GenRandom(double min, double max);
  double GenRandomGauss(double mean, double sigma);
  Vector3D<double> GetInitialPoint();
  Vector3D<double> GetSpecificCoordinate(double zpos);
  int GetLength() { return det.size(); }
  void Print();

  //Functions to be used only by Simulation
  int GetStripNum(Detector *det, double val, bool top){
    double stripLength = 0., stripWidth = 0.;
    if(top){
      stripLength = det->GetPlane(0)->GetScintVector()[0]->GetLength();
      stripWidth = stripLength / det->GetPlane(0)->GetNumOfScintillators();
    }
    else{
      stripLength = det->GetPlane(1)->GetScintVector()[0]->GetLength();
      stripWidth = stripLength / det->GetPlane(1)->GetNumOfScintillators();
    }
    return (val+ stripLength/2)/stripWidth;
  }

  int GetStripNum( double val){
    double stripLength = 1000.;
    double stripWidth =
    			Tomography::DetectorMapping::instance()->GetDetectorVector()[3]->sDetectorLength
    					/ Tomography::DetectorMapping::instance()->GetNumberOfStripsInEachPlane();
    return (val+ stripLength/2)/stripWidth;
  }
};

} // end of Tomography namespace
#endif
