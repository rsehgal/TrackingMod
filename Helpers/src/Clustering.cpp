/*
 * Clustering.cpp
 *
 *  Created on: May 4, 2017
 *      Author: rsehgal
 */

#include "Clustering.h"
#include <fstream>
#include <TFile.h>

namespace Tracking {
using Cluster = std::vector<Point*>;

Clustering::Clustering():fEpsilon(2) {
	// TODO Auto-generated constructor stub

}

void Clustering::ExportClustersData(){
	std::cout<< "Writing Individual Clusters Data file........" << std::endl;
	std::vector<File*> fileHandles;

	//Inserting respective file objects
	for(int i = 0  ; i < (NewCluster::fClusterNum+1) ; i++){
		std::string fileName = "Cluster_"+std::to_string(i)+".txt";
		File *fileObject = new File(fileName);
		fileHandles.push_back(fileObject);
	}

	//Inserting data to respective file
	for(int i = 0 ; i < fPtVect.size() ; i++){
		if(fPtVect[i]->fClusterNum >= 0){
		Vector3D<double> pt = fPtVect[i]->fPt;
		(fileHandles[fPtVect[i]->fClusterNum])->fHandle << pt.x() << " " << pt.y() << " " << pt.z() << " " << pt.GetColor() << std::endl;
		}

	}


	//Closing all the files
	for(int i = 0  ; i < (NewCluster::fClusterNum+1) ; i++){

		(fileHandles[i]->fHandle).close();
	}
	std::cout<<"All Data files written !!!" << std::endl;
}

void Clustering::FillHistogram(){
	//TFile clusterFile("clusterCount.root","recreate");
	fHist1DOfClusterNum = new TH1F("Cluster_1d_Hist","Cluster_1d_Hist",(NewCluster::fClusterNum),0,(NewCluster::fClusterNum));
	fHist1DOfScatterigInClusterNum = new TH1F("Scattering_Cluster_1d_Hist","Scattering_Cluster_1d_Hist",(NewCluster::fClusterNum),0,(NewCluster::fClusterNum));
	for(int i = 0 ; i < fPtVect.size() ; i++){
		fHist1DOfClusterNum->Fill(fPtVect[i]->fClusterNum);
		//fHist1DOfClusterNum->Fill(fPtVect[i]->fClusterNum,500000);
		fHist1DOfScatterigInClusterNum->Fill(fPtVect[i]->fClusterNum, (fPtVect[i]->fPt).GetColor());
	}

	ExportClustersData();
	//This will give the MEAN of Scattering angle of points within the cluster.
	fHist1DOfScatterigInClusterNum->Divide(fHist1DOfClusterNum);

	//One should also try MEDIAN and SD, and these can be calculated very easily in R



/*
	for(int i = 0  ; i < (NewCluster::fClusterNum+1) ; i++){
		std::cout << "Bin Content : " << fHist1DOfScatterigInClusterNum->GetBinContent(i) << std::endl;
	}
*/

	//fHist1DOfClusterNum->Write();
	//fHist1DOfScatterigInClusterNum->Write();
	//clusterFile.Write();
//	/return;

}

void Clustering::NormalizeScatteringValue(){
	std::cout<<"----------- Entered NOrmalizedScatteringValue --------------" << std::endl;
	for(int i = 0  ; i < (NewCluster::fClusterNum+1) ; i++){
			std::cout << "Bin Content : " << fHist1DOfScatterigInClusterNum->GetBinContent(i) << std::endl;
	}
	for(int i = 0 ; i < fPtVect.size() ; i++){
		int clusterNum = fPtVect[i]->fClusterNum ;
		if(clusterNum >= 0){
		//std::cout << "ClusterNum : "<< clusterNum << std::endl;
		//std::cout << fHist1DOfScatterigInClusterNum->GetBinContent(clusterNum) << std::endl;
			//std::cout<<"Color : "<< fHist1DOfScatterigInClusterNum->GetBinContent(clusterNum) << std::endl;
			(fPtVect[i]->fPt).SetColor(fHist1DOfScatterigInClusterNum->GetBinContent(clusterNum));
		}
		//(fPtVect[i]->fPt).SetColor(fHist1DOfScatterigInClusterNum->GetBinContent(fPtVect[i]->fClusterNum + 1));
	}
}

Clustering::Clustering(std::vector<Vec_t> ptVect):fEpsilon(1.1),fMinPtsInCluster(100){
	//Call the required Clustering Algorithm, using DBSCAN by default
	//DBSCAN(ptVect);
	SequentialClustering(ptVect);
}

Clustering::Clustering(std::vector<Vec_t> ptVect, double eps):fEpsilon(eps),fMinPtsInCluster(20){
	//DBSCAN(ptVect);
	//fClusterRootFile = new TFile("clusterCount.root","recreate");
	for(int i=0 ; i <ptVect.size() ; i++){
		fPtVect.push_back(new Point(ptVect[i])); //Filling vector of Points
	}
	//SequentialClustering(ptVect); //currently calling SequentialClustering with vector of Vec_t
	DBSCAN(); //trying DBSCAN algo
}

void Clustering::WriteFile(){
	TFile clusterFile("clusterCount.root","recreate");
	fHist1DOfClusterNum->Write();
	fHist1DOfScatterigInClusterNum->Write();
	clusterFile.Write();


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

void Clustering::WriteClusterToFile(std::string fileName){
	std::ofstream clusterFile;
	clusterFile.open(fileName);
	for(int i=0 ; i < fPtVect.size() ; i++){
		if(fPtVect[i]->fClusterNum >= 0){
			Vec_t pt3d = fPtVect[i]->fPt;
			//clusterFile << pt3d.x() <<" " << pt3d.y() <<" " << pt3d.z() <<" " << (fPtVect[i]->fClusterNum + 1) << std::endl;
			clusterFile << pt3d.x() <<" " << pt3d.y() <<" " << pt3d.z() <<" " << pt3d.GetColor() << std::endl;
		}
	}
	clusterFile.close();
	WriteFile();
}


void Clustering::DBSCAN(/*std::vector<Vec_t> ptVect*/){
	/* For DBSCAN, We need following Subroutines
	 * FindNeighbors() that will return a vector of all the  neighbors of the given point
	 * AddClusters() that will concatenate two clusters elements
	 * ExpandCluster() that will depend on some logic call the AddClusters() method.
	 *
	 */

	for(int i = 0 ; i < fPtVect.size() ; i++){
		if(fPtVect[i]->fVisited)
			continue;
		fPtVect[i]->fVisited = true;
		Neighbors neighbors = FindNeighbors(fPtVect[i]);
		if(neighbors.size() < fMinPtsInCluster)
			fPtVect[i]->fClusterNum = -2;
		else{
			NewCluster C;
			ExpandCluster(fPtVect[i],neighbors,C);
		}

	}

	for(int i=0; i < fPtVect.size() ; i++){
		//std::cout<<"ClusterNum : " << fPtVect[i]->fClusterNum << std::endl;
	}
	std::cout<<"========================================================" << std::endl;
	std::cout<<"Total Number of Clusters Detected using DBSCAN : " << (NewCluster::fClusterNum+1) << std::endl;
	std::cout<<"========================================================" << std::endl;

	std::cout<<"Writing clusters to file ...." << std::endl;
	FillHistogram();
	NormalizeScatteringValue();
	WriteClusterToFile();
	RemoveNoisyCluster();
}

Cluster Clustering::AddClusters(Cluster c1, Cluster c2){

	Cluster total = c1;
	for(int i = 0 ; i < c2.size() ; i++){
		total.push_back(c2[i]);
	}
}

Neighbors Clustering::FindNeighbors(Point *pt){
	Neighbors neighborsVect;
	for(int i = 0 ; i <  fPtVect.size(); i++){
		if(Vector3D<double>::Distance(pt->fPt,fPtVect[i]->fPt) <= fEpsilon){
			neighborsVect.push_back(fPtVect[i]);
		}
	}
	return neighborsVect;
}

void Clustering::ExpandCluster(Point *pt,Neighbors &neighborPts,NewCluster &C){

	pt->fClusterNum = C.fClusterNum;
	C.fCluster.push_back(pt);
	for(int i=0 ; i < neighborPts.size() ; i++){
		if(!neighborPts[i]->fVisited){
			neighborPts[i]->fVisited = true;
			Neighbors newNeighbors= FindNeighbors(neighborPts[i]);
			if(newNeighbors.size() >= fMinPtsInCluster){
				for(int j = 0  ; j < newNeighbors.size() ; j++){
					neighborPts.push_back(newNeighbors[j]);
				}
			}
		}
		if(neighborPts[i]->fClusterNum==-1){
			//Add it to cluster C
			neighborPts[i]->fClusterNum = C.fClusterNum;
			C.fCluster.push_back(neighborPts[i]);
		}
	}
}


void Clustering::SequentialClustering(std::vector<Vec_t> ptVect){
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
