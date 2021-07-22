/*
 * HardWareNomenclature.h
 *
 *  Created on: 09-Sep-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_INC_HARDWARENOMENCLATURE_H_
#define ISMRAN_ANALYSIS_INC_HARDWARENOMENCLATURE_H_

#include <vector>
#include <string>
#include <iostream>
#include "includes.hh"

extern std::string source;

/*
 * Used for Analysis
 */
extern std::vector<std::string> vecOfBarsNamess;

/*
 * Used for Calibration
 */
extern std::vector<std::string> vecOfOnlyBarsNames;
extern std::string mode;
extern std::vector<int> vecOfNearVoltage;
extern std::vector<int> vecOfFarVoltage;
extern std::vector<int> vecOfNearThreshold;
extern std::vector<int> vecOfFarThreshold;
extern std::vector<int> vecOfSourcePositions;
extern std::vector<std::string> sequentialBarNames;
extern std::vector<unsigned int> vecOfPeakPos;
extern std::vector<double> vecOfLayersYPos;
extern std::vector<bool> vecOfLayersOrientation;
extern double GetYOfLayer(unsigned int layerIndex);
extern std::vector<double> vecOfAttenCoeff;
extern std::vector<bool> vecOfMuonCalibIndex;
struct Point2D{
	double x;
	double y;

	Point2D(double s_x, double s_y){
		x = s_x;
		y = s_y;
	}

	Point2D(){x = 100 ; y = 100.;}
	void Print(){
		std::cout << "( " << x << " , " << y <<" )" << std::endl;
	}

	double Distance(Point2D p2){
		return sqrt((p2.x-this->x)*(p2.x-this->x) + (p2.y-this->y)*(p2.y-this->y));
	}

};
extern std::vector<Point2D> vecOfScintXYCenter;
extern void GenerateScintMatrixXYCenters();
extern unsigned int GetPeakPos(std::string barName);
extern unsigned int GetIndexFromBarName(std::string barName);
class HardWareNomenclature {
public:
	virtual ~HardWareNomenclature();
	HardWareNomenclature();
};

#endif /* ISMRAN_ANALYSIS_INC_HARDWARENOMENCLATURE_H_ */
