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
#include "TEvePointSet.h"
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
#include "TGeoManager.h"
#include "Voxel.h"
typedef Tomography::Properties Detector;
namespace Tomography{

class Slicer{

double fXmin, fXmax;
double fYmin, fYmax;
double fZmin, fZmax;
public:
	bool fSlicingRequired;

	Slicer():fSlicingRequired(false), fXmin(0.),fXmax(0.)
								   , fYmin(0.),fYmax(0.)
								   , fZmin(0.),fZmax(0.){}

	Slicer(double xmin,double xmax,double ymin, double ymax, double zmin, double zmax):
	fSlicingRequired(true),fXmin(xmin), fXmax(xmax)
						  ,fYmin(ymin), fYmax(ymax)
						  ,fZmin(zmin), fZmax(zmax) {

	}

	bool PointWithinSlice(Tracking::Vector3D<double> pt){
		return pt.x() >= fXmin && pt.x() <= fXmax
			&& pt.y() >= fYmin && pt.y() <= fYmax
			&& pt.z() >= fZmin && pt.z() <= fZmax;
	}

	~Slicer(){}


};

class VisualizationHelper{

TomographyVisualizer fVis;
bool fSlicingRequired;

//Stuff to make it Singleton
static VisualizationHelper  *s_instance;
VisualizationHelper(){
#ifdef USE_EVE
  TEveManager::Create();
#endif
  fSlicingRequired = true;

}

public:
static VisualizationHelper *instance();

/*
  VisualizationHelper(){
#ifdef USE_EVE
    TEveManager::Create();
#endif
    fSlicingRequired = true;

  }
*/

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

#ifdef USE_EVE
  void CreatePointSetArray(){

    fVis.CreatePointSetArray();
  }
#endif

// This is for visualization during simulation or data analysis
  void Register(Voxel * voxel){
    Tracking::Vector3D<double> voxDim = voxel->GetVoxelDimensions();
    //Tracking::Vector3D<double> voxCenter = voxel->GetVoxelCenter();
    double color = voxel->GetStandardDeviation();
    //Register(voxDim,voxCenter,color);
  }

  
  //To Read from txt file
  void Register(double voxDimX,double voxDimY,double voxDimZ, double voxCenterX,
                double voxCenterY,double voxCenterZ, double color ){

    Register(Tracking::Vector3D<double>(voxDimX,voxDimY,voxDimZ),
            Tracking::Vector3D<double>(voxCenterX,voxCenterY,voxCenterZ),
            color);

  }  

  //To Read from txt file
  void Register(Tracking::Vector3D<double> voxDim,Tracking::Vector3D<double> voxCenter, double color ){

	//std::cout << "Vox Dim : " ; voxDim.Print();
	//std::cout << "Vox Center : " ;  voxCenter.Print();
    TGeoHMatrix m;
    Double_t trans[3] = { 0., 0., 0. };
    m.SetTranslation(trans);
    TGeoBBox *box = new TGeoBBox("Voxel",voxDim.x()/2.,voxDim.y()/2.,voxDim.z()/2.);
    m.SetDx(voxCenter.x());
    m.SetDy(voxCenter.y());
    m.SetDz(voxCenter.z());
    fVis.AddEveShape("Voxel",box,m,color);
    //std::cout << "Set Color : "<< color << std::endl;

  }

  void Register(Tomography::Track *t,double color=5){
	  fVis.AddLine(t->GetP1(),t->GetP2(),color);
  }

  void Register(std::vector<Tomography::Track *>trackVector,double color=5){
  	  fVis.AddTracks(trackVector,color);
    }

#ifdef USE_EVE
  void RemoveTrack(){
	  fVis.RemoveTrack();
  }
#endif

  void RegisterLine(Tracking::Vector3D<double> p1, Tracking::Vector3D<double> p2){
	  fVis.AddLine(p1,p2);
  }


  void Register(Tracking::Vector3D<double> pt){
  #ifdef USE_EVE
	  fVis.AddMarkers_V2(pt);
  #else
    fVis.AddMarkers(pt);
  #endif
  }

#ifdef USE_EVE
  void Register_V2(Tracking::Vector3D<double> pt){
    fVis.AddMarkers_V2(pt);
  }
#endif

#ifdef USE_EVE
  void Lock(){

    fVis.CloseBins();
  }
#endif

  //New overloaded version that also takes slices into consideration
  //and will register the point only if it lie within the slice
  void Register(Slicer slicer, Tracking::Vector3D<double> pt){
	  if(slicer.fSlicingRequired){
		if(slicer.PointWithinSlice(pt))
			fVis.AddMarkers(pt);
	  }

  }

  std::string GetFileName(std::string geomFile){
    std::string fileName =  geomFile.substr(0,geomFile.find(".")-1);
    std::string ext = geomFile.substr(geomFile.find(".")+1);
    if(!ext.compare("gdml")){
      TGeoManager::Import(geomFile.c_str());
      std::string finalFileName = fileName+".root";
      gGeoManager->Export(finalFileName.c_str());
      return finalFileName;
    }else{
      if(!ext.compare("root")){
    	return geomFile;
    }else{
    std::cerr<<"*************************************" << std::endl;
    std::cerr<<"    UnRecognized file format??? " << std::endl;
    std::cerr<<"*************************************" << std::endl;
   }}
   
  }

#ifdef USE_EVE
void InitializeVisualizer(){
  fVis.InitializeVisualizer();
}
#endif
#ifdef USE_EVE
  //Function to register directly from ROOT file
  void Register(std::string geomFile){
    std::string finalName = GetFileName(geomFile);
    

    gGeoManager = gEve->GetGeometry(finalName.c_str());
    int numOfTopNodes = gGeoManager->GetTopVolume()->GetNodes()->GetEntries();
    for(int i = 0 ; i < numOfTopNodes ; i++){
        std::cout<<"Name of : " << i <<" Node : "<< gGeoManager->GetTopVolume()->GetNodes()->At(i)->GetName() << std::endl;
        std::string name = gGeoManager->GetTopVolume()->GetNodes()->At(i)->GetName();
	TGeoNode* node1 = gGeoManager->GetTopVolume()->FindNode(gGeoManager->GetTopVolume()->GetNodes()->At(i)->GetName());
        TGeoHMatrix m;
	m.SetDx(node1->GetMatrix()->GetTranslation()[0]);//; = node1->GetMatrix();
        m.SetDy(node1->GetMatrix()->GetTranslation()[1]);
        m.SetDz(node1->GetMatrix()->GetTranslation()[2]);
     
	TGeoVolume* vol = node1->GetVolume();
	TGeoShape* shape = vol->GetShape();
        fVis.AddEveShape(name,shape,m);

    }
  }

  void Register(std::string geomFile, std::string placedVolumeName){
    gGeoManager = gEve->GetGeometry(geomFile.c_str());
    TGeoNode* node1 = gGeoManager->GetTopVolume()->FindNode(placedVolumeName.c_str());
    TGeoHMatrix m;
    m.SetDx(node1->GetMatrix()->GetTranslation()[0]);//; = node1->GetMatrix();
    m.SetDy(node1->GetMatrix()->GetTranslation()[1]);
    m.SetDz(node1->GetMatrix()->GetTranslation()[2]);

    TGeoVolume* vol = node1->GetVolume();
    TGeoShape* shape = vol->GetShape();
    fVis.AddEveShape(placedVolumeName,shape,m);

    TGeoMatrix* mat = node1->GetMatrix();    
    std::cout<<"Translation Matrix : "<< mat->GetTranslation()[0]<<" , "<< mat->GetTranslation()[1] <<" , " <<  mat->GetTranslation()[2]<< std::endl;
    if(!(mat->IsRotation()))
       std::cout<<"NO Rotation"<< std::endl;
      //std::cout<<"Rotation Matrix : "<< node1->GetMatrix()->GetRotation() << std::endl;
    double local[3]={5.,0.,0.};
    double master[3]={0.,0.,0.};
    mat->LocalToMaster(local,master);
    std::cout<<"W.r.t Local : "<< local[0] <<" , " << local[1] << " , " << local[2] << std::endl;
    std::cout<<"W.r.t Master : "<< master[0] <<" , " << master[1] << " , " << master[2] << std::endl;
  }
#endif

  void Show(){
#ifdef USE_EVE
    Tracking::EveVisualizer::Show();
#else
    fVis.Show();
#endif
  }

 TomographyVisualizer GetVisualizer(){
    return fVis;
 }
 
};




}// end of Tomography namespace


#endif /* for SHOW_VISUALIZATION */
#endif /* INC_VISUALIZATIONHELPER_H_ */
