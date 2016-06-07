#include <iostream>
#include "Coordinates.h"

#include "Properties.h"
typedef Tomography::Properties Detector;
//#undef USE_EVE
//#include "SetupManager.h"

#include "LinesAngle.h"

int main()
{
	Vector3D<double> temp;
	//Tomography::Coordinates c1;

	//Tomography::SetupManager s1;
	//Tomography::SetupManager s2;
	LinesAngle l;
	int totalDetectors_s1;
	int totalDetectors_s2;
	double angle;


	//Creating Detectors
	  Detector *rpc1 = new Tomography::GlassRpc(3,"FirstGlassRpc", 90,-1);
	  Detector *rpc2 = new Tomography::GlassRpc(3,"SecondGlassRpc", 60,-1);
	  Detector *rpc3 = new Tomography::GlassRpc(3,"ThirdGlassRpc", 30,-1);
	  Detector *rpc4 = new Tomography::GlassRpc(3,"FourthGlassRpc", 0,-1);
	  Detector *rpc5 = new Tomography::GlassRpc(3,"FifthGlassRpc", -30,-1);
	  Detector *rpc6 = new Tomography::GlassRpc(3,"SixthGlassRpc", -60,-1);
	  Detector *rpc7 = new Tomography::GlassRpc(3,"SeventhGlassRpc", -90,-1);
	  Detector *rpc8 = new Tomography::GlassRpc(3,"EighthGlassRpc", -120,-1);


	//Registering Detector with SetupManager
	Tomography::SetupManager::instance()->Register(rpc1);
	Tomography::SetupManager::instance()->Register(rpc2);
	Tomography::SetupManager::instance()->Register(rpc3);
	Tomography::SetupManager::instance()->Register(rpc4);
	Tomography::SetupManager::instance()->Register(rpc5);
	Tomography::SetupManager::instance()->Register(rpc6);
	//Tomography::SetupManager::instance()->Register(rpc7);
	//Tomography::SetupManager::instance()->Register(rpc8);



	//std::vector<Detector*> detVector_s1 = s1.GetDetectorVector("GLASS");
	//totalDetectors_s1 = detVector_s1.size();

	std::vector<Detector*> detVectorUpper = Tomography::SetupManager::instance()->GetUpperLayerDetectorVector("GLASS");
	//totalDetectors_s1 = detVector.size();


	std::vector<Detector*> detVectorLower = Tomography::SetupManager::instance()->GetLowerLayerDetectorVector("GLASS");
	//totalDetectors_s1 = detVector.size();




	Tomography::Coordinates c1(detVectorUpper);
	//int N = totalDetectors;
	//int M = 30;
        //for(int j=0;j<4;j++){

	std::cout<<"Detectors Above Object:"<<std::endl;
	std::cout<<""<<std::endl;

	c1.CoGenerator();
	c1.SetStrips();
	c1.SetStripCoordinates();
	c1.Print();

	//Detector *rpc2 = new Tomography::GlassRpc(3,"SecondGlassRpc", 60,-1);
	//Detector *rpc3 = new Tomography::GlassRpc(3,"ThirdGlassRpc", -60,-1);
	//Detector *rpc4 = new Tomography::GlassRpc(3,"FourthGlassRpc", -90,-1);

	
/*
	std::vector<Detector*> detVector = Tomography::SetupManager::instance()->GetDetectorVector("GLASS");
	totalDetectors = detVector.size();

	Vector3D<double> temp;
	int N = totalDetectors;
	//c.CoGenerator(totalDetectors , detVector);
	c.CoGenerator(detVector);
	c.SetStrips();
	c.SetStripCoordinates();
	c.Print();
*/	

	std::cout<<""<<std::endl;
	std::cout<<"Printing MidPoint of all the strips where intersection happens:"<<std::endl;
	std::cout<<""<<std::endl;

	for(int i = 0 ; i < detVectorUpper.size() ; i++)
	{
		temp = c1.GetStripCoordinate(i+1);
		temp.Print();
		
	}
	std::cout<<""<<std::endl;
	std::cout<<"Initial Point"<<std::endl;
	std::cout<<""<<std::endl;
	temp = c1.GetInitialPoint();
	temp.Print();
      //}

	//////////////////////////
		std::cout<<""<<std::endl;
		std::cout<<"Detectors Beneath Object:"<<std::endl;
		std::cout<<""<<std::endl;
	//////////////////////////

		temp = c1.GetSpecificCoordinate(15.0);
			Tomography::Coordinates c2(detVectorLower,temp);

					  //Detector *rpc5 = new Tomography::GlassRpc(3,"FirstGlassRpc", -30,-1);
					  //Detector *rpc6 = new Tomography::GlassRpc(3,"SecondGlassRpc", -60,-1);
					  //Detector *rpc7 = new Tomography::GlassRpc(3,"ThirdGlassRpc", -90,-1);
					  //Detector *rpc8 = new Tomography::GlassRpc(3,"FourthGlassRpc", -120,-1);

					//s2.Register(rpc5);
					//s2.Register(rpc6);
					//s2.Register(rpc7);
					//s2.Register(rpc8);

					//std::vector<Detector*> detVector2 = Tomography::SetupManager::instance()->GetLowerLayerDetectorVector("GLASS");
							//totalDetectors_s2 = detVector2.size();

	c2.CoGenerator();
	c2.SetStrips();
	c2.SetStripCoordinates();
	c2.Print();
	std::cout<<""<<std::endl;
	std::cout<<"Printing MidPoint of all the strips where intersection happens:"<<std::endl;
	std::cout<<""<<std::endl;

	for(int i = 0 ; i < detVectorLower.size(); i++)
	{
		temp = c2.GetStripCoordinate(i+1);
		temp.Print();

	}
	std::cout<<""<<std::endl;
	std::cout<<"Initial Point"<<std::endl;
	std::cout<<""<<std::endl;
	temp = c2.GetInitialPoint();
	temp.Print();

	///////////////////////////
	std::cout<<""<<std::endl;
	std::cout<<"Angle Between Lines In Degree :"<<std::endl;
	std::cout<<""<<std::endl;

	angle = l.CalculateAngle(c1.GetCoordinate(1), c1.GetCoordinate(2), c2.GetCoordinate(1), c2.GetCoordinate(2));
	std::cout<<angle<<std::endl;
	//c1.SetStrips(4.,2.,30.);
	//c1.Print();

	return 0;
}
