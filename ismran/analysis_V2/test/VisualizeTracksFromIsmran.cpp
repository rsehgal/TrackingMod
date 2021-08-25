/*
 * VisualizeTracksFromIsmran.cpp
 *
 *  Created on: 28-Aug-2020
 *      Author: rsehgal
 */

#if(0)
#include <TGeoMatrix.h>
#include "base/Global.h"
#include "HittedPixel.h"
#include "TGeoBBox.h"
#include "visualizer/Eve/Singleton.h"
#include "TThread.h"
#include "LinesAngle.h"
#include "TEveStraightLineSet.h"
#include "TRandom.h"
//#include "Tree.h"
#include "base/Vector3D.h"
//#include "Properties.h"
#include "SetupManager.h"
#include "visualizer/Eve/Singleton.h"
#include "Track.h"
#include <fstream>
#include "CommonFunc.h"
#endif
/*
 * ISMRAN related header file
 */
#include "Coordinates.h"
#include "VisualizationHelper.h"
#include "Calibration.h"
#include <iostream>
//#include "Analyzer_V2.h"
#include "Analyzer.h"
#include "TApplication.h"
#include "TreeEntry.h"
#include "HelperFunctions.h"
//#include "SkimmedTracks.h"

using Tomography::Coordinates;

bool OutsideRange(std::vector<Point3D *> singleTrack)
{
  for (unsigned int i = 0; i < singleTrack.size(); i++) {
    if ((abs(singleTrack[i]->x) > 45.) || (abs(singleTrack[i]->y) > 45.) || (abs(singleTrack[i]->z) > 50.)) {
      return true;
    }
  }
  return false;
}
int main()
{

  TApplication *fApp                 = new TApplication("Test", NULL, NULL);
  Tomography::VisualizationHelper *v = Tomography::VisualizationHelper::instance();
  v->InitializeVisualizer();
  v->Show();

  fApp->Run();
}
#if (0)
int main()
{

  TApplication *fApp                 = new TApplication("Test", NULL, NULL);
  Tomography::VisualizationHelper *v = Tomography::VisualizationHelper::instance();
  v->InitializeVisualizer();
  // v->Register("ismran.gdml");
  /*
   * ISMRAN analysis objects to get the fitted muon tracks
   */

  Calibration *cb = new Calibration("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib.root");
  // Analyzer_V2
  // av2("/home/rsehgal/BackBoneSoftwares/ismranData/ISMRAN_81bars_Th10All_CosmicRun_15hrs42mins_26Aug2020_2.root",cb);
  Analyzer_V2 av2(
      "/home/rsehgal/BackBoneSoftwares/ismranData/ISMRAN_81bars_Th10All_CosmicRun_09hrs34mins_02Sep2020_0.root", cb, 0,
      true);
  // Analyzer_V2 av2("/home/rsehgal/BackBoneSoftwares/ismranData/completeData_02Sep2020.root",cb);
  bool showTracks = false;
  if (showTracks) {
    v->Register("ismran.gdml");
    unsigned long int outsideRangeCounter = 0;
    unsigned int numOfEvents              = av2.fittedMuonTracks.size(); // 500;
    std::cout << "NUM OF TRACKS TO PLOT : " << av2.fittedMuonTracks.size() << std::endl;
    // std::cout << "Size of SkimmedMuonTrackVector : " << vecOfSkimmedMuonTracks.size() << std::endl;
    // for (int evNo = 0; evNo < av2.fittedMuonTracks.size(); evNo++) {
    // for (int evNo = 0; evNo < vecOfSkimmedMuonTracks.size(); evNo++) {
    for (int evNo = 0; evNo < numOfEvents; evNo++) {
      // std::vector<Point3D*> singleTrack = av2.fittedMuonTracks[evNo];

      // std::vector<Point3D*> singleTrack = vecOfSkimmedMuonTracks[evNo]->sFittedMuonTrack;
      std::vector<Point3D *> singleTrack = av2.fittedMuonTracks[evNo];
      if (!OutsideRange(singleTrack)) {
        unsigned long int len = singleTrack.size();
        // Drawing the hit point of the tracks in different scintillator layers
        for (unsigned int i = 0; i < len; i++) {
          v->Register(ConvertToTomoVector3D(*singleTrack[i]), 2);
        }

        len -= 1;
        // Tracking::Vector3D<double> p1(singleTrack[0]->x,singleTrack[0]->y,singleTrack[0]->z);
        Tracking::Vector3D<double> p1 = ConvertToTomoVector3D(*singleTrack[0]);
        // p1.Print();
        // Tracking::Vector3D<double> p2(singleTrack[len]->x,singleTrack[len]->y,singleTrack[len]->z);
        Tracking::Vector3D<double> p2 = ConvertToTomoVector3D(*singleTrack[len]);
        // p2.Print();

        Tomography::Track t(p1, p2);
        v->Register(&t, 7);
        // sleep(1);
        // v->Show();
      } else {
        outsideRangeCounter++;
      }
    }
    std::cout << "Num of Muon Tracks that are outside the range : " << outsideRangeCounter << std::endl;
  }

  // Plotting the fitted tracks as ROOT graph
  /*for(unsigned int i = 0 ; i < 10 ; i++){
    vecOfSkimmedMuonTracks[i]->PlotTrack();
  }*/

  v->Show();

  fApp->Run();
}

#endif