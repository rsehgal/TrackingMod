/*
 * Visualizer.cpp
 *
 *  Created on: Apr 6, 2016
 *      Author: rsehgal
 */

#include "TApplication.h"
#include "TAxis3D.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
//#include "TGeoTranslation.h"
#include "TGeoShape.h"
#include "TPolyLine3D.h"
#include "TPolyMarker3D.h"
#include "TVirtualPad.h"
#include "TView.h"

#include <iostream>
#include <TGeoVolume.h>
#include "Visualizer.h"
#include "base/Vector3D.h"
#include "TPad.h"

 #include "TGeoShape.h"

#include "TGeoBBox.h"

#define kInfinity 10000

namespace Tracking {


Visualizer::Visualizer():fGeoManager(0) {
  // TODO Auto-generated constructor stub
// TEveManager::Create();
 //fApp = new TApplication("VecGeom Visualizer", NULL, NULL);
 matVacuum = new TGeoMaterial("Vacuum", 0,0,0);
 matVacuum->SetTransparency(50);
 Vacuum = new TGeoMedium("Vacuum",1, matVacuum);

}

Visualizer::~Visualizer() {
  // TODO Auto-generated destructor stub
 // delete fApp;
}

void Visualizer::Show(){

  std::cout<<"=============================================="<<std::endl;
  std::cout<<"========= Inside Expected SHOW() ============="<<std::endl;
  std::cout<<"=============================================="<<std::endl;

  //TGeoVolume *top = gGeoManager->MakeBox("Top", NULL, kInfinity, kInfinity, kInfinity);
TGeoVolume *top = gGeoManager->MakeBox("Top", NULL, 1000., 1000., 1000.);
  gGeoManager->SetTopVolume(top);
  for(int i = 0 ; i < fVolumes.size() ; i++){
  top->AddNode(std::get<0>(fVolumes[i]), 1 , std::get<1>(fVolumes[i]));

  }

  top->SetLineColor(kGreen);
  gGeoManager->CloseGeometry();
  #ifndef USE_OGL
  top->Draw();
  for (auto &line : fLines) {
      line->Draw();
  }

  for (auto &marker : fMarkers) {
      marker->Draw();   
  }
   
  #else
  top->Draw("ogl"); //to display the geometry using openGL
  #endif
  //
  //TPad::x3d("OPENGL");
  gGeoManager->Export("plane.root");
  //top->Export("planeTop.root");
  //fApp->Run();
}

void Visualizer::Show(TGeoVolume *vol){
//  TGeoVolume *top = gGeoManager->MakeBox("Top", NULL, kInfinity, kInfinity, kInfinity);
  TGeoVolume *top = gGeoManager->MakeBox("Top", NULL, 100, 100, 100);
  gGeoManager->SetTopVolume(top);
  //TGeoVolume *vol = fGeoManager->MakeSphere("SPHERE", NULL, 30, 40, 0, 180, 0, 360);
  top->AddNode(vol, 1);
  gGeoManager->CloseGeometry();
  top->Draw();
  //fApp->Run();
}

void Visualizer::AddMarkers(Tracking::Vector3D<double> pt,int color){
  TPolyMarker3D *marker = new TPolyMarker3D(1); 
  //pt.SetColor(2); //done just for testing of color change
  //std::cout<<"Color as Seen by visualizer : "<< pt.GetColor() << std::endl;
  marker->SetMarkerColor(pt.GetColor()); 
  marker->SetMarkerSize(1);
  marker->SetMarkerStyle(7); 
  marker->SetNextPoint(pt.x(), pt.y(), pt.z());  
  fMarkers.push_back(marker);
}

void Visualizer::AddLine(Tracking::Vector3D<double> p0,Tracking::Vector3D<double> p1){
TPolyLine3D *line = new TPolyLine3D(2);  
 line->SetPoint(0, p0.x(), p0.y(), p0.z());
   line->SetPoint(1, p1.z(), p1.z(), p1.z());
   line->SetLineColor(kBlue);   
  fLines.push_back(line); 
  /*
  if (fVerbosity > 0) 
  {     std::cout << "Added line " << p0 << "--" << p1 << " to Visualizer.\n";   
  }
  */
}

void Visualizer::AddLine(TPolyLine3D const &line){

}

void Visualizer::AddVolume( TGeoVolume *rootVolume) {
  fVolumes.push_back(std::make_tuple(
        rootVolume,
        new TGeoTranslation(0., 0., 0.)) ) ;

}

void Visualizer::AddVolume( TGeoVolume *rootVolume, Vector3D<Precision> p) {

  fVolumes.push_back(std::make_tuple(
      rootVolume,
      new TGeoTranslation(p.x(), p.y(), p.z())) ) ;

}

void Visualizer::AddVolume( TGeoShape *shape, Vector3D<Precision> p) {

  fVolumes.push_back(std::make_tuple(
      new TGeoVolume("SHAPE", shape, Vacuum),
      new TGeoTranslation(p.x(), p.y(), p.z())) ) ;

}

TGeoVolume* Visualizer::CreateTGeoVolume(TGeoShape *shape){
  return (new TGeoVolume("SHAPE", shape, Vacuum));
}

TGeoVolume* Visualizer::CreateTGeoVolume(std::string name,TGeoShape *shape){
  return (new TGeoVolume(name.c_str(), shape, Vacuum));
}

void AddLine(Tracking::Vector3D<double> pt1,Tracking::Vector3D<double> pt2){

}


} //end of Tracking namespace
