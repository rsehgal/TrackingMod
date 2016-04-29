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
#include <time.h>

int evNo = 0;
ScintillatorPlane *s1;

void Reset(){
  for(int i = 0 ; i < s1->GetNumOfScintillators() ; i++ ){
    s1->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(3);
  }
}

void *handle(void *ptr){
  while(true){
    sleep(1);
    Reset();
evNo++;
std::cout<< "Ev No : "<< evNo << std::endl;
std::cout<<"Plane Name : " << s1->GetName() << " :: PlaneSize : "<< s1->GetNumOfScintillators() << std::endl;
int stripNum = rand() % 8;
s1->GetScintillatorPlane()[stripNum]->GetScintillatorEveGeoShape()->SetMainColor(2);
  }
}


int main(){
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  //TEveManager::Create();

  Tracking::VisualizationHelper v;
  //ScintillatorPlane *s1 = new ScintillatorPlane(2,8,-105.,false,"TopPlane");
  s1 = new ScintillatorPlane(2,8,-105.,false,"TopPlane");
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

  TThread *t1 = new TThread("t1", handle, (void*) 1);
  t1->Run();

  v.Show();
  TTimer timer(1000); // every second
  //timer.SetCommand("gEve->Redraw3D(kTRUE);");
  timer.SetCommand("gEve->DoRedraw3D();");
  timer.TurnOn();

  fApp->Run();

}

