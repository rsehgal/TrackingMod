/*
 * HelperFunctions.h
 *
 *  Created on: 16-Sep-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_INC_HELPERFUNCTIONS_H_
#define ISMRAN_ANALYSIS_INC_HELPERFUNCTIONS_H_

#include <base/Vector3D.h>
#include "includes.hh"
#include <vector>
#include "SingleMuonTrack.h"
class Point3D;
class Point2D;

extern Tracking::Vector3D<double> ConvertToTomoVector3D(Point3D pt);

template<typename T>
void ResetVector(std::vector<T*> vecToReset){
	if(vecToReset.size()!=0){
			for(unsigned int i = 0 ; i < vecToReset.size() ; i++){
				delete vecToReset[i];
			}
		}
		//vecToReset.clear();
}

extern void DrawGrid(std::string t, Int_t ngx, Int_t ngy);
extern void GetFittedXorZ(TGraph *gr);
extern double LinearFit(Double_t *x,Double_t *par);
extern double Pol2(Double_t *x,Double_t *par);
extern double Cos2ThetaFit(Double_t *x, Double_t *par);
extern double NewCos2ThetaFit(Double_t *x, Double_t *par);
extern void PlotFittedLine(TGraph *gr);

extern std::vector<std::vector<unsigned long int>> myhist2D;
extern void Fill2DHist(unsigned int x, unsigned int y);
extern void HistInitializer();
extern TH1D* PlotZenithAngle(std::vector<std::vector<Point3D*>> muonTrackVec, std::string histTitle="Zenith Angle Distribution");
extern TH1D* PlotZenithAngle_XY(std::vector<std::vector<Point3D*>> muonTrackVec, std::string histTitle);
extern bool CheckRange(std::vector<Point3D*> singleMuonTrack);
extern void PrintPoint3DVector(std::vector<Point3D*> vect);

extern ushort GetStartEndIndex(unsigned int layerIndex, bool start=true);
extern ushort GetStartIndex(unsigned int layerIndex);
extern ushort GetEndIndex(unsigned int layerIndex);
extern double Interpolate(Point2D p1, Point2D p2, double y);
extern double Interpolate(lite_interface::Point3D* startPoint,lite_interface::Point3D* endPoint,lite_interface::Point3D* pointOnInspectedLayer);
extern lite_interface::Point3D* Get3DHitPointOnLayer(lite_interface::SingleMuonTrack *smt, unsigned int layerIndex);
extern lite_interface::Point3D* Get3DHitPointOnLayer(lite_interface::SingleMuonTrack *smt, unsigned int layerIndex,
													 lite_interface::Point3D *extrapolatedPt,unsigned int onLayer);
extern lite_interface::Point3D* Get3DHitPointOnLayer(lite_interface::SingleMuonTrack *smt, unsigned int layerIndex,
													 lite_interface::Point3D *extrapolatedPt,unsigned int onLayer,
													 double incomingAngle);
extern lite_interface::Point3D* Get3DHitPointOnLayer_Refined(lite_interface::SingleMuonTrack *smt, unsigned int layerIndex);
extern lite_interface::Point3D* GetHitPointOnLayer_FromParam(lite_interface::SingleMuonTrack *smt, unsigned int layerIndex);
extern std::vector<lite_interface::Point3D*> GetTrackFromLayers(lite_interface::SingleMuonTrack *smt, std::vector<unsigned int> vecOfLayerIndex);

extern lite_interface::Point3D* ExtrapolatePointOnLayer(lite_interface::Point3D *startPt, lite_interface::Point3D *endPt, unsigned int layerIndex);
extern Tracking::Vector3D<double> ExtrapolatePointOnLayer(Tracking::Vector3D<double> start, Tracking::Vector3D<double> end,  unsigned int layerIndex);
extern lite_interface::Point3D* ExtrapolatePointOnLayer(lite_interface::Point3D *startPt,
												 lite_interface::Point3D *endPt,
												 lite_interface::Point3D *intermediatePt,
												 unsigned int layerIndex);
extern Tracking::Vector3D<double> ExtrapolatePointOnLayer(Tracking::Vector3D<double> start,
												   Tracking::Vector3D<double> end,
												   Tracking::Vector3D<double> intermediate,
												   unsigned int layerIndex);
extern TF1* GetMuonPeakFitFormula(std::string barName);


class HelperFunctions {
public:
	HelperFunctions();
	virtual ~HelperFunctions();
};

#endif /* ISMRAN_ANALYSIS_INC_HELPERFUNCTIONS_H_ */
