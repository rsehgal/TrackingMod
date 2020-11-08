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

namespace lite_interface{

	/*
 	 * Some histogram specific to Scintillator
	 * opt : 0 -> QMean
	 * opt : 1 -> QNear
	 * opt : 2 -> QFar
	 */
	extern TH1F* PlotQ_012(std::vector<ScintillatorBar_V2*> scintBarVec, ushort opt);
	extern TH1F* PlotQMean(std::vector<ScintillatorBar_V2*> scintBarVec);
	extern TH1F* PlotQNear(std::vector<ScintillatorBar_V2*> scintBarVec);
	extern TH1F* PlotQFar(std::vector<ScintillatorBar_V2*> scintBarVec);

} /* End of lite_interface */ 
#endif /* ISMRAN_ANALYSIS_V2_INC_PLOTTER_H_ */
