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
namespace lite_interface{

	/*
 	 * Some histogram specific to Scintillator
	 * opt : 0 -> QMean
	 * opt : 1 -> QNear
	 * opt : 2 -> QFar
	 */
	extern TH1F* PlotQ(std::vector<ScintillatorBar_V2*> scintBarVec, ushort barIndex);
	extern TH1F* PlotQ_0123(std::vector<ScintillatorBar_V2*> scintBarVec,ushort barIndex, ushort opt);
	extern TH1F* PlotQMean(std::vector<ScintillatorBar_V2*> scintBarVec,ushort barIndex);
	extern TH1F* PlotQMeanCorrected(std::vector<ScintillatorBar_V2*> scintBarVec,ushort barIndex);
	extern TH1F* PlotQNear(std::vector<ScintillatorBar_V2*> scintBarVec,ushort barIndex);
	extern TH1F* PlotQFar(std::vector<ScintillatorBar_V2*> scintBarVec,ushort barIndex);

	/*
	 * Histogram of DelT
	 */
	extern TH1F* PlotDelT(std::vector<ScintillatorBar_V2*> scintBarVec, ushort barIndex);
	extern TH1F* PlotDelTCorrected(std::vector<ScintillatorBar_V2*> scintBarVec, ushort barIndex);

} /* End of lite_interface */
#endif /* ISMRAN_ANALYSIS_V2_INC_PLOTTER_H_ */
