/*
 * Clustering.cpp
 *
 *  Created on: May 4, 2017
 *      Author: rsehgal
 */

#include "Clustering.h"

namespace Tracking {
using Cluster = std::vector<Point*>;

Clustering::Clustering():fEpsilon(2) {
	// TODO Auto-generated constructor stub

}

Clustering::Clustering(std::vector<Vec_t> ptVect):fEpsilon(1.1),fMinPtsInCluster(20){
	//Call the required Clustering Algorithm, using DBSCAN by default
	DBSCAN(ptVect);
}

Clustering::Clustering(std::vector<Vec_t> ptVect, double eps):fEpsilon(eps),fMinPtsInCluster(20){
	DBSCAN(ptVect);
}

Clustering::~Clustering() {
	// TODO Auto-generated destructor stub
}

void Clustering::RemoveNoisyCluster(){
	for(int i = 0 ; i < fClusterVect.size() ; i++){
		if(fClusterVect[i].size() > fMinPtsInCluster){
			Cluster clus;
			for(int j = 0 ; j < fClusterVect[i].size() ; j++){
				clus.push_back(fClusterVect[i][j]);
			}
			fFilteredClusterVect.push_back(clus);
		}
	}
}

void Clustering::DBSCAN(std::vector<Vec_t> ptVect){
 //Logic to identify Clusters

/* Algorithm : 1)Take first point, mark it visited and put it in ClusterNum(0)
 *
 *			   2)Take next point and check if it is NOT already visited then calculate its distance with
 *				 all the point in all the cluster one by one, if any distance is less than or equal to
 *				 Epsilon then mark that point as visited and put it in the respective cluster
 *
 *			   3)If the distance is not less than Epsilon with any of the points in any of the cluster,
 *			     then create a new cluster and put the point in this new cluster
 *
 *			   4)Repeat Step 2 & 3 until all the points are marked visited
 *
 *			   5)Now traverse all the cluster and get the number of points it contains, if
 *			     num of points in a cluster is less then Minimum Points Required (fMinPtsInCluser),
 *			     then declare it as noise and remove it from the list of clusters
 *
 */
	bool verbose=true;
	int clusterNum = 0;
	for(int i=0 ; i < ptVect.size() ; i++){
		//std::cout<<"-------------------------------------" << std::endl;
		//ptVect[i].Print(); std::cout<<std::endl;
		//std::cout<<"-------------------------------------" << std::endl;
		//std::cout<<" I = " << i << std::endl;
		Point *pt = new Point(ptVect[i]);
		//pt->fVisited=true;
		if(i==0){  //first point is coming
			pt->fClusterNum=clusterNum;
			std::cout<<"--- Creating Initial Cluster with the starting point -----" << std::endl;
			Cluster clus0;
			pt->fVisited=true;
			clus0.push_back(pt);
			fClusterVect.push_back(clus0);
		}
		else{
			//std::cout<<"--- Entere Else ---- : "<< i << std::endl;
			//process the remaining point using above explained algo
			//std::cout<<"ClusterVector Size : " << fClusterVect.size() << std::endl;

			for(int k=0; k < fClusterVect.size() ; k++){

				//std::cout<<"Printing from ClusterVect : " ; (fClusterVect[k][0]->fPt).Print(); std::cout<<std::endl;
				//std::cout<<"Looping of ClusterVector : " << k << std::endl;
				//std::cout<<"Cluster Size : " << fClusterVect[k].size() << std::endl;
				if(!pt->fVisited){
				for(int j=0 ; j<fClusterVect[k].size() ; j++){
					//std::cout<<"Looping of Cluster : " << j << std::endl;

				//	std::cout<<"Distance : " << Vector3D<double>::Distance(pt->fPt,fClusterVect[k][j]->fPt) << std::endl;

			//		(pt->fPt).Print(); std::cout<<std::endl;
			//		(fClusterVect[k][j]->fPt).Print(); std::cout<<std::endl;

					if(Vector3D<double>::Distance(pt->fPt,fClusterVect[k][j]->fPt) <= fEpsilon){
						pt->fClusterNum = fClusterVect[k][j]->fClusterNum;
						pt->fVisited=true;
						fClusterVect[k].push_back(pt);
						break;
					}

				}
				}
				//break;

			}

			if(!pt->fVisited){
				std::cout<<"--- Creating New Cluster -----" << std::endl;
				//Create a new cluster
				Cluster clus;
				clusterNum++;
				pt->fClusterNum = clusterNum;
				pt->fVisited=true;
				clus.push_back(pt);
				fClusterVect.push_back(clus);
			}

		}

		//delete pt;
	}

	RemoveNoisyCluster();

	if(verbose){
	std::cout<<"------------ Final Printing----------------------" << std::endl;
	for(int i=0;i<fClusterVect.size();i++){
		std::cout<<"------- Cluster No : " << i << "----------- " << std::endl;
		for(int j=0 ; j<fClusterVect[i].size();j++){
			(fClusterVect[i][j]->fPt).Print(); std::cout<<std::endl;
		}

	}
   }
}

} /* namespace Tracking */
