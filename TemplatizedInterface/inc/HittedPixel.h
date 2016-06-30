/*
 * HittedPixel.h
 *
 *  Created on: May 25, 2016
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_HITTEDPIXEL_H_
#define TEMPLATIZEDINTERFACE_INC_HITTEDPIXEL_H_

#include <TEveGeoShape.h>
#include <TGeoBBox.h>
#include <TGeoMatrix.h>
#include "Properties.h"
typedef Tomography::Properties Detector;

namespace Tomography{

class HittedPixel{
  TEveGeoShape *fEveShape;

public:
  HittedPixel(TGeoHMatrix m, bool forRpc, bool misMatch){
    fEveShape = new TEveGeoShape("HittedPixel");
    fEveShape->SetMainColor(2);
    if(forRpc){
    	if(misMatch)
          fEveShape->SetMainColor(4);

        fEveShape->SetShape(new TGeoBBox("hittedPixel", 100 / 64., 100 / 64., 1));
    }else{
      fEveShape->SetShape(new TGeoBBox("hittedPixel", 180/2, 144/(8*2), 1));
      if(misMatch)
      fEveShape->SetMainColor(4);
    }
    fEveShape->SetMainTransparency(50);
    fEveShape->SetTransMatrix(m);
  }

  HittedPixel(TGeoHMatrix m) {
    fEveShape = new TEveGeoShape("HittedPixel");
    fEveShape->SetMainColor(2);
    fEveShape->SetShape(new TGeoBBox("hittedPixel", 100 / 64., 100 / 64., 1));
    fEveShape->SetMainTransparency(50);
    fEveShape->SetTransMatrix(m);
  }

  HittedPixel(Detector *det, TGeoHMatrix m) {
    fEveShape = new TEveGeoShape("HittedPixel");
    fEveShape->SetMainColor(2);
    fEveShape->SetShape(new TGeoBBox("hittedPixel", 100 / 64., 100 / 64., 1));
    fEveShape->SetMainTransparency(50);
    fEveShape->SetTransMatrix(m);
  }

  ~HittedPixel(){}
  TEveGeoShape* GetEveGeoShape(){return fEveShape;}

};

} /* tomography namespace */

#endif /* TEMPLATIZEDINTERFACE_INC_HITTEDPIXEL_H_ */
