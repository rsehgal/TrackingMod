/*
 * TestGenTree.cpp
 *
 *  Created on: Jun 28, 2017
 *      Author: rsehgal
 */

#include "Tree.h"
#include "base/Global.h"
#include "base/Vector3D.h"
#include "Track.h"
#include <iostream>

int main() {

	  Tracking::Tree<Tracking::Vector3D<double>> *tree = Tracking::Tree<Tracking::Vector3D<double>>::instance();
	  tree->SetTreeDefaultParameters();
	  tree->InitializeTreeForWriting();
	  Tracking::Vector3D<double> p1(1,2,3),p2(4,5,6);
	  Tomography::Track t(p1,p2);
	  tree->CreateBranch("P1", p1);
	  tree->CreateBranch("P2", p2);
//	  tree->CreateBranch<Tomography::Track>("Track",t);
	  tree->Fill();
	  tree->WriteToFile();

	  return 0;
}




