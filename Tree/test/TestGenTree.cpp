/*
 * TestGenTree.cpp
 *
 *  Created on: Jun 28, 2017
 *      Author: rsehgal
 */

#include "Tree.h"
#include "Global.h"
#include "Vector3D.h"
#include "Track.h"
#include <iostream>

int main() {

	  Tracking::Tree *tree = Tracking::Tree::instance();
	  tree->SetTreeDefaultParameters();
	  tree->InitializeTreeForWriting();
	  Tracking::Vector3D<double> p1,p2;
	  Tomography::Track t(p1,p2);
	  tree->CreateBranch<Tracking::Vector3D<double>>("P1", p1);
	  tree->CreateBranch<Tracking::Vector3D<double>>("P2", p2);
	  tree->CreateBranch<Tomography::Track>("Track",t);
	  tree->Fill();
	  tree->WriteToFile();

	  return 0;
}




