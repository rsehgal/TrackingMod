/*
 * PlotDoCA.cpp
 *
 *  Created on: 21-Jun-2019
 *      Author: rsehgal
 */

#include <TH1F.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <TApplication.h>
#include "CommonFunc.h"

int main(int argc, char *argv[]){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	std::string filename = argv[1];
	std::ifstream infile(filename);
	std::ofstream outfile("filteredByDoca.txt");
	double x =0.,y=0.,z=0.,col=0.,doca=0.;

	int nbinsx = 500;
	int xlow = -1.;
	int xhigh = 10. ;

	TH1F *docaHist = new TH1F("DistanceOfClosestApproach","Distance Of Closest Approach",nbinsx,xlow,xhigh);

	int zeroDocaCounter = 0;
	double epsilon = 1e-4;
	while(!infile.eof()){
		infile >> x >> y >> z >> col >> doca ;
		if(doca < epsilon){
			zeroDocaCounter++;
		}else{
			if(doca >5.){
				outfile << x << " " << y << " " << z << " " << col << " " << doca << std::endl;
			}
			docaHist->Fill(doca);
		}



	}

	std::cout <<"Almost Zero DoCA counter : " << zeroDocaCounter << std::endl;
	std::cout << "ROOT SD : " << docaHist->GetStdDev() << std::endl;
	std::cout << "ROOT RMS : " << docaHist->GetRMS() << std::endl;


	infile.close();
	outfile.close();
	docaHist->Draw();
	fApp->Run();
}




