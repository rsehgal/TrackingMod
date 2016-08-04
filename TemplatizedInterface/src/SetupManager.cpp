#include "SetupManager.h"
#include <TCanvas.h>
#include <TH3F.h>

namespace Tomography{

SetupManager *SetupManager::s_instance = 0;
SetupManager* SetupManager::instance() {
        if (!s_instance)
          s_instance = new SetupManager;
        return s_instance;
    }

    void SetupManager::GetHitPlot(std::string detType) {
      TCanvas *cHitPlot = new TCanvas("FullSetup", "FullSetup", 600, 450);
      double fLength = 100.;
      double fBreadth = 100.;
      TH3F *h3dHitPlot =
          new TH3F("h3dHitPlot", "HitPlot of All the RPCs", 500, -fLength, fLength, 500, -fBreadth, fBreadth, 500, -120., 120.);
      h3dHitPlot->SetMarkerSize(0.5);
      h3dHitPlot->SetMarkerStyle(20);
      int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
      std::vector<Tracking::Vector3D<double>> tempVect;

      for (int evNo = 0; evNo < numOfEvents; evNo++) {

        SetEventDetected("TRG", evNo);
        if (fEventDetected) {

          SetEventDetected(detType, evNo);
          std::vector<Detector *> detectors = GetDetectorVector(detType);
          Vector3D<double> temp(0., 0., 0.);
          tempVect.clear();
          if (fEventDetected) {
            for (int j = 0; j < detectors.size(); j++) {
              // temp1 = temp;
              for (int xval = 0; xval < detectors[j]->GetPlane(0)->GetFiredStripsVector().size(); xval++) {
                for (int yval = 0; yval < detectors[j]->GetPlane(1)->GetFiredStripsVector().size(); yval++) {

                  temp = detectors[j]->GetStripCoordinate(detectors[j]->GetPlane(0)->GetFiredStripsVector()[xval],
                                                          detectors[j]->GetPlane(1)->GetFiredStripsVector()[yval],
                                                          detectors[j]->GetZPos());
                }
              }

              tempVect.push_back(temp);
            }
          }

          for (int k = 0; k < tempVect.size(); k++) {
            h3dHitPlot->Fill(tempVect[k].x(), tempVect[k].y(), tempVect[k].z());
          }
        }
      }
      h3dHitPlot->Draw();
    }

}/* end of tomography namespace */
