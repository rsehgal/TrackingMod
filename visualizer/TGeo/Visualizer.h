/*
 * Visualizer.h
 *
 *  Created on: Apr 6, 2016
 *      Author: rsehgal
 */

#ifndef VISUALIZER_TGEO_VISUALIZER_H_
#define VISUALIZER_TGEO_VISUALIZER_H_
#include <list>
#include <memory>
#include <utility>

class TApplication;
class TGeoManager;
class TGeoMatrix;
class TGeoTranslation;
class TGeoShape;
class TGeoVolume;
class TPolyLine3D;
class TGeoMaterial;
class TGeoMedium;

class TGeoShape;
class TGeoBBox;

#include "TPolyMarker3D.h"
#include "base/Global.h"
#include "base/Vector3D.h"

namespace Tracking {

class Visualizer {
private:
 // TApplication *fApp;       // ROOT application used for visualization
  TGeoManager *fGeoManager; // ROOT geometry manager
  std::vector<std::tuple<TGeoVolume*, TGeoTranslation*> > fVolumes;
  TGeoMaterial *matVacuum; //= new TGeoMaterial("Vacuum", 0,0,0);
  TGeoMedium *Vacuum;// = new TGeoMedium("Vacuum",1, matVacuum);
  
public:
  Visualizer();
  virtual ~Visualizer();
  void Show();
  void Show(TGeoVolume *);
  void AddVolume();
  void AddPoint();
  void AddLine();
  void AddVolume( TGeoVolume *rootVolume);
  void AddVolume( TGeoVolume *rootVolume, Vector3D<Precision> p);
  void AddVolume( TGeoShape *shape, Vector3D<Precision> p);
  TGeoVolume* CreateTGeoVolume(TGeoShape *shape);

};

} //end of Tracking namespace

#endif /* INC_VISUALIZER_H_ */
