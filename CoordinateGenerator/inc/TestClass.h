#include <iostream>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"
#include "TRandom.h"
#include "SetupManager.h"
#include "Properties.h"
#include "Coordinates.h"
#include "base/Vector3D.h"
typedef Tomography::Properties Detector;


class TestClass{
private:
	int totalDetector;
	std::vector <Detector*> detectors;
	std::vector <Detector*> UpperLayer;
	std::vector <Detector*> LowerLayer;
	int totalModule;
	std::vector<int> channelInModule;
public:
	
	//std::vector<int> GetBranchData(float_t branch);
	TestClass();
	void Initialize();
	std::vector<int> MODULE1_LE_CH[128];
	std::vector<int> MODULE2_LE_CH[128];
	std::vector<int> MODULE3_LE_CH[128];
   
	void WriteTree();
	//void ReadTree();
};