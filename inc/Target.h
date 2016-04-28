/*
 * Target.h
 *
 *  Created on: Apr 27, 2016
 *      Author: rsehgal
 */

#ifndef INC_TARGET_H_
#define INC_TARGET_H_

#include <iostream>
#ifdef USE_EVE
#include "Eve/Singleton.h"
#include <TEveGeoShape.h>
#else
#include "TGeoVolume.h"
#endif
#include <TGeoBBox.h>
//#include <TGeoOrb.h>

namespace Tracking{
 class Target{
   TGeoVolume *vol;
 public:

   Target(){
     TGeoBBox *target = new TGeoBBox(5, 5, 5);
#ifdef USE_EVE
     TEveGeoShape *b = new TEveGeoShape("Target");
     b->SetShape(target);
     b->SetMainColor(kRed);
     b->SetMainTransparency(60);
     Singleton::instance()->AddElement(b);
#else
    vol = new TGeoVolume("SHAPE", target, NULL);
#endif
   }

   TGeoVolume* GetTGeoVolume(){return vol;}
 };


}//end of Tracking namespace



#endif /* INC_TARGET_H_ */
