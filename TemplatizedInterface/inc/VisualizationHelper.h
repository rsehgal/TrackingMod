/*
 * VisualizationHelper.h
 *
 *  Created on: Apr 28, 2016
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_VISUALIZATIONHELPER_H_
#define TEMPLATIZEDINTERFACE_INC_VISUALIZATIONHELPER_H_
#ifdef SHOW_VISUALIZATION

#ifdef USE_EVE
#include "Eve/EveVisualizer.h"
#include "TEveManager.h"
#include "TEveGeoShape.h"
#include "TEveStraightLineSet.h"
#include <TGeoMatrix.h>
typedef Tracking::EveVisualizer TomographyVisualizer;
#else
#include "TGeo/Visualizer.h"
typedef Tracking::Visualizer TomographyVisualizer;
#endif

#include "base/Vector3D.h"
#include <TGeoBBox.h>
#include "TApplication.h"
#include "GlassRpc.h"
#include "Properties.h"
#include "Track.h"

typedef Tomography::Properties Detector;
namespace Tomography{


class VisualizationHelper{

TomographyVisualizer fVis;

public:
  VisualizationHelper(){
#ifdef USE_EVE
    TEveManager::Create();
#endif

  }

  void Register(Detector *det){
  //void Register(GlassRpc *obj){
#ifdef USE_EVE
    //TGeoBBox *box = fScintillatorPlane[0]->GetScintShape();
     TGeoHMatrix m;
     Double_t trans[3] = { 0., 0., 0. };
     m.SetTranslation(trans);
     Tracking::Vector3D<double> placedLocation(0.,0.,0.);
     int numOfPlanes = det->GetNumOfPlanes();
        std::cout<<"Num Of Planes : " << numOfPlanes << std::endl;
        for(int i= 0 ; i<numOfPlanes; i++){
          std::cout<<"---------------------------------------------------------------"<<std::endl;
          int numOfChannels = det->GetScintillatorPlaneVector()[i]->GetScintVector().size();
          for(int j = 0 ; j < numOfChannels ; j++){
            TGeoBBox *box = det->GetScintillatorPlaneVector()[i]->GetScintVector()[j]->GetScintShape();
            placedLocation = det->GetScintillatorPlaneVector()[i]->GetScintVector()[j]->GetPlacedLocation();
            m.SetDx(placedLocation.x());
            m.SetDy(placedLocation.y());
            m.SetDz(placedLocation.z());
            fVis.AddEveShape(det->GetScintillatorPlaneVector()[i]->GetScintVector()[j]->GetName(),box,m);

          }
        }
#else
#endif
  }

  void Register(Track *t){
	  fVis.AddLine(t->GetP1(),t->GetP2());
  }

  void RegisterLine(Tracking::Vector3D<double> p1, Tracking::Vector3D<double> p2){
	  fVis.AddLine(p1,p2);
  }

  void Show(){
#ifdef USE_EVE
    Tracking::EveVisualizer::Show();
#else
    fVis.Show();
#endif
  }


};

}// end of Tomography namespace


#endif /* for SHOW_VISUALIZATION */
#endif /* INC_VISUALIZATIONHELPER_H_ */
