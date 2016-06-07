#include "Coordinates.h"
#include <math.h>

namespace Tomography{

	//Coordinates::Coordinates(){}ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
	//Coordinates::~Coordinates(){}
	Coordinates::Coordinates(std::vector<Detector*> det)
	{
		detectors = det;
		totalDetector = detectors.size();
	}

/*   ooooooooooooooooooooooooooooooooooooooooooooooo
	Coordinates::Coordinates(Vector3D<double> init)
	{
		InitialPoint.SetX(init.x());
		InitialPoint.SetY(init.y());
		InitialPoint.SetZ(init.z());
		signal = 1;
	}
*/
	//Coordinates::~Coordinates(Vector3D<double> init){}

	Coordinates::Coordinates(std::vector<Detector*> det, Vector3D<double> init)
	{
		detectors = det;
		totalDetector = detectors.size();
		InitialPoint.SetX(init.x());
		InitialPoint.SetY(init.y());
		InitialPoint.SetZ(init.z());
		signal = 1;
	}

	void Coordinates::InitializeVectors()
	{
		/*  oooooooooooooooooooooooooooooooooooooo
		coordinate.resize(N);
	  //	for (int i = 0; i < N; ++i)
	  //  		coordinate[i].resize(3);

	    stripcoord.resize(N);
	   // for(int i = 0 ; i < N ; i++)
	   // 	stripcoord[i].resize(3);

	    strip.resize(N);
		*/
	    coordinate.resize(totalDetector);
	  //	for (int i = 0; i < N; ++i)
	  //  		coordinate[i].resize(3);

	    stripcoord.resize(totalDetector);
	   // for(int i = 0 ; i < N ; i++)
	   // 	stripcoord[i].resize(3);

	    strip.resize(totalDetector);
	   // for(int i = 0 ; i < N ; i++)
	    //	strip[i].resize(2);

	    
	}
/*       oooooooooooooooooooooooooooooooooooooooooooooooooooo
	void Coordinates::CoGenerator(std::vector <Detector*> det,int N1)
	{
		if(N1==0)
			N = det.size();
		else
		    N = N1;
	//	M = M1;
		
	 	InitializeVectors();
		
		for(int i = 0 ; i < N ; i++)
		{
			//coordinate[i][2] = (double)((N-1-i)*M);
			coordinate[i].SetZ(det[i]->GetZPos());
		}
		//srand(time(NULL));
		//coordinate[0][0] = GenRandom(-50, 50);
		//coordinate[0][1] = GenRandom(-50, 50);
		//coordinate[N-1][0] = GenRandom(-50, 50);
		//coordinate[N-1][1] = GenRandom(-50, 50);
        if(signal == 0)
        {
			InitialPoint.SetX(GenRandom(-50,50));
			InitialPoint.SetY(GenRandom(-50,50));
			InitialPoint.SetZ(det[0]->GetZPos() + 30);
        }
		//coordinate[0].SetX(GenRandom(-50,50));
		//coordinate[0].SetY(GenRandom(-50,50));
		coordinate[N-1].SetX(GenRandom(-50,50));
		coordinate[N-1].SetY(GenRandom(-50,50));

		for(int i = 0 ; i < N - 1 ; i++)
		{
			//temp = ( (i+1) * (double)M - coordinate[0][2] ) / ( coordinate[0][2] - coordinate[N-1][2]);
			//coordinate[N-i-2][0] = (coordinate[0][0] - coordinate[N-1][0]) * temp + coordinate[0][0];
			//coordinate[N-i-2][1] = (coordinate[0][1] - coordinate[N-1][1]) * temp + coordinate[0][1];
			temp = ( coordinate[N-i-2].z()- InitialPoint.z() ) / ( InitialPoint.z() - coordinate[N-1].z());
			coordinate[N-i-2].SetX((InitialPoint.x() - coordinate[N-1].x()) * temp + InitialPoint.x());
			coordinate[N-i-2].SetY((InitialPoint.y() - coordinate[N-1].y()) * temp + InitialPoint.y());
		}		
	}
*/

	void Coordinates::CoGenerator()
	{
		/*   ooooooooooooooooooo
		std::vector<Detector*> det;
		if(flag==0)
		{
			det = Tomography::SetupManager::instance()->GetUpperLayerDetectorVector("GLASS");  //Need to pass value of 'fDetectorType'
			N = det.size();
		}	
		else
		   {
		   	det = Tomography::SetupManager::instance()->GetLowerLayerDetectorVector("GLASS");	//Need to pass value of 'fDetectorType'
			N = det.size();
		   } 
	//	M = M1;
		*/
		  double minRandom, maxRandom;  //added in new version 
	 	InitializeVectors();
		
		/*    ooooooooooooooooooooooooooooooooooo
		for(int i = 0 ; i < N ; i++)
		{
			//coordinate[i][2] = (double)((N-1-i)*M);
			coordinate[i].SetZ(det[i]->GetZPos());
		}
		*/
		for(int i = 0 ; i < detectors.size() ; i++)
		{
			//coordinate[i][2] = (double)((N-1-i)*M);
			coordinate[i].SetZ(detectors[i]->GetZPos());
		}
		
		srand(time(NULL));
		//coordinate[0][0] = GenRandom(-50, 50);
		//coordinate[0][1] = GenRandom(-50, 50);
		//coordinate[N-1][0] = GenRandom(-50, 50);
		//coordinate[N-1][1] = GenRandom(-50, 50);
        if(signal == 0)
        {
			InitialPoint.SetX(GenRandom(-50,50));
			InitialPoint.SetY(GenRandom(-50,50));
			InitialPoint.SetZ(detectors[0]->GetZPos() + 30);

			//initial points are still hard coded
        }
		//coordinate[0].SetX(GenRandom(-50,50));
		//coordinate[0].SetY(GenRandom(-50,50));
		/*    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
		coordinate[N-1].SetX(GenRandom(-50,50));
		coordinate[N-1].SetY(GenRandom(-50,50));
		*/
		//for(int i = 0 ; i < totalDetector ; i++)
		coordinate[totalDetector-1].SetX(GenRandom(detectors[totalDetector-1]->GetPlane(0)->GetBreadth()/(-2), detectors[totalDetector-1]->GetPlane(0)->GetBreadth()/2));
		coordinate[totalDetector-1].SetX(GenRandom(detectors[totalDetector-1]->GetPlane(1)->GetLength()/(-2), detectors[totalDetector-1]->GetPlane(1)->GetLength()/2));

		/*  oooooooooooo
		for(int i = 0 ; i < N - 1 ; i++)
		{
			//temp = ( (i+1) * (double)M - coordinate[0][2] ) / ( coordinate[0][2] - coordinate[N-1][2]);
			//coordinate[N-i-2][0] = (coordinate[0][0] - coordinate[N-1][0]) * temp + coordinate[0][0];
			//coordinate[N-i-2][1] = (coordinate[0][1] - coordinate[N-1][1]) * temp + coordinate[0][1];
			temp = ( coordinate[N-i-2].z()- InitialPoint.z() ) / ( InitialPoint.z() - coordinate[N-1].z());
			coordinate[N-i-2].SetX((InitialPoint.x() - coordinate[N-1].x()) * temp + InitialPoint.x());
			coordinate[N-i-2].SetY((InitialPoint.y() - coordinate[N-1].y()) * temp + InitialPoint.y());
		}		
		*/

		for(int i = 0 ; i < totalDetector - 1 ; i++)
		{
			//temp = ( (i+1) * (double)M - coordinate[0][2] ) / ( coordinate[0][2] - coordinate[N-1][2]);
			//coordinate[N-i-2][0] = (coordinate[0][0] - coordinate[N-1][0]) * temp + coordinate[0][0];
			//coordinate[N-i-2][1] = (coordinate[0][1] - coordinate[N-1][1]) * temp + coordinate[0][1];
			temp = ( coordinate[totalDetector-i-2].z()- InitialPoint.z() ) / ( InitialPoint.z() - coordinate[totalDetector-1].z());
			coordinate[totalDetector-i-2].SetX((InitialPoint.x() - coordinate[totalDetector-1].x()) * temp + InitialPoint.x());
			coordinate[totalDetector-i-2].SetY((InitialPoint.y() - coordinate[totalDetector-1].y()) * temp + InitialPoint.y());
		}
	}


	double Coordinates::GenRandom(double min, double max)
	{
		return (rand() / (static_cast<double>(RAND_MAX) + 1.0)) * (max - min) + min;
	}

	Vector3D<double> Coordinates::GetSpecificCoordinate(double zpos)
	{
		Vector3D<double> vec;
		/*   ooooooooo
		temp = ( zpos - InitialPoint.z() ) / ( InitialPoint.z() - coordinate[N-1].z());
		vec.SetX((InitialPoint.x() - coordinate[N-1].x()) * temp + InitialPoint.x());
		vec.SetY((InitialPoint.y() - coordinate[N-1].y()) * temp + InitialPoint.y());
		vec.SetZ(zpos);
		*/

		temp = ( zpos - InitialPoint.z() ) / ( InitialPoint.z() - coordinate[totalDetector-1].z());
		vec.SetX((InitialPoint.x() - coordinate[totalDetector-1].x()) * temp + InitialPoint.x());
		vec.SetY((InitialPoint.y() - coordinate[totalDetector-1].y()) * temp + InitialPoint.y());
		vec.SetZ(zpos);
		return vec;
	}



	Vector3D<double> Coordinates::GetInitialPoint()
	{
		return InitialPoint;
	}

	

	Vector3D<double> Coordinates::GetCoordinate(int detector)
	{
		Vector3D<double> temp;
		//if(detector > N || detector < 1)     ooooooooooooooooooooooooooooooooo
		if(detector > totalDetector || detector < 1)
		{
			std::cout<<"Inappropriate Request"<<std::endl;
		//	return NULL;
		}

		//for(int i = 0 ; i < 3 ; i++)
		//{
			//temp[i] = coordinate[detector - 1][i];
			temp = coordinate[detector - 1];
		//}

		return temp;
	} 

	void Coordinates::SetStripCoordinates()
	{
		/*    oooooooooooooooooooooooooooooooooooooooooooooooo
		for(int i =  0 ; i < N ; i++)
		{
			//stripcoord[i][0] = (double)-50 + (strip[i][0])*3.125 + 1.5625;
			//stripcoord[i][1] = (double)-50 + (strip[i][1])*3.125 + 1.5625;
			//stripcoord[i][2] = coordinate[i][2];
			stripcoord[i].SetX(-50. + (strip[i].x())*3.125 + 1.5625);
			stripcoord[i].SetY(-50. + (strip[i].y())*3.125 + 1.5625);
			stripcoord[i].SetZ(coordinate[i].z());
		}
		*/

		for(int i =  0 ; i < totalDetector ; i++)
		{
			stripcoord[i].SetX(detectors[i]->GetPlane(0)->GetBreadth()/(-2) + (strip[i].x())*(detectors[i]->GetPlane(0)->GetBreadth() / detectors[i]->GetPlane(0)->GetNumOfScintillators()) + detectors[i]->GetPlane(0)->GetBreadth() / detectors[i]->GetPlane(0)->GetNumOfScintillators() / 2);
			stripcoord[i].SetY(detectors[i]->GetPlane(1)->GetLength()/(-2) + (strip[i].x())*(detectors[i]->GetPlane(1)->GetLength() / detectors[i]->GetPlane(1)->GetNumOfScintillators()) + detectors[i]->GetPlane(1)->GetLength() / detectors[i]->GetPlane(1)->GetNumOfScintillators() / 2);
			stripcoord[i].SetZ(coordinate[i].z());
		}
	}

	Vector3D<double> Coordinates::GetStripCoordinate(int detector)
	{
		Vector3D<double> temp;
		//if(detector > N || detector < 1)   oooooooooooooooooooo
		if(detector > totalDetector || detector < 1)
		{
			std::cout<<"Inappropriate Request"<<std::endl;
		//	return NULL;
		}

		//for(int i = 0 ; i < 3 ; i++)
			temp = stripcoord[detector - 1];

		return temp;

	}

	Vector3D<double> Coordinates::GetStripCoordinate(double x, double y, double z)
	{
		int tmp = 0;
		Vector3D<double> temp;
		/*      oooooooooooooo
		temp.SetX(floor( (x+(double)50) /3.125 ));
		temp.SetY(floor( (y+(double)50) /3.125 ));
		temp.SetZ(z);
		*/

		temp.SetX(floor( (x+(double)50) /3.125 ));
		temp.SetY(floor( (y+(double)50) /3.125 ));
		temp.SetZ(z);

		return temp;
	}

	void Coordinates::SetStrips()
	{
		/*        ooooooooooooooooo
		for(int i =  0 ; i < N ; i++)
		{
		//	strip[i][0] = floor( (coordinate[i][0]+(double)50) /3.125 );
		//	strip[i][1] = floor( (coordinate[i][1]+(double)50) /3.125 );
			strip[i].SetX(floor( (coordinate[i].x()+(double)50) /3.125 ));
			strip[i].SetY(floor( (coordinate[i].y()+(double)50) /3.125 ));
			strip[i].SetZ(coordinate[i].z());

		}
		*/

		for(int i =  0 ; i < totalDetector ; i++)
		{
		
			strip[i].SetX(floor( (coordinate[i].x()+ detectors[i]->GetPlane(0)->GetBreadth()/2) / (detectors[i]->GetPlane(0)->GetBreadth() / detectors[i]->GetPlane(0)->GetNumOfScintillators()) ));
			strip[i].SetY(floor( (coordinate[i].y()+ detectors[i]->GetPlane(1)->GetLength()/2) / (detectors[i]->GetPlane(1)->GetLength() / detectors[i]->GetPlane(1)->GetNumOfScintillators()) ));
			strip[i].SetZ(coordinate[i].z());

		}
	}

	void Coordinates::SetStrips(double x, double y, double z)
	{
		int tmp = 0;
		//while(tmp != N && coordinate[tmp].z()!=z)              ooooooooooooooooooooooooooooooooooooooo
		while(tmp != totalDetector && coordinate[tmp].z()!=z)
			tmp++;
		strip[tmp].Set(x,y,z);
	}

	Vector3D<double> Coordinates::GetStrip(int detector)
	{
		Vector3D<double> temp;
		//if(detector > N || detector < 1)                 oooooooooooooooooooooo
		if(detector > totalDetector || detector < 1)
		{
			std::cout<<"Inappropriate Request"<<std::endl;
			//return NULL;
		}

		//for(int i = 0 ; i < 2 ; i++)
			temp = strip[detector - 1];

		return temp;
	}

	void Coordinates::Print()
	{
		std::cout<<"Coordinates"<<std::endl;
		std::cout<<""<<std::endl;

		//for(int i = 0 ; i < N ; i++)        oooooooooooooooooooooo
		for(int i = 0 ; i < totalDetector ; i++)
		{
			coordinate[i].Print();
		}

		std::cout<<""<<std::endl;
		std::cout<<"Strips:"<<std::endl;
		std::cout<<""<<std::endl;

		//for(int i = 0 ; i < N ; i++)    oooooooooo
		for(int i = 0 ; i < totalDetector ; i++)
		{
			strip[i].Print();
		}		
	}

}//end of Tomography namespace
