/*
 * TreeSingleTon.cpp
 *
 *  Created on: 19-Mar-2019
 *      Author: rsehgal
 */

#include "TreeSingleTon.h"

namespace Tracking {

TreeSingleTon *TreeSingleTon::s_instance = 0;

TreeSingleTon* TreeSingleTon::instance() {
	    if (!s_instance)
	      s_instance = new TreeSingleTon;
	    return s_instance;
}

TreeSingleTon* TreeSingleTon::Create(std::string rootFileName) {
		if (!s_instance)
			s_instance = new TreeSingleTon(rootFileName);
		return s_instance;
}

TreeSingleTon::TreeSingleTon() {
	// TODO Auto-generated constructor stub

}

TreeSingleTon::~TreeSingleTon() {
	// TODO Auto-generated destructor stub
}

TreeSingleTon::TreeSingleTon(std::string rootFileName){
	fROOTFile = TFile::Open(rootFileName.c_str(), "RECREATE");
}

ROOTTree* TreeSingleTon::GetROOTTree(std::string treename){
	for(unsigned int i = 0 ; i < fROOTTreeVect.size() ; i++){
		if(fROOTTreeVect[i]->GetTreeName() == treename)
			return fROOTTreeVect[i];
	}
}
} /* namespace Tracking */
