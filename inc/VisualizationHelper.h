/*
 * VisualizationHelper.h
 *
 *  Created on: Apr 28, 2016
 *      Author: rsehgal
 */

#ifndef INC_VISUALIZATIONHELPER_H_
#define INC_VISUALIZATIONHELPER_H_

#ifdef USE_EVE
#include "Eve/EveVisualizer.h"
#include "TEveManager.h"
#else
#include "TGeo/Visualizer.h"
#endif

#include "TApplication.h"
#include "Scintillator.h"
#include "RPC.h"
#include "Target.h"

namespace Tracking{

class VisualizationHelper{
  TApplication *fApp;
#ifndef USE_EVE
  Tracking::Visualizer v;
#endif

public:
  VisualizationHelper(){
    fApp = new TApplication("Test", NULL, NULL);
#ifdef USE_EVE
    TEveManager::Create();
//#else
//    Tracking::Visualizer v;
#endif

  }

  void Register(ScintillatorPlane *scintPlane){
#ifdef USE_EVE
#else
    v.AddVolume(scintPlane->GetPlaneTGeoVolume());
#endif
  }

  void Register(RPC *rpc){
#ifdef USE_EVE
#else
    v.AddVolume(rpc->GetRpc()->GetPlaneTGeoVolume());
#endif
  }


  void Register(Target *target){
#ifdef USE_EVE
#else
    v.AddVolume(target->GetTGeoVolume());
#endif
  }


  void Show(){
#ifdef USE_EVE
    EveVisualizer::Show();
#else
    v.Show();
#endif
    fApp->Run();
  }


};

}// end of Tracking namespace



#endif /* INC_VISUALIZATIONHELPER_H_ */
