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
#include <TEveStraightLineSet.h>
#include "TGeoNode.h"
#include "TEveGeoNode.h"
//#include "TEveGeoTopNode.h"
#include "TGeoManager.h"
namespace Tracking {

//TEveElementList* EveVisualizer::fEveGeomList = 0;

void EveVisualizer::AddLine(Vector3D<double>p1, Vector3D<double>p2){
  ls = new TEveStraightLineSet();
  ls->AddLine( p1.x(),p1.y(),p1.z(),
		       p2.x(),p2.y(),p2.z());
  AddMarkers(p1);
  AddMarkers(p2);
  //ls->SetMarkerSize(1.5);
  //ls->SetMarkerStyle(4);
  Singleton::instance()->AddLineSet(ls);
  //gEve->AddElement(ls);
  //Singleton::instance()->AddElement(ls);
}

void EveVisualizer::AddMarkers(Vector3D<double> pt) {
     /*for (int i = 0; i < c.GetLength(); i++) {
       ls->AddMarker(pt.x(), pt.y(), pt.z());
     }*/
	ls->AddMarker(pt.x(), pt.y(), pt.z());
     ls->SetMarkerSize(1.3);
     ls->SetMarkerStyle(4);
   }


EveVisualizer::EveVisualizer(){
  //fApp = new TApplication("VecGeom Visualizer", NULL, NULL);
  //fEveGeomList = Singleton::Get();
  //fEveGeomList = new TEveElementList("Geometry");
}

void EveVisualizer::AddEveShape(std::string shapeName,TGeoBBox *shape, TGeoHMatrix &mat){
if(gEve){
  fEveShape = new TEveGeoShape(shapeName.c_str());
  fEveShape->SetShape(shape);
  fEveShape->SetMainColor(kGreen);
  fEveShape->SetMainTransparency(50);
  fEveShape->SetTransMatrix(mat);
  //fEveGeomList->AddElement(fEveShape);
  Singleton::instance()->AddElement(fEveShape);
}
  
}

void EveVisualizer::AddEveShape(std::string shapeName,TGeoBBox *shape, int color, TGeoHMatrix &mat){

if(gEve){
  fEveShape = new TEveGeoShape(shapeName.c_str());
  fEveShape->SetShape(shape);
  fEveShape->SetMainColor(color);
  fEveShape->SetMainTransparency(50);
  fEveShape->SetTransMatrix(mat);
  //fEveGeomList->AddElement(fEveShape);
  Singleton::instance()->AddElement(fEveShape);
 }
}

void EveVisualizer::AddEveShape(TEveGeoShape *eveShape, TGeoHMatrix &mat){

  if(gEve){
  eveShape->SetTransMatrix(mat);
  Singleton::instance()->AddElement(eveShape);
}

}

void EveVisualizer::Show(){
  // TEveManager::Create();
  /*TGeoHMatrix mat;
  Double_t trans[3] = { 0., 0., dZ };
  mat.SetTranslation(trans);
  fEveGeomList->SetTransmatrix(mat);*/


  //gEve->AddGlobalElement(fEveGeomList);
  if(gEve){
  gEve->AddGlobalElement(Singleton::instance()->GetList());
  //gEve->AddGlobalElement(Singleton::instance()->GetLineSet());
  gEve->DoRedraw3D();
}
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


void EveVisualizer::ImportFromROOTFile(std::string geomFile){
gGeoManager = gEve->GetGeometry(geomFile.c_str());
TGeoNode* node1 = gGeoManager->GetTopVolume()->FindNode("INNE_1");
TEveGeoTopNode* inn = new TEveGeoTopNode(gGeoManager, node1);
gEve->AddGlobalElement(inn);
}

}//end of Tracking namespace
