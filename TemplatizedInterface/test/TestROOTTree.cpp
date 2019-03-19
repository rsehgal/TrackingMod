/*
 * TestROOTTree.cpp
 *
 *  Created on: 19-Mar-2019
 *      Author: rsehgal
 */

#include "ROOTTree.h"
#include "TreeSingleTon.h"
#include <TFile.h>

int main(){
	int numOfEvents = 10;
	//TFile *f = TFile::Open("testRootTree.root", "RECREATE");
	Tracking::TreeSingleTon::Create("testRootTree.root");

	{//First Tree
	Tracking::ROOTTree *tr = new Tracking::ROOTTree("FirstTree");
	int ch;
	tr->CreateBranch<int>("channel", ch);
	double doubleVal;
	tr->CreateBranch("doubleDatga", doubleVal);

	for (int i = 0; i < numOfEvents; i++) {
		ch = i;
		doubleVal = (5.0 * i + 8.9) / 2.3;
		tr->Fill();
	}
	}

	{//Second Tree
	Tracking::ROOTTree *tr = new Tracking::ROOTTree("SecondTree");
	int ch;
	tr->CreateBranch<int>("channel", ch);
	double doubleVal;
	tr->CreateBranch("doubleDatga", doubleVal);

	for (int i = 0; i < numOfEvents; i++) {
		ch = i;
		doubleVal = (5.0 * i + 8.9) / 2.3;
		tr->Fill();
	}
	}
	Tracking::TreeSingleTon::instance()->WriteToFile();
	//f->Write();
}


