/* Author(s) : Indroneil Kanungo, Vishal Singh, Gargi Kekre
 * Date : 28th June 2016
 *   Track Validation
 *  
 *  
*/
#include <iostream>
#include <TriggeringPlane.h>
#include <Properties.h>
#include <SetupManager.h>
#include <cstring>
#include "Coordinates.h"
typedef Tomography::Properties Detector;
typedef Tracking::Vector3D<double> LienVector;
using namespace Tomography;

LienVector GetStripCoordinate(double x, double y, double z) {
  int tmp = 0;
  LienVector temp;
  temp.SetX(-50. + (31-x) * 3.125 + 1.5625);
  temp.SetY(-50. + y * 3.125 + 1.5625);
  temp.SetZ(z);

  return temp;
}

int GetScintillatorNoFromCoordinate(LienVector v) {
	double zz=v.z();
	double xx=(zz==-105)?(v.x()):((v.x()));
	return ((xx<0)?(int(xx/18)-1):(int(xx/18)))+4;
}

int main(int argc,char* argv[]) {
	std::string temp_str = std::to_string(atoi(argv[1]));
	temp_str += ".root";
	Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);
	Detector *MT1 = new GlassRpc(2, "MT1", -75, 31);
	Detector *MT2 = new GlassRpc(4, "MT2", 45, 31);
	Detector *ScintTop = new TriggeringPlane(2, "ScintTop", 105, -1);
	Detector *ScintBottom = new TriggeringPlane(2, "ScintBottom", -105, 7);
	MT1->SetClusterSize(1);
	MT2->SetClusterSize(1);
	ScintTop->SetClusterSize(1);
	ScintBottom->SetClusterSize(1);
	SetupManager *setup = SetupManager::instance();
	setup->Register(MT2);
	setup->Register(MT1);
	setup->Register(ScintTop);
	setup->Register(ScintBottom);
	int evNo, i, j, k, l, h;
	double x, y, z;
	Coordinates c;
	std::vector<LienVector> coords(2);
	LienVector TopIntersection, BottomIntersection;
	std::vector<Detector *> DetVect = setup->GetDetectorVector("GLASS");	
	std::vector<Detector *> ScintVect = setup->GetDetectorVector("TRG");
	int fst,fsb,cfst,cfsb;
	int NonShowerEvents=0, SinglePixelEvents=0, ValidEvents=0;
	int clustersize;
	if(argc==3) {
		clustersize=atoi(argv[2]);
	}
	else {
		clustersize=1;
	}
	//ScintillatorPlane::SetClusterSize(clustersize);
	l=0;
	h=1000;
	
	int count=0;
	for(evNo=l;evNo<h;++evNo) {
		setup->SetEventDetected("TRG",evNo);
		if(setup->EventDetected()) {
			NonShowerEvents++;
			setup->SetEventDetected("GLASS",evNo);
			if(setup->EventDetected()) {
				SinglePixelEvents++;
				for (i = 0; i < DetVect.size(); ++i) {
					std::vector<ScintillatorPlane *> DetPlaneVector = DetVect[i]->GetScintillatorPlaneVector();
					x = DetPlaneVector[0]->GetFiredStripsVector()[0];
					y = DetPlaneVector[1]->GetFiredStripsVector()[0];
					z = DetVect[i]->GetZPos();
					coords[i] = GetStripCoordinate(x, y, z);
				}
				
				c.SetP1(coords[0]);
				c.SetP2(coords[1]);
				TopIntersection = c.GetPOI(ScintTop,false);
				BottomIntersection = c.GetPOI(ScintBottom,true);
				
				fst=ScintVect[0]->GetPlane(0)->GetFiredStripsVector()[0];
				fsb=ScintVect[1]->GetPlane(0)->GetFiredStripsVector()[0];
				cfst=GetScintillatorNoFromCoordinate(TopIntersection);
				cfsb=GetScintillatorNoFromCoordinate(BottomIntersection);
/*
				std::cout<<"Event No. : "<<evNo<<std::endl<<std::endl;
				std::cout<<"Top Intersection Coordinates : "<<TopIntersection.x()<<", "<<TopIntersection.y()<<std::endl;
				std::cout<<"Fired Strip : "<<fst<<std::endl;
				std::cout<<"Calculated Fired Strip : "<<cfst<<std::endl<<std::endl;
	
				std::cout<<"Bottom Intersection Coordinates : "<<BottomIntersection.x()<<", "<<BottomIntersection.y()<<std::endl;
				std::cout<<"Fired Strip : "<<fsb<<std::endl;
				std::cout<<"Calculated Fired Strip : "<<cfsb<<std::endl<<std::endl;
*/
				
				if(fsb==cfsb && fst==cfst){

	                                std::cout<<"Event No. : "<<evNo<<std::endl;
/*
					std::cout<<"Top Intersection Coordinates : "<<TopIntersection.x()<<", "<<TopIntersection.y()<<std::endl;
					std::cout<<"Fired Strip : "<<fst<<std::endl;
					std::cout<<"Calculated Fired Strip : "<<cfst<<std::endl<<std::endl;
	
	                                std::cout<<"Bottom Intersection Coordinates : "<<BottomIntersection.x()<<", "<<BottomIntersection.y()<<std::endl;
	                                std::cout<<"Fired Strip : "<<fsb<<std::endl;
	                                std::cout<<"Calculated Fired Strip : "<<cfsb<<std::endl<<std::endl;

*/
					ValidEvents++;
				}
			}
		}

	//	std::cout<<"CLUSTER : " << Tomography::ScintillatorPlane::GetClusterSize() << std::endl;
	}
	
	std::cout<<"Valid Events : "<<ValidEvents<<std::endl;
	std::cout<<"Single Pixel Events : "<<SinglePixelEvents<<std::endl;
	std::cout<<"Non Shower Events : "<<NonShowerEvents<<std::endl;
	std::cout<<"Validity Fraction : "<<ValidEvents*100/NonShowerEvents<<"%"<<std::endl;
	
}
