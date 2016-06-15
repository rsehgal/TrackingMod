
#ifndef TEMPLATIZEDINTERFACE_INC_TREE_H_
#define TEMPLATIZEDINTERFACE_INC_TREE_H_
#include <TTree.h>
#include <vector>
#include <algorithm>
#include "base/Global.h"
#include <string>
#include <TObjArray.h>
#include <iostream>
namespace Tracking{

class Tree{
	
	private:
		//static
	    static Tree *s_instance;
		int numOfEvents;
		//Channel ch;
		Channel ch;
		std::string rootFile;
		//std::string sRootFile;
		TTree *t;
		//TTree t("testTree","A Tree with STL vectors");
		TFile *f;
		std::string fTreeName;
		Tree();
	    Tree(std::string rootFl);
		Tree(std::string rootFl, std::string treeName="testTree",int rw=0);

	public:
		//Tree();
		//Tree(std::string rootFl);
		//Tree(std::string rootFl, std::string treeName="testTree",int rw=0);
		~Tree();
		void ReadTree(std::string rootFl, std::string treeName="testTree",int rw=0);
		void TreeW();
		void TreeW(std::string branchName, Channel ch);
		void TreeR();
		//static 
		int GetNumOfEvents(){return numOfEvents;}
		void TreeR_V2(std::string bName, int entry);
		Channel* GetEntry(std::string bName, int evNo);
		void WriteToFile();
		Tree* GetTree(){return this;}
		static Tree *instance();
		void SetTreeDefaultParameters(){
			fTreeName = "BSC_DATA_TREE";
			rootFile = "hello.root";
		}
		void SetTreeName(std::string treeName){fTreeName = treeName;}
		

		//Some statistical function for Generating Histograms
		void CreateHistogram(std::string bName);
		void PrintBranchNames(){
			 TObjArray* listOfBranches = t->GetListOfBranches();
			 TIter iObj(listOfBranches);
			 while (TObject* obj = iObj()) {
				 std::cout<< obj->GetName() << std::endl;
			 }
		}


};

} /* end of Tomography namespace*/

#endif /* TREEEXERCISE_TREE_H_ */

