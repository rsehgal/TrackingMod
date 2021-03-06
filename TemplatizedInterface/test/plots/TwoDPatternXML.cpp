#include "GlassRpc.h"
#include "CmsRpc.h"
#include "TriggeringPlane.h"
#include <vector>
#include <iostream>
//#include "VisualizationHelper.h"
#include "SetupManager.h"
#include <TApplication.h>
//#include "Update.h"
#include "Tree.h"
#include "Statistics.h"
#include <TStyle.h>
#include "Setup.h"
typedef Tomography::Properties Detector;
using namespace Tomography;

int main(int argc, char *argv[]) {
	std::string temp_str = std::to_string(atoi(argv[1]));
	temp_str += ".root";
	int clusterSize = 10;
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	//gStyle->SetPalette(1);
	Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);
	SetupManager *setup = SetupManager::instance();
	Setup *s = new Setup(argv[2]);
	std::vector<Detector*> detVect = s->GetDetectorVector();
	std::vector<Detector*> trgPlaneVect = s->GetTriggeringPlaneVector();

	for(int i=0 ; i<trgPlaneVect.size() ; i++){
		trgPlaneVect[i]->SetEfficiency();
		trgPlaneVect[i]->SetClusterSize(clusterSize);
		std::cout<<"Eff of Plane : " << (i+1) <<" : " << trgPlaneVect[i]->GetEfficiency()<<std::endl;
	}

#if(0)
	for (int i = 0; i < detVect.size(); i++) {
		detVect[i]->SetClusterSize(clusterSize);
		detVect[i]->GetPlane(0)->SetClusterSize(clusterSize);
		detVect[i]->GetPlane(1)->SetClusterSize(clusterSize);
		setup->Register(detVect[i]);
		//detVect[i]->GetHitPlot3D_V2();
		detVect[i]->GetStripsHitPlot3D();
	}
#endif
	//setup->GetHitPlot("GLASS");
	fApp->Run();
}
