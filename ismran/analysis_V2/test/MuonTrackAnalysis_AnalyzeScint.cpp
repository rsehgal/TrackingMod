/*
**	Filename : MuonTrackAnalysis_AnalyzeScint.cpp
**	2021-07-10
**	username : rsehgal
*/
#include <iostream>
#include <vector>
#include "PairFinder.h"
#include "Analyzer.h"
#include "Calibration.h"
#include <TApplication.h>
#include <TCanvas.h>
#include "Plotter.h"
#include "includes.hh"
#include "includes.h"
#include "HardwareNomenclature.h"
#include <TSpectrum.h>
#include "Histograms.h"
#include <TH2F.h>
#include "colors.h"

int main(int argc, char *argv[])
{

  bool verbose       = false;
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename               = argv[1];
  unsigned int numOfEvents = std::atoi(argv[2]);
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename,numOfEvents);

  std::ofstream outfile("outExp.txt");
  for(unsigned int i = 0 ; i < smtVec.size() ; i++){
		std::vector<lite_interface::ScintillatorBar_V2*> scintVec = smtVec[i]->GetMuonTrack();

		for(unsigned int j = 0 ; j < scintVec.size() ; j++){
			if(scintVec[j]->GetBarIndex()==4)
#ifdef USE_FOR_SIMULATION
				outfile << scintVec[j]->GetLogQNearByQFar_ForSimulation() <<" " << scintVec[j]->GetDelTCorrected()<< " " << scintVec[j]->EstimateHitPosition_QParam()->GetZ() << " " << 0. << std::endl;
#else
				outfile << scintVec[j]->GetLogQNearByQFar() <<" " << scintVec[j]->GetDelTCorrected()<< " " << scintVec[j]->EstimateHitPosition_QParam()->GetZ() << " " << 0. << std::endl;
#endif	
		}
	}
outfile.close();
return 0; 
}
