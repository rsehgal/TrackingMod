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
namespace lite_interface {

/*
 * Some histogram specific to Scintillator
 * opt : 0 -> QMean
 * opt : 1 -> QNear
 * opt : 2 -> QFar
 */
extern TH1F *PlotQ(std::vector<ScintillatorBar_V2 *> scintBarVec, ushort barIndex = 65535);
extern TH1F *PlotQ_0123(std::vector<ScintillatorBar_V2 *> scintBarVec, ushort barIndex, ushort opt,
                        bool energKev = false);
extern TH1F *PlotQMean(std::vector<ScintillatorBar_V2 *> scintBarVec, ushort barIndex = 65535, bool energKev = false);
extern TH1F *PlotQMean(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort barIndex);
extern TH1F *PlotQMeanCorrected(std::vector<ScintillatorBar_V2 *> scintBarVec, ushort barIndex = 65535);
extern TH1F *PlotQNear(std::vector<ScintillatorBar_V2 *> scintBarVec, ushort barIndex = 65535);
extern TH1F *PlotQFar(std::vector<ScintillatorBar_V2 *> scintBarVec, ushort barIndex = 65535);

extern TH1F *PlotQMeanCorrected_V2(std::vector<ScintillatorBar_V2> scintBarVec, ushort barIndex = 65535);
extern TH1F *PlotQMeanCorrected(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort barIndex);
extern TH1F *PlotQMeanCorrectedOfFirstBarWithRespectToSecond(std::vector<lite_interface::SingleMuonTrack *> smtVec,
                                                             ushort barIndex1, ushort barIndex2);
extern TH2F *PlotQMeanCorrectedCorrelationOfFirstBarWithRespectToSecond(
    std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort barIndex1, ushort barIndex2);
/*
 * Histogram of DelT
 */
extern TH1F *PlotDelT(std::vector<ScintillatorBar_V2 *> scintBarVec, ushort barIndex);
extern TH1F *PlotDelTCorrected(std::vector<ScintillatorBar_V2 *> scintBarVec, ushort barIndex);
extern TH1F *PlotDelTCorrected(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort barIndex);
extern TH1F *PlotPixelDelTCorrected(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort barIndex1,
                                    ushort barIndex2);
extern TH1F *PlotPixelDelTBetweenBars(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort barIndex1,
                                      ushort barIndex2);
//#ifdef USE_FOR_SIMULATION
// extern TGraph* PlotDelTvsZ(std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec, ushort barIndex, bool
// linear=false);
extern TH2F *PlotDelTvsZ(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec, ushort barIndex,
                         bool linear = false);
extern TGraph *PlotDelTvsZ(unsigned int barIndex, std::vector<float> delTVec, std::vector<float> zVec);
//#endif

/*
 * Histogram for Bar Profile
 */
extern TH1F *PlotBarsProfile(std::vector<ScintillatorBar_V2 *> scintBarVec);
extern TH2F *PlotHitPointsOnBar(std::vector<ScintillatorBar_V2 *> scintBarVec, ushort barIndex);
extern TH2F *PlotHitPointsOnBar(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort barIndex);
extern TGraph *PlotHitPointsOnBar(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort barIndex1,
                                  ushort barIndex2);
extern TH2F *PlotHitPointsOnBarHist(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort barIndex1,
                                    ushort barIndex2);
extern TH2F *PlotHitPointsOnLayerHist(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort layerIndex);
extern TH2F *PlotHitPointsOnBarHist(std::vector<lite_interface::SingleMuonTrack *> smtVec,
                                    std::vector<unsigned int> vecOfScintId);
extern std::vector<double> GetXYonPixel(lite_interface::SingleMuonTrack *smt, std::vector<unsigned int> vecOfScintId);
extern TH1F *PlotStripProfileOfLayer(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort layerIndex);
extern TH1F *PlotStripProfileOfLayer(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec, ushort layerIndex);
extern TH1F *PlotStripProfileOfVectorOfLayer(std::vector<lite_interface::SingleMuonTrack *> smtVec,
                                             std::vector<unsigned short> vecOfLayerIndices);
extern TH1F *PlotHitMultiplicityOfLayer(std::vector<lite_interface::SingleMuonTrack *> smtVec, ushort layerIndex);

/*
 * Graphs of Single Muon Track
 * opt 1->xy
 * opt 2->zy
 */
extern TGraphErrors *PlotMuonTrack(lite_interface::SingleMuonTrack *smt, int opt = 1);
extern TGraphErrors *PlotMuonTrackXY(lite_interface::SingleMuonTrack *smt);
extern TGraphErrors *PlotMuonTrackZY(lite_interface::SingleMuonTrack *smt);
extern TGraphErrors *PlotMuonTrack(std::vector<lite_interface::Point3D *> vecOfPoint3D, int opt = 1);
extern TGraphErrors *PlotMuonTrackXY(std::vector<lite_interface::Point3D *> vecOfPoint3D);
extern TGraphErrors *PlotMuonTrackZY(std::vector<lite_interface::Point3D *> vecOfPoint3D);
extern TGraphErrors *PlotMuonTrack(std::vector<Double_t> xVec, std::vector<Double_t> yVec);
extern TF1 *GetFittedMuonTrackFormula(std::vector<lite_interface::Point3D *> vecOfPoint3D, bool exact, bool xy);
extern TF1 *GetFittedMuonTrackFormulaXY(std::vector<lite_interface::Point3D *> vecOfPoint3D, bool exact);
extern TF1 *GetFittedMuonTrackFormulaZY(std::vector<lite_interface::Point3D *> vecOfPoint3D, bool exact);

/*
 * Histogram to plot energy sum
 */
extern TH1F *PlotEnergySum(std::vector<lite_interface::SingleMuonTrack *> smtVec);
// extern TCanvas* PlotEnergyDistributionWithMultiplicity(std::vector<SingleMuonTrack*> muonTrackVec, unsigned int
// multiplicity = numOfLayers);
extern std::vector<TH1D *> PlotEnergyDistributionWithMultiplicity(std::vector<SingleMuonTrack *> muonTrackVec);
extern std::vector<TH1D *> PlotEnergyDistributionWithMultiplicity_Old(std::vector<SingleMuonTrack *> muonTrackVec,
                                                                      unsigned int multiplicity = numOfLayers);
extern std::vector<TH1D *> PlotEnergyDistributionWithMultiplicity(std::vector<unsigned int> sizeVector,
                                                                  std::vector<double> energySumVector);
/*
 * Functions to get the fitted tracks
 *
 */
extern std::vector<lite_interface::Point3D *> CreateFittedTrack(std::vector<lite_interface::Point3D *> vecOfPoint3D);
//extern double GetChisquareNDF(std::vector<lite_interface::Point3D *> vecOfPoint3D);
extern TF1* GetFitFormula(std::vector<lite_interface::Point3D*> vecOfPoint3D,unsigned int trackType, bool planeType);
extern std::vector<double> GetFittedXorZ(TGraphErrors *gr, std::vector<lite_interface::Point3D *> vecOfPoint3D);

/*
 * Function to calculate the Zenith angle histograms
 */
extern TH1F *PlotZenithAngle(std::vector<SingleMuonTrack *> muonTrackVec, int opt = 1);
extern TH1F *PlotZenithAngle(std::vector<std::vector<lite_interface::Point3D *>> vecOfVecOfPoint3D);
extern double GetZenithAngle(std::vector<lite_interface::Point3D *> vecOfPoint3D);
extern double GetDeviation(std::vector<lite_interface::Point3D *> incoming,
                           std::vector<lite_interface::Point3D *> outgoing);
extern TH1F *PlotZenithAngle(std::vector<double> zenithAngleVect, int opt);
extern TH1F *Plot_Acc_Corr_ZenithAngle(std::vector<double> zenithAngleVect, int opt);
extern int GetBinNumber(int nbins, double start, double end);

/*
 * Time estimation to form the tag
 * ~20ns
 */
extern ULong64_t GetMuonTrackTimeWindow(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec);
extern ULong64_t GetMuonTrackTimeWindow(std::vector<lite_interface::SingleMuonTrack *> smtVec);

extern TH1F *GetSolidAngleCorrectedHist(TH1F *solidAngleHist);

// Resolution calculation
TH1F *CalculateZResolution(std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec, ushort barIndex);

// Function to get the layer index from Point3D

// Function to plot the hit point on a layer.
extern TH2F *PlotHitPointOnLayer(std::vector<lite_interface::SingleMuonTrack *> smtVec,
                                 unsigned int inspectedLayerIndex);
#ifdef USE_FOR_SIMULATION
extern TH2F *PlotExactHitPointOnLayer(std::vector<lite_interface::SingleMuonTrack *> smtVec,
                                      unsigned int inspectedLayerIndex);
#endif

} // namespace lite_interface
#endif /* ISMRAN_ANALYSIS_V2_INC_PLOTTER_H_ */
