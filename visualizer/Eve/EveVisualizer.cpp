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
#include "TGLViewer.h"
#include "TGeoShape.h"

#include "TEvePointSet.h"
#include "TColor.h"
//#include "TEvePointSetArray.h"
namespace Tracking {

//TEveElementList* EveVisualizer::fEveGeomList = 0;

  void EveVisualizer::InitializeVisualizer(){
    CreatePointSetArray();
    AddLine(Vector3D<double>(0.,0.,0.),Vector3D<double>(0.,0.,0.));
  }

  void EveVisualizer::CreatePointSetArray(){
     if(gEve){
  //  if(!fPointSetArray)
  fPointSetArray = new TEvePointSetArray("points");

  fPointSetArray->SetSourceCS(TEvePointSelectorConsumer::kTVT_RPhiZ);
  fPointSetArray->SetMarkerColor(3);
  fPointSetArray->SetMarkerStyle(4); // antialiased circle
  fPointSetArray->SetMarkerSize(0.8);
  
  // fPointSetArray->CloseBins();
  Singleton::instance()->AddElement(fPointSetArray);
  int bins = 100;
  fPointSetArray->InitBins("Scattering", bins, 0., 100.);

   TColor::SetPalette(1, 0); 
   const Int_t nCol = TColor::GetNumberOfColors();
   for (Int_t i = 1; i <= bins; ++i)
      fPointSetArray->GetBin(i)->SetMainColor(TColor::GetColorPalette(i * nCol / bins));

   fPointSetArray->GetBin(0) ->SetMainColor(kGray);
   fPointSetArray->GetBin(10)->SetMainColor(kWhite);

}

  }

void EveVisualizer::AddLine(Vector3D<double>p1, Vector3D<double>p2){
  ls = new TEveStraightLineSet();
  
  /*fPointSetArray = new TEvePointSetArray("points");
  fPointSetArray->SetSourceCS(TEvePointSelectorConsumer::kTVT_RPhiZ);
  fPointSetArray->SetMarkerColor(3);
  fPointSetArray->SetMarkerStyle(4); // antialiased circle
  fPointSetArray->SetMarkerSize(0.8);
  l->InitBins("Scattering", 15, 5, 100);*/
 


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

void EveVisualizer::AddMarkers_V2(Vector3D<double> pt){

 /* if(gEve){
  //  if(!fPointSetArray)
  fPointSetArray = new TEvePointSetArray("points");

  fPointSetArray->SetSourceCS(TEvePointSelectorConsumer::kTVT_RPhiZ);
  fPointSetArray->SetMarkerColor(3);
  fPointSetArray->SetMarkerStyle(4); // antialiased circle
  fPointSetArray->SetMarkerSize(0.8);
  
  // fPointSetArray->CloseBins();
  Singleton::instance()->AddElement(fPointSetArray);
  fPointSetArray->InitBins("Scattering", 10, 0., 100.);

   TColor::SetPalette(1, 0); 
   const Int_t nCol = TColor::GetNumberOfColors();
   for (Int_t i = 1; i <= 10; ++i)
      fPointSetArray->GetBin(i)->SetMainColor(TColor::GetColorPalette(i * nCol / 10));

   fPointSetArray->GetBin(0) ->SetMainColor(kGray);
   fPointSetArray->GetBin(10)->SetMainColor(kWhite);

}*/
  if(gEve){
  fPointSetArray->Fill(pt.x(),pt.y(),pt.z(),pt.GetColor());
 // fPointSetArray->CloseBins();
}
}

#if(0)
void EveVisualizer::AddMarkers(std::vector<TEvePointSet> pointSetVector) {

	/*ls->AddMarker(pt.x(), pt.y(), pt.z());
     ls->SetMarkerSize(1.3);
     ls->SetMarkerStyle(4);*/

	for(int i = 0 ; i < pointSetVector.size() ; i++){
	  pointSetVector[i]->SetMarkerSize(1.3);
	  //pointSetVector[i]->SetMarkerStyle(4);
	  //pointSetVector[i]->SetMarkerColor(i+1); // need to be changed programmatically
	}

   }
#endif

void EveVisualizer::CloseBins(){
  if(gEve){
  fPointSetArray->CloseBins();
}
}

EveVisualizer::EveVisualizer(){
 // CreatePointSetArray();
 // fPointSetArray = new TEvePointSetArray("points");
    //  Singleton::instance()->AddElement(fPointSetArray);
 
  //fApp = new TApplication("VecGeom Visualizer", NULL, NULL);
  //fEveGeomList = Singleton::Get();
  //fEveGeomList = new TEveElementList("Geometry");
/*if(gEve){
  fPointSetArray = new TEvePointSetArray("points");
  fPointSetArray->SetSourceCS(TEvePointSelectorConsumer::kTVT_RPhiZ);
  fPointSetArray->SetMarkerColor(3);
  fPointSetArray->SetMarkerStyle(4); // antialiased circle
  fPointSetArray->SetMarkerSize(0.8);
  
  // fPointSetArray->CloseBins();
  Singleton::instance()->AddElement(fPointSetArray);
  fPointSetArray->InitBins("Scattering", 10, 0., 100.);

   TColor::SetPalette(1, 0); 
   const Int_t nCol = TColor::GetNumberOfColors();
   for (Int_t i = 1; i <= 10; ++i)
      fPointSetArray->GetBin(i)->SetMainColor(TColor::GetColorPalette(i * nCol / 10));

   fPointSetArray->GetBin(0) ->SetMainColor(kGray);
   fPointSetArray->GetBin(10)->SetMainColor(kWhite);

}*/

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

void EveVisualizer::AddEveShape(std::string shapeName,TGeoBBox *shape,  TGeoHMatrix &mat , int color){

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

void EveVisualizer::AddEveShape(std::string shapeName,TGeoShape *shape, TGeoHMatrix &mat){

if(gEve){
  std::string substr = shapeName.substr(0,5);
  fEveShape = new TEveGeoShape(shapeName.c_str());
  fEveShape->SetShape(shape);
  if(substr == "Voxel")
	  fEveShape->SetMainColor(18);
  if(substr == "Targe"){
  	  fEveShape->SetMainColor(50);
  	  fEveShape->SetMainTransparency(50);
  }else
	  fEveShape->SetMainTransparency(65);
  fEveShape->SetTransMatrix(mat);
  //fEveGeomList->AddElement(fEveShape);
  Singleton::instance()->AddElement(fEveShape);
 }
}

void EveVisualizer::AddEveShape(std::string shapeName,TGeoShape *shape, TGeoHMatrix &mat, int color){

if(gEve){
  fEveShape = new TEveGeoShape(shapeName.c_str());
  fEveShape->SetShape(shape);
  fEveShape->SetMainColor(color);
  fEveShape->SetMainTransparency(65);
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
  //CloseBins();


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
//TGeoNode* node1 = gGeoManager->GetTopVolume()->FindNode("INNE_1");
//
int numOfTopNodes = gGeoManager->GetTopVolume()->GetNodes()->GetEntries();
for(int i = 0 ; i < numOfTopNodes ; i++)
//int i=0;
{
std::cout<<"Name of : " << i <<" Node : "<< gGeoManager->GetTopVolume()->GetNodes()->At(i)->GetName() << std::endl;
TGeoNode* node1 = gGeoManager->GetTopVolume()->FindNode(gGeoManager->GetTopVolume()->GetNodes()->At(i)->GetName());
std::cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;


//double *trans=node1->GetMatrix()->GetTranslation();
//std::cout<< trans[0] << ": "<< trans[1] << " : " <<  trans[2] << std::endl;
std::cout << node1->GetMatrix()->GetTranslation()[2] << std::endl;
std::cout<<"*******************************************************************************"<<std::endl;
std::cout<<"Number of Entries : "<< numOfTopNodes << std::endl;
std::cout<<"*******************************************************************************"<<std::endl;
TEveGeoTopNode* inn = new TEveGeoTopNode(gGeoManager, node1);
gEve->AddGlobalElement(inn);
}
/*
TGLViewer *v = gEve->GetDefaultGLViewer();
   v->GetClipSet()->SetClipType(TGLClip::EType(1));
   v->RefreshPadEditor(v);

   v->CurrentCamera().RotateRad(-.7, 0.5);
   v->DoDraw();
*/

}

}//end of Tracking namespace
