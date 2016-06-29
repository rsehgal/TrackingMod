#ifndef COORDINATEGENERATOR_INC_COORDINATES_H
#define COORDINATEGENERATOR_INC_COORDINATES_H

#include <iostream>
#include <vector>
//#include "TemplatizedInterface/inc/SetupManager.h"
#include "base/Vector3D.h"
#include "Properties.h"
#include "SetupManager.h"
#include "ScintillatorPlane.h"

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

  void SetP1(Vector3D<double> p){fP1 = p;}
  void SetP2(Vector3D<double> p){fP2 = p;}

  // void CoGenerator(std::vector <Detector*> det,int N1=0);
  void CoGenerator();
  Vector3D<double> GetCoordinate(int detector); // Enter value between 0 < detector < total detectors
  Vector3D<double> GetStripCoordinate(double x, double y, double z);
  void SetStrips(double x, double y, double z);
  void SetStripCoordinates();
  Tracking::Vector3D<double> GetStripCoordinate(int detector);
  void SetStrips();
  Tracking::Vector3D<double> GetStrip(int detector);
  double GenRandom(double min, double max);
  Vector3D<double> GetInitialPoint();
  Vector3D<double> GetSpecificCoordinate(double zpos);
  int GetLength() { return det.size(); }
  void Print();
};

} // end of Tomography namespace
#endif
