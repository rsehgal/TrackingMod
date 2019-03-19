/*
 * TreeSingleTon.h
 *
 *  Created on: 19-Mar-2019
 *      Author: rsehgal
 */

#ifndef HELPERS_TREESINGLETON_H_
#define HELPERS_TREESINGLETON_H_
#include "ROOTTree.h"
#include <vector>
#include <TFile.h>

namespace Tracking {

class TreeSingleTon {
	TFile  *fROOTFile;
	std::string fROOTFileName;
	std::vector<ROOTTree*> fROOTTreeVect;

	static TreeSingleTon *s_instance;

	TreeSingleTon();
	TreeSingleTon(std::string rootFileName);
public:

	static TreeSingleTon* instance();
	/*{
	    if (!s_instance)
	      s_instance = new TreeSingleTon;
	    return s_instance;
	}*/

	static TreeSingleTon* Create(std::string rootFileName);
	/*{
		if (!s_instance)
			s_instance = new TreeSingleTon(rootFileName);
		return s_instance;
	}*/

	virtual ~TreeSingleTon();

	void InsertTree(ROOTTree *tree){
		fROOTTreeVect.push_back(tree);
	}

	void WriteToFile(){
		fROOTFile->Write();
	}
};

} /* namespace Tracking */

#endif /* HELPERS_TREESINGLETON_H_ */
