/*
 * ROOTTree.cpp
 *
 *  Created on: 19-Mar-2019
 *      Author: rsehgal
 */

#if(1)
#include "ROOTTree.h"

namespace Tracking {

ROOTTree::ROOTTree() {
	// TODO Auto-generated constructor stub

}


ROOTTree::~ROOTTree() {
	// TODO Auto-generated destructor stub
}


ROOTTree::ROOTTree(std::string treename,std::string briefDesc):fTreeName(treename){
	fTree = new TTree(fTreeName.c_str(),briefDesc.c_str());

}



} /* namespace Tracking */
#endif
