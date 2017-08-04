/*
 * Clustering.h
 *
 *  Created on: May 4, 2017
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_CLUSTERING_H_
#define HELPERS_INC_CLUSTERING_H_
#include <vector>
#include "base/Vector3D.h"
#include "TH1F.h"
#include <fstream>

namespace Tracking {
class Point;
class NewCluster;

using Vec_t = Vector3D<double>;
using Cluster = std::vector<Point*>;
using Neighbors = Cluster;
using ClusterVector = std::vector<Cluster>;

class Clustering {
	ClusterVector fClusterVect;
	ClusterVector fFilteredClusterVect;
	double fEpsilon;
	int fMinPtsInCluster;
	std::vector<Point*> fPtVect;
	TH1F *fHist1DOfClusterNum;
	TH1F *fHist1DOfScatterigInClusterNum;
	//TFile *fClusterRootFile;
public:
	Clustering();
	Clustering(std::vector<Vec_t> ptVect);
	Clustering(std::vector<Vec_t> ptVect, double eps);
	void RemoveNoisyCluster();
	void SetEplsilon(double eps){fEpsilon = eps;}
	virtual ~Clustering();
	ClusterVector GetClusterVector(){return fClusterVect;}
	ClusterVector GetFilteredClusterVector(){return fFilteredClusterVect;}

	void WriteClusterToFile(std::string fileName = "clusters.txt");

	void DBSCAN(/*std::vector<Vec_t> ptVect*/);
	Cluster AddClusters(Cluster c1, Cluster c2);
	Neighbors FindNeighbors(Point *pt);
	void ExpandCluster(Point *pt, Neighbors &neighborPts, NewCluster &C);
	void FillHistogram();
	void NormalizeScatteringValue();
	void WriteFile();


	void SequentialClustering(std::vector<Vec_t> ptVect);
	void KMeans(){}


	//Function to export data for individual cluster to txt, so that it can be easily analyzed in R
	void ExportClustersData();
};

class Point{
public:
	Point():fVisited(false),fClusterNum(-1), fPt(Vector3D<double>(0.,0.,0.)){}
	Point(Vector3D<double> pt):fVisited(false),fClusterNum(-1),fPt(pt){}

	bool fVisited;
	int fClusterNum;
	Vector3D<double> fPt;
};

class NewCluster{
public:
	NewCluster(){fClusterNum++; std::cout<<"=== New Cluster Created : Cluster Number : " << (fClusterNum+1) << " ===" << std::endl;}
	Cluster fCluster;
	static int fClusterNum;
};

int NewCluster::fClusterNum = -1;

class File{
public:
	File(){}
	File(std::string fileName){
		fHandle.open(fileName,std::ios::out);
	}
	void Close() {
		fHandle.close();
	}
	std::ofstream fHandle;

};

} /* namespace Tracking */

#endif /* HELPERS_INC_CLUSTERING_H_ */
