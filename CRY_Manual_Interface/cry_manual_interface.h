
#ifndef CRY_MANUAL_INTERFACE_CRY_MANUAL_INTERFACE_CPP_
#define CRY_MANUAL_INTERFACE_CRY_MANUAL_INTERFACE_CPP_


#include "CRYGenerator.h"
#include "CRYSetup.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>  // For Ubuntu Linux

#include <TH1F.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TTree.h>
#include <TFile.h>

struct Muon{
	double angleX, angleY, angleZ;
	double energy;

	Muon(){
		angleX = 0.;
		angleY = 0.;
		angleZ = 0.;
		energy = 0.;

	}

	Muon(Muon &m){
		angleX = m.angleX;
		angleY = m.angleY;
		angleZ = m.angleZ;
		energy = m.energy;

	}

	Muon(double anglex, double angley, double anglez, double ener){
		angleX = anglex;
		angleY = angley;
		angleZ = anglez;
		energy = ener;

	}

	void Print(){
		std::cout << "(" << angleX << " , " << angleY << " , " << angleZ << ") : Energy : " << energy << std::endl;
	}

};

extern TH1F* GetDistribution(std::vector<Muon*> muonVec);
extern void Save(std::vector<Muon*> muonVec);
extern std::vector<Muon*> SampleMuonParticle(int n);


#endif /* CRY_MANUAL_INTERFACE_CRY_MANUAL_INTERFACE_CPP_ */