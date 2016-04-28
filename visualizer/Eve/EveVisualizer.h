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
//#include "SingleTon.h"
class TEveElementList;
class TEveGeoShape;
class TApplication;
class TGeoBBox;
class TGeoMatrix;



namespace Tracking {

class EveVisualizer{
private:
  //TApplication *fApp;
  TEveElementList *fEveGeomList;
  TEveGeoShape *fEveShape;

public:
  EveVisualizer();
  void AddEveShape(std::string shapeName,TGeoBBox *shape, TGeoHMatrix &mat);
  void AddEveShape(std::string shapeName,TGeoBBox *shape, int color, TGeoHMatrix &mat);
  static void Show();

};

} //end of Tracking namespace
#endif
