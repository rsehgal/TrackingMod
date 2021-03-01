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



int main(int argc, char *argv[]){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	lite_interface::Analyzer analyzerObj(argv[1]);
	std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec = analyzerObj.GetVectorOfScintillators();
	ULong64_t timeWindow = GetMuonTrackTimeWindow(scintBarVec);
	fApp->Run();

	return 0;

}
