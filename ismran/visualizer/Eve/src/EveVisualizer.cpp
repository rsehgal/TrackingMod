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
#include "EveVisualizer.h"
//#include "Eve/Singleton.h"
#include <TEveStraightLineSet.h>
#include "TGeoNode.h"
#include "TEveGeoNode.h"
//#include "TEveGeoTopNode.h"
#include "TGeoManager.h"
#include "TGLViewer.h"
#include "TGeoShape.h"

#include "TEvePointSet.h"
#include "TColor.h"
#include "CommonFunc.h"
//#include "TEvePointSetArray.h"
#include "TROOT.h"
namespace Tracking {

//TEveElementList* EveVisualizer::fEveGeomList = 0;

Tracking::Vector3D<double> EveVisualizer::UnpackColor(double val){
	return CommonFunc::Functions::instance()->UnpackColor(val);
}

double EveVisualizer::PackColor(Tracking::Vector3D<double> color){
	return CommonFunc::Functions::instance()->PackColor(color);
}

TColor* EveVisualizer::CreateColor(double val){
	  TColor *color = gROOT->GetColor(10);
	  Tracking::Vector3D<double> colVect = CommonFunc::Functions::instance()->UnpackColor(val);
	  color->SetRGB(colVect.x(),colVect.y(),colVect.z());
	  return color;
  }

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
  int bins = 200;
  fPointSetArray->InitBins("Scattering", bins, 0., 200.);

   TColor::SetPalette(1, 0); 
   const Int_t nCol = TColor::GetNumberOfColors();
   for (Int_t i = 1; i <= bins; ++i)
      fPointSetArray->GetBin(i)->SetMainColor(TColor::GetColorPalette(i * nCol / bins));

   fPointSetArray->GetBin(0) ->SetMainColor(kGray);
   fPointSetArray->GetBin(10)->SetMainColor(kWhite);

}

  }

void EveVisualizer::AddLine(Vector3D<double>p1, Vector3D<double>p2,double color){
	std::cout<<"Add line called ...." << std::endl;
//if(!ls)
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
  ls->SetMainColor(color);
  //ls->SetMarkerSize(1.5);
  //ls->SetMarkerStyle(4);
  Singleton::instance()->AddLineSet(ls);
  //gEve->AddElement(ls);
  //Singleton::instance()->AddElement(ls);
  std::cout<<"Exiting AddLIne .....  " << std::endl;
}

void EveVisualizer::AddTracks(std::vector<Tomography::Track*> trackVector,double color){
	ls = new TEveStraightLineSet();
	for(int i = 0 ; i < trackVector.size() ; i++){
	Tracking::Vector3D<double> p1 = trackVector[i]->GetP1();
	Tracking::Vector3D<double> p2 = trackVector[i]->GetP2();
	ls->AddLine( p1.x(),p1.y(),p1.z(),
			       p2.x(),p2.y(),p2.z());
	AddMarkers(p1);
	AddMarkers(p2);
	}

	ls->SetMainColor(color);
	Singleton::instance()->AddLineSet(ls);


}


void EveVisualizer::RemoveLine(){
	//fEveGeomList->RemoveElement(ls);
	Tracking::Singleton::instance()->RemoveElement();
}

void EveVisualizer::RemoveTrack(){
	RemoveLine();
}

void EveVisualizer::AddMarkers(Vector3D<double> pt) {
     /*for (int i = 0; i < c.GetLength(); i++) {
       ls->AddMarker(pt.x(), pt.y(), pt.z());
     }*/

	ls->AddMarker(pt.x(), pt.y(), pt.z());
     ls->SetMarkerSize(1.3);
     ls->SetMarkerStyle(4);



   }

void EveVisualizer::AddMarkers_V2(Vector3D<double> pt, double color){

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
	  std::cout<<"Color of POCA : " << pt.GetColor() << std::endl;
	  if(color < 0)
		  fPointSetArray->Fill(pt.x(),pt.y(),pt.z(),pt.GetColor());
	  else
		  fPointSetArray->Fill(pt.x(),pt.y(),pt.z(),color);
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
  fEveShape->SetMainColor(kYellow);
  fEveShape->SetMainTransparency(80);
  fEveShape->SetTransMatrix(mat);
  //fEveGeomList->AddElement(fEveShape);
  Singleton::instance()->AddElement(fEveShape);
}
  
}
void EveVisualizer::AddEveShape(std::string shapeName,TGeoBBox *shape,  TGeoHMatrix &mat , int color,int transparency){
std::cout << "Adding EVE Shape from : " << __FILE__ << " : " << __LINE__ << std::endl;
sleep(5);
if(gEve){
  fEveShape = new TEveGeoShape(shapeName.c_str());
  fEveShape->SetShape(shape);
  fEveShape->SetMainColor(color);
  fEveShape->SetMainTransparency(transparency);
  fEveShape->SetTransMatrix(mat);
  //fEveGeomList->AddElement(fEveShape);
  Singleton::instance()->AddElement(fEveShape);
 }
}

void EveVisualizer::AddEveShape(std::string shapeName,TGeoBBox *shape,  TGeoHMatrix &mat , int color){
std::cout << "Adding EVE Shape from : " << __FILE__ << " : " << __LINE__ << std::endl;
sleep(5);
if(gEve){
  fEveShape = new TEveGeoShape(shapeName.c_str());
  fEveShape->SetShape(shape);
  fEveShape->SetMainColor(color);
  fEveShape->SetMainTransparency(20);
  fEveShape->SetTransMatrix(mat);
  //fEveGeomList->AddElement(fEveShape);
  Singleton::instance()->AddElement(fEveShape);
 }
}

double EveVisualizer::CreateColorVal(double color){
	 TColor::SetPalette(1, 0);
	 const Int_t nCol = 100;//TColor::GetNumberOfColors();
	 float min = 0, max = 50.; // your range of values
	 double colorVal = TColor::GetColorPalette((color - min)/(max-min) * nCol);
	 return colorVal;
  }

void EveVisualizer::AddEveShape(std::string shapeName,TGeoBBox *shape,  TGeoHMatrix &mat , double color){
std::cout << "From ADDEVESHAPE of " << __FILE__ << " : " << __LINE__ << std::endl;
if(gEve){
  fEveShape = new TEveGeoShape(shapeName.c_str());
  fEveShape->SetShape(shape);
  TColor *col = CreateColor(color);
  //------
/*
  TColor::SetPalette(1, 0);
  //TColor::SetPalette(53);
  const Int_t nCol = 100;//TColor::GetNumberOfColors();
  float min = 0, max = 50.; // your range of values
  double colorVal = TColor::GetColorPalette((color - min)/(max-min) * nCol);
  fEveShape->SetMainColor(colorVal);
  //------
  //fEveShape->SetMainColor(color);
  double setcolor = PackColor(UnpackColor(color));
  std::cout << "Actual Color : " << color <<" : SetColor : " << setcolor << std::endl;
  //fEveShape->SetFillColor(setcolor*255);
*/
  //double colorVal = CreateColorVal(color);
  double colorVal = CommonFunc::Functions::instance()->CreateColorVal(color);
  fEveShape->SetMainColor(colorVal);
  fEveShape->SetMainTransparency(colorVal);
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
