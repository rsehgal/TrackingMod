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

class ROOTTree {
	TTree *fTree;
	std::string fTreeName;
public:
	ROOTTree();
	ROOTTree(std::string treename,std::string briefDesc = "A Simple Tree");
	virtual ~ROOTTree();

	//Functions to create a branch and fill a TTree
	template<typename T>
	void CreateBranch(std::string branchName, T &br);
	void Fill(){fTree->Fill();}
};

template<typename T>
void ROOTTree::CreateBranch(std::string branchName, T &br){
	fTree->Branch(branchName.c_str(),&br);
}


} /* namespace Tracking */

#endif /* HELPERS_INC_ROOTTREE_H_ */
