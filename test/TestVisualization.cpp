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
//#include <time.h>


int main(){
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  //TEveManager::Create();

  Tracking::VisualizationHelper v;
  ScintillatorPlane *s1 = new ScintillatorPlane(2,8,-105.,false,"TopPlane");
  ScintillatorPlane *s2 = new ScintillatorPlane(2,8,105.,false,"BottomPlane");
  RPC *rpc1 = new RPC(2,32,"FirstRpc",-75.);
  RPC *rpc2 = new RPC(3,32,"SecondRpc",-45.);
  RPC *rpc3 = new RPC(4,32,"ThirdRpc",-15.);
  RPC *rpc4 = new RPC(5, 32, "ThirdRpc", 15.);
  RPC *rpc5 = new RPC(6, 32, "SecondRpc", 45.);
  RPC *rpc6 = new RPC(7, 32, "FirstRpc", 75.);
  Target *t = new Target();

  v.Register(s1);
  v.Register(s2);
  v.Register(rpc1);
  v.Register(rpc2);
  v.Register(rpc3);
  v.Register(rpc4);
  v.Register(rpc5);
  v.Register(rpc6);
  v.Register(t);
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

  s1->RunThread();
  s2->RunThread();
  rpc1->GetRpc()->RunThread();
  rpc2->GetRpc()->RunThread();
  rpc3->GetRpc()->RunThread();
  rpc4->GetRpc()->RunThread();
  rpc5->GetRpc()->RunThread();
  rpc6->GetRpc()->RunThread();
  v.Show();


  TTimer timer(1000); // every second
  //timer.SetCommand("gEve->Redraw3D(kTRUE);");
  timer.SetCommand("gEve->DoRedraw3D();");
  timer.TurnOn();


  fApp->Run();

}

