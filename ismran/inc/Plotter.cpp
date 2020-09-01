/*
 * Plotter.cpp
 *
 *  Created on: 01-Sep-2020
 *      Author: rsehgal
 */


#include "Plotter.h"
#include "TreeEntry.h"

void PlotMuonTrack(std::vector<ScintillatorBar*> singleMuonTrack){
	std::vector<Double_t> xvec,zvec;
	for(unsigned int index = 0 ; index < singleMuonTrack.size() ; index++){
		xvec.push_back((singleMuonTrack[index]->hitPosition).x);
		zvec.push_back((singleMuonTrack[index]->hitPosition).z);
	}
	TGraph *gr = new TGraph(xvec.size(),&zvec[0],&xvec[0]);
	gr->SetMarkerStyle(8);
	gr->SetTitle("Muon Hit Points in different layers");
	new TCanvas();
	gr->Draw("ap");
	gPad->RedrawAxis("g");
}

void PlotNMuonTrack(std::vector<std::vector<ScintillatorBar*>> muonTrackVec, unsigned int numOfMuons){

	std::cout << "===== Muon Track Vector Size : " << muonTrackVec.size() <<" : numOfMuons : " << numOfMuons << " : NumOfLayers : " << numOfLayers << std::endl;

	if(muonTrackVec.size()>=numOfMuons){
		/*for(unsigned int index = 0 ; index < numOfMuons ; index++){
			PlotMuonTrack(muonTrackVec[index]);
		}*/

		unsigned int i = 0 ;
		unsigned int n = numOfMuons;
		while(n){
			//std::cout << "====================== Before Plotting Track with index : " << i << " ================= " << std::endl;
			if (muonTrackVec[i].size() >= numOfLayers) {
				std::cout << "====================== Plotting Track with index : " << i << " ================= " << std::endl;
				PrintMuonTrack(muonTrackVec[i]);
				n--;
				PlotMuonTrack(muonTrackVec[i]);

			}
			i++;
		}
	}


}
