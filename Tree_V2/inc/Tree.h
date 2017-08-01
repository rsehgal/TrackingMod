
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

template<typename T>
struct Branch{
	std::string sBranchName;
	T *vpx;
	TBranch *bvpx;
	Long64_t tentry;
};

template<typename T>
class Tree{
	
	private:
	    static Tree *s_instance;
		int numOfEvents;
		T ch;
		std::string rootFile;
		TTree *t;
		TFile *f;
		std::string fTreeName;
		Tree();
	    Tree(std::string rootFl);
		Tree(std::string rootFl, std::string treeName="testTree",int rw=0);
		std::vector<std::string> fBranchNamesVector;
		std::map<std::string,Branch<T>> fBranchMap;

	public:
		~Tree();
		void CreateBranch(std::string branchName, T &br);
		void Fill() { t->Fill(); }
		void TreeR();
		void TreeW();
		void ReadTree(std::string rootFl, std::string treeName="testTree",int rw=0);
		int GetNumOfEvents(){return numOfEvents;}
		T GetEntry(std::string bName,int evNo);
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
		//void CreateHistogram(std::string bName);
		void FillBranchNamesVector();
		std::vector<std::string> GetBranchNamesVector(){return fBranchNamesVector;}
		void PrintBranchNames();
		void Initialize();


};

template<typename T>
Tree<T> *Tree<T>::s_instance = 0;

template<typename T>
Tree<T>* Tree<T>::instance() {
        if (!s_instance)
          s_instance = new Tree;
        return s_instance;
    }

template<typename T>
Tree<T>::Tree(){
	numOfEvents = 100;
	rootFile = "test.root";
	//sRootFile = rootFile;
}

template<typename T>
Tree<T>::Tree(std::string rootFl){

  rootFile = rootFl;
  f = TFile::Open(rootFile.c_str(),"READ");
  if (!f) { return; }
  f->GetObject("BSC_DATA_TREE",t);
  numOfEvents = t->GetEntries();
}

template<typename T>
Tree<T>::Tree(std::string rootFl, std::string treeName, int rw){

  rootFile = rootFl;
  fTreeName = treeName;
  if (rw)
    f = TFile::Open(rootFile.c_str(), "RECREATE");
  else {
    f = TFile::Open(rootFile.c_str(), "READ");
    if (!f) {
      return;
    }
    f->GetObject(treeName.c_str(), t);
    numOfEvents = t->GetEntries();
  }
}

template<typename T>
void Tree<T>::Initialize(){
	for(int i = 0 ; i < fBranchNamesVector.size() ; i++){
		// t->SetBranchAddress(bName.c_str(),&vpx,&bvpx);
		std::string bName = fBranchNamesVector[i];
		std::size_t pos = 0;
		pos = bName.find("NUMBER_OF_CHANNELS");
		//std::cout<<"Position : " << pos << std::endl;
		if(!pos || pos > 20)
		t->SetBranchAddress(fBranchNamesVector[i].c_str(),&fBranchMap[bName].vpx,&fBranchMap[bName].bvpx);
	}
}

template<typename T>
Tree<T>::~Tree(){ f->Close(); }

template<typename T>
void Tree<T>::TreeW(){
    //Using modified tree
    InitializeTreeForWriting();
    T ch;
    CreateBranch("channel",ch);
    numOfEvents =10;
        for(int i= 0 ; i < numOfEvents ; i++){
        	ch.clear(); // clearing the vector
        	ch.push_back(1);
        	ch.push_back(8);
       		ch.push_back(3);
       		Fill();
        }
     WriteToFile();
}

template<typename T>
void Tree<T>::ReadTree(std::string rootFl, std::string treeName, int rw){
	rootFile = rootFl;
	  fTreeName = treeName;
	  //t = new TTree(treeName.c_str(),"A Tree with STL vectors");
	  if (rw)
	    f = TFile::Open(rootFile.c_str(), "RECREATE");
	  else {
	    f = TFile::Open(rootFile.c_str(), "READ");
	    if (!f) {
	      return;
	    }
	    f->GetObject(treeName.c_str(), t);
	    numOfEvents = t->GetEntries();
	  }
	  FillBranchNamesVector();
	  Initialize();
}

template<typename T>
void Tree<T>::WriteToFile(){
	f->Write();
}

template<typename T>
void Tree<T>::CreateBranch(std::string branchName, T &br){
	t->Branch(branchName.c_str(),&br);
}

template<typename T>
T Tree<T>::GetEntry(std::string bName,int evNo){
	fBranchMap[bName].tentry = t->LoadTree(evNo);
	fBranchMap[bName].bvpx->GetEntry(fBranchMap[bName].tentry);
	return fBranchMap[bName].vpx;
}

template<typename T>
void Tree<T>::FillBranchNamesVector(){
  TObjArray *listOfBranches = t->GetListOfBranches();
  TIter iObj(listOfBranches);
  while (TObject *obj = iObj()) {
	  std::string name = (std::string)obj->GetName();
      fBranchNamesVector.push_back(name);
  }
}

template<typename T>
void Tree<T>::PrintBranchNames() {
  for (int i = 0; i < fBranchNamesVector.size(); i++) {
    std::cout << fBranchNamesVector[i] << std::endl;
  }
}


} /* end of Tomography namespace*/

#endif /* TREEEXERCISE_TREE_H_ */

