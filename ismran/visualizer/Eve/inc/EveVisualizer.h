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
#include "Track.h"
#include "Singleton.h"
class TEveElementList;
class TEveGeoShape;
class TApplication;
class TGeoBBox;
class TGeoMatrix;
class TEveStraightLineSet;
class TGeoShape;
class TEvePointSet;
class TEvePointSetArray;
class TColor;

namespace Tracking {

class EveVisualizer{
private:
  //TApplication *fApp;
  TEveElementList *fEveGeomList;
  TEveGeoShape *fEveShape;
  TEveStraightLineSet *ls;
  TEvePointSet *fPointSet;
  TEvePointSetArray *fPointSetArray;



public:
  EveVisualizer();
  void AddEveShape(std::string shapeName,TGeoBBox *shape, TGeoHMatrix &mat);
  void AddEveShape(std::string shapeName,TGeoBBox *shape, TGeoHMatrix &mat,int color);
  void AddEveShape(std::string shapeName,TGeoBBox *shape, TGeoHMatrix &mat,double color);
  void AddEveShape(TEveGeoShape *eveShape, TGeoHMatrix &mat);
  void AddEveShape(std::string shapeName,TGeoShape *shape, TGeoHMatrix &mat, int color);
  void AddEveShape(std::string shapeName,TGeoShape *shape, TGeoHMatrix &mat);
  void AddEveShape(std::string shapeName,TGeoBBox *shape,  TGeoHMatrix &mat , int color,int transparency);
  void AddLine(Vector3D<double>p1, Vector3D<double>p2,double color=5);
  void AddTracks(std::vector<Tomography::Track*> trackVector,double color=5);
  void RemoveLine();
  void RemoveTrack();
  static void Show();
  static void UpdateScene();
  void AddMarkers(Vector3D<double> pt);
  void AddMarkers_V2(Vector3D<double> pt,double color=-1.);
  void AddMarkers(std::vector<TEvePointSet> pointSetVector);
  void ImportFromROOTFile(std::string geomFile);
  void CloseBins();
  void CreatePointSetArray();
  void InitializeVisualizer();
  TColor* CreateColor(double val);
  double CreateColorVal(double val);

protected:
  Tracking::Vector3D<double> UnpackColor(double val);
  double PackColor(Tracking::Vector3D<double>);

};

} //end of Tracking namespace
#endif
