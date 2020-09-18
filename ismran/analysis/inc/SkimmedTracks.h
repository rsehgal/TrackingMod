/*
 * SkimmedTracks.h
 *
 *  Created on: 18-Sep-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_INC_SKIMMEDTRACKS_H_
#define ISMRAN_ANALYSIS_INC_SKIMMEDTRACKS_H_

#include "PsBar.h"
#include "HelperFunctions.h"
#include <vector>
struct SkimmedMuonTrack{
	std::vector<Point3D*> sRawMuonTrack;
	std::vector<Point3D*> sFittedMuonTrack;
	double sEnergyDeposited;
	double sAverageTimeStamp;
	unsigned long int sMuonTrackNum;

	SkimmedMuonTrack(){}
	SkimmedMuonTrack(std::vector<Point3D*> rawMuonTrack,
					 std::vector<Point3D*> fittedMuonTrack,
					 double energyDepo, double averageTimestamp,
					 unsigned long int trackNum){

		sRawMuonTrack = rawMuonTrack;
		sFittedMuonTrack = fittedMuonTrack;
		sEnergyDeposited = energyDepo;
		sAverageTimeStamp = averageTimestamp;
		sMuonTrackNum = trackNum;
	}

	void Reset(){
		ResetVector<Point3D>(sRawMuonTrack);
		ResetVector<Point3D>(sFittedMuonTrack);
	}

	void PlotTrack(){
		std::vector<Double_t> xRawVec, yRawVec, zRawVec;
		std::vector<Double_t> xFittedVec, yFittedVec, zFittedVec;
		for (unsigned int index = 0; index < sRawMuonTrack.size(); index++) {
			xRawVec.push_back(sRawMuonTrack[index]->x);
			yRawVec.push_back(sRawMuonTrack[index]->y);
			zRawVec.push_back(sRawMuonTrack[index]->z);
		}
		for (unsigned int index = 0; index < sFittedMuonTrack.size(); index++) {
			xFittedVec.push_back(sFittedMuonTrack[index]->x);
			yFittedVec.push_back(sFittedMuonTrack[index]->y);
			zFittedVec.push_back(sFittedMuonTrack[index]->z);
		}

		std::string trackname = "MuonTrack-" + std::to_string(sMuonTrackNum);
		TCanvas *c = new TCanvas(trackname.c_str(), trackname.c_str(), 800, 800);
		c->Divide(2,2);

		c->cd(1);
		TGraph *grxy = new TGraph(xRawVec.size(), &xRawVec[0], &yRawVec[0]);
		PlotFittedLine(grxy);
		grxy->SetMarkerStyle(8);
		DrawGrid("Muon Track in XY plane; X axis ; Y axis", 9, 9);
		grxy->Draw("p");

		c->cd(2);
		TGraph *grzy = new TGraph(zRawVec.size(), &zRawVec[0], &yRawVec[0]);
		PlotFittedLine(grzy);
		grzy->SetMarkerStyle(8);
		DrawGrid("Muon Track in ZY plane; Z axis ; Y axis", 9, 9);
		grzy->Draw("p");

		c->cd(3);
		TGraph *grxyF = new TGraph(xFittedVec.size(), &xFittedVec[0], &yFittedVec[0]);
		grxyF->SetMarkerStyle(8);
		grxyF->SetMarkerColor(2);
		DrawGrid("Fitted Muon Track in XY plane; X axis ; Y axis", 9, 9);
		grxyF->Draw("p");

		c->cd(4);
		TGraph *grzyF = new TGraph(zFittedVec.size(), &zFittedVec[0], &yFittedVec[0]);
		grzyF->SetMarkerStyle(8);
		grzyF->SetMarkerColor(2);
		DrawGrid("Fitted Muon Track in ZY plane; Z axis ; Y axis", 9, 9);
		grzyF->Draw("p");


	}
};

extern std::vector<SkimmedMuonTrack*> vecOfSkimmedMuonTracks;

extern void ResetVectorOfSkimmedMuonTracks();


#endif /* ISMRAN_ANALYSIS_INC_SKIMMEDTRACKS_H_ */
