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
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TLegend.h>
#include "Setup.h"
typedef Tomography::Properties Detector;
using namespace Tomography;

int main(int argc, char *argv[]) {

	bool verbose = true;
	int clusterSize=1;

	std::string temp_str = std::to_string(atoi(argv[1]));
	temp_str += ".root";

	Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);

	Setup *s = new Setup(argv[2]);
	std::vector<Detector*> detVect = s->GetDetectorVector();
#if(0)
	for (int i = 0; i < detVect.size(); i++) {
		//sleep(5);
		detVect[i]->SetClusterSize(clusterSize);


		detVect[i]->SetEfficiency();
		detVect[i]->GetPlane(0)->SetEfficiency();
		std::cout<< "Name : " << detVect[i]->GetPlane(0)->GetName() << std::endl;
		detVect[i]->GetPlane(1)->SetEfficiency();
		if (verbose) {
			std::cout << "Eff of Det-1 : " << detVect[i]->GetEfficiency()
					<< std::endl;
			std::cout << "Eff w.r.t Plane 1 : "
					<< detVect[i]->GetPlane(0)->GetEfficiency() << std::endl;
			std::cout << "Eff w.r.t Plane 2 : "
					<< detVect[i]->GetPlane(1)->GetEfficiency() << std::endl;
			std::cout << "--------------------------------------" << std::endl;
		}
	}
#endif
	std::cout << "--------------------------------------" << std::endl;
	std::vector<Detector*> trgPlaneVect = s->GetTriggeringPlaneVector();
	for(int i=0;i<trgPlaneVect.size();i++){
	  trgPlaneVect[i]->SetEfficiency();
	  std::cout << "Eff of Det : " <<(i+1)<<" : " << trgPlaneVect[i]->GetEfficiency() << std::endl;

	}
}
