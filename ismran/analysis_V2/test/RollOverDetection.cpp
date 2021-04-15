/*
 * TestMuonTrackTimeEstimation.cpp
 *
 *  Created on: 01-Mar-2021
 *      Author: rsehgal
 */

#include "PairFinder.h"
#include "Analyzer.h"
#include "Calibration.h"
#include <TApplication.h>
#include <TCanvas.h>
#include "Plotter.h"
#include "includes.hh"
#include "HardwareNomenclature.h"
#include <TGraph.h>
#include <fstream>


int main(int argc, char *argv[]){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	lite_interface::Analyzer analyzerObj(argv[1]);
	std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec = analyzerObj.GetVectorOfScintillators();
	std::vector<ULong64_t> tNearVec;
	std::vector<ULong64_t> tFarVec;
	std::vector<ULong64_t> tWallVec;
	std::vector<ULong64_t> xVec;

	std::ofstream outfile(("outfile_"+std::string(argv[1])+".txt"));

	//TH1F *histDelTNear = new TH1F("DeltTNearHist","DeltTNearHist",1000000000,-1000000000,1000000000);

	for(unsigned int i = 0 ; i < scintBarVec.size()-1 ; i++){
		//xVec.push_back(i);
		/*tNearVec.push_back(scintBarVec[i]->GetTNear());
		tFarVec.push_back(scintBarVec[i]->GetTFar());*/
		//tWallVec.push_back();
		outfile << i <<"," << scintBarVec[i]->GetTNear()<<"," << scintBarVec[i]->GetTFar() << std::endl;
		//histDelTNear->Fill(scintBarVec[i]->GetTNear()-scintBarVec[i+1]->GetTNear());
	}
	//ULong64_t timeWindow = GetMuonTrackTimeWindow(scintBarVec);
	//TGraph *gr = new TGraph( xVec.size(), &xVec[0], &tNearVec[0] );


	outfile.close();
	std::cout << "File Written and closed......" << std::endl;

	//new TCanvas();
	//histDelTNear->Draw();

	//new TCanvas("Can_TNear","Can_TNear");
	//gr->Draw("ap");
	fApp->Run();


	return 0;

}
