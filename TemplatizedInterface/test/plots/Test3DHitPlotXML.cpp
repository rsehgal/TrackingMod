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
	int clusterSize = 2;
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	//gStyle->SetPalette(1);
	Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);
	SetupManager *setup = SetupManager::instance();
	Setup *s = new Setup(argv[2]);
	std::vector<Detector*> detVect = s->GetDetectorVector();
	for (int i = 0; i < detVect.size(); i++) {
		detVect[i]->SetClusterSize(clusterSize);
		setup->Register(detVect[i]);
		detVect[i]->GetHitPlot3D_V2();
	}

	setup->GetHitPlot("GLASS");
	fApp->Run();
}
