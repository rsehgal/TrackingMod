/*
 * TestVisualization.cpp
 *
 *  Created on: Apr 28, 2016
 *      Author: rsehgal
 */
#include "VisualizationHelper.h"
#include "TEveManager.h"
#include "TSystem.h"
#include "TTimer.h"
#include "TThread.h"
#include "SetupManager.h"
//#include <time.h>


int main(){
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  //TEveManager::Create();

  Tracking::VisualizationHelper v;
  ScintillatorPlane *s1 = new ScintillatorPlane(2,8,-105.,false,"TopPlane");
  ScintillatorPlane *s2 = new ScintillatorPlane(2,8,105.,false,"BottomPlane");
  //RPC *rpc1 = new RPC(2,96,"FirstRpc",-75.);
//  RPC *rpc2 = new RPC(3,96,"SecondRpc",-45.);
//  RPC *rpc3 = new RPC(4,96,"ThirdRpc",-15.);
  // RPC *rpc4 = new RPC(5, 32, "ThirdRpc", 15.);
  // RPC *rpc5 = new RPC(6, 32, "SecondRpc", 45.);
  // RPC *rpc6 = new RPC(7, 32, "FirstRpc", 75.);
  Target *t = new Target();

  CmsRpc *rpc1 = new CmsRpc(2,"FirstCmsRpc",-75.);
  CmsRpc *rpc2 = new CmsRpc(3,"SecondRpc",-45.);
  CmsRpc *rpc3 = new CmsRpc(4,"ThirdRpc",-15.);

  //For Visualization
  v.Register(s1);
  v.Register(s2);
  v.Register(rpc1);
  v.Register(rpc2);
  v.Register(rpc3);
//  v.Register(rpc1);
//  v.Register(rpc2);
//  v.Register(rpc3);
  // v.Register(rpc4);
  // v.Register(rpc5);
  // v.Register(rpc6);
  v.Register(t);


  //For Update
  SetupManager s;
  s.RegisterScintillatorPlane(s1);
  s.RegisterScintillatorPlane(s2);
  s.RegisterRpc(rpc1);
  s.RegisterRpc(rpc2);
  s.RegisterRpc(rpc3);
  // s.RegisterRpc(rpc4);
  // s.RegisterRpc(rpc5);
  // s.RegisterRpc(rpc6);
  //v.Show();
  int evNo=0;
/*
  while(true) {
    gSystem->Sleep(500);
    gSystem->ProcessEvents();
    Update();
     v.Show();
  }
*/

  s.RunThread();
/*  s2->RunThread();
  rpc1->GetRpc()->RunThread();
  rpc2->GetRpc()->RunThread();
  rpc3->GetRpc()->RunThread();
  rpc4->GetRpc()->RunThread();
  rpc5->GetRpc()->RunThread();
  rpc6->GetRpc()->RunThread();
*/  v.Show();


  TTimer timer(20); // every 20 millisecond
  //timer.SetCommand("gEve->Redraw3D(kTRUE);");
  timer.SetCommand("gEve->DoRedraw3D();");
  timer.TurnOn();


  fApp->Run();

}

