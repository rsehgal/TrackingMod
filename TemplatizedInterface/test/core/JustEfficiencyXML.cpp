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

	std::string temp_str = std::to_string(atoi(argv[2]));
	temp_str += ".root";

	Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);

	Setup *s = new Setup(argv[1]);
	std::vector<Detector*> detVect = s->GetDetectorVector();
	for (int i = 0; i < detVect.size(); i++) {
		detVect[i]->SetEfficiency();
		detVect[i]->GetPlane(0)->SetEfficiency();
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

}
