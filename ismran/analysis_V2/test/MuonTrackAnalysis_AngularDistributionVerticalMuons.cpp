/*
**	Filename : MuonTrackAnalysis_AngularDistributionVerticalMuons.cpp
**	2021-08-09
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

/*
** Trying to get the angular distribution of vertically going muons,
** ie. those muons which passes throught same pixel in each layer
*/

int main(int argc, char *argv[])
{
  std::ofstream outtrack("verticalTrack.txt");
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  // Tomography::VisualizationHelper *v = new Tomography::VisualizationHelper();
  std::string calibType = "Source";
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename                                  = argv[1];
  unsigned int numOfTracks                              = 0;
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename, numOfTracks);

  // v->Register(argv[2]);

  std::vector<unsigned int> subsetLayer = {0, 1, 2, 3};
  std::vector<unsigned int> oblong      = {0, 2};
  std::vector<unsigned int> cross       = {1, 3};

  unsigned int barIndexInLayer = 4;
  unsigned int counter         = 0;

  std::vector<double> vecOfMyY = {25, 15, 5, -5, -15, -25, -35, -45, -55, -65};
  // std::vector<Tracking::Vector3D<double>> vecOfPt;
  std::vector<lite_interface::Point3D *> vecOfPt;

  unsigned int numOfTracksToVisualize = 0;
  TH1F *angleHist                     = new TH1F("AngularDistribution", "", 1000, -1.57, 1.57);
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    if (!(i % 100000)) std::cout << "Processed : " << i << " tracks......" << std::endl;
    lite_interface::SingleMuonTrack *smt = smtVec[i]->GetTrackSubset(subsetLayer);
    {

      if (smt != NULL) {
        std::vector<lite_interface::ScintillatorBar_V2 *> vecOfScintOblong =
            smt->CheckTrackForRequiredLayers_V2(oblong);
        std::vector<lite_interface::ScintillatorBar_V2 *> vecOfScintCross = smt->CheckTrackForRequiredLayers_V2(cross);
        bool verticalOblong                                               = true;
        bool verticalCross                                                = true;

        if (vecOfScintOblong.size() == oblong.size() && vecOfScintCross.size() == cross.size()) {
          for (unsigned int o = 1; o < vecOfScintOblong.size(); o++) {
            verticalOblong &=
                (vecOfScintOblong[o - 1]->GetBarIndexInLayer() == barIndexInLayer) &&
                (vecOfScintOblong[o - 1]->GetBarIndexInLayer() == vecOfScintOblong[o]->GetBarIndexInLayer());
          }
          for (unsigned int c = 1; c < vecOfScintCross.size(); c++) {
            verticalCross &= (vecOfScintCross[c - 1]->GetBarIndexInLayer() == barIndexInLayer) &&
                             (vecOfScintCross[c - 1]->GetBarIndexInLayer() == vecOfScintCross[c]->GetBarIndexInLayer());
          }

          if (verticalOblong && verticalCross) {
            vecOfPt.clear();
            std::vector<lite_interface::Point3D *> vecOf3DPoint = smt->Get3DHitPointVector_QParam();

            std::vector<TF1 *> vecOfFormula_XY_ZY = smt->GetFittedFormula_XY_ZY(subsetLayer);
            unsigned int sizeCounter              = 0;
            // for (unsigned int j = 0; j < vecOfMyY.size(); j++) {
            for (unsigned int j = 0; j < vecOf3DPoint.size(); j++) {

              double xval = vecOfFormula_XY_ZY[0]->GetX(vecOf3DPoint[j]->GetY());
              // double xval = vecOfFormula_XY_ZY[0]->GetX(vecOfMyY[j]);
              double zval = vecOfFormula_XY_ZY[1]->GetX(vecOf3DPoint[j]->GetY());
              // double zval = vecOfFormula_XY_ZY[1]->GetX(vecOfMyY[j]);
              // std::cout << RED << "XVal : " << xval << " : ZVal : " << zval << RESET << std::endl;
              if (xval > -48. && xval < 48. && zval > -48. && zval < 48.) {
                sizeCounter++;
                // vecOfPt.push_back(Tracking::Vector3D<double>(xval, vecOfMyY[j], zval));
                vecOfPt.push_back(new lite_interface::Point3D(xval, vecOf3DPoint[j]->GetY(), zval));
              }
            }

            // Printing just to cross check the values

#if (1)
            if (sizeCounter > 3) {
              //  if (numOfTracksToVisualize < 1000)
              {
                numOfTracksToVisualize++;
                std::cout << "======================================" << std::endl;
                outtrack << "==========================================" << std::endl;
                for (unsigned int j = 0; j < vecOfPt.size(); j++) {
                  std::cout << RED;
                  vecOfPt[j]->Print();
                  //outtrack << vecOfPt[j]->GetX() << "," << vecOfPt[j]->GetY() << "," << vecOfPt[j]->GetZ() << std::endl;
                  std::cout << RESET;
                }

                // v->Register(vecOfPt, 5);
                /*TVector3 endPt(vecOfPt[0]->GetX(), vecOfPt[0]->GetY(), vecOfPt[0]->GetZ());
                TVector3 startPt(vecOfPt[vecOfPt.size() - 1]->GetX(), vecOfPt[vecOfPt.size() - 1]->GetY(),
                                 vecOfPt[vecOfPt.size() - 1]->GetZ());*/
                TVector3 endPt(vecOf3DPoint[0]->GetX(), vecOf3DPoint[0]->GetY(), vecOf3DPoint[0]->GetZ());
                TVector3 startPt(vecOf3DPoint[2]->GetX(), vecOf3DPoint[2]->GetY(), vecOf3DPoint[2]->GetZ());
                TVector3 trackVec = endPt - startPt;
                TVector3 ref(0., -1., 0.);
                outtrack << vecOf3DPoint[2]->GetX() << "," << vecOf3DPoint[2]->GetY() << "," << vecOf3DPoint[2]->GetZ() << std::endl;
                outtrack << vecOf3DPoint[0]->GetX() << "," << vecOf3DPoint[0]->GetY() << "," << vecOf3DPoint[0]->GetZ() << std::endl;
                double angle = atan(std::fabs(vecOf3DPoint[0]->GetZ()-vecOf3DPoint[2]->GetZ())/20.);//trackVec.Angle(ref);
                //double angle = atan((vecOf3DPoint[0]->GetZ()-vecOf3DPoint[2]->GetZ())/(vecOf3DPoint[0]->GetY()-vecOf3DPoint[2]->GetY()));//trackVec.Angle(ref);
                angleHist->Fill(angle);
              }
            }
            counter++;

#endif
          }
        }
      }
    }
  }
  outtrack.close();
  angleHist->Draw();
  // v->Show();
  fApp->Run();
  return 0;
}
