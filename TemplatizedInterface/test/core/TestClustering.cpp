#include "Clustering.h"
#include "base/Vector3D.h"
#include "Coordinates.h"
#include <vector>
#include <fstream>

#define PI 3.14159265359
using Vec_t = Tracking::Vector3D<double>;

void ReadDataFromFile();
void GenerateRandomDataOnCircle();
void FindClusters(std::vector<Vec_t> ptVect, double);

int main(int argc, char **argv){

	Vec_t pt1(1.,0.,0.),pt2(2.,0.,0.),pt3(3.,0.,0.),pt4(4.,0.,0.);
	std::vector<Vec_t> ptVect;
/*
	ptVect.push_back(pt1);
	ptVect.push_back(pt2);
	ptVect.push_back(pt3);
	ptVect.push_back(pt4);


	//ptVect.push_back(Vec_t(5.,0.,0.));
	ptVect.push_back(Vec_t(6.,0.,0.));
	ptVect.push_back(Vec_t(7.,0.,0.));
	ptVect.push_back(Vec_t(8.,0.,0.));
*/

	//Random number between 2 & 3

#if(0)
	Tomography::Coordinates crd;
	for(int i = 0 ; i < 50 ; i++){
	  double x = crd.GenRandom(2.,3.);
	  double y = crd.GenRandom(2.,3.);
	  double z = crd.GenRandom(2.,3.);
	  ptVect.push_back(Vec_t(x,y,z));
	//}

	//Random number between 8 & 9
	//for(int i = 0 ; i < 50 ; i++){
	  x = crd.GenRandom(8.,9.);
	  y = crd.GenRandom(8.,9.);
	  z = crd.GenRandom(8.,9.);
	  ptVect.push_back(Vec_t(x,y,z));
	}
	double epsilon = 1;
	if(argc > 1)
		epsilon = atof(argv[1]);

		FindClusters(ptVect,epsilon);

#endif


		//GenerateRandomDataOnCircle();
		ReadDataFromFile();
		return 0;

}

void ReadDataFromFile(){
	std::ifstream input;
	//input.open("ThreeD.txt");
	input.open("tracks.txt");
	std::vector<Vec_t> ptVect;
	double x=0.,y=0.,z=0.,color=0.;
	double deno=1;
	if(input.is_open()){
		while(!input.eof()){
			input >> x >> y >> z >> color;
			ptVect.push_back(Vec_t(x/deno,y/deno,z/deno));
		}
	}
	double epsilon = 10.0;
	FindClusters(ptVect,epsilon);
}

void GenerateRandomDataOnCircle(){
	double radius=8.;
	Tomography::Coordinates crd;
	std::vector<Vec_t> ptVect;

	for(int i=0 ; i< 100 ; i++){
	  double radMod = radius+(crd.GenRandom(-0.0002,0.0002));
	  double theta = crd.GenRandom(0.,2*PI);
	  double x = radMod * std::cos(theta);
	  double y = radMod * std::sin(theta);
	  double z=0.;
	  ptVect.push_back(Vec_t(x,y,z));
	}

#if(1)
	//another smaller circle
	for(int i=0 ; i< 99 ; i++){
		  radius = 7.0;
		  double radMod = radius+(crd.GenRandom(-0.0002,0.0002));
		  double theta = crd.GenRandom(0.,2*PI);
		  double x = radMod * std::cos(theta);
		  double y = radMod * std::sin(theta);
		  double z=0.;
		  ptVect.push_back(Vec_t(x,y,z));
		}
#endif
	double epsilon = 1.0;
	FindClusters(ptVect,epsilon);


}

void FindClusters(std::vector<Vec_t> ptVect, double epsilon){
	std::ofstream clusterFile;
		clusterFile.open("clusters.txt");
		Tracking::ClusterVector clusterVect;
/*

		if(argc > 1)
			clusterVect = (new Tracking::Clustering(ptVect,atof(argv[1])))->GetClusterVector();
		else
*/
			//clusterVect = (new Tracking::Clustering(ptVect,epsilon))->GetClusterVector();
			clusterVect = (new Tracking::Clustering(ptVect,epsilon))->GetFilteredClusterVector();

				for(int i = 0 ; i< clusterVect.size(); i++){
			for(int j=0 ; j < clusterVect[i].size() ; j++){
				(clusterVect[i][j]->fPt).SetColor(clusterVect[i][j]->fClusterNum);
				clusterFile << (clusterVect[i][j]->fPt).x() << " " << (clusterVect[i][j]->fPt).y() << " " <<
						       (clusterVect[i][j]->fPt).z() << " " << ((clusterVect[i][j]->fPt).GetColor()+1) << std::endl;
			}
		}

		clusterFile.close();

		std::cout<<"=========================================="<<std::endl;
		std::cout<<"     Total Cluster Detected : " << clusterVect.size() << std::endl;
		std::cout<<"=========================================="<<std::endl;


}
