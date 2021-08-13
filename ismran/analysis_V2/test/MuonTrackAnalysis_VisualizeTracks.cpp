/*
**	Filename : MuonTrackAnalysis_VisualizeTracks.cpp
**	2021-08-06
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
#include <TMath.h>

#include "VisualizationHelper.h"
#include "base/Vector3D.h"
#include <iostream>
#include <fstream>
#include "Track.h"

// using Tomography::VisualizationHelper;
// using Tracking::Vector3D;

/*
Currently visualizer work with Vector3D, hence muon tracks
muon be transformed to Vector3D from Point3D
*/
int main(int argc, char *argv[])
{
  TApplication *fApp                 = new TApplication("Test", NULL, NULL);
  Tomography::VisualizationHelper *v = new Tomography::VisualizationHelper();
  std::string calibType              = "Source";
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename                                  = argv[1];
  unsigned int numOfTracks                              = 0;
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename, numOfTracks);

  v->Register(argv[2]);

  std::vector<unsigned int> subsetLayer = {0, 1, 2, 3};
  unsigned int counter                  = 0;

  std::vector<double> vecOfMyY = {25, 15, 5, -5, -15, -25, -35, -45, -55, -65};
  std::vector<Tracking::Vector3D<double>> vecOfPt;

  unsigned int numOfTracksToVisualize = 0;
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    if (!(i % 100000)) std::cout << "Processed : " << i << " tracks......" << std::endl;
    lite_interface::SingleMuonTrack *smt = smtVec[i]->GetTrackSubset(subsetLayer);
    {
      if (smt != NULL) {

        vecOfPt.clear();
        std::vector<lite_interface::Point3D *> vecOf3DPoint = smt->Get3DHitPointVector_QParam();

        /*std::cout << "======================================" << std::endl;
        smt->Print();

        std::cout << "++++++++++++++++++++++++++++++++++++++" << std::endl;
        for (unsigned int m = 0; m < vecOf3DPoint.size(); m++) {
          vecOf3DPoint[m]->Print();
        }*/

        std::vector<TF1 *> vecOfFormula_XY_ZY = smt->GetFittedFormula_XY_ZY(subsetLayer);
        unsigned int sizeCounter              = 0;
        for (unsigned int j = 0; j < vecOfMyY.size(); j++) {
          // for (unsigned int j = 0; j < vecOf3DPoint.size(); j++) {

          // double xval = vecOfFormula_XY_ZY[0]->GetX(vecOf3DPoint[j]->GetY());
          double xval = vecOfFormula_XY_ZY[0]->GetX(vecOfMyY[j]);
          // double zval = vecOfFormula_XY_ZY[1]->GetX(vecOf3DPoint[j]->GetY());
          double zval = vecOfFormula_XY_ZY[1]->GetX(vecOfMyY[j]);
          // std::cout << RED << "XVal : " << xval << " : ZVal : " << zval << RESET << std::endl;
          if (xval > -48. && xval < 48. && zval > -48. && zval < 48.) {
            sizeCounter++;
            vecOfPt.push_back(Tracking::Vector3D<double>(xval, vecOfMyY[j], zval));
            // vecOfPt.push_back(Tracking::Vector3D<double>(xval, vecOf3DPoint[j]->GetY(), zval));
          }
        }
        // Printing just to cross check the values

#if (1)
        if (sizeCounter > 3) {
          if (numOfTracksToVisualize < 1000) {
            numOfTracksToVisualize++;
            /*std::cout << "======================================" << std::endl;
            for (unsigned int j = 0; j < vecOfPt.size(); j++) {
              vecOfPt[j].Print();
            }*/

            v->Register(vecOfPt,5);
          }
        }
        counter++;

#endif
      }
    }
  }
  v->Show();
  fApp->Run();
  return 0;
}
