
#ifndef TEMPLATIZEDINTERFACE_INC_TREE_H_
#define TEMPLATIZEDINTERFACE_INC_TREE_H_
#include <TTree.h>
#include <vector>
#include <algorithm>
#include "base/Global.h"
#include <string>
#include <TTree.h>

namespace Tracking{

class Tree{
	
	private:
		//static
		int numOfEvents;
		//Channel ch;
		Channel ch;
		std::string rootFile;
		TTree *t;
		//TTree t("testTree","A Tree with STL vectors");
		TFile *f;
		std::string fTreeName;

	public:
		Tree();
		Tree(std::string rootFl);
		Tree(std::string rootFl, std::string treeName="testTree",int rw=0);
		~Tree();
		void TreeW();
		void TreeW(std::string branchName, Channel ch);
		void TreeR();
		//static 
		int GetNumOfEvents(){return numOfEvents;}
		void TreeR_V2(std::string bName, int entry);
		Channel* GetEntry(std::string bName, int evNo);
		void WriteToFile();
		

		//Some statistical function for Generating Histograms
		void CreateHistogram(std::string bName);


};

} /* end of Tomography namespace*/

#endif /* TREEEXERCISE_TREE_H_ */

