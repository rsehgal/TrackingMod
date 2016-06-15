/*
 *  Created on: June 15, 2016
 *      Author: rsehgal
 */

#include "GlassRpc.h"
#include "CmsRpc.h"
#include "TriggeringPlane.h"
#include <vector>
#include <iostream>
#include "VisualizationHelper.h"
#include "SetupManager.h"
#include <TApplication.h>
#include "Update.h"
#include "Tree.h"
typedef Tomography::Properties Detector;
using namespace Tomography;

int main() {
 
  Tracking::Tree::instance()->ReadTree("6853.root", "BSC_DATA_TREE", 0);
  std::cout << "===========================================================================" << std::endl;
  Detector *rpc1 = new GlassRpc(2, "FirstGlassRpc", 30, 31);
  Detector *rpc2 = new GlassRpc(4, "SecondGlassRpc", -30, 31);
  rpc1->SetEfficiency();
  rpc1->GetPlane(0)->SetEfficiency();
  rpc1->GetPlane(1)->SetEfficiency();
  std::cout<<"Eff of Det-1 : " << rpc1->GetEfficiency() << std::endl;
  std::cout<<"Eff w.r.t Plane 1 : " << rpc1->GetPlane(0)->GetEfficiency() << std::endl;
  std::cout<<"Eff w.r.t Plane 2 : " << rpc1->GetPlane(1)->GetEfficiency() << std::endl;
  std::cout<<"--------------------------------------" << std::endl;
  rpc2->SetEfficiency();
  rpc2->GetPlane(0)->SetEfficiency();
  rpc2->GetPlane(1)->SetEfficiency();
  std::cout<<"Eff of Det-2 : " << rpc2->GetEfficiency() << std::endl;
  std::cout<<"Eff w.r.t Plane 1 : " << rpc2->GetPlane(0)->GetEfficiency() << std::endl;
  std::cout<<"Eff w.r.t Plane 2 : " << rpc2->GetPlane(1)->GetEfficiency() << std::endl;
  return 0; 
 }
