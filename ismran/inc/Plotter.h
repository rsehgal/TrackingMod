/*
 * Plotter.h
 *
 *  Created on: 01-Sep-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_INC_PLOTTER_H_
#define ISMRAN_INC_PLOTTER_H_

#include <vector>
#include <TGraph.h>
#include <TApplication.h>
#include "PsBar.h"

extern void PlotMuonTrack(std::vector<ScintillatorBar*> singleMuonTrack);
extern void PlotNMuonTrack(std::vector<std::vector<ScintillatorBar*>> muonTrackVec, unsigned int numOfMuons=5);




#endif /* ISMRAN_INC_PLOTTER_H_ */
