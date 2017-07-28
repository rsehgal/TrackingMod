/*
 * EveVisualizer.h
 *
 *  Created on: Apr 24, 2016
 *      Author: rsehgal
 */

#ifndef VISUALIZER_EVE_EVEVISUALIZER_H_
#define VISUALIZER_EVE_EVEVISUALIZER_H_
#include <list>
#include <memory>
#include <utility>
#include "TGeoMatrix.h"
#include "base/Vector3D.h"
//#include "SingleTon.h"
class TEveElementList;
class TEveGeoShape;
class TApplication;
class TGeoBBox;
class TGeoMatrix;
class TEveStraightLineSet;
class TGeoShape;
class TEvePointSet;

namespace Tracking {

class EveVisualizer{
private:
  //TApplication *fApp;
  TEveElementList *fEveGeomList;
  TEveGeoShape *fEveShape;
  TEveStraightLineSet *ls;
  TEvePointSet *fPointSet;



public:
  EveVisualizer();
  void AddEveShape(std::string shapeName,TGeoBBox *shape, TGeoHMatrix &mat);
  void AddEveShape(std::string shapeName,TGeoBBox *shape, TGeoHMatrix &mat,int color);
  void AddEveShape(TEveGeoShape *eveShape, TGeoHMatrix &mat);
  void AddEveShape(std::string shapeName,TGeoShape *shape, TGeoHMatrix &mat);
  void AddLine(Vector3D<double>p1, Vector3D<double>p2);
  static void Show();
  static void UpdateScene();
  void AddMarkers(Vector3D<double> pt);
  void AddMarkers(std::vector<TEvePointSet> pointSetVector);
  void ImportFromROOTFile(std::string geomFile);
};

} //end of Tracking namespace
#endif
