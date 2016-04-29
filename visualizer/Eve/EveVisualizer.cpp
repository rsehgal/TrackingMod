/*
 * EveVisualizer.cpp
 *
 *  Created on: Apr 24, 2016
 *      Author: rsehgal
 */

#include "TApplication.h"
#include "TEveElement.h"
#include "TEveGeoShape.h"
#include "TEveManager.h"
#include "TGeoBBox.h"
#include "TGeoMatrix.h"
#include "Eve/EveVisualizer.h"
#include "Eve/Singleton.h"


namespace Tracking {

//TEveElementList* EveVisualizer::fEveGeomList = 0;

EveVisualizer::EveVisualizer(){
  //fApp = new TApplication("VecGeom Visualizer", NULL, NULL);
  //fEveGeomList = Singleton::Get();
  //fEveGeomList = new TEveElementList("Geometry");
}

void EveVisualizer::AddEveShape(std::string shapeName,TGeoBBox *shape, TGeoHMatrix &mat){

  fEveShape = new TEveGeoShape(shapeName.c_str());
  fEveShape->SetShape(shape);
  fEveShape->SetMainColor(kGreen);
  fEveShape->SetMainTransparency(50);
  fEveShape->SetTransMatrix(mat);
  //fEveGeomList->AddElement(fEveShape);
  Singleton::instance()->AddElement(fEveShape);
  
}

void EveVisualizer::AddEveShape(std::string shapeName,TGeoBBox *shape, int color, TGeoHMatrix &mat){

  fEveShape = new TEveGeoShape(shapeName.c_str());
  fEveShape->SetShape(shape);
  fEveShape->SetMainColor(color);
  fEveShape->SetMainTransparency(50);
  fEveShape->SetTransMatrix(mat);
  //fEveGeomList->AddElement(fEveShape);
  Singleton::instance()->AddElement(fEveShape);

}

void EveVisualizer::AddEveShape(TEveGeoShape *eveShape, TGeoHMatrix &mat){

  eveShape->SetTransMatrix(mat);
  Singleton::instance()->AddElement(eveShape);

}

void EveVisualizer::Show(){
  // TEveManager::Create();
  /*TGeoHMatrix mat;
  Double_t trans[3] = { 0., 0., dZ };
  mat.SetTranslation(trans);
  fEveGeomList->SetTransmatrix(mat);*/


  //gEve->AddGlobalElement(fEveGeomList);
  gEve->AddGlobalElement(Singleton::instance()->GetList());
  gEve->DoRedraw3D();
  /*TTimer timer(1000); // every second
  timer.SetCommand("gEve->DoRedraw3D();");
  timer.TurnOn();*/
  //UpdateScene();
  //fApp->Run();
}


void EveVisualizer::UpdateScene(){
  //TTimer timer(1000); // every second
  //timer.SetCommand("gEve->DoRedraw3D();");
  //timer.TurnOn();
}



}//end of Tracking namespace
