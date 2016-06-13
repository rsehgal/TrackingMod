#include "TestClass.h"

TestClass::TestClass()
{
	  Detector *rpc1 = new Tomography::GlassRpc(3,"FirstGlassRpc", 90,-1);
	  Detector *rpc2 = new Tomography::GlassRpc(3,"SecondGlassRpc", 60,-1);
	  Detector *rpc3 = new Tomography::GlassRpc(3,"ThirdGlassRpc", 30,-1);
	  Detector *rpc4 = new Tomography::GlassRpc(3,"FourthGlassRpc", 0,-1);
	  Detector *rpc5 = new Tomography::GlassRpc(3,"FifthGlassRpc", -30,-1);
	  Detector *rpc6 = new Tomography::GlassRpc(3,"SixthGlassRpc", -60,-1);

	//Registering Detector with SetupManager
	Tomography::SetupManager::instance()->Register(rpc1);
	Tomography::SetupManager::instance()->Register(rpc2);
	Tomography::SetupManager::instance()->Register(rpc3);
	Tomography::SetupManager::instance()->Register(rpc4);
	Tomography::SetupManager::instance()->Register(rpc5);
	Tomography::SetupManager::instance()->Register(rpc6);
	
}

void TestClass::Initialize()
{
	detectors =  Tomography::SetupManager::instance()->GetDetectorVector("GLASS");
	UpperLayer = Tomography::SetupManager::instance()->GetUpperLayerDetectorVector("GLASS");
	LowerLayer = Tomography::SetupManager::instance()->GetLowerLayerDetectorVector("GLASS");

	totalDetector = UpperLayer.size() + LowerLayer.size();

	totalModule = totalDetector / 2;
	channelInModule.resize(totalModule);
	
	for(int i = 0 ; i < channelInModule.size() ; i++)    // will not work if total detectors are odd number
	{
		channelInModule[i] = detectors[2*i]->GetPlane(0)->GetNumOfScintillators() + detectors[2*i]->GetPlane(1)->GetNumOfScintillators() 
									+ detectors[2*i + 1]->GetPlane(0)->GetNumOfScintillators() + detectors[2*i + 1]->GetPlane(1)->GetNumOfScintillators(); 
	}

	//MODULE1_LE_CH.resize(channelInModule[0]);
	//MODULE2_LE_CH.resize(channelInModule[0]);
	//MODULE3_LE_CH.resize(channelInModule[0]);
}

	void TestClass::WriteTree()
{
	 

   TFile f("BSCDATA_TREE.root","recreate");
   TTree t("DataTree","Detected Muon Data");
  // int totalchannel = (Tomography::SetupManager::instance()->GetDetectorVector("GLASS")).size() * 64;
   //int totalchannel = 64;
   //std::vector<int> MODULE1_LE_CH[128];
   //std::vector<int> MODULE2_LE_CH[128];
   //std::vector<int> MODULE3_LE_CH[128];
   std::string tmp1 = "MODULE1_LE_CH";
   std::string tmp2 = "MODULE2_LE_CH";
   std::string tmp3 = "MODULE3_LE_CH";
   std::string tmp0;
   //Double_t random;
   //Int_t ev;


   for(int i = 0 ; i < channelInModule[0] ; i++)
   	{
   		tmp0 = tmp1+std::to_string(i);
   		t.Branch(tmp0.c_str(),MODULE1_LE_CH+i);
   	}	

 for(int i = 0 ; i < channelInModule[1] ; i++)
	{
		tmp0 = tmp2+std::to_string(i);
		t.Branch(tmp0.c_str(),MODULE2_LE_CH+i);
	}
 for(int i = 0 ; i < channelInModule[2] ; i++)
	{
		tmp0 = tmp3+std::to_string(i);
		t.Branch(tmp0.c_str(),MODULE3_LE_CH+i);
	}
     	

    Tomography::Coordinates c1(UpperLayer);

     	//Tomography::Coordinates c2;
	srand(time(NULL));
   for(int i = 0 ; i < 10 ; i++)
   {
    	usleep(10);
   
   	 c1.CoGenerator();
   	 c1.SetStrips();
	 c1.SetStripCoordinates();
	 c1.Print();
	 std::cout<<""<<std::endl;
	//c2 = new Coordinates
	 Tomography::Coordinates c2(LowerLayer, c1.GetSpecificCoordinate(15.));
	 c2.CoGenerator();
   	 c2.SetStrips();
	 c2.SetStripCoordinates();
	 c2.Print();

	for(int j = 0 ; j < channelInModule[0] ; j++)
		MODULE1_LE_CH[j].clear();
	for(int j = 0 ; j < channelInModule[1] ; j++)
		MODULE2_LE_CH[j].clear();
	for(int j = 0 ; j < channelInModule[2] ; j++)
		MODULE3_LE_CH[j].clear();


	for(int j = 0 ; j < channelInModule[0] ; j++)
		MODULE1_LE_CH[j].push_back(0);
	for(int j = 0 ; j < channelInModule[1] ; j++)
		MODULE2_LE_CH[j].push_back(0);
	for(int j = 0 ; j < channelInModule[2] ; j++)
		MODULE3_LE_CH[j].push_back(0);

	MODULE1_LE_CH[(int)(c1.GetStrip(1)).x()].pop_back();
	MODULE1_LE_CH[((int)(c1.GetStrip(1)).y())+32].pop_back();

	MODULE1_LE_CH[((int)(c1.GetStrip(2)).x())+64].pop_back();
	MODULE1_LE_CH[((int)(c1.GetStrip(2)).y())+96].pop_back();

	MODULE2_LE_CH[(int)(c1.GetStrip(3)).x()].pop_back();
	MODULE2_LE_CH[((int)(c1.GetStrip(3)).y())+32].pop_back();


	MODULE2_LE_CH[((int)(c2.GetStrip(1)).x())+64].pop_back();
	MODULE2_LE_CH[((int)(c2.GetStrip(1)).y())+96].pop_back();

	MODULE3_LE_CH[(int)(c2.GetStrip(2)).x()].pop_back();
	MODULE3_LE_CH[((int)(c2.GetStrip(2)).y())+32].pop_back();

	MODULE3_LE_CH[((int)(c2.GetStrip(3)).x())+64].pop_back();
	MODULE3_LE_CH[((int)(c2.GetStrip(3)).y())+96].pop_back();









	MODULE1_LE_CH[(int)(c1.GetStrip(1)).x()].push_back(1);
	MODULE1_LE_CH[((int)(c1.GetStrip(1)).y())+32].push_back(1);

	MODULE1_LE_CH[((int)(c1.GetStrip(2)).x())+64].push_back(1);
	MODULE1_LE_CH[((int)(c1.GetStrip(2)).y())+96].push_back(1);

	MODULE2_LE_CH[(int)(c1.GetStrip(3)).x()].push_back(1);
	MODULE2_LE_CH[((int)(c1.GetStrip(3)).y())+32].push_back(1);


	MODULE2_LE_CH[((int)(c2.GetStrip(1)).x())+64].push_back(1);
	MODULE2_LE_CH[((int)(c2.GetStrip(1)).y())+96].push_back(1);

	MODULE3_LE_CH[(int)(c2.GetStrip(2)).x()].push_back(1);
	MODULE3_LE_CH[((int)(c2.GetStrip(2)).y())+32].push_back(1);

	MODULE3_LE_CH[((int)(c2.GetStrip(3)).x())+64].push_back(1);
	MODULE3_LE_CH[((int)(c2.GetStrip(3)).y())+96].push_back(1);

	t.Fill();
	
   }

   f.Write();
}
