/*
 * Author(s) : Indroneil Kanungo, Vishal Singh, Gargi Kekre
 * Date : 28th June 2016
 * Track Validation
 *
 */

#include <iostream>
#include <TriggeringPlane.h>
#include <Properties.h>
#include <SetupManager.h>
#include <cstring>
#include "Coordinates.h"
typedef Tomography::Properties Detector;
typedef Tracking::Vector3D<double> LienVector;
using namespace Tomography;

LienVector GetStripCoordinate(double x, double y, double z) {
  int tmp = 0;
  LienVector temp;
  temp.SetX(-50. + x * 3.125 + 1.5625);
  temp.SetY(-50. + y * 3.125 + 1.5625);
  temp.SetZ(z);

  return temp;
}

LienVector GetCoordinatesAtZ(std::vector<LienVector> coords, double zz) {
  double x[2], y[2], z[2];
  double xx, yy;
  x[0] = coords[0].x();
  x[1] = coords[1].x();
  y[0] = coords[0].y();
  y[1] = coords[1].y();
  z[0] = coords[0].z();
  z[1] = coords[1].z();

  double r = (zz - z[0]) / (z[0] - z[1]);
  xx = x[0] + r * (x[0] - x[1]);
  yy = y[0] + r * (y[0] - y[1]);
  LienVector result(xx, yy, zz);
  return result;
}

int main(int argc, char *argv[]) {
  std::cout << "Lienordni" << std::endl;
  std::string temp_str = std::to_string(atoi(argv[1]));
  temp_str += ".root";
  Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);
  Detector *MT1 = new GlassRpc(2, "MT1", -75, 31);
  Detector *MT2 = new GlassRpc(4, "MT2", 45, 31);
  Detector *ScintTop = new TriggeringPlane(2, "ScintTop", 105, -1);
  Detector *ScintBottom = new TriggeringPlane(2, "ScintBottom", -105, 7);
  SetupManager *setup = SetupManager::instance();
  setup->Register(MT2);
  setup->Register(MT1);
  setup->Register(ScintTop);
  setup->Register(ScintBottom);
  int evNo, i, j, k, l;
  std::vector<LienVector> coords(2);
  double x, y, z;
  LienVector TopIntersection, BottomIntersection;
  std::vector<Detector *> DetVect = setup->GetDetectorVector("GLASS");
  std::vector<Detector *> ScintVect = setup->GetDetectorVector("TRG");
  ScintillatorPlane::SetClusterSize(1);
  int count = 0, othercount = 0;

  Coordinates c;

  for (evNo = 0; evNo < 1000; ++evNo) {
    setup->SetEventDetected("TRG", evNo);
    if (setup->EventDetected()) {
      count++;
      setup->SetEventDetected("GLASS", evNo);
      if (setup->EventDetected()) {
        othercount++;
        std::cout << "========== Event no. : " << evNo << " << OtherCount : "<< othercount << "===================" << std::endl;
        for (i = 0; i < DetVect.size(); ++i) {
          std::vector<ScintillatorPlane *> DetPlaneVector = DetVect[i]->GetScintillatorPlaneVector();
          y = DetPlaneVector[0]->GetFiredStripsVector()[0];
          x = DetPlaneVector[1]->GetFiredStripsVector()[0];
          z = DetVect[i]->GetZPos();
          coords[i] = GetStripCoordinate(x, y, z);
        }

//        TopIntersection = GetCoordinatesAtZ(coords, 105);
//        BottomIntersection = GetCoordinatesAtZ(coords, -105);

        c.SetP1(coords[0]);
        c.SetP2(coords[1]);
        TopIntersection = c.GetPOI(ScintTop,false);
        BottomIntersection = c.GetPOI(ScintBottom,true);

        std::cout << "Top Detector Coordinates : " << coords[1].x() << ", " << coords[1].y() << ", " << coords[1].z()
                  << "\n";
        std::cout << "Bottom Detector Coordinates : " << coords[0].x() << ", " << coords[0].y() << ", " << coords[0].z()
                  << "\n";
        std::cout << "Top Intersection Coordinates : " << TopIntersection.x() << ", " << TopIntersection.y() << ", "
                  << TopIntersection.z() << "\n";
        std::cout << "Bottom Intersection Coordinates : " << BottomIntersection.x() << ", " << BottomIntersection.y()
                  << ", " << BottomIntersection.z() << "\n";

        std::cout << "Top Scintillator Number : " << ScintVect[0]->GetPlane(0)->GetFiredStripsVector()[0] << std::endl;
        std::cout << "Bottom Scintillator Number : " << ScintVect[1]->GetPlane(0)->GetFiredStripsVector()[0]
                  << std::endl
                  << std::endl;

/*
        std::cout<<"=============================================== PREVIOUS ================================================================" << std::endl;
        TopIntersection = GetCoordinatesAtZ(coords, 105);
        BottomIntersection = GetCoordinatesAtZ(coords, -105);
        std::cout << "Top Intersection Coordinates : " << TopIntersection.x() << ", " << TopIntersection.y() << ", "
                          << TopIntersection.z() << "\n";
                std::cout << "Bottom Intersection Coordinates : " << BottomIntersection.x() << ", " << BottomIntersection.y()
                          << ", " << BottomIntersection.z() << "\n";*/
      }
    }
  }
  std::cout << othercount << std::endl;
  std::cout << count << std::endl;
}
