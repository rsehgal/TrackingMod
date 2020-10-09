/*
 * FittedTracks.h
 *
 *  Created on: 09-Oct-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_INC_FITTEDTRACKS_H_
#define ISMRAN_ANALYSIS_INC_FITTEDTRACKS_H_

#include <vector>
#include "includes.hh"
class Point3D;

class FittedTracks {
	std::vector<Double_t> xVec, yVec, zVec;
	std::vector<Double_t> xVecErr, yVecErr, zVecErr;
	std::vector<Point3D*> fittedMuonTrack;

public:
	FittedTracks();
	FittedTracks(std::vector<Double_t> xvec, std::vector<Double_t> yvec, std::vector<Double_t> zvec);
	FittedTracks(std::vector<Double_t> xvec, std::vector<Double_t> yvec, std::vector<Double_t> zvec,
			     std::vector<Double_t> xvecerror, std::vector<Double_t> yvecerror, std::vector<Double_t> zvecerror);
	void CreateFittedTrack();
	std::vector<double> GetFittedXorZ(TGraphErrors *gr);
	virtual ~FittedTracks();
	std::vector<Point3D*>  GetFittedTrack() const;
};

#endif /* ISMRAN_ANALYSIS_INC_FITTEDTRACKS_H_ */
