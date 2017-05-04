#include "Clustering.h"
#include "base/Vector3D.h"
#include "Coordinates.h"
#include <vector>
#include <fstream>
using Vec_t = Tracking::Vector3D<double>;
double GetRandomNumber(int num);
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

	std::ofstream clusterFile;
	clusterFile.open("clusters.txt");

	Tracking::ClusterVector clusterVect;
	if(argc > 1)
		clusterVect = (new Tracking::Clustering(ptVect,atof(argv[1])))->GetClusterVector();
	else
		clusterVect = (new Tracking::Clustering(ptVect))->GetClusterVector();

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
	return 0;

}

double GetRandomNumber(int num){
	srand(time(NULL));
	return (rand()%(num+1) + num);

}
