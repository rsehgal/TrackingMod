#include "GlassRpc.h"
#include "CmsRpc.h"
#include "TriggeringPlane.h"
#include <vector>
#include <iostream>
//#include "VisualizationHelper.h"
#include "SetupManager.h"
#include <TApplication.h>
//#include "Update.h"
#include "Tree.h"
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TLegend.h>
#include "GenericXYDetector.h"
typedef Tomography::Properties Detector;
using namespace Tomography;

int main(int argc, char *argv[]) {

  Detector *rpc1 = new GlassRpc(2, "FirstGlassRpc", 30, 31);
  Detector *rpc2 = new GlassRpc(3, "SecondGlassRpc", -30, 31);
  Detector *rpc3 = new GlassRpc(4, "SecondGlassRpc", -30, 31);
  //Detector *rpc4 = new GlassRpc(5, "SecondGlassRpc", -30, 31);


#ifndef CLUSTER_SIZE
   //if CLUSTER_SIZE not define, then allow all the scintillator to be fired
   rpc1->SetClusterSize(32);
   rpc2->SetClusterSize(32);
   rpc3->SetClusterSize(32);
#else
   rpc1->SetClusterSize(1);
   rpc2->SetClusterSize(1);
   rpc3->SetClusterSize(1);
#endif


  //rpc1->SetClusterSize(3);
  //rpc2->SetClusterSize(2);
  //rpc3->SetClusterSize(3);
 // rpc4->SetClusterSize(2);
  bool verbose = true;

//  Tomography::SetupManager::instance()->Register(rpc1);
//  Tomography::SetupManager::instance()->Register(rpc3);

  std::string temp_str = std::to_string(atoi(argv[1]));
  temp_str += ".root";

  Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);

  rpc1->SetEfficiency();
  rpc1->GetPlane(0)->SetEfficiency();
  rpc1->GetPlane(1)->SetEfficiency();
  if (verbose) {
    std::cout << "Eff of Det-1 : " << rpc1->GetEfficiency() << std::endl;
    std::cout << "Eff w.r.t Plane 1 : " << rpc1->GetPlane(0)->GetEfficiency() << std::endl;
    std::cout << "Eff w.r.t Plane 2 : " << rpc1->GetPlane(1)->GetEfficiency() << std::endl;
    std::cout << "--------------------------------------" << std::endl;
  }

  rpc2->SetEfficiency();
  rpc2->GetPlane(0)->SetEfficiency();
  rpc2->GetPlane(1)->SetEfficiency();
  if (verbose) {
    std::cout << "Eff of Det-2 : " << rpc2->GetEfficiency() << std::endl;
    std::cout << "Eff w.r.t Plane 1 : " << rpc2->GetPlane(0)->GetEfficiency() << std::endl;
    std::cout << "Eff w.r.t Plane 2 : " << rpc2->GetPlane(1)->GetEfficiency() << std::endl;
    std::cout << "--------------------------------------" << std::endl;
  }


  rpc3->SetEfficiency();
    rpc3->GetPlane(0)->SetEfficiency();
    rpc3->GetPlane(1)->SetEfficiency();
    if (verbose) {
      std::cout << "Eff of Det-2 : " << rpc3->GetEfficiency() << std::endl;
      std::cout << "Eff w.r.t Plane 1 : " << rpc3->GetPlane(0)->GetEfficiency() << std::endl;
      std::cout << "Eff w.r.t Plane 2 : " << rpc3->GetPlane(1)->GetEfficiency() << std::endl;
      std::cout << "--------------------------------------" << std::endl;
    }


   /* Now trying to calculate the efficiency of Triggering plane,
    * which ideally should be 100%, but that also include event where more
    * than one scintillator fired in a plane. So depending upon the cluster
    * size the efficiency may NOT be 100%.
    */

   Detector *trgPlanes = new Tomography::GenericXYDetector(2,"XYTriggeringPlanes",0,-1,8,100.,100.,1);
#ifndef CLUSTER_SIZE
   //if CLUSTER_SIZE not define, then allow all the scintillator to be fired
   trgPlanes->SetClusterSize(8);
#else
   trgPlanes->SetClusterSize(1);
#endif
   //trgPlanes->SetClusterSize(1);
   trgPlanes->SetEfficiency();
   trgPlanes->GetPlane(0)->SetEfficiency();
   trgPlanes->GetPlane(1)->SetEfficiency();
        if (verbose) {
          std::cout << "Eff of trgPlanes : " << trgPlanes->GetEfficiency() << std::endl;
          std::cout << "Eff w.r.t trgPlanes Plane 1 : " << trgPlanes->GetPlane(0)->GetEfficiency() << std::endl;
          std::cout << "Eff w.r.t trgPlanes Plane 2 : " << trgPlanes->GetPlane(1)->GetEfficiency() << std::endl;
          std::cout << "--------------------------------------" << std::endl;
        }


   Tomography::SetupManager::instance()->SetEfficiency("GLASS");
   std::cout<<"Setup Efficiency : " << Tomography::SetupManager::instance()->GetEfficiency() << std::endl;
/*
    rpc4->SetEfficiency();
      rpc4->GetPlane(0)->SetEfficiency();
      rpc4->GetPlane(1)->SetEfficiency();
      if (verbose) {
        std::cout << "Eff of Det-2 : " << rpc4->GetEfficiency() << std::endl;
        std::cout << "Eff w.r.t Plane 1 : " << rpc4->GetPlane(0)->GetEfficiency() << std::endl;
        std::cout << "Eff w.r.t Plane 2 : " << rpc4->GetPlane(1)->GetEfficiency() << std::endl;
        std::cout << "--------------------------------------" << std::endl;
      }
*/

/*
  std::cout << "--------------------------------------" << std::endl;
  Detector *topPlane = new TriggeringPlane(2, "TopPlane", -150, -1);
  Detector *bottomPlane = new TriggeringPlane(2, "BottomPlane", 150, 7);
  topPlane->SetEfficiency();
  bottomPlane->SetEfficiency();
  std::cout << "Eff of Plane 1 : " << topPlane->GetEfficiency() << std::endl;
  std::cout << "Eff of Plane-2 : " << bottomPlane->GetEfficiency() << std::endl;
*/

}
