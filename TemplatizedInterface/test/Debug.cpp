/*
 * Debug.cpp
 *
 *  Created on: Jul 1, 2016
 *      Author: rsehgal
 */


#include "GlassRpc.h"
#include "CmsRpc.h"
#include "TriggeringPlane.h"
#include <vector>
#include <iostream>
#include "VisualizationHelper.h"
#include "SetupManager.h"
#include <TApplication.h>
#include "Update.h"
#include "Tree.h"
 #include "Statistics.h"
typedef Tomography::Properties Detector;
using namespace Tomography;

int main(int argc, char *argv[]) {
	std::string temp_str = std::to_string(atoi(argv[1]));
	  temp_str += ".root";
	  Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);


	  Detector *topPlane = new TriggeringPlane(2,"TopPlane", 105, -1);
	  Detector *bottomPlane = new TriggeringPlane(2,"BottomPlane",-105, 7);

	  Tomography::SetupManager::instance()->Register(topPlane);
	  Tomography::SetupManager::instance()->Register(bottomPlane);

	  //Tomography::ScintillatorPlane::SetClusterSize(1);

	  Tomography::SetupManager::instance()->SetEventDetected("TRG",152);
	  std::cout<<"EventDetectd : " << Tomography::SetupManager::instance()->EventDetected() << std::endl;


	  //topPlane->SetFiredStripsVector(152);
	  //bottomPlane->SetFiredStripsVector(152);

	  std::vector<Detector *> ScintVect = Tomography::SetupManager::instance()->GetDetectorVector("TRG");
      std::cout<<"No of fired strips in top plane : "<< ScintVect[0]->GetPlane(0)->GetFiredStripsVector().size() << std::endl;
	  std::cout<<"No of fired strips in bottom plane : "<< ScintVect[1]->GetPlane(0)->GetFiredStripsVector().size() << std::endl;

	  /*std::vector<Tracking::Vector3D<double>> vertexVect = topPlane->GetPlane(0)->GetScintVector()[0]->GetVertices();
	  for(int i=0 ; i< vertexVect.size() ; i++){
		  vertexVect[i].Print();
	  }*/

	  return 0;
}

