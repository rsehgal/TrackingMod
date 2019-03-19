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
	TFile *f = TFile::Open("testLocalRootTree.root", "RECREATE");
	//Tracking::TreeSingleTon::Create("testRootTree.root");

/*	{//First Tree
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
	}*/

	{//TMVA Tree
		Tracking::ROOTTree *tr = new Tracking::ROOTTree("Signal");
		double deltaTheta;
		double deltaThetaX;
		double deltaThetaY;
		double deltaX;
		double deltaY;
		tr->CreateBranch("deltaTheta", deltaTheta);
		tr->CreateBranch("deltaThetaX", deltaThetaX);
		tr->CreateBranch("deltaThetaY", deltaThetaY);
		tr->CreateBranch("deltaX", deltaX);
		tr->CreateBranch("deltaY", deltaY);
		tr->InitializeTree();

		//Event loop
		for(int i = 0 ; i < 10 ; i++){
			tr->FillBranch("deltaTheta",i+1.);
			tr->Fill();
		}
		//


	}

	{//TMVA Tree
		Tracking::ROOTTree *tr = new Tracking::ROOTTree("Background");
		double deltaTheta;
		double deltaThetaX;
		double deltaThetaY;
		double deltaX;
		double deltaY;
		tr->CreateBranch("deltaTheta", deltaTheta);
		tr->CreateBranch("deltaThetaX", deltaThetaX);
		tr->CreateBranch("deltaThetaY", deltaThetaY);
		tr->CreateBranch("deltaX", deltaX);
		tr->CreateBranch("deltaY", deltaY);
		tr->InitializeTree();

		tr->Fill();
	}
	//Tracking::TreeSingleTon::instance()->WriteToFile();
	f->Write();

	{
		Tracking::TreeSingleTon::Create("testSingleTonRootTree.root");

		Tracking::TreeSingleTon* singleTonObject = Tracking::TreeSingleTon::instance();

		{//TMVA Tree
			Tracking::ROOTTree *tr = new Tracking::ROOTTree("Signal");
			double deltaTheta;
			double deltaThetaX;
			double deltaThetaY;
			double deltaX;
			double deltaY;
			tr->CreateBranch("deltaTheta", deltaTheta);
			tr->CreateBranch("deltaThetaX", deltaThetaX);
			tr->CreateBranch("deltaThetaY", deltaThetaY);
			tr->CreateBranch("deltaX", deltaX);
			tr->CreateBranch("deltaY", deltaY);
			tr->InitializeTree();
			singleTonObject->InsertTree(tr);

			//Event loop
			/*for(int i = 0 ; i < 10 ; i++){
				tr->FillBranch("deltaTheta",i+1.);
				tr->Fill();
			}*/
			//


		}


		{//TMVA Tree
			Tracking::ROOTTree *tr = new Tracking::ROOTTree("Background");
			double deltaTheta;
			double deltaThetaX;
			double deltaThetaY;
			double deltaX;
			double deltaY;
			tr->CreateBranch("deltaTheta", deltaTheta);
			tr->CreateBranch("deltaThetaX", deltaThetaX);
			tr->CreateBranch("deltaThetaY", deltaThetaY);
			tr->CreateBranch("deltaX", deltaX);
			tr->CreateBranch("deltaY", deltaY);
			tr->InitializeTree();
			singleTonObject->InsertTree(tr);
		}

		{
			//EventLoop
			for(int i = 0 ; i < 10 ; i++){
				Tracking::ROOTTree *signal = singleTonObject->GetROOTTree("Signal");
				signal->FillBranch("deltaThetaX", i+1);
				signal->Fill();
				Tracking::ROOTTree *background = singleTonObject->GetROOTTree("Background");
				background->FillBranch("deltaThetaY", i+3);
				background->Fill();
			}

		}


	//	Tracking::TreeSingleTon::instance()->WriteToFile();
		//singleTonObject->SelfDestruct();
		//delete singleTonObject;
	}

	{

		Tracking::TreeSingleTon* singleTonObject = Tracking::TreeSingleTon::instance();

		{//TMVA Tree
			Tracking::ROOTTree *tr = new Tracking::ROOTTree("TMVASignal");
			double deltaTheta;
			double deltaThetaX;
			double deltaThetaY;
			double deltaX;
			double deltaY;
			tr->CreateBranch("deltaTheta", deltaTheta);
			tr->CreateBranch("deltaThetaX", deltaThetaX);
			tr->CreateBranch("deltaThetaY", deltaThetaY);
			tr->CreateBranch("deltaX", deltaX);
			tr->CreateBranch("deltaY", deltaY);
			tr->InitializeTree();
			singleTonObject->InsertTree(tr);

			//Event loop
			/*for(int i = 0 ; i < 10 ; i++){
				tr->FillBranch("deltaTheta",i+1.);
				tr->Fill();
			}*/
			//


		}


/*
		{//TMVA Tree
			Tracking::ROOTTree *tr = new Tracking::ROOTTree("Background");
			double deltaTheta;
			double deltaThetaX;
			double deltaThetaY;
			double deltaX;
			double deltaY;
			tr->CreateBranch("deltaTheta", deltaTheta);
			tr->CreateBranch("deltaThetaX", deltaThetaX);
			tr->CreateBranch("deltaThetaY", deltaThetaY);
			tr->CreateBranch("deltaX", deltaX);
			tr->CreateBranch("deltaY", deltaY);
			tr->InitializeTree();
			singleTonObject->InsertTree(tr);
		}
*/

		{
			//EventLoop
			for(int i = 0 ; i < 10 ; i++){
				Tracking::ROOTTree *signal = singleTonObject->GetROOTTree("TMVASignal");
				signal->FillBranch("deltaThetaX", i+1);
				signal->Fill();
				/*Tracking::ROOTTree *background = singleTonObject->GetROOTTree("Background");
				background->FillBranch("deltaThetaY", i+3);
				background->Fill();*/
			}

		}


		Tracking::TreeSingleTon::instance()->WriteToFile();
	}

}


