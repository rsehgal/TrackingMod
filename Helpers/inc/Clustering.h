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

namespace Tracking {
class Point;
using Vec_t = Vector3D<double>;
using Cluster = std::vector<Point*>;
using ClusterVector = std::vector<Cluster>;

class Clustering {
	ClusterVector fClusterVect;
	ClusterVector fFilteredClusterVect;
	double fEpsilon;
	int fMinPtsInCluster;
public:
	Clustering();
	Clustering(std::vector<Vec_t> ptVect);
	Clustering(std::vector<Vec_t> ptVect, double eps);
	void RemoveNoisyCluster();
	void SetEplsilon(double eps){fEpsilon = eps;}
	virtual ~Clustering();
	ClusterVector GetClusterVector(){return fClusterVect;}
	ClusterVector GetFilteredClusterVector(){return fFilteredClusterVect;}
	void DBSCAN(std::vector<Vec_t> ptVect);
	void KMeans(){}
};

class Point{
public:
	Point():fVisited(false),fClusterNum(-1), fPt(Vector3D<double>(0.,0.,0.)){}
	Point(Vector3D<double> pt):fVisited(false),fClusterNum(-1),fPt(pt){}

	bool fVisited;
	int fClusterNum;
	Vector3D<double> fPt;
};

} /* namespace Tracking */

#endif /* HELPERS_INC_CLUSTERING_H_ */
