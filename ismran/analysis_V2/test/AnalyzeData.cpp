#include "PairFinder.h"
#include "Analyzer.h"
#include "Calibration.h"
#include <TApplication.h>
#include <TCanvas.h>
#include "Plotter.h"
#include "includes.hh"
#include "HardwareNomenclature.h"



int main(int argc, char *argv[]){
	GenerateScintMatrixXYCenters();
	ushort barIndex = 25;
	lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
	lite_interface::Analyzer::numOfShots = std::atoi(argv[3]);
	lite_interface::Analyzer::shotNo = std::atoi(argv[4]);
	lite_interface::PairFinder::numOfShots = lite_interface::Analyzer::numOfShots;
	lite_interface::PairFinder::shotNo = lite_interface::Analyzer::shotNo;
	lite_interface::Analyzer pf(argv[1],argv[2]);
	return 0;
}
