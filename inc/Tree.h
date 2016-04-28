
#ifndef INC_TREE_H_
#define INC_TREE_H_
#include <TTree.h>
#include <vector>
#include <algorithm>
#include "base/Global.h"
#include <string>
#include <TTree.h>

using namespace Tracking;

class Tree{
	
	private:
		//static
		int numOfEvents;
		//Channel ch;
		Channel ch;
		std::string rootFile;
		TTree *t;
		TFile *f;

	public:
		Tree();
		Tree(std::string rootFl);
		Tree(std::string rootFl, std::string treeName);
		~Tree();
		void TreeW();
		void TreeR();
		//static 
		int GetNumOfEvents(){return numOfEvents;}
		void TreeR_V2(std::string bName, int entry);
		Channel* GetEntry(std::string bName, int evNo);
		

		//Some statistical function for Generating Histograms
		void CreateHistogram(std::string bName);


};
#endif /* TREEEXERCISE_TREE_H_ */
