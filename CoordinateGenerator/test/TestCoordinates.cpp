#include <iostream>
#include "Coordinates.h"

//#include "Properties.h"
typedef Tomography::Properties Detector;
//#undef USE_EVE
#include "SetupManager.h"
//typedef Tomography::SetupManager::instance() uManager;
int main()
{
	Coordinates c;
	int totalDetectors;

	//Creating Detectors
	Detector *rpc1 = new Tomography::GlassRpc(3,"FirstGlassRpc", 90,-1);
	Detector *rpc2 = new Tomography::GlassRpc(3,"SecondGlassRpc", 60,-1);
	Detector *rpc3 = new Tomography::GlassRpc(3,"ThirdGlassRpc", -60,-1);
	Detector *rpc4 = new Tomography::GlassRpc(3,"FourthGlassRpc", -90,-1);

	//Registering Detector with SetupManager
	Tomography::SetupManager::instance()->Register(rpc1);
	Tomography::SetupManager::instance()->Register(rpc2);
	Tomography::SetupManager::instance()->Register(rpc3);
	Tomography::SetupManager::instance()->Register(rpc4);

	std::vector<Detector*> detVector = Tomography::SetupManager::instance()->GetDetectorVector("GLASS");
	totalDetectors = detVector.size();

	Vector3D<double> temp;
	int N = totalDetectors;
	//c.CoGenerator(totalDetectors , detVector);
	c.CoGenerator(detVector);
	c.SetStrips();
	c.SetStripCoordinates();
	c.Print();
	std::cout<<""<<std::endl;
	std::cout<<"Printing MidPoint of all the strips where intersection happens:"<<std::endl;
	std::cout<<""<<std::endl;

	for(int i = 0 ; i < N ; i++)
	{
		temp = c.GetStripCoordinate(i+1);
		temp.Print();
		
	}
      //}

	return 0;
}
