/*
 * PlotAngularDeviation.cpp
 *
 *  Created on: 13-Nov-2018
 *      Author: rsehgal
 */


#include <TH1F.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <TApplication.h>
#include "CommonFunc.h"

int main(){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	std::ifstream infile("StatsFromEventAction.txt");
	double incoming = 0., outgoing = 0., diff = 0., incomingFitted = 0., outgoingFitted = 0., diffFitted = 0.;
	double incomingSampledFited = 0., outgoingSampledFited = 0., diffSampledFitted = 0., energy = 0.;
	int nbinsx = 100;
	int xlow = -10.;
	int xhigh = 10. ;
	double epsilon = 1e-4;
	TH1F *deviationHist = new TH1F("deviation","deviation",nbinsx,xlow,xhigh);
	std::vector<double> scatteringAngleVector;
	while(!infile.eof()){
		infile >> incoming >> outgoing >> diff >> incomingFitted >> outgoingFitted >> diffFitted >> incomingSampledFited >> outgoingSampledFited >> diffSampledFitted >> energy ;
		//if(std::fabs(diff) > epsilon)
		{
			deviationHist->Fill(diff*1000);
			scatteringAngleVector.push_back(diff);
		}
	}

	std::ofstream outfile("yld.txt");
	for(int i = 1 ; i < nbinsx ; i++){
		outfile << deviationHist->GetBinCenter(i) << " " << deviationHist->GetBinContent(i) << std::endl ;
	}

	std::cout << "ROOT SD : " << deviationHist->GetStdDev() << std::endl;
	std::cout << "ROOT RMS : " << deviationHist->GetRMS() << std::endl;


	double rl = CommonFunc::Functions::instance()->RadiationLength(scatteringAngleVector,200);
	std::cout << "RadiationLength : " << rl << std::endl;

	outfile.close();
	infile.close();
	deviationHist->Draw();
	fApp->Run();
}

