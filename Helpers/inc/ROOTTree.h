/*
 * ROOTTree.h
 *
 *  Created on: 19-Mar-2019
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_ROOTTREE_H_
#define HELPERS_INC_ROOTTREE_H_

#include <TTree.h>

namespace Tracking {

template<typename T>
struct Branch{
	std::string sBranchName;
	T sBranchVal;
	Branch(std::string bname,T val):sBranchName(bname),sBranchVal(val){}
};

class ROOTTree {
	TTree *fTree;
	std::string fTreeName;

	std::vector<Branch<double>> fBranchVect;
public:
	ROOTTree();
	ROOTTree(std::string treename,std::string briefDesc = "A Simple Tree");
	virtual ~ROOTTree();

	std::string GetTreeName() const {return fTreeName;}

	//Functions to create a branch and fill a TTree
	template<typename T>
	void CreateBranch(std::string branchName, T &br);
	void Fill(){fTree->Fill();}
	void InitializeTree(){
				for(unsigned int i = 0 ; i < fBranchVect.size() ; i++){
			fTree->Branch(fBranchVect[i].sBranchName.c_str(),&fBranchVect[i].sBranchVal);
		}
	}

	template<typename T>
	void FillBranch(std::string brName,T val){

		for(int i = 0 ; i < fBranchVect.size() ; i++){
			if(fBranchVect[i].sBranchName == brName){
				fBranchVect[i].sBranchVal = val;
			}
		}
	}
};

template<typename T>
void ROOTTree::CreateBranch(std::string branchName, T &br){
	fBranchVect.push_back(Branch<double>(branchName,br));
	//fTree->Branch(branchName.c_str(),&br);
}


} /* namespace Tracking */

#endif /* HELPERS_INC_ROOTTREE_H_ */
