
#ifndef TEMPLATIZEDINTERFACE_INC_TREE_H_
#define TEMPLATIZEDINTERFACE_INC_TREE_H_
#include <TTree.h>
#include <vector>
#include <algorithm>
#include "base/Global.h"
#include <string>
#include <TObjArray.h>
#include <iostream>
#include <map>
#include <TFile.h>
namespace Tracking{

struct Branch{
	std::string sBranchName;
	Channel *vpx;
	TBranch *bvpx;
	Long64_t tentry;
};

class Tree{
	
	private:
	    static Tree *s_instance;
		int numOfEvents;
		Channel ch;
		std::string rootFile;
		TTree *t;
		TFile *f;
		std::string fTreeName;
		Tree();
	    Tree(std::string rootFl);
		Tree(std::string rootFl, std::string treeName="testTree",int rw=0);
		std::vector<std::string> fBranchNamesVector;
		std::map<std::string,Branch> fBranchMap;

	public:
		//Tree();
		//Tree(std::string rootFl);
		//Tree(std::string rootFl, std::string treeName="testTree",int rw=0);
		~Tree();
		void ReadTree(std::string rootFl, std::string treeName="testTree",int rw=0);
		void TreeW(); // This guy is currently used only for testing.

		//Trying to templatized it so that it can be used to store single branch of any type
		template<typename T>
		void TreeW(std::string branchName, T ch);

		template<typename T>
		void CreateBranch(std::string branchName, T &br);

		//Create branches from STL vector of Type T
		template<typename T>
		void CreateBranches(std::string branchName, std::vector<T> &br);

		void Fill() { t->Fill(); }

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

		void InitializeTreeForWriting(){
			f = TFile::Open(rootFile.c_str(), "RECREATE");
			t = new TTree(fTreeName.c_str(),"A Tree with STL vectors");

		}

		void SetTreeName(std::string treeName){fTreeName = treeName;}
		

		//Some statistical function for Generating Histograms
		void CreateHistogram(std::string bName);
		void FillBranchNamesVector();
		std::vector<std::string> GetBranchNamesVector(){return fBranchNamesVector;}
		void PrintBranchNames();
		void Initialize();

		std::string GetRootFileName() const {return rootFile;}
		std::string GetRunNumber(){
			size_t lastindex = rootFile.find_last_of(".");
			std::string rawname = rootFile.substr(0, lastindex);
			return rawname;
		}


};

template<typename T>
void Tree::CreateBranch(std::string branchName, T &br){
	t->Branch(branchName.c_str(),&br);
}

template<typename T>
void Tree::CreateBranches(std::string branchName, std::vector<T> &br){
	for(int i = 0 ; i < br.size() ; i++){
		t->Branch(branchName.c_str(),&br[i]);
	}
}




template<typename T>
void Tree::TreeW(std::string branchName, T ch){


}

} /* end of Tomography namespace*/

#endif /* TREEEXERCISE_TREE_H_ */

