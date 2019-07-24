#include "DetectorMapping.h"
#include "Coordinates.h"
#include "Track.h"
#include <iostream>
#include <fstream>
#include <sstream>  // for string streams
#include <string>  // for string
#include "TrackPredictor.h"

#include "sha256.h"
using Tomography::TrackPredictor;
using Tomography::Coordinates;

std::string name1;
std::string name2;
std::string name3;
std::string name4;
std::string name5;
std::string name6;
std::string name;

std::string output1;
std::ofstream sfile("shacodeNew.txt");
/*
int main(){
	//Tomography::DetectorMapping *detectorMap = new Tomography::DetectorMapping("testMapping.txt");
//	std::cout << "Stack Length : " << Tomography::DetectorMapping::instance()->GetNumOfDetectors() << std::endl;

	std::cout << "User Defined file Stack Length : " << Tomography::DetectorMapping::create("testMapping.txt")->GetNumOfDetectors() << std::endl;
	std::vector<std::string> detectorNamesVector = Tomography::DetectorMapping::instance()->GetDetectorNamesVector();

	for(int i = 0 ; i < detectorNamesVector.size(); i++){
		std::cout << "Name of Detector : " << (i+1) << " : " << detectorNamesVector[i] << std::endl;
	}

	return 0;
}
*/

int main()

{
        SHA256 sha256;
        Coordinates c;
	TrackPredictor t;

       using std::string;
       using std::to_string;
        std::ifstream infile("ActualHitAndPixelCenter.txt");
        double x1=0.,y1=0.,z1=0.,x2=0.,y2=0.,z2=0.;
        Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::create("testMapping.txt");
  	int numOfRpcs = detectorMap->GetNumOfDetectors("GLASS");
	std::vector<std::string> detNamesVector = detectorMap->GetDetectorNamesVector("GLASS");
	std::cout <<"=======================================================" << std::endl;
    	std::cout << "Num Of Rpc :  "<< numOfRpcs << std::endl;

	for(auto &detName : detNamesVector)
            {
		std::cout <<"DetectorName : " << detName << std::endl;
            }

	int evId =0;
	std::vector< std::vector< Vector3D<double> > > superVect;
      	 while(!infile.eof())
	{
	  std::string evString="";
	  std::vector< Vector3D<double> > vectOfVector3D;
      for(int i=0; i<numOfRpcs;i++)
               {
                infile >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
		vectOfVector3D.push_back(Vector3D<double>(x2,y2,z2));
                //string str = detNamesVector[i]+"_"+to_string(x1);
		//str += "_ "+to_string(y1);
		//str += "_ "+to_string(z1);
		//str += "_ "+to_string(x2);
		//str += "_ "+to_string(y2);
		//str += "_ "+to_string(z2);

                //std::cout << strx1 << "  " << stry1 << "  " << strz1 << "  ::  " << strx2 << "  "<< stry2 << "  " << strz2 << std::endl;
		//std::cout <<"Full String : " << str << std::endl;
		//evString+=str;


       	//	 std::cout << "Starting STrip num : " << c.GetStripNum(x1) << std::endl;
		// std::cout << "End STrip num : " << c.GetStripNum(y1) << std::endl;
                // name= detNamesVector[i]+ c.GetStripNumXYZString(Vector3D<double>(x1,y1,z1));
		 name1= detNamesVector[i]+ c.GetStripNum(x2) +  c.GetStripNum(y2);

		// name=name1+name2+name3+name4+name5+name6;
        	  std::cout << "XYZString : " << name1  <<std::endl;





       }  // end of for loop


               output1 = sha256(name1);


                       std::cout << "sha256('"<< name1  << "'):" << output1 << std::endl;
                             sfile << output1 << " " <<std::endl ;


         std::cout << " ----------------------------------------------------------------- " << std::endl;
         std::cout << " ----------------------------------------------------------------- " << std::endl;
	     t.Process(evString,vectOfVector3D);
        superVect.push_back(vectOfVector3D);

	//std::cout << "Event string of event id : " << evId <<" : " << evString << std::endl;
	//evId++;



	} // end of while loop






  return 0;
}

