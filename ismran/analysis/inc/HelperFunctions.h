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
class Point3D;

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

class HelperFunctions {
public:
	HelperFunctions();
	virtual ~HelperFunctions();
};

#endif /* ISMRAN_ANALYSIS_INC_HELPERFUNCTIONS_H_ */
