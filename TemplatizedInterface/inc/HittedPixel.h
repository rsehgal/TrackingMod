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


namespace Tomography{

class HittedPixel{
  TEveGeoShape *fEveShape;

public:
  HittedPixel(TGeoHMatrix m){
    fEveShape = new TEveGeoShape("HittedPixel");
    fEveShape->SetShape(new TGeoBBox("hittedPixel",100/64., 100/64., 1));
    fEveShape->SetMainColor(2);
    fEveShape->SetMainTransparency(50);
    fEveShape->SetTransMatrix(m);
  }
  ~HittedPixel(){}
  TEveGeoShape* GetEveGeoShape(){return fEveShape;}

};

} /* tomography namespace */

#endif /* TEMPLATIZEDINTERFACE_INC_HITTEDPIXEL_H_ */
