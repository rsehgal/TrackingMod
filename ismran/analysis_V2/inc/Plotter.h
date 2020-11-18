/*
 * Plotter.h
 *
 *  Created on: 08-Nov-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_V2_INC_PLOTTER_H_
#define ISMRAN_ANALYSIS_V2_INC_PLOTTER_H_

#include <vector>
#include <TGraph.h>
#include <TApplication.h>
#include "PsBar.h"
#include <TH1F.h>
#include "ScintillatorBar_V2.h"
#include "SingleMuonTrack.h"
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
namespace lite_interface{

	/*
 	 * Some histogram specific to Scintillator
	 * opt : 0 -> QMean
	 * opt : 1 -> QNear
	 * opt : 2 -> QFar
	 */
	extern TH1F* PlotQ(std::vector<ScintillatorBar_V2*> scintBarVec, ushort barIndex=65535);
	extern TH1F* PlotQ_0123(std::vector<ScintillatorBar_V2*> scintBarVec,ushort barIndex, ushort opt);
	extern TH1F* PlotQMean(std::vector<ScintillatorBar_V2*> scintBarVec,ushort barIndex=65535);
	extern TH1F* PlotQMeanCorrected(std::vector<ScintillatorBar_V2*> scintBarVec,ushort barIndex=65535);
	extern TH1F* PlotQNear(std::vector<ScintillatorBar_V2*> scintBarVec,ushort barIndex=65535);
	extern TH1F* PlotQFar(std::vector<ScintillatorBar_V2*> scintBarVec,ushort barIndex=65535);

	extern TH1F* PlotQMeanCorrected_V2(std::vector<ScintillatorBar_V2> scintBarVec,ushort barIndex=65535);

	/*
	 * Histogram of DelT
	 */
	extern TH1F* PlotDelT(std::vector<ScintillatorBar_V2*> scintBarVec, ushort barIndex);
	extern TH1F* PlotDelTCorrected(std::vector<ScintillatorBar_V2*> scintBarVec, ushort barIndex);

	/*
	 * Histogram for Bar Profile
	 */
	extern TH1F* PlotBarsProfile(std::vector<ScintillatorBar_V2*> scintBarVec);
	extern TH2F* PlotHitPointsOnBar(std::vector<ScintillatorBar_V2*> scintBarVec, ushort barIndex);

	/*
	 * Graphs of Single Muon Track
	 * opt 1->xy
	 * opt 2->zy
	 */
	extern TGraphErrors* PlotMuonTrack(lite_interface::SingleMuonTrack *smt,int opt=1);
	extern TGraphErrors* PlotMuonTrackXY(lite_interface::SingleMuonTrack *smt);
	extern TGraphErrors* PlotMuonTrackZY(lite_interface::SingleMuonTrack *smt);
	extern TGraphErrors* PlotMuonTrack(std::vector<lite_interface::Point3D*> vecOfPoint3D, int opt=1);
	extern TGraphErrors* PlotMuonTrackXY(std::vector<lite_interface::Point3D*> vecOfPoint3D);
	extern TGraphErrors* PlotMuonTrackZY(std::vector<lite_interface::Point3D*> vecOfPoint3D);

	/*
	 * Histogram to plot energy sum
	 */
	extern TH1F* PlotEnergySum(std::vector<lite_interface::SingleMuonTrack*> smtVec);
	extern TCanvas* PlotEnergyDistributionWithMultiplicity(std::vector<SingleMuonTrack*> muonTrackVec, unsigned int multiplicity = numOfLayers);

	/*
	 * Functions to get the fitted tracks
	 *
	 */
	extern std::vector<lite_interface::Point3D*> CreateFittedTrack(std::vector<lite_interface::Point3D*> vecOfPoint3D);
	extern std::vector<double> GetFittedXorZ(TGraphErrors *gr,std::vector<lite_interface::Point3D*> vecOfPoint3D);

	/*
	 * Function to calculate the Zenith angle histograms
	 */
	extern TH1F* PlotZenithAngle(std::vector<SingleMuonTrack*> muonTrackVec, int opt=1);

} /* End of lite_interface */
#endif /* ISMRAN_ANALYSIS_V2_INC_PLOTTER_H_ */