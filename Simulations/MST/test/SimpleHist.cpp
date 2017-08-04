/*
 * SimpleHist.cpp
 *
 *  Created on: Aug 3, 2017
 *      Author: rsehgal
 */
#include <TH1F.h>
#include <iostream>
#include <fstream>
#include <TCanvas.h>
#include <TApplication.h>
#include <vector>
#include <algorithm>
#include <cstring>
#include <sstream>
#include "Database.h"
int main(int argc, char *argv[]){

    TApplication *fApp = new TApplication("Test", NULL, NULL);
	TCanvas *c = new TCanvas("TestCanvas", "ScatteringAngleCanvas", 800, 600);

	int nxbins = 1000;
	TH1F *hist = new TH1F("Data", "Data", nxbins, -10, 450);
	std::ifstream runfile ("run.txt");
	std::string material="";
	double energy = 0.;
	double scatteringAngle = 0.;
	int thickness = 0;
	int numOfEvents = std::atoi(argv[1]);
	runfile >> energy >> material >> thickness;
	std::stringstream ss;
	ss << energy/1000.;

	while(numOfEvents){
		runfile >> scatteringAngle;
		hist->Fill(scatteringAngle*1000.);
		std::cout << "ScatterinAngle : " << scatteringAngle << std::endl;
		numOfEvents--;
	}

	hist->Draw();
	fApp->Run();

}



